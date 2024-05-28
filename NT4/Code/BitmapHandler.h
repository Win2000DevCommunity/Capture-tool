#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <windows.h>

class BitmapHandler {
public:
   // static void DisplayBitmap(HDC hdc, RECT* rect, HBITMAP hBitmap, int capturedWidth, int capturedHeight);
    static bool SaveBitmapFile(HBITMAP hBitmap, const char* filename);
    static void OpenWithPaint(const char* filename);
};

#endif // BITMAPHANDLER_H
