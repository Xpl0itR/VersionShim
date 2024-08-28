// Copyright © 2023 Xpl0itR
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// ReSharper disable once CppUnusedIncludeDirective
#include "exports.h"
#include "util.h"

BOOL APIENTRY DllMain(CONST HMODULE hModule, CONST DWORD fdwReason, CONST LPVOID lpvReserved)
{
    if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

    LPSTR fileStr = NULL;
    DWORD fileLen = 0;

    if (!OpenReadFileUtf8("libraries.txt", &fileStr, &fileLen))
    {
        MessageBoxA(NULL, "Failed to read libraries.txt", PROJECT_NAME, ErrBoxType);
        return TRUE;
    }

    LPSTR callerPath[MAX_PATH + 1];
    if (!GetModuleFileNameA(NULL, callerPath, MAX_PATH + 1))  // NOLINT(clang-diagnostic-incompatible-pointer-types)
    {
        MessageBoxA(NULL, "Failed to read caller path", PROJECT_NAME, ErrBoxType);
        return TRUE;
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

                if (StrEndsWith(callerPath, line + 1))  // NOLINT(clang-diagnostic-incompatible-pointer-types)
                {
                    targetMatched = TRUE;
                }
            }

            continue;
        }

        if (hasTarget && !targetMatched)
            return TRUE;

        // ReSharper disable once CppPointerToIntegralConversion
        if (!LoadLibraryA(line))
        {
            MessageBoxA(NULL, line, PROJECT_NAME" - Failed to load library", ErrBoxType);
        }
    }

    HeapFree(GetProcessHeap(), 0, fileStr);
    return TRUE;
}