#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <windows.h>

class ScreenCapture {
public:
    static HBITMAP CaptureScreen(int &width, int &height);
    static HBITMAP CaptureRegion(RECT* rect, int &width, int &height);
    static HBITMAP CaptureSelectedWindow(int &width, int &height); // Declare the new function
};

#endif // SCREENCAPTURE_H
