VersionShim
===========
A Dynamic-Link Library which exports the same methods as version.dll (winver.h) in addition to chainloading additional user defined libraries.

Usage
-----
Rename VersionShim.dll to `version.dll` and place it in the target application's search location,
then create a file named `Libraries.txt` in the same directory and list the paths to all the libraries you'd like to load,
each on a new line. Lines beginning with `#` will be skipped.

Libraries.txt *MUST* use CRLF line endings and be encoded in UTF-8 with *no* BOM.

License
-------
This project is subject to the terms of the [Mozilla Public License, v. 2.0](./LICENSE).