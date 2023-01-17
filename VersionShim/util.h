#pragma once

UINT ErrBoxType = MB_ICONERROR | MB_TOPMOST;

BOOL OpenReadFileUtf8(LPCSTR lpFileName, LPCH* fileBuffer, DWORD* fileLength);
INT TerminateLineCrlf(LPCH fileString);