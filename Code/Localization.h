// Localization.h
#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <windows.h>

// Global variables for language ID and resource handle
extern LANGID g_LangId;
extern HINSTANCE g_hLangRes;

// Function to load the localized resource DLL
HINSTANCE LoadLocalizedResource();

#endif // LOCALIZATION_H

