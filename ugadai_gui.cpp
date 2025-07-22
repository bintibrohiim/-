#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>

#define ID_INPUT    101
#define ID_BUTTON   102
#define ID_TEXT     103
#define ID_RESTART  104

int secretNumber;
int attempts = 0;

HFONT hFont;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit, hButton, hStatic, hRestart;

    switch (uMsg) {
        case WM_CREATE:
            srand(time(0));
            secretNumber = rand() % 100 + 1;
            attempts = 0;

            hFont = CreateFont(
                18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                TEXT("Segoe UI"));

            hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                                 50, 50, 100, 25, hwnd, (HMENU)ID_INPUT, NULL, NULL);

            hButton = CreateWindow("BUTTON", "Check", WS_CHILD | WS_VISIBLE,
                                   160, 50, 70, 25, hwnd, (HMENU)ID_BUTTON, NULL, NULL);

            hRestart = CreateWindow("BUTTON", "Restart", WS_CHILD | WS_VISIBLE,
                                    240, 50, 70, 25, hwnd, (HMENU)ID_RESTART, NULL, NULL);

            hStatic = CreateWindow("STATIC", "Enter a number (1-100)",
                                   WS_CHILD | WS_VISIBLE,
                                   50, 90, 260, 40, hwnd, (HMENU)ID_TEXT, NULL, NULL);

            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hRestart, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON) {
                char buffer[256];
                GetWindowText(hEdit, buffer, 256);
                int guess = atoi(buffer);
                attempts++;

                std::string result;
                if (guess < secretNumber)
                    result = "Too low! Attempts: " + std::to_string(attempts);
                else if (guess > secretNumber)
                    result = "Too high! Attempts: " + std::to_string(attempts);
                else
                    result = "You guessed it! 🎉 Attempts: " + std::to_string(attempts);

                SetWindowText(hStatic, result.c_str());
            }
            else if (LOWORD(wParam) == ID_RESTART) {
                secretNumber = rand() % 100 + 1;
                attempts = 0;
                SetWindowText(hStatic, "Enter a number (1-100)");
                SetWindowText(hEdit, "");
            }
            break;

        case WM_DESTROY:
            DeleteObject(hFont);
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "GuessNumberWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // заменим позже

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Guess the Number - GUI",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 380, 200,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
