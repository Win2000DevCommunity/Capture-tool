#include "OverlayWindow.h"
#include "BitmapHandler.h"
#include "ScreenCapture.h"
#include <algorithm>
extern HBITMAP hBitmap;
extern HWND hWndMain;
extern RECT captureRect;
extern POINT ptStart, ptEnd;
extern bool isCapturing;
extern int capturedWidth, capturedHeight;

LRESULT CALLBACK OverlayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message) {
    case WM_LBUTTONDOWN:
        ptStart.x = LOWORD(lParam);
        ptStart.y = HIWORD(lParam);
        ptEnd = ptStart;
        break;
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) {
            hdc = GetDC(hWnd);
            SetROP2(hdc, R2_NOTXORPEN);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);  // Erase previous rectangle
            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);
            Rectangle(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);  // Draw new rectangle
            ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        isCapturing = false;
        captureRect = { std::min(ptStart.x, ptEnd.x), std::min(ptStart.y, ptEnd.y), std::max(ptStart.x, ptEnd.x), std::max(ptStart.y, ptEnd.y) };
        if (hBitmap) {
            DeleteObject(hBitmap);
            hBitmap = NULL;
        }
        hBitmap = ScreenCapture::CaptureRegion(&captureRect, capturedWidth, capturedHeight);
        if (hBitmap == NULL) {
            MessageBox(hWndMain, "Failed to capture region.", "Capture Error", MB_ICONERROR);
        }
        InvalidateRect(hWndMain, NULL, TRUE);
        ShowWindow(hWndMain, SW_SHOW);  // Show the main window again
        DestroyWindow(hWnd);  // Close the overlay window
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            ReleaseCapture();
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            isCapturing = false;
            ShowWindow(hWndMain, SW_SHOW);  // Show the main window again
            InvalidateRect(hWndMain, NULL, TRUE);  // Clear the selection rectangle
            DestroyWindow(hWnd);  // Close the overlay window
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        if (hBitmap) {
         //   BitmapHandler::DisplayBitmap(hdc, &ps.rcPaint, hBitmap, capturedWidth, capturedHeight);
        }
        EndPaint(hWnd, &ps);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OverlayWindow::ShowOverlayWindow(HINSTANCE hInstance, HWND hWndMain) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = OverlayWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "OverlayClass";
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hOverlayWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED, "OverlayClass", NULL,
        WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, wc.hInstance, NULL);

    SetLayeredWindowAttributes(hOverlayWnd, 0, (255 * 50) / 100, LWA_ALPHA);
    ShowWindow(hOverlayWnd, SW_SHOW);
    UpdateWindow(hOverlayWnd);

    // Set up message loop to capture ESC key to cancel operation
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) {
            PostMessage(hOverlayWnd, WM_KEYDOWN, VK_ESCAPE, 0);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

