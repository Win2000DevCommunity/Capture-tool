#include "BitmapHandler.h"
#include <commdlg.h>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>

bool BitmapHandler::SaveBitmapFile(HBITMAP hBitmap, const char* filename) {
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), bmp.bmWidth, bmp.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
    DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);

    HDC hdc = CreateCompatibleDC(NULL);
    GetDIBits(hdc, hBitmap, 0, bmp.bmHeight, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    BITMAPFILEHEADER bfh = { 0x4D42, dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), 0, 0, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) };
    HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    DWORD dwBytesWritten = 0;
    WriteFile(hFile, &bfh, sizeof(bfh), &dwBytesWritten, NULL);
    WriteFile(hFile, &bi, sizeof(bi), &dwBytesWritten, NULL);
    WriteFile(hFile, lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    CloseHandle(hFile);
    DeleteDC(hdc);

    return true;
}

void BitmapHandler::OpenWithPaint(const char* filename) {
    ShellExecute(NULL, "open", "mspaint.exe", filename, NULL, SW_SHOW);
}
