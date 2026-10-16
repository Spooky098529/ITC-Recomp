# Older hardware: below Vulkan 1.3, or OpenGL

Status: idea, not started (written 2026-10-16 on `windows`). Nothing here
has been tried yet; line numbers are as of commit `fdc5f0e`.

The native renderer refuses to start without Vulkan 1.3 (the README lists
it as a requirement). The question was whether to port it to OpenGL for
older GPUs. Conclusion: lower the Vulkan floor to 1.1/1.2 first. It's cheap,
and it covers most of the hardware an OpenGL port would reach. OpenGL is
only worth it for a specific GPU that has no Vulkan at all.

## What actually needs 1.3

`Renderer`'s device setup ([renderer.cpp:452-516](../itc_recomp/src/native/renderer.cpp)):

- It rejects `properties.apiVersion < VK_API_VERSION_1_3`, and then
  rejects devices without `dynamicRendering`. **Dynamic rendering is the
  only hard requirement.**
- Optional, enabled only where the device has them:
  `shaderDemoteToHelperInvocation` (1.3 struct),
  `samplerMirrorClampToEdge` (1.2 struct), and from Vulkan 1.0:
  clip/cull distance, `fullDrawIndexUint32`, independent blend,
  anisotropy, cube arrays.
- The instance asks for `min(loader version, 1.3)` (renderer.cpp:256-260),
  so it's already fine with older loaders.
- Shaders: the SDK's `SpirvShaderTranslator` is asked for SPIR-V 1.5
  ([shaders.cpp:38](../itc_recomp/src/native/shaders.cpp)), which needs Vulkan 1.2.

Dynamic rendering is used in three passes in
[passes.cpp](../itc_recomp/src/native/passes.cpp). Each has a
`VkPipelineRenderingCreateInfo` at pipeline creation and a
`VkRenderingInfo` + `vkCmdBeginRendering`/`vkCmdEndRendering` when drawing:

| Pipeline info | Begin rendering |
|---|---|
| passes.cpp:1904 | passes.cpp:2012, 2039 |
| passes.cpp:2214 | passes.cpp:2330, 2335 |
| passes.cpp:3405 | passes.cpp:3465, 3472 |

Barriers use the original `vkCmdPipelineBarrier` (no synchronization2), so
they need no change.

## Plan A: Vulkan 1.1/1.2 (recommended, a few days)

1. **Dynamic rendering.** Pick one:
   - Use `VK_KHR_dynamic_rendering` when the device is below 1.3: enable
     the extension and chain `VkPhysicalDeviceDynamicRenderingFeaturesKHR`.
     Also load `vkCmdBeginRenderingKHR`/`vkCmdEndRenderingKHR` in
     [vulkan_functions.h](../itc_recomp/src/native/vulkan_functions.h),
     which currently loads only the core names. Small, but drivers that
     stopped at 1.1/1.2 often lack this extension.
   - Or replace the three sites with `VkRenderPass` + `VkFramebuffer`.
     That works on every Vulkan 1.0+ device, but means render pass objects
     (and their compatibility with pipelines) and framebuffers cached per
     attachment set. It reaches more hardware.
2. **SPIR-V version.** Pass the translator the device's highest version:
   1.5 on 1.2, 1.3 on 1.1. Check that the translator emits valid code at
   1.3 (run with the validation layer, which renderer.cpp already knows how
   to load). Add the SPIR-V version to the shader disk cache's header
   ([shaders.cpp](../itc_recomp/src/native/shaders.cpp), around line 244–250, next to
   `demote_to_helper_invocation`) so that cached translations from another
   version aren't reused.
3. **Demote to helper.** Below 1.3, read
   `VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT` (extension
   `VK_EXT_shader_demote_to_helper_invocation`) instead of the 1.3 struct,
   or leave it off. The translator already handles its absence.
4. **Mirror clamp to edge.** On 1.1 it's the extension
   `VK_KHR_sampler_mirror_clamp_to_edge`, not the 1.2 struct.
5. Update the README's requirement and the log line
   (`"native renderer: needs Vulkan 1.3"`).

Before changing anything (per CLAUDE.md, logs first): add logging of the
device's `apiVersion`, driver version, and whether it has
`VK_KHR_dynamic_rendering`, demote and mirror clamp. Then have testers with
older GPUs send logs, which shows which option in step 1 is needed. The user
has no such GPU themselves. Wine can't stand in for this either: it uses
the host's Vulkan driver.

Which GPUs gain from this (from memory, not checked): ones whose drivers
stopped at Vulkan 1.1/1.2, such as NVIDIA Kepler, older AMD GCN on
Windows drivers, and older Intel iGPUs on Windows. On Linux, Mesa already
gives 1.3 to many of these.

## Plan B: OpenGL (weeks, not recommended for now)

- **A second backend.** `src/native/` is ~11k lines, mostly
  `passes.cpp` (4.6k), `textures.cpp` (1.2k) and `buffers.cpp` (0.7k).
  They're built around Vulkan: descriptor sets with dynamic offsets (one
  constant buffer a frame serving every draw), recorded command buffers,
  explicit barriers, the pipeline cache on disk. The replay, interpolation,
  shader/interface keys and draw state logic would carry over. Every
  renderer fix (rainbow effect, gamma, textures; milestone 7 in
  `native-renderer.md`) would then need doing twice.
- **Shaders are the hard part.** The translator produces SPIR-V only.
  GL 4.6's `ARB_gl_spirv` is missing or unreliable on the old hardware this
  is for. Two alternatives: SPIRV-Cross to GLSL at runtime (a new
  dependency, and another compile on each new shader, so more first-use
  hitching), or a GLSL backend for the translator in the SDK (big, and
  against decoupling from ReXGlue).
- **Floor of ~GL 4.3.** Vertex fetch reads guest memory through a storage
  buffer (passes.cpp:1081, 1518), which in GL needs SSBOs (4.3). So GL 3.x
  hardware is out anyway, and most GPUs with good 4.3+ drivers also have
  some Vulkan.
- **Speed.** The render thread is the limit on the Steam Deck (~7 µs a
  draw). The recent gains (descriptor reuse, skipping repeated state calls)
  are Vulkan-specific; GL has higher driver CPU cost per draw.

Only consider this if a tester has a GPU with no Vulkan driver at all
(e.g. NVIDIA Fermi or AMD TeraScale) and its GL 4.3+ driver can keep up
with the game's draw count.
