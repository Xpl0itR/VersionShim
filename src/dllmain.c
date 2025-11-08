// Copyright © 2023-2025 Xpl0itR
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// ReSharper disable once CppUnusedIncludeDirective
#include "exports.h"
#include "util.h"

VOID WINAPI SafeDllMain(_In_ ULONG_PTR _)
{
    HANDLE hHeap   = GetProcessHeap();
    LPSTR  fileStr = NULL;
    DWORD  fileLen = 0;

    if (!OpenReadFileUtf8("libraries.txt", hHeap, &fileStr, &fileLen))
    {
        MessageBoxA(NULL, "Failed to read libraries.txt", PROJECT_NAME, ErrBoxType);
        return;
    }

    CHAR callerPath[MAX_PATH + 1];
    if (!GetModuleFileNameA(NULL, callerPath, MAX_PATH + 1))
    {
        MessageBoxA(NULL, "Failed to read caller path", PROJECT_NAME, ErrBoxType);
        return;
    }

    int read, hasTarget = FALSE, targetMatched = FALSE;
    for (LPSTR line = SkipBomUtf8(fileStr), fileEnd = fileStr + fileLen; line < fileEnd; line += read)
    {
        read = TerminateLine(line);

        if (*line == '#')
            continue;

        if (*line == '*')
        {
            if (!targetMatched)
            {
                hasTarget = TRUE;

                if (StrEndsWith(callerPath, line + 1))
                {
                    targetMatched = TRUE;
                }
            }

            continue;
        }

        if (hasTarget && !targetMatched)
            return;

        if (!LoadLibraryA(line))
        {
            MessageBoxA(NULL, line, PROJECT_NAME" - Failed to load library", ErrBoxType);
        }
    }

    HeapFree(hHeap, 0, fileStr);
}

BOOL APIENTRY DllMain(_In_ HMODULE hModule, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
    DisableThreadLibraryCalls(hModule);

    if (fdwReason == DLL_PROCESS_ATTACH)
        QueueUserAPC(SafeDllMain, GetCurrentThread(), 0);

    return TRUE;
}