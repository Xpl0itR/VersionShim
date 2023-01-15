// Copyright © 2023 Xpl0itR
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "pch.h"
#include "util.h"

#pragma region Forward functions to version.dll
#pragma comment(linker, "/EXPORT:GetFileVersionInfoA=C:\\Windows\\System32\\version.GetFileVersionInfoA,@1")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoByHandle=C:\\Windows\\System32\\version.GetFileVersionInfoByHandle,@2")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExA=C:\\Windows\\System32\\version.GetFileVersionInfoExA,@3")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExW=C:\\Windows\\System32\\version.GetFileVersionInfoExW,@4")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeA=C:\\Windows\\System32\\version.GetFileVersionInfoSizeA,@5")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExA=C:\\Windows\\System32\\version.GetFileVersionInfoSizeExA,@6")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExW=C:\\Windows\\System32\\version.GetFileVersionInfoSizeExW,@7")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeW=C:\\Windows\\System32\\version.GetFileVersionInfoSizeW,@8")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoW=C:\\Windows\\System32\\version.GetFileVersionInfoW,@9")
#pragma comment(linker, "/EXPORT:VerFindFileA=C:\\Windows\\System32\\version.VerFindFileA,@10")
#pragma comment(linker, "/EXPORT:VerFindFileW=C:\\Windows\\System32\\version.VerFindFileW,@11")
#pragma comment(linker, "/EXPORT:VerInstallFileA=C:\\Windows\\System32\\version.VerInstallFileA,@12")
#pragma comment(linker, "/EXPORT:VerInstallFileW=C:\\Windows\\System32\\version.VerInstallFileW,@13")
#pragma comment(linker, "/EXPORT:VerLanguageNameA=C:\\Windows\\System32\\version.VerLanguageNameA,@14")
#pragma comment(linker, "/EXPORT:VerLanguageNameW=C:\\Windows\\System32\\version.VerLanguageNameW,@15")
#pragma comment(linker, "/EXPORT:VerQueryValueA=C:\\Windows\\System32\\version.VerQueryValueA,@16")
#pragma comment(linker, "/EXPORT:VerQueryValueW=C:\\Windows\\System32\\version.VerQueryValueW,@17")
#pragma endregion

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD fdwReason, LPCVOID lpReserved)
{
    if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

    HANDLE hFile = NULL;
    if (!OpenRead("Libraries.txt", &hFile))
        return FALSE;

    LPCH  fileBuf = NULL;
    DWORD fileLen = 0;
    BOOL  success = ReadUtf8File(hFile, &fileBuf, &fileLen);
    CloseHandle(hFile);
    if (!success)
        return Error(L"Failed to read Libraries.txt");
    if (fileLen > INT_MAX)
        return Error(L"Libraries.txt was too large to read");

    LPWSTR fileStr = NULL;
    success = Utf8ToUtf16(fileBuf, &fileStr, (int)fileLen);
    free(fileBuf);
    if (!success)
        return Error(L"Failed to read Libraries.txt as UTF-16");

    LPWSTR end = fileStr + fileLen;
    for (LPWSTR dllPath = fileStr; dllPath < end;)
    {
        int numCharInVal = NextLine(dllPath);

        LoadLibrary(dllPath);

        dllPath += numCharInVal + 2;
    }

    free(fileStr);
    return TRUE;
}