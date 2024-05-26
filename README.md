# Capture-tool
Capture tool for Windows NT 5.x (2000/XP/2003)

Steps:

    Open Project Options in Dev-C++:
        Open your project in Dev-C++ (5.11 or newer compilator that support c++11.
        Go to Project in the menu bar and select Project Options or press Alt + P.

    Set Compiler Parameters:
        In the Project Options window, go to the Parameters tab.
        Under the C++ Compiler section, add the necessary compiler flags. 
        
            Example: -std=c++11

    Set Linker Parameters:
        In the same Parameters tab, go to the Linker section.
        Add the required linker flags:
            -lgdi32
            -lcomdlg32
            -mwindows
            -lcomctl32

    Save Settings:
        Click the OK button to save your settings.
