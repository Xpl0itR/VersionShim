# Copyright © 2025 Xpl0itR
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import sys
import pefile
from pathlib import Path

is_nativeaot = sys.argv[1].lower() == "nativeaot"
dll_path = Path(sys.argv[2] if is_nativeaot else sys.argv[1])
exports = pefile.PE(dll_path).DIRECTORY_ENTRY_EXPORT.symbols

if is_nativeaot:
    for export in exports:
        export_name = export.name.decode()
        print(f'<LinkerArg Include="/EXPORT:{export_name}={dll_path.parent/dll_path.stem}.{export_name}" />')
else:
    is_system_dll = dll_path.parent.samefile("C:\Windows\System32")
    (macro_name, dll_name) = ("FORWARD_SYSTEM32", dll_path.stem) if is_system_dll else ("FORWARD", str(dll_path.parent/dll_path.stem).replace("\\", "\\\\"))
    for export in exports:
        print(f'{macro_name}("{dll_name}", "{export.name.decode()}")')