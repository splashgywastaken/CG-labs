#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <windowsx.h>
#include <vector>
#include "Line.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    constexpr wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    const HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        nullptr,       // Parent window    
        nullptr,       // Menu
        hInstance,  // Instance handle
        nullptr
        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    auto* scene_objects = new std::vector<Line>();

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
    {

        LineTo(GetDC(hwnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

        // HDC dc = GetDC(hwnd);
        // auto* obj = new Line(
        //     dc,
        //     GET_X_LPARAM(lParam),
        //     GET_Y_LPARAM(lParam)
        // );
        //
        // scene_objects->push_back(
        //     *obj
        // );

	}
    break;
    default: ;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}