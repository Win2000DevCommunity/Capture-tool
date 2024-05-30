# Capture-tool
Capture tool for Windows NT4 and NT 5.x (2000/XP/2003)

![image](https://github.com/Win2000DevCommunity/Capture-tool/assets/154258820/56a4e172-6b09-4ad7-86aa-4f2346681202)


![image](https://github.com/Win2000DevCommunity/Capture-tool/assets/154258820/11c0f2d6-fcb1-4574-bd45-8f7c2fce216c)


![image](https://github.com/Win2000DevCommunity/Capture-tool/assets/154258820/0e99839d-b386-4e5a-8c3e-85cf109f8214)


Features:

    Full-Screen Capture:
    The application currently supports capturing the entire screen, allowing users to take comprehensive screenshots of all visible content on their display.

    Capture Region Selection:
    Users can select a specific region of the screen to capture, providing flexibility to focus on and share only the desired part of the screen.

    Select Window Capture Mode:
    Users can capture the content of a single window, enabling more focused and precise screen captures, thus improving user experience and usability.

        
Future Support Planning:

    Add Support for All Languages:
    Expand the application's language support to include all major languages. This will make the application accessible to a wider audience, catering to users from diverse linguistic backgrounds.

V1.1:

    Added capture select window mode.
    Removed the transparent background in      select mode due to the problem with the 16-color.
    Fixed the issue with the process not being closed.

        
Steps to compile :

    Open Project Options in Dev-C++:
        Open your project in Dev-C++ (5.11 or newer compilator that support c++11 ).
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
