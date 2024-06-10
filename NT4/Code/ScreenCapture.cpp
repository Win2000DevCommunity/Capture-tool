#include "ScreenCapture.h"

HBITMAP ScreenCapture::CaptureScreen(int &width, int &height) {
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return hBitmap;
}

HBITMAP ScreenCapture::CaptureRegion(RECT* rect, int &width, int &height) {
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    width = rect->right - rect->left;
    height = rect->bottom - rect->top;
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, rect->left, rect->top, SRCCOPY);

    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return hBitmap;
}
HBITMAP ScreenCapture::CaptureSelectedWindow(int &width, int &height) {
    HWND hwndSelected = GetForegroundWindow();  // Get the handle of the selected window
    if (!hwndSelected) {
        return NULL;
    }

    HDC hWindowDC = GetWindowDC(hwndSelected);
    HDC hMemoryDC = CreateCompatibleDC(hWindowDC);

    RECT windowRect;
    GetWindowRect(hwndSelected, &windowRect);

    width = windowRect.right - windowRect.left;
    height = windowRect.bottom - windowRect.top;
    HBITMAP hBitmap = CreateCompatibleBitmap(hWindowDC, width, height);

    SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, width, height, hWindowDC, 0, 0, SRCCOPY);

    DeleteDC(hMemoryDC);
    ReleaseDC(hwndSelected, hWindowDC);

    return hBitmap;
}

