// Copyright © 2023-2025 Xpl0itR
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

CONST UINT ErrBoxType = MB_ICONERROR | MB_TOPMOST;

inline BOOL OpenFileRead(_In_ LPCSTR fileName, _Outptr_ LPHANDLE lphFile)
{
    OFSTRUCT ofStruct;
    HFILE hFile = OpenFile(fileName, &ofStruct, OF_READ | OF_PROMPT);

    *lphFile = (HANDLE)(INT_PTR)hFile;  // NOLINT(performance-no-int-to-ptr)
    return hFile != HFILE_ERROR;
}

inline BOOL ReadFileUtf8(_In_ HANDLE hFile, _In_ HANDLE hHeap, _Outptr_ LPSTR* lpFileString, _Out_ LPDWORD lpFileLength)
{
    DWORD fileLen = GetFileSize(hFile, NULL);
    *lpFileLength = fileLen;

    LPSTR buffer = HeapAlloc(hHeap, 0, fileLen + 1);
    if (!buffer)
        return FALSE;

    if (!ReadFile(hFile, buffer, fileLen, NULL, NULL))
    {
        HeapFree(hHeap, 0, buffer);
        return FALSE;
    }

    buffer[fileLen] = '\0';
    *lpFileString = buffer;

    return TRUE;
}

inline BOOL OpenReadFileUtf8(_In_ LPCSTR fileName, _In_ HANDLE hHeap, _Outptr_ LPSTR* lpFileString, _Out_ LPDWORD lpFileLength)
{
    HANDLE hFile = NULL;
    if (!OpenFileRead(fileName, &hFile))
        return FALSE;

    BOOL success = ReadFileUtf8(hFile, hHeap, lpFileString, lpFileLength);
    CloseHandle(hFile);

    return success;
}

inline BOOL StrEndsWith(_In_ LPSTR str, _In_ LPSTR target)
{
    DWORD strLen = lstrlenA(str), targetLen = lstrlenA(target);

    if (strLen < targetLen)
        return FALSE;

    return lstrcmpA(str + strLen - targetLen, target) == 0;
}

inline LPSTR SkipBomUtf8(_In_ LPSTR str)
{
    if (str[0] == (char)0xEF
     && str[1] == (char)0xBB
     && str[2] == (char)0xBF)
    {
        return str + 3;
    }

    return str;
}

inline INT TerminateLine(_Inout_ LPSTR str)
{
    for (INT i = 0;; i++)
    {
        if (str[i] == '\0')
            return i + 1;

        if (str[i] == '\r' || str[i] == '\n')
        {
            str[i] = '\0';

            if (str[i + 1] == '\n')
            {
                str[++i] = '\0';
            }

            return i + 1;
        }
    }
}