#!/usr/bin/env python3
"""Fetch the Visual C++ runtime DLLs the Windows build ships next to itc.exe.

xwin splats the CRT's libraries but not its redistributable DLLs
(MSVCP140.dll, VCRUNTIME140.dll, ...), which Windows doesn't come with.
This reads the same Visual Studio manifest xwin does, takes the redist
package of the MSVC version xwin splatted (so the DLLs match the libraries
we link against), checks its SHA-256 and copies out the DLLs asked for.
Microsoft's license allows shipping them with the program.

Usage: fetch_vc_redist.py <xwin output dir> <output dir> <DLL name>...
"""

import hashlib
import io
import json
import os
import sys
import urllib.request
import zipfile

CHANNEL = 'https://aka.ms/vs/17/release/channel'


def fetch(url):
    with urllib.request.urlopen(url) as response:
        return response.read()


def msvc_version(xwin):
    """The CRT version xwin splatted: VC/Tools/MSVC/<version>."""
    versions = os.listdir(os.path.join(xwin, 'VC', 'Tools', 'MSVC'))
    if len(versions) != 1:
        sys.exit(f'expected one MSVC version in {xwin}, found {versions}')
    return versions[0]


def main():
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    xwin, out, wanted = sys.argv[1], sys.argv[2], sys.argv[3:]

    version = msvc_version(xwin)
    package_id = f'Microsoft.VC.{version}.CRT.Redist.X64.base'.lower()

    channel = json.loads(fetch(CHANNEL))
    manifest_item = next(i for i in channel['channelItems']
                         if i['id'] == 'Microsoft.VisualStudio.Manifests.VisualStudio')
    manifest = json.loads(fetch(manifest_item['payloads'][0]['url']))
    package = next((p for p in manifest['packages'] if p['id'].lower() == package_id), None)
    if package is None:
        sys.exit(f'no package {package_id} in the Visual Studio manifest')
    payload = package['payloads'][0]

    data = fetch(payload['url'])
    if hashlib.sha256(data).hexdigest().lower() != payload['sha256'].lower():
        sys.exit(f'{payload["fileName"]}: SHA-256 mismatch')

    # The DLLs are at Contents/VC/Redist/MSVC/<version>/x64/Microsoft.VC143.CRT/.
    dlls = {}
    with zipfile.ZipFile(io.BytesIO(data)) as vsix:
        for name in vsix.namelist():
            parts = name.split('/')
            if len(parts) > 1 and parts[-2].lower().endswith('.crt') and \
                    parts[-1].lower().endswith('.dll'):
                dlls[parts[-1].lower()] = (parts[-1], vsix.read(name))

    missing = [w for w in wanted if w.lower() not in dlls]
    if missing:
        sys.exit(f'not in {payload["fileName"]}: {", ".join(missing)} '
                 f'(it has {", ".join(sorted(n for n, _ in dlls.values()))})')
    os.makedirs(out, exist_ok=True)
    for w in wanted:
        name, contents = dlls[w.lower()]
        with open(os.path.join(out, name), 'wb') as f:
            f.write(contents)
        print(f'{name} ({package["version"]})')


if __name__ == '__main__':
    main()
