#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <windows.h>

class BitmapHandler {
public:
    static bool SaveBitmapFile(HBITMAP hBitmap, const char* filename);
    static void OpenWithPaint(const char* filename);
};

#endif 
