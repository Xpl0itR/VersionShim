// Copyright © 2023 Xpl0itR
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "pch.h"

BOOL Error(const LPWSTR message)
{
    MessageBox(NULL, message, L"VersionShim", MB_ICONHAND | MB_OK);
    return FALSE;
}

BOOL OpenRead(LPCSTR lpFileName, HANDLE* fileHandle)
{
    OFSTRUCT ofStruct = { 0 };
    HFILE hFile = OpenFile(lpFileName, &ofStruct, OF_READ | OF_PROMPT);

    *fileHandle = (HANDLE)hFile;
    return hFile != HFILE_ERROR;
}

BOOL ReadUtf8File(const HANDLE fileHandle, LPCH* fileBuffer, DWORD* fileLength)
{
    const DWORD fileLen = GetFileSize(fileHandle, NULL);
    *fileLength = fileLen;

    LPCH buffer = malloc(fileLen + 1);
    if (!buffer)
        return FALSE;

    if (!ReadFile(fileHandle, buffer, fileLen, NULL, NULL))
    {
        free(buffer);
        return FALSE;
    }

    buffer[fileLen] = '\0';
    *fileBuffer = buffer;

    return TRUE;
}

BOOL Utf8ToUtf16(LPCCH utf8String, const PZPWSTR utf16String, const int length)
{
    LPWSTR string = malloc(length * 2 + 2);
    if (!string)
        return FALSE;

    if (!MultiByteToWideChar(CP_UTF8, 0, utf8String, length, string, length))
    {
        free(string);
        return FALSE;
    }

    string[length] = L'\0';
    *utf16String = string;

    return TRUE;
}

int NextLine(const LPWSTR text)
{
    for (int i = 0; ; i++)
    {
        if (text[i]     == L'\r' &&
            text[i + 1] == L'\n')
        {
            text[i] = text[i + 1] = L'\0';
        }

        if (text[i] == L'\0')
        {
            return i;
        }
    }
}