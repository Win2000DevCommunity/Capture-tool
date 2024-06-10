#ifndef OVERLAYWINDOW_H
#define OVERLAYWINDOW_H

#include <windows.h>

class OverlayWindow {
public:
    static void ShowOverlayWindow(HINSTANCE hInstance, HWND hWndMain);
    static void resetCaptureRect();
};

#endif // OVERLAYWINDOW_H
