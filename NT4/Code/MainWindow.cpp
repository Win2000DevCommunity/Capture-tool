#include "MainWindow.h"
#include "ScreenCapture.h"
#include "BitmapHandler.h"
#include "OverlayWindow.h"
#include "Localization.h"
#include <commdlg.h>
#include <algorithm>
#include "../resource.h"
#include <commctrl.h>
#include <tchar.h> // Include tchar.h for _tcscat

#pragma comment(lib, "comctl32.lib")

HBITMAP hBitmap = NULL;
HWND hWndButtonCapture, hWndButtonSave, hWndButtonOpen, hWndComboBox, hWndMain;
RECT captureRect;
POINT ptStart, ptEnd;
bool isCapturing = false;
int capturedWidth = 0, capturedHeight = 0;
HBRUSH hBrushWhite, hBrushMainWindow;

void MainWindow::RegisterAndCreateWindow(HINSTANCE hInstance, int nCmdShow) {
    // Load localization resources
    if (LoadLocalizedResource() == NULL) {
        MessageBox(NULL, TEXT("Failed to load language resources."), TEXT("Error"), MB_OK | MB_ICONERROR);
        return;
    }

         InitCommonControls(); // Use InitCommonControls for NT4

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = MainWindow::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("ScreenCaptureClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Set the main window background color
    RegisterClass(&wc);

    TCHAR szAppTitle[100];
    LoadString(g_hLangRes, IDS_APP_TITLE, szAppTitle, sizeof(szAppTitle) / sizeof(szAppTitle[0]));

    // Initial dummy window size, will be adjusted later
    hWndMain = CreateWindow(wc.lpszClassName, szAppTitle,
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 10, 110, NULL, NULL, hInstance, NULL);

    // Create white brush for static controls
    hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
    // Create brush for the main window background
    hBrushMainWindow = CreateSolidBrush(RGB(255, 255, 255));

    // Load localized strings
    TCHAR szSelectMode[100], szCapture[100], szSave[100], szOpen[100];
    LoadString(g_hLangRes, IDS_SELECT_MODE, szSelectMode, sizeof(szSelectMode) / sizeof(szSelectMode[0]));
    LoadString(g_hLangRes, IDS_CAPTURE, szCapture, sizeof(szCapture) / sizeof(szCapture[0]));
    LoadString(g_hLangRes, IDS_SAVE, szSave, sizeof(szSave) / sizeof(szSave[0]));
    LoadString(g_hLangRes, IDS_OPEN, szOpen, sizeof(szOpen) / sizeof(szOpen[0]));

    // Create a device context for measuring text
    HDC hdc = GetDC(NULL);

    SIZE size;
    int xPosLabel = 10, yPosLabel = 10, yPosControl = 35;
    int buttonColumnWidth = 30; // Width for the button columns
    int labelSpacing = 20; // Space between labels

    // Calculate the width for the labels based on the text size
    GetTextExtentPoint32(hdc, szSelectMode, lstrlen(szSelectMode), &size);
    int comboBoxColumnWidth = size.cx + 3;

    // Add items to the combo box and calculate the width of the longest item
    TCHAR szFullScreen[100], szSelectRegion[100], szSelectedWindow[100];
    LoadString(g_hLangRes, IDS_FULL_SCREEN, szFullScreen, sizeof(szFullScreen) / sizeof(szFullScreen[0]));
    LoadString(g_hLangRes, IDS_SELECT_REGION, szSelectRegion, sizeof(szSelectRegion) / sizeof(szSelectRegion[0]));
    LoadString(g_hLangRes, IDS_SELECTED_WINDOW, szSelectedWindow, sizeof(szSelectedWindow) / sizeof(szSelectedWindow[0]));

    const TCHAR* comboBoxItems[] = {
        szFullScreen,
        szSelectRegion,
        szSelectedWindow
    };
    int longestItemWidth = 0;
    for (int i = 0; i < sizeof(comboBoxItems) / sizeof(comboBoxItems[0]); ++i) {
        GetTextExtentPoint32(hdc, comboBoxItems[i], lstrlen(comboBoxItems[i]), &size);
        if (size.cx > longestItemWidth) {
            longestItemWidth = size.cx;
        }
    }
    comboBoxColumnWidth = std::max(comboBoxColumnWidth, longestItemWidth + 20);

    // Create "Select Mode" label and combo box
    HWND hWndStatic1 = CreateWindow(TEXT("STATIC"), szSelectMode, WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, xPosLabel, yPosLabel, comboBoxColumnWidth, 20, hWndMain, NULL, hInstance, NULL);
    hWndComboBox = CreateWindow(TEXT("COMBOBOX"), TEXT(""), CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_TABSTOP, xPosLabel, yPosControl, comboBoxColumnWidth, 300, hWndMain, NULL, hInstance, NULL);

    // Add items to the combo box
    for (int i = 0; i < sizeof(comboBoxItems) / sizeof(comboBoxItems[0]); ++i) {
        SendMessage(hWndComboBox, CB_ADDSTRING, 0, (LPARAM)comboBoxItems[i]);
    }
    SendMessage(hWndComboBox, CB_SETCURSEL, 0, 0);

    // Move xPosLabel for buttons and labels
    xPosLabel += comboBoxColumnWidth + 20; // Add space after combo box column

    // Create "Capture" label and button
    GetTextExtentPoint32(hdc, szCapture, lstrlen(szCapture), &size);
    int captureLabelWidth = size.cx + 1;
    HWND hWndStatic2 = CreateWindow(TEXT("STATIC"), szCapture, WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, xPosLabel, yPosLabel, captureLabelWidth, 20, hWndMain, NULL, hInstance, NULL);
    hWndButtonCapture = CreateWindow(TEXT("BUTTON"), TEXT(""), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON, xPosLabel, yPosControl, buttonColumnWidth, buttonColumnWidth, hWndMain, (HMENU)1, hInstance, NULL);

    // Move xPosLabel for next button and label
    xPosLabel += captureLabelWidth + labelSpacing;

    // Create "Save" label and button
    GetTextExtentPoint32(hdc, szSave, lstrlen(szSave), &size);
    int saveLabelWidth = size.cx + 1;
    HWND hWndStatic3 = CreateWindow(TEXT("STATIC"), szSave, WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, xPosLabel, yPosLabel, saveLabelWidth, 20, hWndMain, NULL, hInstance, NULL);
    hWndButtonSave = CreateWindow(TEXT("BUTTON"), TEXT(""), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON, xPosLabel, yPosControl, buttonColumnWidth, buttonColumnWidth, hWndMain, (HMENU)2, hInstance, NULL);

    // Move xPosLabel for next button and label
    xPosLabel += saveLabelWidth + labelSpacing;

    // Create "Open" label and button
    GetTextExtentPoint32(hdc, szOpen, lstrlen(szOpen), &size);
    int openLabelWidth = size.cx + 1;
    HWND hWndStatic4 = CreateWindow(TEXT("STATIC"), szOpen, WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, xPosLabel, yPosLabel, openLabelWidth, 20, hWndMain, NULL, hInstance, NULL);
    hWndButtonOpen = CreateWindow(TEXT("BUTTON"), TEXT(""), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON, xPosLabel, yPosControl, buttonColumnWidth, buttonColumnWidth, hWndMain, (HMENU)3, hInstance, NULL);

    // Adjust window width based on the last control position
    int windowWidth = xPosLabel + openLabelWidth + labelSpacing + 10;

    // Release the device context
    ReleaseDC(NULL, hdc);

    // Adjust the main window size
    SetWindowPos(hWndMain, NULL, 0, 0, windowWidth, 110, SWP_NOMOVE | SWP_NOZORDER);

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
             OverlayWindow::resetCaptureRect();
            ShowWindow(hWnd, SW_HIDE);  // Hide the main window
            UpdateWindow(hWnd);  // Ensure the window is hidden before capturing
            Sleep(100);

            switch (SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0)) {
                case 0: // Full Screen
                    hBitmap = ScreenCapture::CaptureScreen(capturedWidth, capturedHeight);
                    if (hBitmap == NULL) {
                        TCHAR szCaptureError[100];
                        LoadString(g_hLangRes, IDS_CAPTURE_ERROR, szCaptureError, sizeof(szCaptureError) / sizeof(szCaptureError[0]));
                        MessageBox(hWnd, szCaptureError, TEXT("Error"), MB_ICONERROR);
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
                        TCHAR szCaptureError[100];
                        LoadString(g_hLangRes, IDS_CAPTURE_ERROR, szCaptureError, sizeof(szCaptureError) / sizeof(szCaptureError[0]));
                        MessageBox(hWnd, szCaptureError, TEXT("Error"), MB_ICONERROR);
                    }
                    break;
            }
            ShowWindow(hWnd, SW_SHOW);
            break;

        case 2:  // Save button
            if (hBitmap) {
                OPENFILENAME ofn;
                TCHAR szFileName[MAX_PATH] = TEXT("");
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = TEXT("Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0");
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                ofn.lpstrDefExt = TEXT("bmp");

                if (GetSaveFileName(&ofn)) {
                    BitmapHandler::SaveBitmapFile(hBitmap, szFileName);
                }
            } else {
                TCHAR szSaveError[100];
                LoadString(g_hLangRes, IDS_SAVE_ERROR, szSaveError, sizeof(szSaveError) / sizeof(szSaveError[0]));
                MessageBox(hWnd, szSaveError, TEXT("Error"), MB_ICONWARNING);
            }
            break;
        case 3:  // Open with Paint button
            if (hBitmap) {
                TCHAR tempPath[MAX_PATH];
                GetTempPath(MAX_PATH, tempPath);
                TCHAR tempFile[MAX_PATH];
                GetTempFileName(tempPath, TEXT("cap"), 0, tempFile);
                _tcscat(tempFile, TEXT(".bmp"));
                if (BitmapHandler::SaveBitmapFile(hBitmap, tempFile)) {
                    BitmapHandler::OpenWithPaint(tempFile);
                } else {
                    TCHAR szSaveError[100];
                    LoadString(g_hLangRes, IDS_SAVE_ERROR, szSaveError, sizeof(szSaveError) / sizeof(szSaveError[0]));
                    MessageBox(hWnd, szSaveError, TEXT("Error"), MB_ICONERROR);
                }
            } else {
                TCHAR szOpenError[100];
                LoadString(g_hLangRes, IDS_OPEN_ERROR, szOpenError, sizeof(szOpenError) / sizeof(szOpenError[0]));
                MessageBox(hWnd, szOpenError, TEXT("Error"), MB_ICONWARNING);
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

