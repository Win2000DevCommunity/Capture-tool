!include "MUI2.nsh"

; Define basic information
Name "Accessories Extender Pack for Windows NT 4.0 / 5.x"
OutFile "ScreenCaptureToolInstaller.exe"
InstallDir "$PROGRAMFILES\Screen Capture Tool"

; Modern UI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "app.ico"
!define MUI_UNICON "app_uninstall.ico"

; Welcome, License, Directory, Installation, and Finish Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstallation Pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Languages
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Arabic"


Section "Install"
    ; Set output path to the installation directory
    SetOutPath $INSTDIR

    ; Files to be installed
    File "ScreenCaptureTool.exe"

    ; Include all language DLLs
    SetOutPath "$INSTDIR\Languages\Ar"
    File "Languages\Ar\*.dll"
    SetOutPath "$INSTDIR\Languages\De"
    File "Languages\De\*.dll"
    SetOutPath "$INSTDIR\Languages\En"
    File "Languages\En\*.dll"
    SetOutPath "$INSTDIR\Languages\Es"
    File "Languages\Es\*.dll"
    SetOutPath "$INSTDIR\Languages\Fr"
    File "Languages\Fr\*.dll"
    SetOutPath "$INSTDIR\Languages\Hi"
    File "Languages\Hi\*.dll"
    SetOutPath "$INSTDIR\Languages\Ja"
    File "Languages\Ja\*.dll"
    SetOutPath "$INSTDIR\Languages\Ko"
    File "Languages\Ko\*.dll"
    SetOutPath "$INSTDIR\Languages\Zh_Cn"
    File "Languages\Zh_Cn\*.dll"
    SetOutPath "$INSTDIR\Languages\Zh_Tw"
    File "Languages\Zh_Tw\*.dll"

    ; Create a start menu shortcut in Accessories
    CreateShortCut "$SMPROGRAMS\Accessories\Screen Capture Tool.lnk" "$INSTDIR\ScreenCaptureTool.exe"

    ; Create an uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; Add registry entries for Add/Remove Programs
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "DisplayName" "Accessories Extender Pack for Windows NT 4.0 / 5.x"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "DisplayIcon" "$INSTDIR\ScreenCaptureTool.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "Publisher" "Your Company Name"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "DisplayVersion" "1.0"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool" "NoRepair" 1
SectionEnd

Section "Uninstall"
    ; Remove the files
    Delete "$INSTDIR\ScreenCaptureTool.exe"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir /r "$INSTDIR\Languages"

    ; Remove the shortcuts
    Delete "$SMPROGRAMS\Accessories\Screen Capture Tool.lnk"

    ; Remove registry entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ScreenCaptureTool"

    ; Remove the installation directory
    RMDir "$INSTDIR"
SectionEnd