# Copyright © 2025 Xpl0itR
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import sys
import pefile
from pathlib import Path

dll_path = Path(sys.argv[1]).resolve()
(macro_name, dll_name) = ("FORWARD_SYSTEM32", dll_path.stem) if dll_path.parent.samefile("C:\Windows\System32") else ("FORWARD", str(dll_path).replace("\\", "\\\\"))

for export in pefile.PE(dll_path).DIRECTORY_ENTRY_EXPORT.symbols:
    print(f'{macro_name}("{dll_name}", "{export.name.decode()}")')