// Copyright © 2023 Xpl0itR
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "pch.h"

BOOL OpenFileRead(const LPCSTR lpFileName, HANDLE* fileHandle)
{
    OFSTRUCT ofStruct = { 0 };
    HFILE hFile = OpenFile(lpFileName, &ofStruct, OF_READ | OF_PROMPT);

    *fileHandle = (HANDLE)(INT_PTR)hFile;
    return hFile != HFILE_ERROR;
}

BOOL ReadFileUtf8(const HANDLE fileHandle, LPCH* fileString, DWORD* fileLength)
{
    DWORD fileLen = GetFileSize(fileHandle, NULL);
    *fileLength = fileLen;

    HANDLE heap = GetProcessHeap();
    LPCH buffer = HeapAlloc(heap, 0, fileLen + 1);
    if (!buffer)
        return FALSE;

    if (!ReadFile(fileHandle, buffer, fileLen, NULL, NULL))
    {
        HeapFree(heap, 0, buffer);
        return FALSE;
    }

    buffer[fileLen] = '\0';
    *fileString = buffer;

    return TRUE;
}

BOOL OpenReadFileUtf8(const LPCSTR lpFileName, LPCH* fileString, DWORD* fileLength)
{
    HANDLE hFile = NULL;
    if (!OpenFileRead(lpFileName, &hFile))
        return FALSE;

    BOOL success = ReadFileUtf8(hFile, fileString, fileLength);
    CloseHandle(hFile);

    return success;
}

INT TerminateLineCrlf(const LPCH fileString)
{
    for (INT i = 0; ; i++)
    {
        if (fileString[i]     == '\r' &&
            fileString[i + 1] == '\n')
        {
            fileString[i] = fileString[i + 1] = '\0';
        }

        if (fileString[i] == '\0')
        {
            return i;
        }
    }
}