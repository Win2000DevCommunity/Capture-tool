#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>

class MainWindow {
public:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void RegisterAndCreateWindow(HINSTANCE hInstance, int nCmdShow);
};

#endif // MAINWINDOW_H

