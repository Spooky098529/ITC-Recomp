// The views of docs/functions/device-state.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/device-state.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// gpu; measure: Device
struct Device : View {
  using View::View;
  static constexpr uint32_t kSize = 20444;
  // the block the device was allocated in (it starts 4 bytes in); freed by Device::Release
  uint32_t allocation() const { return guest::LoadU32(base, address + -4); }
  void set_allocation(uint32_t v) const { guest::StoreU32(base, address + -4, v); }
  uint32_t allocation_address() const { return address + -4; }
  // its top byte (only Device::PatchVertexShader's shader, a measure fault)
  uint8_t vertex_float_dirty_top() const { return guest::LoadU8(base, address + 0); }
  void set_vertex_float_dirty_top(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t vertex_float_dirty_top_address() const { return address + 0; }
  // its high word (also where measure faults land: SetScissorRect's rectangle, SetDisplayMode's and the workers' r3)
  uint32_t vertex_float_dirty_high() const { return guest::LoadU32(base, address + 0); }
  void set_vertex_float_dirty_high(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vertex_float_dirty_high_address() const { return address + 0; }
  // the vertex float constants' dirty mask (a bit a constant block)
  uint64_t vertex_float_dirty() const { return guest::LoadU64(base, address + 0); }
  void set_vertex_float_dirty(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t vertex_float_dirty_address() const { return address + 0; }
  // its low word (measure faults only)
  uint32_t vertex_float_dirty_low() const { return guest::LoadU32(base, address + 4); }
  void set_vertex_float_dirty_low(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vertex_float_dirty_low_address() const { return address + 4; }
  // its high word (Device::LoadShaders; SetScissorRect's rectangle, a measure fault)
  uint32_t pixel_float_dirty_high() const { return guest::LoadU32(base, address + 8); }
  void set_pixel_float_dirty_high(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pixel_float_dirty_high_address() const { return address + 8; }
  // the pixel float constants' dirty mask
  uint64_t pixel_float_dirty() const { return guest::LoadU64(base, address + 8); }
  void set_pixel_float_dirty(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t pixel_float_dirty_address() const { return address + 8; }
  // its low word (SetScissorRect's rectangle, a measure fault)
  uint32_t pixel_float_dirty_low() const { return guest::LoadU32(base, address + 12); }
  void set_pixel_float_dirty_low(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pixel_float_dirty_low_address() const { return address + 12; }
  // the shader state's dirty mask: bits 0-11 0x2200-0x220B (bit 11 0x2200), 12-43 the fetch constants (bit 43 constant 0), 44-48 0x2180-0x2184, 49-52 the shaders (gpu-functions.md)
  uint64_t shader_dirty() const { return guest::LoadU64(base, address + 16); }
  void set_shader_dirty(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t shader_dirty_address() const { return address + 16; }
  // its high word (Device::LoadShaders; VertexDeclaration::FromFvf's element count, a measure fault)
  uint32_t render_dirty_high() const { return guest::LoadU32(base, address + 24); }
  void set_render_dirty_high(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t render_dirty_high_address() const { return address + 24; }
  // the render state's dirty mask: bit 1 the bool and loop constants, 2-22 0x2100-0x2114, 23-38 0x2000-0x200F, 39-59 0x2280-0x2294
  uint64_t render_dirty() const { return guest::LoadU64(base, address + 24); }
  void set_render_dirty(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t render_dirty_address() const { return address + 24; }
  // its low word (VertexDeclaration::FromFvf's +28, a measure fault)
  uint32_t render_dirty_low() const { return guest::LoadU32(base, address + 28); }
  void set_render_dirty_low(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t render_dirty_low_address() const { return address + 28; }
  // its high word (Device::Swap)
  uint32_t other_dirty_high() const { return guest::LoadU32(base, address + 32); }
  void set_other_dirty_high(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t other_dirty_high_address() const { return address + 32; }
  // the other state's dirty mask: bits 0-37 0x2300-0x2325, 38-45 0x2380-0x2387, 49-54 sub_822ADEF0's registers
  uint64_t other_dirty() const { return guest::LoadU64(base, address + 32); }
  void set_other_dirty(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t other_dirty_address() const { return address + 32; }
  // its low word (Device::Swap)
  uint32_t other_dirty_low() const { return guest::LoadU32(base, address + 36); }
  void set_other_dirty_low(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t other_dirty_low_address() const { return address + 36; }
  // the command buffer's write position: the last word written (stored with a pre-increment; command-buffer.md)
  uint32_t write_position() const { return guest::LoadU32(base, address + 40); }
  void set_write_position(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t write_position_address() const { return address + 40; }
  // +40 and +44 as one doubleword (Device::LoadShaders)
  uint64_t write_position_and_end() const { return guest::LoadU64(base, address + 40); }
  void set_write_position_and_end(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t write_position_and_end_address() const { return address + 40; }
  // the open segment's end
  uint32_t segment_end() const { return guest::LoadU32(base, address + 44); }
  void set_segment_end(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t segment_end_address() const { return address + 44; }
  // the segment's soft end (+44 - 160): writers end the segment past it
  uint32_t soft_end() const { return guest::LoadU32(base, address + 48); }
  void set_soft_end(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t soft_end_address() const { return address + 48; }
  // a measure fault: VertexDeclaration::FromFvf's element stores (its declaration isn't the device)
  uint16_t reference_count_high() const { return guest::LoadU16(base, address + 52); }
  void set_reference_count_high(uint16_t v) const { guest::StoreU16(base, address + 52, v); }
  uint32_t reference_count_high_address() const { return address + 52; }
  // 1 at construction (sub_822AD228); sub_8229D978 drops it and releases the device at 1
  uint32_t reference_count() const { return guest::LoadU32(base, address + 52); }
  void set_reference_count(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t reference_count_address() const { return address + 52; }
  // a measure fault, as +52's halfword
  uint16_t reference_count_low() const { return guest::LoadU16(base, address + 54); }
  void set_reference_count_low(uint16_t v) const { guest::StoreU16(base, address + 54, v); }
  uint32_t reference_count_low_address() const { return address + 54; }
  // the render state setters by state (byte offsets 0-380, D3DRS_* order), filled by SetDefaultState from 0x826388F8 (VertexDeclaration::FromFvf's and PatchVertexShader's accesses here are measure faults)
  uint32_t render_state_setters_address() const { return address + 56; }
  // the sampler state setters by state (0-76)
  uint32_t sampler_state_setters_address() const { return address + 440; }
  // the render state getters by state (Device::LoadShaders' accesses here are a measure fault: a shader's file header)
  uint32_t render_state_getters_address() const { return address + 520; }
  // the sampler state getters by state
  uint32_t sampler_state_getters_address() const { return address + 904; }
  // the fetch constants (0x4800-): 32 of 24 bytes; the textures' (sampler n at +24n) and the vertex fetch constants (95 - stream at +1784 - 8 x stream)
  uint32_t fetch_constants_address() const { return address + 1024; }
  // the vertex float constants (256 of 16 bytes)
  uint32_t vertex_float_constants_address() const { return address + 1792; }
  // the pixel float constants (256 of 16 bytes)
  uint32_t pixel_float_constants_address() const { return address + 5888; }
  // the bool and loop constants (0x4900-, 40 words); the pixel bools at +10000
  uint32_t bool_loop_constants_address() const { return address + 9984; }
  // its high halfword
  uint16_t rb_surface_info_high() const { return guest::LoadU16(base, address + 10240); }
  void set_rb_surface_info_high(uint16_t v) const { guest::StoreU16(base, address + 10240, v); }
  uint32_t rb_surface_info_high_address() const { return address + 10240; }
  // 0x2000 RB_SURFACE_INFO (Device::SetSurfaceInfo; top 14 bits from the tiling's +12912)
  uint32_t rb_surface_info() const { return guest::LoadU32(base, address + 10240); }
  void set_rb_surface_info(uint32_t v) const { guest::StoreU32(base, address + 10240, v); }
  uint32_t rb_surface_info_address() const { return address + 10240; }
  // 0x2001 RB_COLOR_INFO (render target 0's)
  uint32_t rb_color_info() const { return guest::LoadU32(base, address + 10244); }
  void set_rb_color_info(uint32_t v) const { guest::StoreU32(base, address + 10244, v); }
  uint32_t rb_color_info_address() const { return address + 10244; }
  // its high halfword
  uint16_t rb_depth_info_high() const { return guest::LoadU16(base, address + 10248); }
  void set_rb_depth_info_high(uint16_t v) const { guest::StoreU16(base, address + 10248, v); }
  uint32_t rb_depth_info_high_address() const { return address + 10248; }
  // 0x2002 RB_DEPTH_INFO
  uint32_t rb_depth_info() const { return guest::LoadU32(base, address + 10248); }
  void set_rb_depth_info(uint32_t v) const { guest::StoreU32(base, address + 10248, v); }
  uint32_t rb_depth_info_address() const { return address + 10248; }
  // 0x2003 RB_COLOR1_INFO
  uint32_t rb_color1_info() const { return guest::LoadU32(base, address + 10252); }
  void set_rb_color1_info(uint32_t v) const { guest::StoreU32(base, address + 10252, v); }
  uint32_t rb_color1_info_address() const { return address + 10252; }
  // 0x2004 RB_COLOR2_INFO
  uint32_t rb_color2_info() const { return guest::LoadU32(base, address + 10256); }
  void set_rb_color2_info(uint32_t v) const { guest::StoreU32(base, address + 10256, v); }
  uint32_t rb_color2_info_address() const { return address + 10256; }
  // 0x2005 RB_COLOR3_INFO
  uint32_t rb_color3_info() const { return guest::LoadU32(base, address + 10260); }
  void set_rb_color3_info(uint32_t v) const { guest::StoreU32(base, address + 10260, v); }
  uint32_t rb_color3_info_address() const { return address + 10260; }
  // 0x2007 (Device::Resolve); not read
  uint32_t reg_2007() const { return guest::LoadU32(base, address + 10268); }
  void set_reg_2007(uint32_t v) const { guest::StoreU32(base, address + 10268, v); }
  uint32_t reg_2007_address() const { return address + 10268; }
  // 0x200D: 0x1000 with clip planes on, else 0 (SetRenderState_ClipPlaneEnable)
  uint32_t reg_200d() const { return guest::LoadU32(base, address + 10292); }
  void set_reg_200d(uint32_t v) const { guest::StoreU32(base, address + 10292, v); }
  uint32_t reg_200d_address() const { return address + 10292; }
  // 0x200F (sub_822AC2A0 at boot); not read
  uint32_t reg_200f() const { return guest::LoadU32(base, address + 10300); }
  void set_reg_200f(uint32_t v) const { guest::StoreU32(base, address + 10300, v); }
  uint32_t reg_200f_address() const { return address + 10300; }
  // PA_SC_WINDOW_SCISSOR_TL's shadow (x bits 0-14, y bits 16-30)
  uint32_t pa_sc_window_scissor_tl() const { return guest::LoadU32(base, address + 10308); }
  void set_pa_sc_window_scissor_tl(uint32_t v) const { guest::StoreU32(base, address + 10308, v); }
  uint32_t pa_sc_window_scissor_tl_address() const { return address + 10308; }
  // PA_SC_WINDOW_SCISSOR_BR's shadow
  uint32_t pa_sc_window_scissor_br() const { return guest::LoadU32(base, address + 10312); }
  void set_pa_sc_window_scissor_br(uint32_t v) const { guest::StoreU32(base, address + 10312, v); }
  uint32_t pa_sc_window_scissor_br_address() const { return address + 10312; }
  // 0x2100 (sub_822AC2A0 at boot); not read
  uint32_t reg_2100() const { return guest::LoadU32(base, address + 10316); }
  void set_reg_2100(uint32_t v) const { guest::StoreU32(base, address + 10316, v); }
  uint32_t reg_2100_address() const { return address + 10316; }
  // 0x2103: the primitive reset index
  uint32_t vgt_multi_prim_ib_reset_indx() const { return guest::LoadU32(base, address + 10328); }
  void set_vgt_multi_prim_ib_reset_indx(uint32_t v) const { guest::StoreU32(base, address + 10328, v); }
  uint32_t vgt_multi_prim_ib_reset_indx_address() const { return address + 10328; }
  // 0x2104 RB_COLOR_MASK: 4 bits a render target
  uint32_t rb_color_mask() const { return guest::LoadU32(base, address + 10332); }
  void set_rb_color_mask(uint32_t v) const { guest::StoreU32(base, address + 10332, v); }
  uint32_t rb_color_mask_address() const { return address + 10332; }
  // 0x2105 RB_BLEND_RED: the blend factor's
  float rb_blend_red() const { return guest::LoadFloat(base, address + 10336); }
  void set_rb_blend_red(float v) const { guest::StoreFloat(base, address + 10336, v); }
  uint32_t rb_blend_red_address() const { return address + 10336; }
  // 0x2106 RB_BLEND_GREEN
  float rb_blend_green() const { return guest::LoadFloat(base, address + 10340); }
  void set_rb_blend_green(float v) const { guest::StoreFloat(base, address + 10340, v); }
  uint32_t rb_blend_green_address() const { return address + 10340; }
  // 0x2107 RB_BLEND_BLUE
  float rb_blend_blue() const { return guest::LoadFloat(base, address + 10344); }
  void set_rb_blend_blue(float v) const { guest::StoreFloat(base, address + 10344, v); }
  uint32_t rb_blend_blue_address() const { return address + 10344; }
  // 0x2108 RB_BLEND_ALPHA
  float rb_blend_alpha() const { return guest::LoadFloat(base, address + 10348); }
  void set_rb_blend_alpha(float v) const { guest::StoreFloat(base, address + 10348, v); }
  uint32_t rb_blend_alpha_address() const { return address + 10348; }
  // 0x210C RB_STENCILREFMASK_BF: the counterclockwise faces' ref, mask, write mask
  uint32_t rb_stencilrefmask_bf() const { return guest::LoadU32(base, address + 10364); }
  void set_rb_stencilrefmask_bf(uint32_t v) const { guest::StoreU32(base, address + 10364, v); }
  uint32_t rb_stencilrefmask_bf_address() const { return address + 10364; }
  // RB_STENCILREFMASK_BF bits 16-23
  uint8_t ccw_stencil_write_mask() const { return guest::LoadU8(base, address + 10365); }
  void set_ccw_stencil_write_mask(uint8_t v) const { guest::StoreU8(base, address + 10365, v); }
  uint32_t ccw_stencil_write_mask_address() const { return address + 10365; }
  // RB_STENCILREFMASK_BF bits 8-15
  uint8_t ccw_stencil_mask() const { return guest::LoadU8(base, address + 10366); }
  void set_ccw_stencil_mask(uint8_t v) const { guest::StoreU8(base, address + 10366, v); }
  uint32_t ccw_stencil_mask_address() const { return address + 10366; }
  // RB_STENCILREFMASK_BF bits 0-7
  uint8_t ccw_stencil_ref() const { return guest::LoadU8(base, address + 10367); }
  void set_ccw_stencil_ref(uint8_t v) const { guest::StoreU8(base, address + 10367, v); }
  uint32_t ccw_stencil_ref_address() const { return address + 10367; }
  // 0x210D RB_STENCILREFMASK
  uint32_t rb_stencilrefmask() const { return guest::LoadU32(base, address + 10368); }
  void set_rb_stencilrefmask(uint32_t v) const { guest::StoreU32(base, address + 10368, v); }
  uint32_t rb_stencilrefmask_address() const { return address + 10368; }
  // RB_STENCILREFMASK bits 16-23
  uint8_t stencil_write_mask() const { return guest::LoadU8(base, address + 10369); }
  void set_stencil_write_mask(uint8_t v) const { guest::StoreU8(base, address + 10369, v); }
  uint32_t stencil_write_mask_address() const { return address + 10369; }
  // RB_STENCILREFMASK bits 8-15
  uint8_t stencil_mask() const { return guest::LoadU8(base, address + 10370); }
  void set_stencil_mask(uint8_t v) const { guest::StoreU8(base, address + 10370, v); }
  uint32_t stencil_mask_address() const { return address + 10370; }
  // RB_STENCILREFMASK bits 0-7
  uint8_t stencil_ref() const { return guest::LoadU8(base, address + 10371); }
  void set_stencil_ref(uint8_t v) const { guest::StoreU8(base, address + 10371, v); }
  uint32_t stencil_ref_address() const { return address + 10371; }
  // 0x210E RB_ALPHA_REF
  float rb_alpha_ref() const { return guest::LoadFloat(base, address + 10372); }
  void set_rb_alpha_ref(float v) const { guest::StoreFloat(base, address + 10372, v); }
  uint32_t rb_alpha_ref_address() const { return address + 10372; }
  // 0x210F
  float pa_cl_vport_xscale() const { return guest::LoadFloat(base, address + 10376); }
  void set_pa_cl_vport_xscale(float v) const { guest::StoreFloat(base, address + 10376, v); }
  uint32_t pa_cl_vport_xscale_address() const { return address + 10376; }
  // 0x2110
  float pa_cl_vport_xoffset() const { return guest::LoadFloat(base, address + 10380); }
  void set_pa_cl_vport_xoffset(float v) const { guest::StoreFloat(base, address + 10380, v); }
  uint32_t pa_cl_vport_xoffset_address() const { return address + 10380; }
  // 0x2111
  float pa_cl_vport_yscale() const { return guest::LoadFloat(base, address + 10384); }
  void set_pa_cl_vport_yscale(float v) const { guest::StoreFloat(base, address + 10384, v); }
  uint32_t pa_cl_vport_yscale_address() const { return address + 10384; }
  // 0x2112
  float pa_cl_vport_yoffset() const { return guest::LoadFloat(base, address + 10388); }
  void set_pa_cl_vport_yoffset(float v) const { guest::StoreFloat(base, address + 10388, v); }
  uint32_t pa_cl_vport_yoffset_address() const { return address + 10388; }
  // 0x2113
  float pa_cl_vport_zscale() const { return guest::LoadFloat(base, address + 10392); }
  void set_pa_cl_vport_zscale(float v) const { guest::StoreFloat(base, address + 10392, v); }
  uint32_t pa_cl_vport_zscale_address() const { return address + 10392; }
  // 0x2114: a float's bits (SetViewport copies the min Z's)
  uint32_t pa_cl_vport_zoffset() const { return guest::LoadU32(base, address + 10396); }
  void set_pa_cl_vport_zoffset(uint32_t v) const { guest::StoreU32(base, address + 10396, v); }
  uint32_t pa_cl_vport_zoffset_address() const { return address + 10396; }
  // 0x2180 SQ_PROGRAM_CNTL (Device::LoadShaders)
  uint32_t sq_program_cntl() const { return guest::LoadU32(base, address + 10400); }
  void set_sq_program_cntl(uint32_t v) const { guest::StoreU32(base, address + 10400, v); }
  uint32_t sq_program_cntl_address() const { return address + 10400; }
  // 0x2181 SQ_CONTEXT_MISC
  uint32_t sq_context_misc() const { return guest::LoadU32(base, address + 10404); }
  void set_sq_context_misc(uint32_t v) const { guest::StoreU32(base, address + 10404, v); }
  uint32_t sq_context_misc_address() const { return address + 10404; }
  // 0x2182 SQ_INTERPOLATOR_CNTL
  uint32_t sq_interpolator_cntl() const { return guest::LoadU32(base, address + 10408); }
  void set_sq_interpolator_cntl(uint32_t v) const { guest::StoreU32(base, address + 10408, v); }
  uint32_t sq_interpolator_cntl_address() const { return address + 10408; }
  // 0x2183 SQ_WRAPPING_0: wraps 0-7, 4 bits each
  uint32_t sq_wrapping_0() const { return guest::LoadU32(base, address + 10412); }
  void set_sq_wrapping_0(uint32_t v) const { guest::StoreU32(base, address + 10412, v); }
  uint32_t sq_wrapping_0_address() const { return address + 10412; }
  // 0x2184 SQ_WRAPPING_1: wraps 8-15
  uint32_t sq_wrapping_1() const { return guest::LoadU32(base, address + 10416); }
  void set_sq_wrapping_1(uint32_t v) const { guest::StoreU32(base, address + 10416, v); }
  uint32_t sq_wrapping_1_address() const { return address + 10416; }
  // 0x2200 RB_DEPTHCONTROL
  uint32_t rb_depthcontrol() const { return guest::LoadU32(base, address + 10420); }
  void set_rb_depthcontrol(uint32_t v) const { guest::StoreU32(base, address + 10420, v); }
  uint32_t rb_depthcontrol_address() const { return address + 10420; }
  // 0x2201 RB_BLENDCONTROL0
  uint32_t rb_blendcontrol0() const { return guest::LoadU32(base, address + 10424); }
  void set_rb_blendcontrol0(uint32_t v) const { guest::StoreU32(base, address + 10424, v); }
  uint32_t rb_blendcontrol0_address() const { return address + 10424; }
  // 0x2202 RB_COLORCONTROL
  uint32_t rb_colorcontrol() const { return guest::LoadU32(base, address + 10428); }
  void set_rb_colorcontrol(uint32_t v) const { guest::StoreU32(base, address + 10428, v); }
  uint32_t rb_colorcontrol_address() const { return address + 10428; }
  // 0x2203 RB_HIZCONTROL (the hi-z control; from the depth surface's +32)
  uint32_t rb_hizcontrol() const { return guest::LoadU32(base, address + 10432); }
  void set_rb_hizcontrol(uint32_t v) const { guest::StoreU32(base, address + 10432, v); }
  uint32_t rb_hizcontrol_address() const { return address + 10432; }
  // RB_HIZCONTROL's byte 2 (SetRenderState_HiStencilRef)
  uint8_t hi_stencil_ref() const { return guest::LoadU8(base, address + 10434); }
  void set_hi_stencil_ref(uint8_t v) const { guest::StoreU8(base, address + 10434, v); }
  uint32_t hi_stencil_ref_address() const { return address + 10434; }
  // 0x2204 PA_CL_CLIP_CNTL
  uint32_t pa_cl_clip_cntl() const { return guest::LoadU32(base, address + 10436); }
  void set_pa_cl_clip_cntl(uint32_t v) const { guest::StoreU32(base, address + 10436, v); }
  uint32_t pa_cl_clip_cntl_address() const { return address + 10436; }
  // 0x2205 PA_SU_SC_MODE_CNTL
  uint32_t pa_su_sc_mode_cntl() const { return guest::LoadU32(base, address + 10440); }
  void set_pa_su_sc_mode_cntl(uint32_t v) const { guest::StoreU32(base, address + 10440, v); }
  uint32_t pa_su_sc_mode_cntl_address() const { return address + 10440; }
  // 0x2206 PA_CL_VTE_CNTL
  uint32_t pa_cl_vte_cntl() const { return guest::LoadU32(base, address + 10444); }
  void set_pa_cl_vte_cntl(uint32_t v) const { guest::StoreU32(base, address + 10444, v); }
  uint32_t pa_cl_vte_cntl_address() const { return address + 10444; }
  // 0x2208 RB_MODECONTROL (Device::LoadShaders, sub_822AC2A0)
  uint32_t rb_modecontrol() const { return guest::LoadU32(base, address + 10452); }
  void set_rb_modecontrol(uint32_t v) const { guest::StoreU32(base, address + 10452, v); }
  uint32_t rb_modecontrol_address() const { return address + 10452; }
  // 0x2209 RB_BLENDCONTROL1
  uint32_t rb_blendcontrol1() const { return guest::LoadU32(base, address + 10456); }
  void set_rb_blendcontrol1(uint32_t v) const { guest::StoreU32(base, address + 10456, v); }
  uint32_t rb_blendcontrol1_address() const { return address + 10456; }
  // 0x220A RB_BLENDCONTROL2
  uint32_t rb_blendcontrol2() const { return guest::LoadU32(base, address + 10460); }
  void set_rb_blendcontrol2(uint32_t v) const { guest::StoreU32(base, address + 10460, v); }
  uint32_t rb_blendcontrol2_address() const { return address + 10460; }
  // 0x220B RB_BLENDCONTROL3
  uint32_t rb_blendcontrol3() const { return guest::LoadU32(base, address + 10464); }
  void set_rb_blendcontrol3(uint32_t v) const { guest::StoreU32(base, address + 10464, v); }
  uint32_t rb_blendcontrol3_address() const { return address + 10464; }
  // 0x2280 PA_SU_POINT_SIZE's high half
  uint16_t pa_su_point_height() const { return guest::LoadU16(base, address + 10468); }
  void set_pa_su_point_height(uint16_t v) const { guest::StoreU16(base, address + 10468, v); }
  uint32_t pa_su_point_height_address() const { return address + 10468; }
  // PA_SU_POINT_SIZE's low half
  uint16_t pa_su_point_width() const { return guest::LoadU16(base, address + 10470); }
  void set_pa_su_point_width(uint16_t v) const { guest::StoreU16(base, address + 10470, v); }
  uint32_t pa_su_point_width_address() const { return address + 10470; }
  // 0x2281 PA_SU_POINT_MINMAX's high half
  uint16_t pa_su_point_max() const { return guest::LoadU16(base, address + 10472); }
  void set_pa_su_point_max(uint16_t v) const { guest::StoreU16(base, address + 10472, v); }
  uint32_t pa_su_point_max_address() const { return address + 10472; }
  // PA_SU_POINT_MINMAX's low half
  uint16_t pa_su_point_min() const { return guest::LoadU16(base, address + 10474); }
  void set_pa_su_point_min(uint16_t v) const { guest::StoreU16(base, address + 10474, v); }
  uint32_t pa_su_point_min_address() const { return address + 10474; }
  // 0x2282 (sub_822AC2A0 at boot)
  uint32_t pa_su_line_cntl() const { return guest::LoadU32(base, address + 10476); }
  void set_pa_su_line_cntl(uint32_t v) const { guest::StoreU32(base, address + 10476, v); }
  uint32_t pa_su_line_cntl_address() const { return address + 10476; }
  // 0x2285: the tessellation mode, bits 0-1
  uint32_t vgt_hos_cntl() const { return guest::LoadU32(base, address + 10488); }
  void set_vgt_hos_cntl(uint32_t v) const { guest::StoreU32(base, address + 10488, v); }
  uint32_t vgt_hos_cntl_address() const { return address + 10488; }
  // 0x2286: a float's bits
  uint32_t vgt_hos_max_tess_level() const { return guest::LoadU32(base, address + 10492); }
  void set_vgt_hos_max_tess_level(uint32_t v) const { guest::StoreU32(base, address + 10492, v); }
  uint32_t vgt_hos_max_tess_level_address() const { return address + 10492; }
  // 0x2287: a float's bits
  uint32_t vgt_hos_min_tess_level() const { return guest::LoadU32(base, address + 10496); }
  void set_vgt_hos_min_tess_level(uint32_t v) const { guest::StoreU32(base, address + 10496, v); }
  uint32_t vgt_hos_min_tess_level_address() const { return address + 10496; }
  // 0x2288 (sub_822AC2A0 at boot)
  uint32_t reg_2288() const { return guest::LoadU32(base, address + 10500); }
  void set_reg_2288(uint32_t v) const { guest::StoreU32(base, address + 10500, v); }
  uint32_t reg_2288_address() const { return address + 10500; }
  // 0x2300, the 0x2300 block's first
  uint32_t reg_2300() const { return guest::LoadU32(base, address + 10552); }
  void set_reg_2300(uint32_t v) const { guest::StoreU32(base, address + 10552, v); }
  uint32_t reg_2300_address() const { return address + 10552; }
  // 0x2301: the AA config (0, 0x8001 or 0xC003 by the surface's MSAA)
  uint32_t pa_sc_aa_config() const { return guest::LoadU32(base, address + 10556); }
  void set_pa_sc_aa_config(uint32_t v) const { guest::StoreU32(base, address + 10556, v); }
  uint32_t pa_sc_aa_config_address() const { return address + 10556; }
  // 0x2302 PA_SU_VTX_CNTL: bit 0 the half pixel offset
  uint32_t pa_su_vtx_cntl() const { return guest::LoadU32(base, address + 10560); }
  void set_pa_su_vtx_cntl(uint32_t v) const { guest::StoreU32(base, address + 10560, v); }
  uint32_t pa_su_vtx_cntl_address() const { return address + 10560; }
  // 0x2303: the guard band's Y, a float's bits
  uint32_t pa_cl_gb_vert_clip_adj() const { return guest::LoadU32(base, address + 10564); }
  void set_pa_cl_gb_vert_clip_adj(uint32_t v) const { guest::StoreU32(base, address + 10564, v); }
  uint32_t pa_cl_gb_vert_clip_adj_address() const { return address + 10564; }
  // 0x2304: the discard band's Y, a float's bits
  uint32_t pa_cl_gb_vert_disc_adj() const { return guest::LoadU32(base, address + 10568); }
  void set_pa_cl_gb_vert_disc_adj(uint32_t v) const { guest::StoreU32(base, address + 10568, v); }
  uint32_t pa_cl_gb_vert_disc_adj_address() const { return address + 10568; }
  // 0x2305: the guard band's X, a float's bits
  uint32_t pa_cl_gb_horz_clip_adj() const { return guest::LoadU32(base, address + 10572); }
  void set_pa_cl_gb_horz_clip_adj(uint32_t v) const { guest::StoreU32(base, address + 10572, v); }
  uint32_t pa_cl_gb_horz_clip_adj_address() const { return address + 10572; }
  // 0x2306: the discard band's X, a float's bits
  uint32_t pa_cl_gb_horz_disc_adj() const { return guest::LoadU32(base, address + 10576); }
  void set_pa_cl_gb_horz_disc_adj(uint32_t v) const { guest::StoreU32(base, address + 10576, v); }
  uint32_t pa_cl_gb_horz_disc_adj_address() const { return address + 10576; }
  // 0x2307 (sub_822AC2A0 at boot)
  uint32_t reg_2307() const { return guest::LoadU32(base, address + 10580); }
  void set_reg_2307(uint32_t v) const { guest::StoreU32(base, address + 10580, v); }
  uint32_t reg_2307_address() const { return address + 10580; }
  // 0x2308 (sub_822AC2A0 at boot)
  uint32_t reg_2308() const { return guest::LoadU32(base, address + 10584); }
  void set_reg_2308(uint32_t v) const { guest::StoreU32(base, address + 10584, v); }
  uint32_t reg_2308_address() const { return address + 10584; }
  // 0x2312 PA_SC_AA_MASK: the multisample mask's low 16 bits
  uint32_t pa_sc_aa_mask() const { return guest::LoadU32(base, address + 10624); }
  void set_pa_sc_aa_mask(uint32_t v) const { guest::StoreU32(base, address + 10624, v); }
  uint32_t pa_sc_aa_mask_address() const { return address + 10624; }
  // 0x2316 (sub_822AC2A0 at boot)
  uint32_t reg_2316() const { return guest::LoadU32(base, address + 10640); }
  void set_reg_2316(uint32_t v) const { guest::StoreU32(base, address + 10640, v); }
  uint32_t reg_2316_address() const { return address + 10640; }
  // 0x2317 (sub_822AC2A0 at boot)
  uint32_t reg_2317() const { return guest::LoadU32(base, address + 10644); }
  void set_reg_2317(uint32_t v) const { guest::StoreU32(base, address + 10644, v); }
  uint32_t reg_2317_address() const { return address + 10644; }
  // 0x2318 RB_COPY_CONTROL (Device::Resolve)
  uint32_t rb_copy_control() const { return guest::LoadU32(base, address + 10648); }
  void set_rb_copy_control(uint32_t v) const { guest::StoreU32(base, address + 10648, v); }
  uint32_t rb_copy_control_address() const { return address + 10648; }
  // 0x2319 RB_COPY_DEST_BASE
  uint32_t rb_copy_dest_base() const { return guest::LoadU32(base, address + 10652); }
  void set_rb_copy_dest_base(uint32_t v) const { guest::StoreU32(base, address + 10652, v); }
  uint32_t rb_copy_dest_base_address() const { return address + 10652; }
  // 0x231A RB_COPY_DEST_PITCH: the pitch and rows
  uint32_t rb_copy_dest_pitch() const { return guest::LoadU32(base, address + 10656); }
  void set_rb_copy_dest_pitch(uint32_t v) const { guest::StoreU32(base, address + 10656, v); }
  uint32_t rb_copy_dest_pitch_address() const { return address + 10656; }
  // 0x231B RB_COPY_DEST_INFO
  uint32_t rb_copy_dest_info() const { return guest::LoadU32(base, address + 10660); }
  void set_rb_copy_dest_info(uint32_t v) const { guest::StoreU32(base, address + 10660, v); }
  uint32_t rb_copy_dest_info_address() const { return address + 10660; }
  // 0x231C (sub_822AF438); probably a clear value
  uint32_t reg_231c() const { return guest::LoadU32(base, address + 10664); }
  void set_reg_231c(uint32_t v) const { guest::StoreU32(base, address + 10664, v); }
  uint32_t reg_231c_address() const { return address + 10664; }
  // 0x231D (sub_822AF438); probably a clear value
  uint32_t reg_231d() const { return guest::LoadU32(base, address + 10668); }
  void set_reg_231d(uint32_t v) const { guest::StoreU32(base, address + 10668, v); }
  uint32_t reg_231d_address() const { return address + 10668; }
  // 0x231E (sub_822AF8A0)
  uint32_t reg_231e() const { return guest::LoadU32(base, address + 10672); }
  void set_reg_231e(uint32_t v) const { guest::StoreU32(base, address + 10672, v); }
  uint32_t reg_231e_address() const { return address + 10672; }
  // 0x231F (sub_822AF8A0)
  uint32_t reg_231f() const { return guest::LoadU32(base, address + 10676); }
  void set_reg_231f(uint32_t v) const { guest::StoreU32(base, address + 10676, v); }
  uint32_t reg_231f_address() const { return address + 10676; }
  // 0x2323: for tiled destinations (Device::Resolve's kCopyFunc)
  uint32_t rb_copy_func() const { return guest::LoadU32(base, address + 10692); }
  void set_rb_copy_func(uint32_t v) const { guest::StoreU32(base, address + 10692, v); }
  uint32_t rb_copy_func_address() const { return address + 10692; }
  // 0x2324 (sub_822AC2A0 at boot)
  uint32_t reg_2324() const { return guest::LoadU32(base, address + 10696); }
  void set_reg_2324(uint32_t v) const { guest::StoreU32(base, address + 10696, v); }
  uint32_t reg_2324_address() const { return address + 10696; }
  // 0x2380 (the slope scale x 16)
  float pa_su_poly_offset_front_scale() const { return guest::LoadFloat(base, address + 10704); }
  void set_pa_su_poly_offset_front_scale(float v) const { guest::StoreFloat(base, address + 10704, v); }
  uint32_t pa_su_poly_offset_front_scale_address() const { return address + 10704; }
  // 0x2381 (the depth bias)
  float pa_su_poly_offset_front_offset() const { return guest::LoadFloat(base, address + 10708); }
  void set_pa_su_poly_offset_front_offset(float v) const { guest::StoreFloat(base, address + 10708, v); }
  uint32_t pa_su_poly_offset_front_offset_address() const { return address + 10708; }
  // its bits (SetRenderState_DepthBias stores the value as given)
  uint32_t pa_su_poly_offset_front_offset_bits() const { return guest::LoadU32(base, address + 10708); }
  void set_pa_su_poly_offset_front_offset_bits(uint32_t v) const { guest::StoreU32(base, address + 10708, v); }
  uint32_t pa_su_poly_offset_front_offset_bits_address() const { return address + 10708; }
  // 0x2382
  float pa_su_poly_offset_back_scale() const { return guest::LoadFloat(base, address + 10712); }
  void set_pa_su_poly_offset_back_scale(float v) const { guest::StoreFloat(base, address + 10712, v); }
  uint32_t pa_su_poly_offset_back_scale_address() const { return address + 10712; }
  // 0x2383
  float pa_su_poly_offset_back_offset() const { return guest::LoadFloat(base, address + 10716); }
  void set_pa_su_poly_offset_back_offset(float v) const { guest::StoreFloat(base, address + 10716, v); }
  uint32_t pa_su_poly_offset_back_offset_address() const { return address + 10716; }
  // its bits
  uint32_t pa_su_poly_offset_back_offset_bits() const { return guest::LoadU32(base, address + 10716); }
  void set_pa_su_poly_offset_back_offset_bits(uint32_t v) const { guest::StoreU32(base, address + 10716, v); }
  uint32_t pa_su_poly_offset_back_offset_bits_address() const { return address + 10716; }
  // from the image's 0x8260 8ED0 at construction (sub_822AD228); not read
  uint64_t unknown_10752() const { return guest::LoadU64(base, address + 10752); }
  void set_unknown_10752(uint64_t v) const { guest::StoreU64(base, address + 10752, v); }
  uint32_t unknown_10752_address() const { return address + 10752; }
  // the thread id while a thread holds the device lock or the movie player draws (video-output.md), else 0
  uint32_t owning_thread() const { return guest::LoadU32(base, address + 10760); }
  void set_owning_thread(uint32_t v) const { guest::StoreU32(base, address + 10760, v); }
  uint32_t owning_thread_address() const { return address + 10760; }
  // 0 at construction (sub_822AD228)
  uint32_t unknown_10764() const { return guest::LoadU32(base, address + 10764); }
  void set_unknown_10764(uint32_t v) const { guest::StoreU32(base, address + 10764, v); }
  uint32_t unknown_10764_address() const { return address + 10764; }
  // the 96-byte progress block: the fences our kick writes (command-buffer.md)
  template <typename T = CommandProgressBlock>
  T progress_block() const { return T(base, guest::LoadU32(base, address + 10768)); }
  void set_progress_block(const View& v) const { guest::StoreU32(base, address + 10768, v.address); }
  void set_progress_block(uint32_t v) const { guest::StoreU32(base, address + 10768, v); }
  uint32_t progress_block_address() const { return address + 10768; }
  // the 32-byte callback block: the scratch set-up our kick uses (Device::SetUpRing)
  uint32_t callback_block() const { return guest::LoadU32(base, address + 10772); }
  void set_callback_block(uint32_t v) const { guest::StoreU32(base, address + 10772, v); }
  uint32_t callback_block_address() const { return address + 10772; }
  // the command position (the open segment's fence; +2 a segment): a replaced resource's +8 is set to it
  uint32_t fence() const { return guest::LoadU32(base, address + 10780); }
  void set_fence(uint32_t v) const { guest::StoreU32(base, address + 10780, v); }
  uint32_t fence_address() const { return address + 10780; }
  // the resource flags (+0) that put a replaced resource on the fence list when +10780 is 0
  uint32_t fence_mask() const { return guest::LoadU32(base, address + 10784); }
  void set_fence_mask(uint32_t v) const { guest::StoreU32(base, address + 10784, v); }
  uint32_t fence_mask_address() const { return address + 10784; }
  // register 0xF01's value: bits 12-13 from sub_8229B7D8 (its packets Dropped)
  uint32_t reg_f01() const { return guest::LoadU32(base, address + 10788); }
  void set_reg_f01(uint32_t v) const { guest::StoreU32(base, address + 10788, v); }
  uint32_t reg_f01_address() const { return address + 10788; }
  // the GPU register split (SQ_GPR_MANAGEMENT; read by nothing)
  uint32_t sq_gpr_management() const { return guest::LoadU32(base, address + 10792); }
  void set_sq_gpr_management(uint32_t v) const { guest::StoreU32(base, address + 10792, v); }
  uint32_t sq_gpr_management_address() const { return address + 10792; }
  // the last SET_BIN_MASK's select (two bits a tile)
  uint32_t predication_select() const { return guest::LoadU32(base, address + 10800); }
  void set_predication_select(uint32_t v) const { guest::StoreU32(base, address + 10800, v); }
  uint32_t predication_select_address() const { return address + 10800; }
  // cleared with +10800
  uint32_t predication_select_high() const { return guest::LoadU32(base, address + 10804); }
  void set_predication_select_high(uint32_t v) const { guest::StoreU32(base, address + 10804, v); }
  uint32_t predication_select_high_address() const { return address + 10804; }
  // bit 1 predicated tiling on, 3, 4 and 5 (0x20 tiling under way), 7 (0x80) a recording command buffer
  uint8_t tiling_flags() const { return guest::LoadU8(base, address + 10808); }
  void set_tiling_flags(uint8_t v) const { guest::StoreU8(base, address + 10808, v); }
  uint32_t tiling_flags_address() const { return address + 10808; }
  // bit 1 (0x2) the GPU hung, 4 the display mode to set at the next Swap, 5, 6 (0x40) out of command buffer space
  uint8_t flags() const { return guest::LoadU8(base, address + 10809); }
  void set_flags(uint8_t v) const { guest::StoreU8(base, address + 10809, v); }
  uint32_t flags_address() const { return address + 10809; }
  // bit 2, 3 (0x8) the buffer wrapped, 5 (first Swap), 6 (0x40) the hi-z bit from the depth surface, 7 cleared by SetVertexShader
  uint8_t flags2() const { return guest::LoadU8(base, address + 10810); }
  void set_flags2(uint8_t v) const { guest::StoreU8(base, address + 10810, v); }
  uint32_t flags2_address() const { return address + 10810; }
  // bit 6 (0x40) tiling with EDRAM offsets, 7 cleared by Swap
  uint8_t flags3() const { return guest::LoadU8(base, address + 10811); }
  void set_flags3(uint8_t v) const { guest::StoreU8(base, address + 10811, v); }
  uint32_t flags3_address() const { return address + 10811; }
  // the workers' shared state (+10812..+11208): +0 a spin lock
  uint32_t worker_lock() const { return guest::LoadU32(base, address + 10812); }
  void set_worker_lock(uint32_t v) const { guest::StoreU32(base, address + 10812, v); }
  uint32_t worker_lock_address() const { return address + 10812; }
  // the shared state's +4: the primary ring (+13496); 0 tells the workers to exit
  uint32_t worker_ring() const { return guest::LoadU32(base, address + 10816); }
  void set_worker_ring(uint32_t v) const { guest::StoreU32(base, address + 10816, v); }
  uint32_t worker_ring_address() const { return address + 10816; }
  // the primary ring's write position (the shared state's +8)
  uint32_t primary_ring_write() const { return guest::LoadU32(base, address + 10820); }
  void set_primary_ring_write(uint32_t v) const { guest::StoreU32(base, address + 10820, v); }
  uint32_t primary_ring_write_address() const { return address + 10820; }
  // the shared state's +12: the ring's +13500
  uint32_t worker_ring_mask() const { return guest::LoadU32(base, address + 10824); }
  void set_worker_ring_mask(uint32_t v) const { guest::StoreU32(base, address + 10824, v); }
  uint32_t worker_ring_mask_address() const { return address + 10824; }
  // the shared state's +56: kicks deferred until the GPU's interrupts (always 0 in play)
  uint32_t pending_kicks() const { return guest::LoadU32(base, address + 10868); }
  void set_pending_kicks(uint32_t v) const { guest::StoreU32(base, address + 10868, v); }
  uint32_t pending_kicks_address() const { return address + 10868; }
  // the shared state's +60: a spin lock
  uint32_t worker_lock2() const { return guest::LoadU32(base, address + 10872); }
  void set_worker_lock2(uint32_t v) const { guest::StoreU32(base, address + 10872, v); }
  uint32_t worker_lock2_address() const { return address + 10872; }
  // the shared state's +64: an event (KEVENT)
  uint32_t worker_event_address() const { return address + 10876; }
  // the event's type
  uint8_t worker_event_type() const { return guest::LoadU8(base, address + 10876); }
  void set_worker_event_type(uint8_t v) const { guest::StoreU8(base, address + 10876, v); }
  uint32_t worker_event_type_address() const { return address + 10876; }
  // the event's signal state
  uint32_t worker_event_signal_state() const { return guest::LoadU32(base, address + 10880); }
  void set_worker_event_signal_state(uint32_t v) const { guest::StoreU32(base, address + 10880, v); }
  uint32_t worker_event_signal_state_address() const { return address + 10880; }
  // the event's wait list head (points to itself when empty)
  View worker_event_wait_list_flink() const { return View(base, guest::LoadU32(base, address + 10884)); }
  void set_worker_event_wait_list_flink(uint32_t v) const { guest::StoreU32(base, address + 10884, v); }
  uint32_t worker_event_wait_list_flink_address() const { return address + 10884; }
  // the wait list head's back link
  View worker_event_wait_list_blink() const { return View(base, guest::LoadU32(base, address + 10888)); }
  void set_worker_event_wait_list_blink(uint32_t v) const { guest::StoreU32(base, address + 10888, v); }
  uint32_t worker_event_wait_list_blink_address() const { return address + 10888; }
  // the shared state's +80: the callback's argument (the tiling header)
  uint32_t worker_callback_argument() const { return guest::LoadU32(base, address + 10892); }
  void set_worker_callback_argument(uint32_t v) const { guest::StoreU32(base, address + 10892, v); }
  uint32_t worker_callback_argument_address() const { return address + 10892; }
  // the shared state's +92: 0xC0000000
  uint32_t worker_c0000000() const { return guest::LoadU32(base, address + 10904); }
  void set_worker_c0000000(uint32_t v) const { guest::StoreU32(base, address + 10904, v); }
  uint32_t worker_c0000000_address() const { return address + 10904; }
  // the shared state's +108: the L2-locked area (0x7F100000)
  uint32_t worker_l2_area() const { return guest::LoadU32(base, address + 10920); }
  void set_worker_l2_area(uint32_t v) const { guest::StoreU32(base, address + 10920, v); }
  uint32_t worker_l2_area_address() const { return address + 10920; }
  // the CPU of the last worker made (the shared state's +396)
  uint32_t last_worker_cpu() const { return guest::LoadU32(base, address + 11208); }
  void set_last_worker_cpu(uint32_t v) const { guest::StoreU32(base, address + 11208, v); }
  uint32_t last_worker_cpu_address() const { return address + 11208; }
  // a 56-byte block a CPU (0-5): +0 the shared state, +4 its CPU, +8 and +16 a 32 KB slice of the L2 area, +20 its end, +32 its event, +48 the thread's handle, +52 its id
  uint32_t workers_address() const { return address + 11212; }
  // CPU 0's block: the shared state (+10812)
  View worker0_state() const { return View(base, guest::LoadU32(base, address + 11212)); }
  void set_worker0_state(uint32_t v) const { guest::StoreU32(base, address + 11212, v); }
  uint32_t worker0_state_address() const { return address + 11212; }
  // its CPU
  uint32_t worker0_cpu() const { return guest::LoadU32(base, address + 11216); }
  void set_worker0_cpu(uint32_t v) const { guest::StoreU32(base, address + 11216, v); }
  uint32_t worker0_cpu_address() const { return address + 11216; }
  // its L2 slice
  View worker0_l2_slice() const { return View(base, guest::LoadU32(base, address + 11220)); }
  void set_worker0_l2_slice(uint32_t v) const { guest::StoreU32(base, address + 11220, v); }
  uint32_t worker0_l2_slice_address() const { return address + 11220; }
  // its +12
  uint32_t worker0_l2_slice_word() const { return guest::LoadU32(base, address + 11224); }
  void set_worker0_l2_slice_word(uint32_t v) const { guest::StoreU32(base, address + 11224, v); }
  uint32_t worker0_l2_slice_word_address() const { return address + 11224; }
  // its +16 (the slice)
  View worker0_l2_cursor() const { return View(base, guest::LoadU32(base, address + 11228)); }
  void set_worker0_l2_cursor(uint32_t v) const { guest::StoreU32(base, address + 11228, v); }
  uint32_t worker0_l2_cursor_address() const { return address + 11228; }
  // its +20: the slice's end
  uint32_t worker0_l2_end() const { return guest::LoadU32(base, address + 11232); }
  void set_worker0_l2_end(uint32_t v) const { guest::StoreU32(base, address + 11232, v); }
  uint32_t worker0_l2_end_address() const { return address + 11232; }
  // its +24
  uint32_t worker0_c0000000() const { return guest::LoadU32(base, address + 11236); }
  void set_worker0_c0000000(uint32_t v) const { guest::StoreU32(base, address + 11236, v); }
  uint32_t worker0_c0000000_address() const { return address + 11236; }
  // its +28
  uint32_t worker0_word28() const { return guest::LoadU32(base, address + 11240); }
  void set_worker0_word28(uint32_t v) const { guest::StoreU32(base, address + 11240, v); }
  uint32_t worker0_word28_address() const { return address + 11240; }
  // its event (+32)'s type
  uint8_t worker0_event_type() const { return guest::LoadU8(base, address + 11244); }
  void set_worker0_event_type(uint8_t v) const { guest::StoreU8(base, address + 11244, v); }
  uint32_t worker0_event_type_address() const { return address + 11244; }
  // its event's signal state
  uint32_t worker0_event_signal_state() const { return guest::LoadU32(base, address + 11248); }
  void set_worker0_event_signal_state(uint32_t v) const { guest::StoreU32(base, address + 11248, v); }
  uint32_t worker0_event_signal_state_address() const { return address + 11248; }
  // its event's wait list head
  View worker0_event_wait_list_flink() const { return View(base, guest::LoadU32(base, address + 11252)); }
  void set_worker0_event_wait_list_flink(uint32_t v) const { guest::StoreU32(base, address + 11252, v); }
  uint32_t worker0_event_wait_list_flink_address() const { return address + 11252; }
  // its back link
  View worker0_event_wait_list_blink() const { return View(base, guest::LoadU32(base, address + 11256)); }
  void set_worker0_event_wait_list_blink(uint32_t v) const { guest::StoreU32(base, address + 11256, v); }
  uint32_t worker0_event_wait_list_blink_address() const { return address + 11256; }
  // its thread's handle
  uint32_t worker0_thread() const { return guest::LoadU32(base, address + 11260); }
  void set_worker0_thread(uint32_t v) const { guest::StoreU32(base, address + 11260, v); }
  uint32_t worker0_thread_address() const { return address + 11260; }
  // its thread's id
  uint32_t worker0_thread_id() const { return guest::LoadU32(base, address + 11264); }
  void set_worker0_thread_id(uint32_t v) const { guest::StoreU32(base, address + 11264, v); }
  uint32_t worker0_thread_id_address() const { return address + 11264; }
  // the bound vertex declaration (Device::SetVertexDeclaration; SetFVF sets it to +11936)
  uint32_t vertex_declaration() const { return guest::LoadU32(base, address + 11552); }
  void set_vertex_declaration(uint32_t v) const { guest::StoreU32(base, address + 11552, v); }
  uint32_t vertex_declaration_address() const { return address + 11552; }
  // the pending CPU-written range to flush: +11560 the end, +11564 the start (empty: 0, FFFFFFFF)
  uint64_t pending_flush_range() const { return guest::LoadU64(base, address + 11560); }
  void set_pending_flush_range(uint64_t v) const { guest::StoreU64(base, address + 11560, v); }
  uint32_t pending_flush_range_address() const { return address + 11560; }
  // the bound vertex shader's stride in words, a byte a stream
  uint32_t shader_strides_address() const { return address + 11568; }
  // streams 0-7 (Device::LoadShaders)
  uint64_t shader_strides_0_7() const { return guest::LoadU64(base, address + 11568); }
  void set_shader_strides_0_7(uint64_t v) const { guest::StoreU64(base, address + 11568, v); }
  uint32_t shader_strides_0_7_address() const { return address + 11568; }
  // streams 8-15
  uint64_t shader_strides_8_15() const { return guest::LoadU64(base, address + 11576); }
  void set_shader_strides_8_15(uint64_t v) const { guest::StoreU64(base, address + 11576, v); }
  uint32_t shader_strides_8_15_address() const { return address + 11576; }
  // the HiZ word (set when render state bit 1 is dirty; the draws)
  uint64_t hiz_state() const { return guest::LoadU64(base, address + 11584); }
  void set_hiz_state(uint64_t v) const { guest::StoreU64(base, address + 11584, v); }
  uint32_t hiz_state_address() const { return address + 11584; }
  // the blend control's software copy (color bits 0-12, alpha 16-28)
  uint32_t blend_control() const { return guest::LoadU32(base, address + 11592); }
  void set_blend_control(uint32_t v) const { guest::StoreU32(base, address + 11592, v); }
  uint32_t blend_control_address() const { return address + 11592; }
  // bit 31 blending on, 30 separate alpha, 23-29 the present immediate threshold
  uint32_t blend_flags() const { return guest::LoadU32(base, address + 11596); }
  void set_blend_flags(uint32_t v) const { guest::StoreU32(base, address + 11596, v); }
  uint32_t blend_flags_address() const { return address + 11596; }
  // render state 200 (the renderer reads it)
  uint32_t scissor_test_enable() const { return guest::LoadU32(base, address + 11600); }
  void set_scissor_test_enable(uint32_t v) const { guest::StoreU32(base, address + 11600, v); }
  uint32_t scissor_test_enable_address() const { return address + 11600; }
  // the color write masks as set (render targets 0-3)
  uint32_t color_write_enable0() const { return guest::LoadU32(base, address + 11604); }
  void set_color_write_enable0(uint32_t v) const { guest::StoreU32(base, address + 11604, v); }
  uint32_t color_write_enable0_address() const { return address + 11604; }
  uint32_t color_write_enable1() const { return guest::LoadU32(base, address + 11608); }
  void set_color_write_enable1(uint32_t v) const { guest::StoreU32(base, address + 11608, v); }
  uint32_t color_write_enable1_address() const { return address + 11608; }
  uint32_t color_write_enable2() const { return guest::LoadU32(base, address + 11612); }
  void set_color_write_enable2(uint32_t v) const { guest::StoreU32(base, address + 11612, v); }
  uint32_t color_write_enable2_address() const { return address + 11612; }
  uint32_t color_write_enable3() const { return guest::LoadU32(base, address + 11616); }
  void set_color_write_enable3(uint32_t v) const { guest::StoreU32(base, address + 11616, v); }
  uint32_t color_write_enable3_address() const { return address + 11616; }
  // as set: applied again when a depth surface is bound
  uint32_t z_enable() const { return guest::LoadU32(base, address + 11620); }
  void set_z_enable(uint32_t v) const { guest::StoreU32(base, address + 11620, v); }
  uint32_t z_enable_address() const { return address + 11620; }
  // as set
  uint32_t stencil_enable() const { return guest::LoadU32(base, address + 11624); }
  void set_stencil_enable(uint32_t v) const { guest::StoreU32(base, address + 11624, v); }
  uint32_t stencil_enable_address() const { return address + 11624; }
  // as set
  uint32_t point_sprite_enable() const { return guest::LoadU32(base, address + 11628); }
  void set_point_sprite_enable(uint32_t v) const { guest::StoreU32(base, address + 11628, v); }
  uint32_t point_sprite_enable_address() const { return address + 11628; }
  // as set: a float's bits
  uint32_t point_size() const { return guest::LoadU32(base, address + 11636); }
  void set_point_size(uint32_t v) const { guest::StoreU32(base, address + 11636, v); }
  uint32_t point_size_address() const { return address + 11636; }
  // as set: a float's bits
  uint32_t point_size_min() const { return guest::LoadU32(base, address + 11640); }
  void set_point_size_min(uint32_t v) const { guest::StoreU32(base, address + 11640, v); }
  uint32_t point_size_min_address() const { return address + 11640; }
  // as set: a float's bits
  uint32_t point_size_max() const { return guest::LoadU32(base, address + 11644); }
  void set_point_size_max(uint32_t v) const { guest::StoreU32(base, address + 11644, v); }
  uint32_t point_size_max_address() const { return address + 11644; }
  // a byte a sampler
  uint32_t max_anisotropy_address() const { return address + 11668; }
  // a byte a sampler
  uint32_t max_mip_level_address() const { return address + 11694; }
  // a byte a sampler
  uint32_t min_mip_level_address() const { return address + 11720; }
  // a byte a sampler: bit 0 mag, 1 min, 2 separate
  uint32_t z_filters_address() const { return address + 11746; }
  // the high-precision blend as set, render targets 0-3
  uint32_t high_precision_blend0() const { return guest::LoadU32(base, address + 11772); }
  void set_high_precision_blend0(uint32_t v) const { guest::StoreU32(base, address + 11772, v); }
  uint32_t high_precision_blend0_address() const { return address + 11772; }
  uint32_t high_precision_blend1() const { return guest::LoadU32(base, address + 11776); }
  void set_high_precision_blend1(uint32_t v) const { guest::StoreU32(base, address + 11776, v); }
  uint32_t high_precision_blend1_address() const { return address + 11776; }
  uint32_t high_precision_blend2() const { return guest::LoadU32(base, address + 11780); }
  void set_high_precision_blend2(uint32_t v) const { guest::StoreU32(base, address + 11780, v); }
  uint32_t high_precision_blend2_address() const { return address + 11780; }
  uint32_t high_precision_blend3() const { return guest::LoadU32(base, address + 11784); }
  void set_high_precision_blend3(uint32_t v) const { guest::StoreU32(base, address + 11784, v); }
  uint32_t high_precision_blend3_address() const { return address + 11784; }
  // predicated tiling only with it and +11931 0 (or +10811 bit 6)
  uint8_t unknown_11865() const { return guest::LoadU8(base, address + 11865); }
  void set_unknown_11865(uint8_t v) const { guest::StoreU8(base, address + 11865, v); }
  uint32_t unknown_11865_address() const { return address + 11865; }
  // as +11865
  uint8_t unknown_11931() const { return guest::LoadU8(base, address + 11931); }
  void set_unknown_11931(uint8_t v) const { guest::StoreU8(base, address + 11931, v); }
  uint32_t unknown_11931_address() const { return address + 11931; }
  // the device's own vertex declaration (SetFVF builds it: elements from +52)
  uint32_t fvf_declaration_address() const { return address + 11936; }
  // 5 (SetDefaultState)
  uint32_t fvf_declaration_type() const { return guest::LoadU32(base, address + 11936); }
  void set_fvf_declaration_type(uint32_t v) const { guest::StoreU32(base, address + 11936, v); }
  uint32_t fvf_declaration_type_address() const { return address + 11936; }
  // 1 (SetDefaultState)
  uint32_t fvf_declaration_count() const { return guest::LoadU32(base, address + 11940); }
  void set_fvf_declaration_count(uint32_t v) const { guest::StoreU32(base, address + 11940, v); }
  uint32_t fvf_declaration_count_address() const { return address + 11940; }
  // the flexible vertex format (Device::SetFVF)
  uint32_t fvf() const { return guest::LoadU32(base, address + 12176); }
  void set_fvf(uint32_t v) const { guest::StoreU32(base, address + 12176, v); }
  uint32_t fvf_address() const { return address + 12176; }
  // the bound index buffer (Device::SetIndices; the renderer reads it)
  uint32_t index_buffer() const { return guest::LoadU32(base, address + 12180); }
  void set_index_buffer(uint32_t v) const { guest::StoreU32(base, address + 12180, v); }
  uint32_t index_buffer_address() const { return address + 12180; }
  // the bound render targets 0-3 (4 bytes an index)
  template <typename T = DeviceSurface>
  T render_target0() const { return T(base, guest::LoadU32(base, address + 12184)); }
  void set_render_target0(const View& v) const { guest::StoreU32(base, address + 12184, v.address); }
  void set_render_target0(uint32_t v) const { guest::StoreU32(base, address + 12184, v); }
  uint32_t render_target0_address() const { return address + 12184; }
  template <typename T = DeviceSurface>
  T render_target1() const { return T(base, guest::LoadU32(base, address + 12188)); }
  void set_render_target1(const View& v) const { guest::StoreU32(base, address + 12188, v.address); }
  void set_render_target1(uint32_t v) const { guest::StoreU32(base, address + 12188, v); }
  uint32_t render_target1_address() const { return address + 12188; }
  template <typename T = DeviceSurface>
  T render_target2() const { return T(base, guest::LoadU32(base, address + 12192)); }
  void set_render_target2(const View& v) const { guest::StoreU32(base, address + 12192, v.address); }
  void set_render_target2(uint32_t v) const { guest::StoreU32(base, address + 12192, v); }
  uint32_t render_target2_address() const { return address + 12192; }
  template <typename T = DeviceSurface>
  T render_target3() const { return T(base, guest::LoadU32(base, address + 12196)); }
  void set_render_target3(const View& v) const { guest::StoreU32(base, address + 12196, v.address); }
  void set_render_target3(uint32_t v) const { guest::StoreU32(base, address + 12196, v); }
  uint32_t render_target3_address() const { return address + 12196; }
  // the bound depth surface
  template <typename T = DeviceSurface>
  T depth_surface() const { return T(base, guest::LoadU32(base, address + 12200)); }
  void set_depth_surface(const View& v) const { guest::StoreU32(base, address + 12200, v.address); }
  void set_depth_surface(uint32_t v) const { guest::StoreU32(base, address + 12200, v); }
  uint32_t depth_surface_address() const { return address + 12200; }
  // the stream sources' buffers, 4 bytes a stream
  uint32_t streams_address() const { return address + 12204; }
  // the streams' strides in words, a byte a stream
  uint32_t stream_strides_address() const { return address + 12272; }
  // streams 0-7 (Device::LoadShaders)
  uint64_t stream_strides_0_7() const { return guest::LoadU64(base, address + 12272); }
  void set_stream_strides_0_7(uint64_t v) const { guest::StoreU64(base, address + 12272, v); }
  uint32_t stream_strides_0_7_address() const { return address + 12272; }
  // streams 8-15
  uint64_t stream_strides_8_15() const { return guest::LoadU64(base, address + 12280); }
  void set_stream_strides_8_15(uint64_t v) const { guest::StoreU64(base, address + 12280, v); }
  uint32_t stream_strides_8_15_address() const { return address + 12280; }
  // the bound textures, 4 bytes a stage (26)
  uint32_t textures_address() const { return address + 12288; }
  // the viewport (Device::SetViewport, GetViewport's copy)
  uint32_t viewport_x() const { return guest::LoadU32(base, address + 12392); }
  void set_viewport_x(uint32_t v) const { guest::StoreU32(base, address + 12392, v); }
  uint32_t viewport_x_address() const { return address + 12392; }
  uint32_t viewport_y() const { return guest::LoadU32(base, address + 12396); }
  void set_viewport_y(uint32_t v) const { guest::StoreU32(base, address + 12396, v); }
  uint32_t viewport_y_address() const { return address + 12396; }
  uint32_t viewport_width() const { return guest::LoadU32(base, address + 12400); }
  void set_viewport_width(uint32_t v) const { guest::StoreU32(base, address + 12400, v); }
  uint32_t viewport_width_address() const { return address + 12400; }
  uint32_t viewport_height() const { return guest::LoadU32(base, address + 12404); }
  void set_viewport_height(uint32_t v) const { guest::StoreU32(base, address + 12404, v); }
  uint32_t viewport_height_address() const { return address + 12404; }
  // a float's bits
  uint32_t viewport_min_z() const { return guest::LoadU32(base, address + 12408); }
  void set_viewport_min_z(uint32_t v) const { guest::StoreU32(base, address + 12408, v); }
  uint32_t viewport_min_z_address() const { return address + 12408; }
  // a float's bits
  uint32_t viewport_max_z() const { return guest::LoadU32(base, address + 12412); }
  void set_viewport_max_z(uint32_t v) const { guest::StoreU32(base, address + 12412, v); }
  uint32_t viewport_max_z_address() const { return address + 12412; }
  // the scissor rectangle (the renderer reads it)
  uint32_t scissor_left() const { return guest::LoadU32(base, address + 12416); }
  void set_scissor_left(uint32_t v) const { guest::StoreU32(base, address + 12416, v); }
  uint32_t scissor_left_address() const { return address + 12416; }
  uint32_t scissor_top() const { return guest::LoadU32(base, address + 12420); }
  void set_scissor_top(uint32_t v) const { guest::StoreU32(base, address + 12420, v); }
  uint32_t scissor_top_address() const { return address + 12420; }
  uint32_t scissor_right() const { return guest::LoadU32(base, address + 12424); }
  void set_scissor_right(uint32_t v) const { guest::StoreU32(base, address + 12424, v); }
  uint32_t scissor_right_address() const { return address + 12424; }
  uint32_t scissor_bottom() const { return guest::LoadU32(base, address + 12428); }
  void set_scissor_bottom(uint32_t v) const { guest::StoreU32(base, address + 12428, v); }
  uint32_t scissor_bottom_address() const { return address + 12428; }
  // the bound pixel shader (Device::SetPixelShader)
  uint32_t pixel_shader() const { return guest::LoadU32(base, address + 12432); }
  void set_pixel_shader(uint32_t v) const { guest::StoreU32(base, address + 12432, v); }
  uint32_t pixel_shader_address() const { return address + 12432; }
  // the bound vertex shader (Device::SetVertexShader)
  uint32_t vertex_shader() const { return guest::LoadU32(base, address + 12436); }
  void set_vertex_shader(uint32_t v) const { guest::StoreU32(base, address + 12436, v); }
  uint32_t vertex_shader_address() const { return address + 12436; }
  // written by Device::LoadShaders
  uint32_t shader_word() const { return guest::LoadU32(base, address + 12440); }
  void set_shader_word(uint32_t v) const { guest::StoreU32(base, address + 12440, v); }
  uint32_t shader_word_address() const { return address + 12440; }
  // the predication select (-1 none)
  uint32_t bin_mask() const { return guest::LoadU32(base, address + 12448); }
  void set_bin_mask(uint32_t v) const { guest::StoreU32(base, address + 12448, v); }
  uint32_t bin_mask_address() const { return address + 12448; }
  uint32_t bin_mask_high() const { return guest::LoadU32(base, address + 12452); }
  void set_bin_mask_high(uint32_t v) const { guest::StoreU32(base, address + 12452, v); }
  uint32_t bin_mask_high_address() const { return address + 12452; }
  // 1 while a tile's predication is set
  uint32_t predicated() const { return guest::LoadU32(base, address + 12456); }
  void set_predicated(uint32_t v) const { guest::StoreU32(base, address + 12456, v); }
  uint32_t predicated_address() const { return address + 12456; }
  // 2 while tiling
  uint32_t tiling_state() const { return guest::LoadU32(base, address + 12460); }
  void set_tiling_state(uint32_t v) const { guest::StoreU32(base, address + 12460, v); }
  uint32_t tiling_state_address() const { return address + 12460; }
  // render target 0, else the depth surface (BeginTiling)
  template <typename T = DeviceSurface>
  T tiled_surface() const { return T(base, guest::LoadU32(base, address + 12464)); }
  void set_tiled_surface(const View& v) const { guest::StoreU32(base, address + 12464, v.address); }
  void set_tiled_surface(uint32_t v) const { guest::StoreU32(base, address + 12464, v); }
  uint32_t tiled_surface_address() const { return address + 12464; }
  // the bound set saved at the tiling's start: render targets 0-3, the depth surface
  template <typename T = DeviceSurface>
  T saved_render_target0() const { return T(base, guest::LoadU32(base, address + 12468)); }
  void set_saved_render_target0(const View& v) const { guest::StoreU32(base, address + 12468, v.address); }
  void set_saved_render_target0(uint32_t v) const { guest::StoreU32(base, address + 12468, v); }
  uint32_t saved_render_target0_address() const { return address + 12468; }
  template <typename T = DeviceSurface>
  T saved_render_target1() const { return T(base, guest::LoadU32(base, address + 12472)); }
  void set_saved_render_target1(const View& v) const { guest::StoreU32(base, address + 12472, v.address); }
  void set_saved_render_target1(uint32_t v) const { guest::StoreU32(base, address + 12472, v); }
  uint32_t saved_render_target1_address() const { return address + 12472; }
  template <typename T = DeviceSurface>
  T saved_render_target2() const { return T(base, guest::LoadU32(base, address + 12476)); }
  void set_saved_render_target2(const View& v) const { guest::StoreU32(base, address + 12476, v.address); }
  void set_saved_render_target2(uint32_t v) const { guest::StoreU32(base, address + 12476, v); }
  uint32_t saved_render_target2_address() const { return address + 12476; }
  template <typename T = DeviceSurface>
  T saved_render_target3() const { return T(base, guest::LoadU32(base, address + 12480)); }
  void set_saved_render_target3(const View& v) const { guest::StoreU32(base, address + 12480, v.address); }
  void set_saved_render_target3(uint32_t v) const { guest::StoreU32(base, address + 12480, v); }
  uint32_t saved_render_target3_address() const { return address + 12480; }
  template <typename T = DeviceSurface>
  T saved_depth_surface() const { return T(base, guest::LoadU32(base, address + 12484)); }
  void set_saved_depth_surface(const View& v) const { guest::StoreU32(base, address + 12484, v.address); }
  void set_saved_depth_surface(uint32_t v) const { guest::StoreU32(base, address + 12484, v); }
  uint32_t saved_depth_surface_address() const { return address + 12484; }
  uint32_t tile_count() const { return guest::LoadU32(base, address + 12488); }
  void set_tile_count(uint32_t v) const { guest::StoreU32(base, address + 12488, v); }
  uint32_t tile_count_address() const { return address + 12488; }
  // 16 bytes a tile: left, top, right, bottom
  uint32_t tiles_address() const { return address + 12492; }
  uint32_t tile0_left() const { return guest::LoadU32(base, address + 12492); }
  void set_tile0_left(uint32_t v) const { guest::StoreU32(base, address + 12492, v); }
  uint32_t tile0_left_address() const { return address + 12492; }
  uint32_t tile0_top() const { return guest::LoadU32(base, address + 12496); }
  void set_tile0_top(uint32_t v) const { guest::StoreU32(base, address + 12496, v); }
  uint32_t tile0_top_address() const { return address + 12496; }
  uint32_t tile0_right() const { return guest::LoadU32(base, address + 12500); }
  void set_tile0_right(uint32_t v) const { guest::StoreU32(base, address + 12500, v); }
  uint32_t tile0_right_address() const { return address + 12500; }
  uint32_t tile0_bottom() const { return guest::LoadU32(base, address + 12504); }
  void set_tile0_bottom(uint32_t v) const { guest::StoreU32(base, address + 12504, v); }
  uint32_t tile0_bottom_address() const { return address + 12504; }
  // the tiles' top-left rounded down to 32, 8 bytes each
  uint32_t tile_corners_address() const { return address + 12732; }
  uint32_t tile0_corner_x() const { return guest::LoadU32(base, address + 12732); }
  void set_tile0_corner_x(uint32_t v) const { guest::StoreU32(base, address + 12732, v); }
  uint32_t tile0_corner_x_address() const { return address + 12732; }
  uint32_t tile0_corner_y() const { return guest::LoadU32(base, address + 12736); }
  void set_tile0_corner_y(uint32_t v) const { guest::StoreU32(base, address + 12736, v); }
  uint32_t tile0_corner_y_address() const { return address + 12736; }
  // the tiles' offsets in EDRAM tiles (with flags bit 1)
  uint32_t tile_offsets_address() const { return address + 12852; }
  uint32_t tile0_offset() const { return guest::LoadU32(base, address + 12852); }
  void set_tile0_offset(uint32_t v) const { guest::StoreU32(base, address + 12852, v); }
  uint32_t tile0_offset_address() const { return address + 12852; }
  // the pitch rounded to the tile size (flags bit 1)
  uint32_t tiling_pitch() const { return guest::LoadU32(base, address + 12912); }
  void set_tiling_pitch(uint32_t v) const { guest::StoreU32(base, address + 12912, v); }
  uint32_t tiling_pitch_address() const { return address + 12912; }
  // the widest right
  uint32_t tiling_width() const { return guest::LoadU32(base, address + 12916); }
  void set_tiling_width(uint32_t v) const { guest::StoreU32(base, address + 12916, v); }
  uint32_t tiling_width_address() const { return address + 12916; }
  // the tallest bottom
  uint32_t tiling_height() const { return guest::LoadU32(base, address + 12920); }
  void set_tiling_height(uint32_t v) const { guest::StoreU32(base, address + 12920, v); }
  uint32_t tiling_height_address() const { return address + 12920; }
  // BeginTiling's flags (bits 24-29 the CPUs for the worker's wake)
  uint32_t tiling_options() const { return guest::LoadU32(base, address + 12924); }
  void set_tiling_options(uint32_t v) const { guest::StoreU32(base, address + 12924, v); }
  uint32_t tiling_options_address() const { return address + 12924; }
  // the clear color for the tiles after the first
  uint32_t tiling_clear_color_address() const { return address + 12928; }
  uint64_t tiling_clear_color_01() const { return guest::LoadU64(base, address + 12928); }
  void set_tiling_clear_color_01(uint64_t v) const { guest::StoreU64(base, address + 12928, v); }
  uint32_t tiling_clear_color_01_address() const { return address + 12928; }
  uint64_t tiling_clear_color_23() const { return guest::LoadU64(base, address + 12936); }
  void set_tiling_clear_color_23(uint64_t v) const { guest::StoreU64(base, address + 12936, v); }
  uint32_t tiling_clear_color_23_address() const { return address + 12936; }
  float tiling_clear_z() const { return guest::LoadFloat(base, address + 12944); }
  void set_tiling_clear_z(float v) const { guest::StoreFloat(base, address + 12944, v); }
  uint32_t tiling_clear_z_address() const { return address + 12944; }
  uint32_t tiling_clear_stencil() const { return guest::LoadU32(base, address + 12948); }
  void set_tiling_clear_stencil(uint32_t v) const { guest::StoreU32(base, address + 12948, v); }
  uint32_t tiling_clear_stencil_address() const { return address + 12948; }
  // how far the tiled frame's segments reach (0 when not tiling)
  uint32_t tiling_end() const { return guest::LoadU32(base, address + 12960); }
  void set_tiling_end(uint32_t v) const { guest::StoreU32(base, address + 12960, v); }
  uint32_t tiling_end_address() const { return address + 12960; }
  uint32_t tiling_generation() const { return guest::LoadU32(base, address + 12964); }
  void set_tiling_generation(uint32_t v) const { guest::StoreU32(base, address + 12964, v); }
  uint32_t tiling_generation_address() const { return address + 12964; }
  // the last segment tail's position and generation
  uint32_t tail_position() const { return guest::LoadU32(base, address + 12968); }
  void set_tail_position(uint32_t v) const { guest::StoreU32(base, address + 12968, v); }
  uint32_t tail_position_address() const { return address + 12968; }
  uint32_t tail_generation() const { return guest::LoadU32(base, address + 12972); }
  void set_tail_generation(uint32_t v) const { guest::StoreU32(base, address + 12972, v); }
  uint32_t tail_generation_address() const { return address + 12972; }
  // the tiled draws' chain (20 bytes: first block, current block, cursor, end, device)
  uint32_t draw_chain_first() const { return guest::LoadU32(base, address + 12976); }
  void set_draw_chain_first(uint32_t v) const { guest::StoreU32(base, address + 12976, v); }
  uint32_t draw_chain_first_address() const { return address + 12976; }
  uint32_t draw_chain_cursor() const { return guest::LoadU32(base, address + 12984); }
  void set_draw_chain_cursor(uint32_t v) const { guest::StoreU32(base, address + 12984, v); }
  uint32_t draw_chain_cursor_address() const { return address + 12984; }
  uint32_t draw_chain_end() const { return guest::LoadU32(base, address + 12988); }
  void set_draw_chain_end(uint32_t v) const { guest::StoreU32(base, address + 12988, v); }
  uint32_t draw_chain_end_address() const { return address + 12988; }
  // a third chain
  uint32_t third_chain_first() const { return guest::LoadU32(base, address + 12996); }
  void set_third_chain_first(uint32_t v) const { guest::StoreU32(base, address + 12996, v); }
  uint32_t third_chain_first_address() const { return address + 12996; }
  // the segments' chain
  uint32_t segment_chain_first() const { return guest::LoadU32(base, address + 13016); }
  void set_segment_chain_first(uint32_t v) const { guest::StoreU32(base, address + 13016, v); }
  uint32_t segment_chain_first_address() const { return address + 13016; }
  uint32_t segment_chain_cursor() const { return guest::LoadU32(base, address + 13024); }
  void set_segment_chain_cursor(uint32_t v) const { guest::StoreU32(base, address + 13024, v); }
  uint32_t segment_chain_cursor_address() const { return address + 13024; }
  uint32_t segment_chain_end() const { return guest::LoadU32(base, address + 13028); }
  void set_segment_chain_end(uint32_t v) const { guest::StoreU32(base, address + 13028, v); }
  uint32_t segment_chain_end_address() const { return address + 13028; }
  // the segment chain saved at the tiling's start (the list the tiling's end defers its kicks on)
  uint32_t saved_chain_first() const { return guest::LoadU32(base, address + 13036); }
  void set_saved_chain_first(uint32_t v) const { guest::StoreU32(base, address + 13036, v); }
  uint32_t saved_chain_first_address() const { return address + 13036; }
  // SaveSegment's six words: +40, +44, +13528, +13524, +13516, +13520
  uint32_t saved_write_position() const { return guest::LoadU32(base, address + 13056); }
  void set_saved_write_position(uint32_t v) const { guest::StoreU32(base, address + 13056, v); }
  uint32_t saved_write_position_address() const { return address + 13056; }
  uint32_t saved_segment_end() const { return guest::LoadU32(base, address + 13060); }
  void set_saved_segment_end(uint32_t v) const { guest::StoreU32(base, address + 13060, v); }
  uint32_t saved_segment_end_address() const { return address + 13060; }
  uint32_t saved_segment_start() const { return guest::LoadU32(base, address + 13064); }
  void set_saved_segment_start(uint32_t v) const { guest::StoreU32(base, address + 13064, v); }
  uint32_t saved_segment_start_address() const { return address + 13064; }
  uint32_t saved_wrap_cursor() const { return guest::LoadU32(base, address + 13068); }
  void set_saved_wrap_cursor(uint32_t v) const { guest::StoreU32(base, address + 13068, v); }
  uint32_t saved_wrap_cursor_address() const { return address + 13068; }
  uint32_t saved_data_cursor() const { return guest::LoadU32(base, address + 13072); }
  void set_saved_data_cursor(uint32_t v) const { guest::StoreU32(base, address + 13072, v); }
  uint32_t saved_data_cursor_address() const { return address + 13072; }
  uint32_t saved_generation() const { return guest::LoadU32(base, address + 13076); }
  void set_saved_generation(uint32_t v) const { guest::StoreU32(base, address + 13076, v); }
  uint32_t saved_generation_address() const { return address + 13076; }
  // called with the time waited
  uint32_t wait_callback() const { return guest::LoadU32(base, address + 13084); }
  void set_wait_callback(uint32_t v) const { guest::StoreU32(base, address + 13084, v); }
  uint32_t wait_callback_address() const { return address + 13084; }
  // where the inline draw's End puts the write position
  uint32_t inline_position() const { return guest::LoadU32(base, address + 13092); }
  void set_inline_position(uint32_t v) const { guest::StoreU32(base, address + 13092, v); }
  uint32_t inline_position_address() const { return address + 13092; }
  uint32_t inline_vertices() const { return guest::LoadU32(base, address + 13096); }
  void set_inline_vertices(uint32_t v) const { guest::StoreU32(base, address + 13096, v); }
  uint32_t inline_vertices_address() const { return address + 13096; }
  uint32_t inline_indices() const { return guest::LoadU32(base, address + 13100); }
  void set_inline_indices(uint32_t v) const { guest::StoreU32(base, address + 13100, v); }
  uint32_t inline_indices_address() const { return address + 13100; }
  uint32_t inline_vertex_words() const { return guest::LoadU32(base, address + 13104); }
  void set_inline_vertex_words(uint32_t v) const { guest::StoreU32(base, address + 13104, v); }
  uint32_t inline_vertex_words_address() const { return address + 13104; }
  uint32_t inline_index_words() const { return guest::LoadU32(base, address + 13108); }
  void set_inline_index_words(uint32_t v) const { guest::StoreU32(base, address + 13108, v); }
  uint32_t inline_index_words_address() const { return address + 13108; }
  // the vertex shader sub_822A8958 was given (read by nothing)
  uint32_t entry_vertex_shader() const { return guest::LoadU32(base, address + 13156); }
  void set_entry_vertex_shader(uint32_t v) const { guest::StoreU32(base, address + 13156, v); }
  uint32_t entry_vertex_shader_address() const { return address + 13156; }
  // a recording command buffer (never in play)
  template <typename T = CommandRecorder>
  T recorder() const { return T(base, guest::LoadU32(base, address + 13160)); }
  void set_recorder(const View& v) const { guest::StoreU32(base, address + 13160, v.address); }
  void set_recorder(uint32_t v) const { guest::StoreU32(base, address + 13160, v); }
  uint32_t recorder_address() const { return address + 13160; }
  // the fence list's block in the command buffer (sub_822AD790)
  template <typename T = DeviceDummySegment>
  T fence_block() const { return T(base, guest::LoadU32(base, address + 13164)); }
  void set_fence_block(const View& v) const { guest::StoreU32(base, address + 13164, v.address); }
  void set_fence_block(uint32_t v) const { guest::StoreU32(base, address + 13164, v); }
  uint32_t fence_block_address() const { return address + 13164; }
  // the fence list's cursor: 8-byte entries
  uint32_t fence_list() const { return guest::LoadU32(base, address + 13168); }
  void set_fence_list(uint32_t v) const { guest::StoreU32(base, address + 13168, v); }
  uint32_t fence_list_address() const { return address + 13168; }
  uint32_t fence_list_end() const { return guest::LoadU32(base, address + 13172); }
  void set_fence_list_end(uint32_t v) const { guest::StoreU32(base, address + 13172, v); }
  uint32_t fence_list_end_address() const { return address + 13172; }
  // the recorder's segment list cursor (Device::KickSegment)
  uint32_t kick_list() const { return guest::LoadU32(base, address + 13180); }
  void set_kick_list(uint32_t v) const { guest::StoreU32(base, address + 13180, v); }
  uint32_t kick_list_address() const { return address + 13180; }
  uint32_t kick_list_end() const { return guest::LoadU32(base, address + 13184); }
  void set_kick_list_end(uint32_t v) const { guest::StoreU32(base, address + 13184, v); }
  uint32_t kick_list_end_address() const { return address + 13184; }
  // a copy of SetUpDisplay's 124 bytes (+64 the front buffer format, +68 the display format, +96 the scaler's)
  uint32_t presentation_parameters() const { return guest::LoadU32(base, address + 13188); }
  void set_presentation_parameters(uint32_t v) const { guest::StoreU32(base, address + 13188, v); }
  uint32_t presentation_parameters_address() const { return address + 13188; }
  // render state 376 (read by sub_822A0718 before each Swap)
  uint32_t present_interval() const { return guest::LoadU32(base, address + 13240); }
  void set_present_interval(uint32_t v) const { guest::StoreU32(base, address + 13240, v); }
  uint32_t present_interval_address() const { return address + 13240; }
  // the parameters' +64
  uint32_t front_buffer_format() const { return guest::LoadU32(base, address + 13252); }
  void set_front_buffer_format(uint32_t v) const { guest::StoreU32(base, address + 13252, v); }
  uint32_t front_buffer_format_address() const { return address + 13252; }
  // the parameters' +68
  uint32_t display_format() const { return guest::LoadU32(base, address + 13256); }
  void set_display_format(uint32_t v) const { guest::StoreU32(base, address + 13256, v); }
  uint32_t display_format_address() const { return address + 13256; }
  // the parameters' +96 (Swap's r5 compare)
  uint8_t scaler_parameters() const { return guest::LoadU8(base, address + 13284); }
  void set_scaler_parameters(uint8_t v) const { guest::StoreU8(base, address + 13284, v); }
  uint32_t scaler_parameters_address() const { return address + 13284; }
  // 56 bytes (the first Swap)
  uint8_t scaler() const { return guest::LoadU8(base, address + 13312); }
  void set_scaler(uint8_t v) const { guest::StoreU8(base, address + 13312, v); }
  uint32_t scaler_address() const { return address + 13312; }
  // SetUpDisplay's depth surface
  uint32_t display_depth_surface() const { return guest::LoadU32(base, address + 13424); }
  void set_display_depth_surface(uint32_t v) const { guest::StoreU32(base, address + 13424, v); }
  uint32_t display_depth_surface_address() const { return address + 13424; }
  // SetUpDisplay's front buffer
  uint32_t front_buffer() const { return guest::LoadU32(base, address + 13428); }
  void set_front_buffer(uint32_t v) const { guest::StoreU32(base, address + 13428, v); }
  uint32_t front_buffer_address() const { return address + 13428; }
  // SetUpDisplay's render target
  uint32_t display_render_target() const { return guest::LoadU32(base, address + 13432); }
  void set_display_render_target(uint32_t v) const { guest::StoreU32(base, address + 13432, v); }
  uint32_t display_render_target_address() const { return address + 13432; }
  // the ring was allocated by Device::SetUpRing
  uint32_t ring_allocated() const { return guest::LoadU32(base, address + 13436); }
  void set_ring_allocated(uint32_t v) const { guest::StoreU32(base, address + 13436, v); }
  uint32_t ring_allocated_address() const { return address + 13436; }
  // the command buffer was allocated by Device::SetUpRing
  uint32_t command_buffer_allocated() const { return guest::LoadU32(base, address + 13440); }
  void set_command_buffer_allocated(uint32_t v) const { guest::StoreU32(base, address + 13440, v); }
  uint32_t command_buffer_allocated_address() const { return address + 13440; }
  // the front buffer's 52-byte header, copied by Swap
  uint32_t front_buffer_header() const { return guest::LoadU32(base, address + 13444); }
  void set_front_buffer_header(uint32_t v) const { guest::StoreU32(base, address + 13444, v); }
  uint32_t front_buffer_header_address() const { return address + 13444; }
  // the header's +32: an address (sub_822A0718)
  uint32_t front_buffer_header_32() const { return guest::LoadU32(base, address + 13476); }
  void set_front_buffer_header_32(uint32_t v) const { guest::StoreU32(base, address + 13476, v); }
  uint32_t front_buffer_header_32_address() const { return address + 13476; }
  // the primary ring
  uint32_t ring() const { return guest::LoadU32(base, address + 13496); }
  void set_ring(uint32_t v) const { guest::StoreU32(base, address + 13496, v); }
  uint32_t ring_address() const { return address + 13496; }
  uint32_t ring_mask() const { return guest::LoadU32(base, address + 13500); }
  void set_ring_mask(uint32_t v) const { guest::StoreU32(base, address + 13500, v); }
  uint32_t ring_mask_address() const { return address + 13500; }
  uint32_t command_buffer_start() const { return guest::LoadU32(base, address + 13504); }
  void set_command_buffer_start(uint32_t v) const { guest::StoreU32(base, address + 13504, v); }
  uint32_t command_buffer_start_address() const { return address + 13504; }
  uint32_t command_buffer_end() const { return guest::LoadU32(base, address + 13508); }
  void set_command_buffer_end(uint32_t v) const { guest::StoreU32(base, address + 13508, v); }
  uint32_t command_buffer_end_address() const { return address + 13508; }
  // a segment's size in bytes
  uint32_t segment_size() const { return guest::LoadU32(base, address + 13512); }
  void set_segment_size(uint32_t v) const { guest::StoreU32(base, address + 13512, v); }
  uint32_t segment_size_address() const { return address + 13512; }
  uint32_t data_cursor() const { return guest::LoadU32(base, address + 13516); }
  void set_data_cursor(uint32_t v) const { guest::StoreU32(base, address + 13516, v); }
  uint32_t data_cursor_address() const { return address + 13516; }
  uint32_t wrap_generation() const { return guest::LoadU32(base, address + 13520); }
  void set_wrap_generation(uint32_t v) const { guest::StoreU32(base, address + 13520, v); }
  uint32_t wrap_generation_address() const { return address + 13520; }
  uint32_t wrap_cursor() const { return guest::LoadU32(base, address + 13524); }
  void set_wrap_cursor(uint32_t v) const { guest::StoreU32(base, address + 13524, v); }
  uint32_t wrap_cursor_address() const { return address + 13524; }
  // the open segment's start
  uint32_t segment_start() const { return guest::LoadU32(base, address + 13528); }
  void set_segment_start(uint32_t v) const { guest::StoreU32(base, address + 13528, v); }
  uint32_t segment_start_address() const { return address + 13528; }
  // the data reserved for the open segment
  uint32_t segment_data() const { return guest::LoadU32(base, address + 13532); }
  void set_segment_data(uint32_t v) const { guest::StoreU32(base, address + 13532, v); }
  uint32_t segment_data_address() const { return address + 13532; }
  uint32_t segment_data_budget() const { return guest::LoadU32(base, address + 13536); }
  void set_segment_data_budget(uint32_t v) const { guest::StoreU32(base, address + 13536, v); }
  uint32_t segment_data_budget_address() const { return address + 13536; }
  // the last Swap's fence (+13544 with +20440 bit 0)
  uint32_t throttle_fence() const { return guest::LoadU32(base, address + 13540); }
  void set_throttle_fence(uint32_t v) const { guest::StoreU32(base, address + 13540, v); }
  uint32_t throttle_fence_address() const { return address + 13540; }
  // the one before
  uint32_t throttle_fence2() const { return guest::LoadU32(base, address + 13544); }
  void set_throttle_fence2(uint32_t v) const { guest::StoreU32(base, address + 13544, v); }
  uint32_t throttle_fence2_address() const { return address + 13544; }
  // a critical section (the device lock; a host mutex in the native code)
  uint32_t device_lock_address() const { return address + 13548; }
  // a critical section (Device::Init)
  uint32_t critical_section2_address() const { return address + 13576; }
  // the gamma ramp: three channels of 256 halfwords
  uint32_t gamma_ramp_address() const { return address + 13608; }
  // its first byte (Device::SetGammaRamp, sub_822A3AA8)
  uint8_t gamma_ramp_first() const { return guest::LoadU8(base, address + 13608); }
  void set_gamma_ramp_first(uint8_t v) const { guest::StoreU8(base, address + 13608, v); }
  uint32_t gamma_ramp_first_address() const { return address + 13608; }
  // called by the interrupt handler on vblanks
  uint32_t vblank_callback() const { return guest::LoadU32(base, address + 15144); }
  void set_vblank_callback(uint32_t v) const { guest::StoreU32(base, address + 15144, v); }
  uint32_t vblank_callback_address() const { return address + 15144; }
  uint32_t vblank_count() const { return guest::LoadU32(base, address + 15148); }
  void set_vblank_count(uint32_t v) const { guest::StoreU32(base, address + 15148, v); }
  uint32_t vblank_count_address() const { return address + 15148; }
  // the console's video flags (0x00040000)
  uint32_t video_flags() const { return guest::LoadU32(base, address + 15164); }
  void set_video_flags(uint32_t v) const { guest::StoreU32(base, address + 15164, v); }
  uint32_t video_flags_address() const { return address + 15164; }
  // the 4,800-byte segment writers get when out of space
  template <typename T = DeviceDummySegment>
  T dummy_segment() const { return T(base, guest::LoadU32(base, address + 15176)); }
  void set_dummy_segment(const View& v) const { guest::StoreU32(base, address + 15176, v.address); }
  void set_dummy_segment(uint32_t v) const { guest::StoreU32(base, address + 15176, v); }
  uint32_t dummy_segment_address() const { return address + 15176; }
  // a capture object (set: the hung GPU's progress not written)
  template <typename T = CommandTap>
  T tap() const { return T(base, guest::LoadU32(base, address + 19980)); }
  void set_tap(const View& v) const { guest::StoreU32(base, address + 19980, v.address); }
  void set_tap(uint32_t v) const { guest::StoreU32(base, address + 19980, v); }
  uint32_t tap_address() const { return address + 19980; }
  uint32_t display_width() const { return guest::LoadU32(base, address + 19988); }
  void set_display_width(uint32_t v) const { guest::StoreU32(base, address + 19988, v); }
  uint32_t display_width_address() const { return address + 19988; }
  uint32_t display_height() const { return guest::LoadU32(base, address + 19992); }
  void set_display_height(uint32_t v) const { guest::StoreU32(base, address + 19992, v); }
  uint32_t display_height_address() const { return address + 19992; }
  uint32_t display_pitch() const { return guest::LoadU32(base, address + 19996); }
  void set_display_pitch(uint32_t v) const { guest::StoreU32(base, address + 19996, v); }
  uint32_t display_pitch_address() const { return address + 19996; }
  // the display mode's format (0)
  uint32_t display_format_mode() const { return guest::LoadU32(base, address + 20000); }
  void set_display_format_mode(uint32_t v) const { guest::StoreU32(base, address + 20000, v); }
  uint32_t display_format_mode_address() const { return address + 20000; }
  // register 0x1844 (Swap)
  uint32_t front_buffer_register() const { return guest::LoadU32(base, address + 20004); }
  void set_front_buffer_register(uint32_t v) const { guest::StoreU32(base, address + 20004, v); }
  uint32_t front_buffer_register_address() const { return address + 20004; }
  // -1 at Device::Init and sub_822AD4A0
  uint32_t unknown_20008() const { return guest::LoadU32(base, address + 20008); }
  void set_unknown_20008(uint32_t v) const { guest::StoreU32(base, address + 20008, v); }
  uint32_t unknown_20008_address() const { return address + 20008; }
  // -1 at Device::Init and sub_822AD4A0
  uint32_t unknown_20012() const { return guest::LoadU32(base, address + 20012); }
  void set_unknown_20012(uint32_t v) const { guest::StoreU32(base, address + 20012, v); }
  uint32_t unknown_20012_address() const { return address + 20012; }
  // read by Device::Release
  uint32_t unknown_20020() const { return guest::LoadU32(base, address + 20020); }
  void set_unknown_20020(uint32_t v) const { guest::StoreU32(base, address + 20020, v); }
  uint32_t unknown_20020_address() const { return address + 20020; }
  // the counter's rate
  float timer_rate() const { return guest::LoadFloat(base, address + 20040); }
  void set_timer_rate(float v) const { guest::StoreFloat(base, address + 20040, v); }
  uint32_t timer_rate_address() const { return address + 20040; }
  // the counter's period (the waits' time scale)
  float timer_period() const { return guest::LoadFloat(base, address + 20044); }
  void set_timer_period(float v) const { guest::StoreFloat(base, address + 20044, v); }
  uint32_t timer_period_address() const { return address + 20044; }
  // read by Swap
  uint32_t unknown_20072() const { return guest::LoadU32(base, address + 20072); }
  void set_unknown_20072(uint32_t v) const { guest::StoreU32(base, address + 20072, v); }
  uint32_t unknown_20072_address() const { return address + 20072; }
  // read and written by Swap
  uint32_t unknown_20076() const { return guest::LoadU32(base, address + 20076); }
  void set_unknown_20076(uint32_t v) const { guest::StoreU32(base, address + 20076, v); }
  uint32_t unknown_20076_address() const { return address + 20076; }
  // the GPU timestamps on (the debugger's)
  uint32_t timestamps() const { return guest::LoadU32(base, address + 20080); }
  void set_timestamps(uint32_t v) const { guest::StoreU32(base, address + 20080, v); }
  uint32_t timestamps_address() const { return address + 20080; }
  // read by Device::Release
  uint32_t unknown_20084() const { return guest::LoadU32(base, address + 20084); }
  void set_unknown_20084(uint32_t v) const { guest::StoreU32(base, address + 20084, v); }
  uint32_t unknown_20084_address() const { return address + 20084; }
  // the GPU performance counters (sub_822B0F70)
  uint32_t performance_counters() const { return guest::LoadU32(base, address + 20100); }
  void set_performance_counters(uint32_t v) const { guest::StoreU32(base, address + 20100, v); }
  uint32_t performance_counters_address() const { return address + 20100; }
  // 0 off, 1 starting, 2 on
  uint32_t performance_counters_state() const { return guest::LoadU32(base, address + 20104); }
  void set_performance_counters_state(uint32_t v) const { guest::StoreU32(base, address + 20104, v); }
  uint32_t performance_counters_state_address() const { return address + 20104; }
  uint32_t performance_counters_8() const { return guest::LoadU32(base, address + 20108); }
  void set_performance_counters_8(uint32_t v) const { guest::StoreU32(base, address + 20108, v); }
  uint32_t performance_counters_8_address() const { return address + 20108; }
  uint32_t performance_counters_12() const { return guest::LoadU32(base, address + 20112); }
  void set_performance_counters_12(uint32_t v) const { guest::StoreU32(base, address + 20112, v); }
  uint32_t performance_counters_12_address() const { return address + 20112; }
  // Swap's 64 words (with +20432)
  uint32_t swap_words() const { return guest::LoadU32(base, address + 20120); }
  void set_swap_words(uint32_t v) const { guest::StoreU32(base, address + 20120, v); }
  uint32_t swap_words_address() const { return address + 20120; }
  // the debugger's callback
  uint32_t debugger_callback() const { return guest::LoadU32(base, address + 20124); }
  void set_debugger_callback(uint32_t v) const { guest::StoreU32(base, address + 20124, v); }
  uint32_t debugger_callback_address() const { return address + 20124; }
  // written by sub_822B43C0
  uint32_t unknown_20136() const { return guest::LoadU32(base, address + 20136); }
  void set_unknown_20136(uint32_t v) const { guest::StoreU32(base, address + 20136, v); }
  uint32_t unknown_20136_address() const { return address + 20136; }
  // with +20120
  uint32_t swap_words_end() const { return guest::LoadU32(base, address + 20432); }
  void set_swap_words_end(uint32_t v) const { guest::StoreU32(base, address + 20432, v); }
  uint32_t swap_words_end_address() const { return address + 20432; }
  // bit 0 picks +13544, bit 8 no workers, bits 24-29 the workers' CPUs
  uint32_t options() const { return guest::LoadU32(base, address + 20440); }
  void set_options(uint32_t v) const { guest::StoreU32(base, address + 20440, v); }
  uint32_t options_address() const { return address + 20440; }
};

// gpu; measure: Command
struct ReplayCommand : View {
  using View::View;
  static constexpr uint32_t kSize = 280;
  // the item's first word (a stream, a state, a type, a flag word: by command)
  uint32_t arg0() const { return guest::LoadU32(base, address + 0); }
  void set_arg0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t arg0_address() const { return address + 0; }
  // SetViewports' x, SetScissorRects' left
  uint16_t arg0_high() const { return guest::LoadU16(base, address + 0); }
  void set_arg0_high(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t arg0_high_address() const { return address + 0; }
  // SetViewports' y, SetScissorRects' top
  uint16_t arg0_low() const { return guest::LoadU16(base, address + 2); }
  void set_arg0_low(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t arg0_low_address() const { return address + 2; }
  // the second word (Clears' rectangles, Resolves' r4, SetStreamSources' buffer)
  uint32_t arg1() const { return guest::LoadU32(base, address + 4); }
  void set_arg1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t arg1_address() const { return address + 4; }
  // SetViewports' width, SetScissorRects' right, DrawVerticesUP's r5
  uint16_t arg1_high() const { return guest::LoadU16(base, address + 4); }
  void set_arg1_high(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t arg1_high_address() const { return address + 4; }
  // SetViewports' height, SetScissorRects' bottom, DrawVerticesUP's vertex count (r6)
  uint16_t arg1_low() const { return guest::LoadU16(base, address + 6); }
  void set_arg1_low(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t arg1_low_address() const { return address + 6; }
  // the third word, also a float (SetViewports' min Z); Clears' r6, Resolves' source rectangle
  uint32_t arg2() const { return guest::LoadU32(base, address + 8); }
  void set_arg2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t arg2_address() const { return address + 8; }
  // the fourth word, also a float (SetViewports' max Z); SetStreamSources' stride, Clears' r5
  uint32_t arg3() const { return guest::LoadU32(base, address + 12); }
  void set_arg3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t arg3_address() const { return address + 12; }
  // DrawIndexedVerticesUP's +16 (1: 16-bit indices)
  uint32_t arg4() const { return guest::LoadU32(base, address + 16); }
  void set_arg4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t arg4_address() const { return address + 16; }
  // DrawIndexedVerticesUP's +20
  uint32_t arg5() const { return guest::LoadU32(base, address + 20); }
  void set_arg5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t arg5_address() const { return address + 20; }
  // Resolves' destination texture, DrawIndexedVerticesUP's +24
  uint32_t arg6() const { return guest::LoadU32(base, address + 24); }
  void set_arg6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t arg6_address() const { return address + 24; }
  // Resolves' r7 (item + 28), DrawIndexedVerticesUP's +28
  uint32_t arg7() const { return guest::LoadU32(base, address + 28); }
  void set_arg7(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t arg7_address() const { return address + 28; }
  // Resolves' +36
  uint32_t arg9() const { return guest::LoadU32(base, address + 36); }
  void set_arg9(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t arg9_address() const { return address + 36; }
  // Resolves' +40
  uint32_t arg10() const { return guest::LoadU32(base, address + 40); }
  void set_arg10(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t arg10_address() const { return address + 40; }
  // Resolves' clear color: the bytes +44 to +47 as floats
  uint8_t clear_color_0() const { return guest::LoadU8(base, address + 44); }
  void set_clear_color_0(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t clear_color_0_address() const { return address + 44; }
  uint8_t clear_color_1() const { return guest::LoadU8(base, address + 45); }
  void set_clear_color_1(uint8_t v) const { guest::StoreU8(base, address + 45, v); }
  uint32_t clear_color_1_address() const { return address + 45; }
  uint8_t clear_color_2() const { return guest::LoadU8(base, address + 46); }
  void set_clear_color_2(uint8_t v) const { guest::StoreU8(base, address + 46, v); }
  uint32_t clear_color_2_address() const { return address + 46; }
  uint8_t clear_color_3() const { return guest::LoadU8(base, address + 47); }
  void set_clear_color_3(uint8_t v) const { guest::StoreU8(base, address + 47, v); }
  uint32_t clear_color_3_address() const { return address + 47; }
  // Resolves' +48
  float clear_z() const { return guest::LoadFloat(base, address + 48); }
  void set_clear_z(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t clear_z_address() const { return address + 48; }
  // Resolves' +52
  uint32_t clear_stencil() const { return guest::LoadU32(base, address + 52); }
  void set_clear_stencil(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t clear_stencil_address() const { return address + 52; }
  // Clears' r7 (probably the color)
  uint32_t clears_r7() const { return guest::LoadU32(base, address + 268); }
  void set_clears_r7(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t clears_r7_address() const { return address + 268; }
  // Clears' f1 (probably Z)
  float clears_z() const { return guest::LoadFloat(base, address + 272); }
  void set_clears_z(float v) const { guest::StoreFloat(base, address + 272, v); }
  uint32_t clears_z_address() const { return address + 272; }
  // Clears' r9 (probably the stencil)
  uint32_t clears_stencil() const { return guest::LoadU32(base, address + 276); }
  void set_clears_stencil(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t clears_stencil_address() const { return address + 276; }
};

// gpu; measure: sub_822AD790.ret
struct DeviceFenceEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // both words, as the originals store them
  uint64_t entry() const { return guest::LoadU64(base, address + 0); }
  void set_entry(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t entry_address() const { return address + 0; }
  // the resource's address / 4 in bits 0-29
  uint32_t resource() const { return guest::LoadU32(base, address + 0); }
  void set_resource(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t resource_address() const { return address + 0; }
  // -1
  uint32_t marker() const { return guest::LoadU32(base, address + 4); }
  void set_marker(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t marker_address() const { return address + 4; }
};

// allocated; measure: new<4800>@sub_822AD228
struct DeviceDummySegment : View {
  using View::View;
  static constexpr uint32_t kSize = 4800;
  // the fence block's first word (0 when taken)
  uint32_t header() const { return guest::LoadU32(base, address + 0); }
  void set_header(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t header_address() const { return address + 0; }
  // the fence block's second word: bit 31 kept, the rest the entries' count when the next block is taken
  uint32_t count() const { return guest::LoadU32(base, address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_address() const { return address + 4; }
};

// gpu; measure: Device.+12468, Device.+12472, Device.+12476, Device.+12480, Device.+12484, Device::SetDepthStencilSurface.r4, Device::SetRenderTarget.r5
struct DeviceSurface : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // RB_SURFACE_INFO (bits 16-17 the MSAA)
  uint32_t surface_info() const { return guest::LoadU32(base, address + 24); }
  void set_surface_info(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t surface_info_address() const { return address + 24; }
  // RB_COLOR_INFO (the color format in bits 16-19) or RB_DEPTH_INFO
  uint32_t format_info() const { return guest::LoadU32(base, address + 28); }
  void set_format_info(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t format_info_address() const { return address + 28; }
  // RB_HIZCONTROL for a depth surface
  uint32_t hiz() const { return guest::LoadU32(base, address + 32); }
  void set_hiz(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t hiz_address() const { return address + 32; }
  // width - 1 in bits 0-13, height - 1 in bits 15-29
  uint32_t size() const { return guest::LoadU32(base, address + 36); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t size_address() const { return address + 36; }
};

// gpu; measure: Device::SetTexture.r5
struct DeviceTexture : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // word 0 (the clamps, bits 10-21, are the stage's)
  uint32_t fetch0() const { return guest::LoadU32(base, address + 28); }
  void set_fetch0(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t fetch0_address() const { return address + 28; }
  // word 1: the base address
  uint32_t base_address() const { return guest::LoadU32(base, address + 32); }
  void set_base_address(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t base_address_address() const { return address + 32; }
  // word 2: the size
  uint32_t fetch2() const { return guest::LoadU32(base, address + 36); }
  void set_fetch2(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t fetch2_address() const { return address + 36; }
  // word 3 (the filters, bits 19-30, are the stage's)
  uint32_t fetch3() const { return guest::LoadU32(base, address + 40); }
  void set_fetch3(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t fetch3_address() const { return address + 40; }
  // word 4: the max mip level in bits 2-5, the min in 6-9
  uint32_t mip_levels() const { return guest::LoadU32(base, address + 44); }
  void set_mip_levels(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t mip_levels_address() const { return address + 44; }
  // word 5: the mip address
  uint32_t mip_address() const { return guest::LoadU32(base, address + 48); }
  void set_mip_address(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t mip_address_address() const { return address + 48; }
};

// gpu; measure: Device::SetStreamSource.r5
struct DeviceVertexBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // 1 vertex, 2 index; the resource flags the fence mask meets
  uint32_t type() const { return guest::LoadU32(base, address + 0); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  uint32_t reference_count() const { return guest::LoadU32(base, address + 4); }
  void set_reference_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reference_count_address() const { return address + 4; }
  // the command position when last replaced
  uint32_t fence() const { return guest::LoadU32(base, address + 8); }
  void set_fence(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fence_address() const { return address + 8; }
  // the data's address (a vertex buffer's with its low 2 bits 3)
  uint32_t data() const { return guest::LoadU32(base, address + 24); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t data_address() const { return address + 24; }
  uint32_t data_size() const { return guest::LoadU32(base, address + 28); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t data_size_address() const { return address + 28; }
};

// gpu; measure: Device::SetViewport.r4
struct DeviceViewport : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t x() const { return guest::LoadU32(base, address + 0); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  uint32_t y() const { return guest::LoadU32(base, address + 4); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  uint32_t width() const { return guest::LoadU32(base, address + 8); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t width_address() const { return address + 8; }
  uint32_t height() const { return guest::LoadU32(base, address + 12); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t height_address() const { return address + 12; }
  float min_z() const { return guest::LoadFloat(base, address + 16); }
  void set_min_z(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t min_z_address() const { return address + 16; }
  uint32_t min_z_bits() const { return guest::LoadU32(base, address + 16); }
  void set_min_z_bits(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t min_z_bits_address() const { return address + 16; }
  float max_z() const { return guest::LoadFloat(base, address + 20); }
  void set_max_z(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t max_z_address() const { return address + 20; }
  uint32_t max_z_bits() const { return guest::LoadU32(base, address + 20); }
  void set_max_z_bits(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t max_z_bits_address() const { return address + 20; }
};

// gpu; measure: ComputeGammaRamp.r3, ComputeGammaRamp.r4
struct DeviceGammaRamp : View {
  using View::View;
  static constexpr uint32_t kSize = 1536;
  uint32_t red_address() const { return address + 0; }
  uint32_t green_address() const { return address + 512; }
  uint32_t blue_address() const { return address + 1024; }
};

// gpu; measure: Device::SetPixelShaderConstantF.ret
struct DeviceBoolConstant : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // bit 0 the bool
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// The image words docs/functions/device-state.md's code uses.
struct DeviceStateGlobals {
  // 96 entries of 12 bytes: the getter, the setter, the default value (by state / 4)
  static constexpr uint32_t render_state_table_address() { return 0x826388F8u; }
  // 20 entries of 12 bytes, as the render states'
  static constexpr uint32_t sampler_state_table_address() { return 0x82638D78u; }
  // 0, 0, 0x7FFFFFFF, 0x7FFFFFFF, Z 0.0 to 1.0 (SetSurfaceInfo)
  static constexpr uint32_t full_viewport_address() { return 0x82013600u; }
  // 0, 0, 0x7FFFFFFF, 0x7FFFFFFF (SetSurfaceInfo)
  static constexpr uint32_t full_scissor_address() { return 0x82013618u; }
  // max anisotropy -> the filter's bits: 0, 0, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5
  static constexpr uint32_t anisotropy_table_address() { return 0x82013628u; }
  // FVF texture coordinate formats: a byte a format, then the element types (VertexDeclaration::FromFvf)
  static constexpr uint32_t tex_coord_table_address() { return 0x82013690u; }
  // the colors' bytes to floats
  static float one_over_255(uint8_t* base) { return guest::LoadFloat(base, 0x82002200u); }
  static void set_one_over_255(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002200u, v); }
  static constexpr uint32_t one_over_255_address() { return 0x82002200u; }
  // the point size to 12.4 (x 8)
  static float times_8(uint8_t* base) { return guest::LoadFloat(base, 0x82000C34u); }
  static void set_times_8(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000C34u, v); }
  static constexpr uint32_t times_8_address() { return 0x82000C34u; }
  // point sizes and the depth slope (x 16)
  static float times_16(uint8_t* base) { return guest::LoadFloat(base, 0x820021ECu); }
  static void set_times_16(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021ECu, v); }
  static constexpr uint32_t times_16_address() { return 0x820021ECu; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 0.5 (the viewport's scales)
  static float one_half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_one_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t one_half_address() { return 0x82001BE4u; }
  // the LOD bias to fixed point
  static float mip_map_lod_bias_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820029CCu); }
  static void set_mip_map_lod_bias_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029CCu, v); }
  static constexpr uint32_t mip_map_lod_bias_scale_address() { return 0x820029CCu; }
  // the anisotropy bias to fixed point
  static float anisotropy_bias_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82000F18u); }
  static void set_anisotropy_bias_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000F18u, v); }
  static constexpr uint32_t anisotropy_bias_scale_address() { return 0x82000F18u; }
};

}  // namespace schema
