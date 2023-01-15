#pragma once
#include "pch.h"

BOOL Error(LPWSTR message);
BOOL OpenRead(LPCSTR lpFileName, HANDLE* fileHandle);
BOOL ReadUtf8File(HANDLE fileHandle, LPCH* fileBuffer, DWORD* fileLength);
BOOL Utf8ToUtf16(LPCCH utf8String, PZPWSTR utf16String, int length);
int NextLine(LPWSTR text);