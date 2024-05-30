#include "MainWindow.h"
#include "ScreenCapture.h"
#include "BitmapHandler.h"
#include "OverlayWindow.h"
#include <commdlg.h>
#include <algorithm>
#include "resource.h"
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

HBITMAP hBitmap = NULL;
HWND hWndButtonCapture, hWndButtonSave, hWndButtonOpen, hWndComboBox, hWndMain;
RECT captureRect;
POINT ptStart, ptEnd;
bool isCapturing = false;
int capturedWidth = 0, capturedHeight = 0;
HBRUSH hBrushWhite, hBrushMainWindow;

void MainWindow::RegisterAndCreateWindow(HINSTANCE hInstance, int nCmdShow) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = MainWindow::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ScreenCaptureClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Set the main window background color
    RegisterClass(&wc);

    hWndMain = CreateWindow(wc.lpszClassName, "Screen Capture Tool",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 340, 100, NULL, NULL, hInstance, NULL);

    // Create white brush for static controls
    hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
    // Create brush for the main window background
    hBrushMainWindow = CreateSolidBrush(RGB(255, 255, 255));

    // Create static text for labels
    HWND hWndStatic1 = CreateWindow("STATIC", "Select Mode:", WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, 10, 10, 150, 20, hWndMain, NULL, hInstance, NULL);
    HWND hWndStatic2 = CreateWindow("STATIC", "Capture", WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, 170, 10, 50, 20, hWndMain, NULL, hInstance, NULL);
    HWND hWndStatic3 = CreateWindow("STATIC", "Save", WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, 225, 10, 35, 20, hWndMain, NULL, hInstance, NULL);
    HWND hWndStatic4 = CreateWindow("STATIC", "Open", WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, 280, 10, 35, 20, hWndMain, NULL, hInstance, NULL);

    // Create buttons with BS_ICON style
    hWndButtonCapture = CreateWindow("BUTTON", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON, 170, 30, 30, 30, hWndMain, (HMENU)1, hInstance, NULL);
    hWndButtonSave = CreateWindow("BUTTON", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON, 225, 30, 30, 30, hWndMain, (HMENU)2, hInstance, NULL);
    hWndButtonOpen = CreateWindow("BUTTON", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON, 280, 30, 30, 30, hWndMain, (HMENU)3, hInstance, NULL);

    hWndComboBox = CreateWindow("COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 10, 30, 150, 300, hWndMain, NULL, hInstance, NULL);
    SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)"Full Screen");
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)"Select Region");
	SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)"Selected Window");
	SendMessage(hWndComboBox, CB_SETCURSEL, 0, 0);

    // Load icons and set them to the buttons
    HICON hIconCapture = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_CAPTURE));
    HICON hIconSave = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SAVE));
    HICON hIconOpen = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_OPEN));

    SendMessage(hWndButtonCapture, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconCapture);
    SendMessage(hWndButtonSave, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconSave);
    SendMessage(hWndButtonOpen, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconOpen);

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetBkColor(hdcStatic, RGB(255, 255, 255));
        return (LRESULT)hBrushWhite;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:  // Capture button
    ShowWindow(hWnd, SW_HIDE);  // Hide the main window
    UpdateWindow(hWnd);  // Ensure the window is hidden before capturing
    Sleep(100);

    switch (SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0)) {
        case 0: // Full Screen
            hBitmap = ScreenCapture::CaptureScreen(capturedWidth, capturedHeight);
            if (hBitmap == NULL) {
                MessageBox(hWnd, "Failed to capture full screen.", "Capture Error", MB_ICONERROR);
            }
            break;
        case 1: // Select Region
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            isCapturing = true;
            OverlayWindow::ShowOverlayWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd);
            break;
        case 2: // Capture Selected Window
            hBitmap = ScreenCapture::CaptureSelectedWindow(capturedWidth, capturedHeight);
            if (hBitmap == NULL) {
                MessageBox(hWnd, "Failed to capture selected window.", "Capture Error", MB_ICONERROR);
            }
            break;
    }
    ShowWindow(hWnd, SW_SHOW);
    break;

        case 2:  // Save button
            if (hBitmap) {
                OPENFILENAME ofn;
                char szFileName[MAX_PATH] = "";
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                ofn.lpstrDefExt = "bmp";

                if (GetSaveFileName(&ofn)) {
                    BitmapHandler::SaveBitmapFile(hBitmap, szFileName);
                }
            } else {
                MessageBox(hWnd, "No image captured to save.", "Save Error", MB_ICONWARNING);
            }
            break;
        case 3:  // Open with Paint button
            if (hBitmap) {
                char tempPath[MAX_PATH];
                GetTempPath(MAX_PATH, tempPath);
                char tempFile[MAX_PATH];
                GetTempFileName(tempPath, "cap", 0, tempFile);
                strcat(tempFile, ".bmp");
                if (BitmapHandler::SaveBitmapFile(hBitmap, tempFile)) {
                    BitmapHandler::OpenWithPaint(tempFile);
                } else {
                    MessageBox(hWnd, "Failed to save.", "Error", MB_ICONERROR);
                }
            } else {
                MessageBox(hWnd, "No image captured to open", "Error", MB_ICONWARNING);
            }
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        if (hBitmap) DeleteObject(hBitmap);
        DeleteObject(hBrushWhite);
        DeleteObject(hBrushMainWindow);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
