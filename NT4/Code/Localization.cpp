#include "Localization.h"
#include <windows.h>
#include <iostream>

LANGID g_LangId = 0;
HINSTANCE g_hLangRes = NULL;

HINSTANCE LoadLocalizedResource() {
    g_LangId = GetSystemDefaultLangID();
    switch (g_LangId) {
        case 0x040c: // French
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Fr\\Fr.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0c0a: // Spanish
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Es\\Es.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0407: // German
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\De\\De.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0411: // Japanese
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Ja\\Ja.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0804: // Chinese Simplified
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Zh_CN\\Zh_CN.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0404: // Chinese Traditional
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Zh_TW\\Zh_TW.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0412: // Korean
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Ko\\Ko.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0401: // Arabic
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Ar\\Ar.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        case 0x0439: // Hindi
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\Hi\\Hi.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
        default: // Default to English
            g_hLangRes = LoadLibraryEx(TEXT("Languages\\En\\En.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
            break;
    }

    if (!g_hLangRes) {
        MessageBox(NULL, TEXT("Failed to load language resources"), TEXT("Error"), MB_OK | MB_ICONERROR);
    }
    return g_hLangRes;
}
