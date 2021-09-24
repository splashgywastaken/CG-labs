#ifndef UNICODE
#define UNICODE
#endif 

#define _CRT_SECURE_NO_WARNINGS

#include <clocale>
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <vector>
#include "line.h"

line* scene_object = nullptr;
auto* mouse_pos = new std::vector<POINT>();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AddControls(HWND& hwnd);
void AddMenus(HWND& hwnd);

bool display_confirmation_message_box(const char* message);
int display_warning_message_box(const char* message);

int delete_scene_object(line* object, HWND &hwnd);

void set_first_coordinates(HWND& hwnd, DWORD flags);
void set_second_coordinates(HWND& hwnd);
void on_mouse_move(HWND& hwnd, DWORD flags);

wchar_t pchar_to_wchar(const char* str);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    setlocale(LC_ALL, "");

    // Register the window class.
    constexpr wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    //Additional inits
    mouse_pos->resize(2);

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
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
	{

        AddMenus(hwnd);
        AddControls(hwnd);

	}
    break;

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

        if (GetKeyState(VK_MENU) & 0x8000) {

            set_second_coordinates(hwnd);

            HDC hdc = GetDC(hwnd);

            //Code for a Line
            if (scene_object == nullptr) {
                scene_object =
                    new line(
                        hdc,
                        static_cast<int>(mouse_pos->at(0).x),
                        static_cast<int>(mouse_pos->at(0).y),
                        static_cast<int>(mouse_pos->at(1).x),
                        static_cast<int>(mouse_pos->at(1).y)
                    );
            }
        	/*else
        	{
                display_warning_message_box("Object had been already created.\nPlease, delete it firstly");
        	}*/

        }
        else
        {
            set_first_coordinates(hwnd, true);
        }
	}
    break;

    case WM_LBUTTONUP:
    {

    }
    break;

    case WM_KEYDOWN:
    {

        switch (wParam)
        {
	        case VK_ESCAPE:
		        {
		            //if (display_confirmation_message_box("Do you realy want to delete an object?"))
		            delete_scene_object(scene_object, hwnd);
		        }
	            break;

	        default:
	            break;
        }
	    

	}
    break;

    default: ;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

wchar_t pchar_to_wchar(const char* str)
{
    auto* wc = new wchar_t[strlen(str) + 1];
    mbstowcs(wc, str, strlen(str));

    return *wc;
}

int display_warning_message_box(const char* message)
{
    int msgboxID = MessageBox(
        NULL,
        LPCWSTR(pchar_to_wchar(message)),
        L"Warning",
        MB_ICONEXCLAMATION
    );  

    return msgboxID;

}

bool display_confirmation_message_box(const char* message)
{
    int msgboxID = MessageBox(
        NULL,
        LPCWSTR(pchar_to_wchar(message)),
        L"Confirm your action",
        MB_ICONEXCLAMATION | MB_YESNO
    );

    return msgboxID == IDYES;
}

void AddControls(HWND& hwnd)
{

}

void AddMenus(HWND& hwnd)
{
		
}

int delete_scene_object(line* object, HWND& hwnd)
{
    if (object != nullptr) {

        object->clear(GetDC(hwnd));
        delete object;
        return 0;
    }

    return -1;
}

void set_first_coordinates(HWND& hwnd, DWORD flags)
{

    GetCursorPos(&mouse_pos->at(0));

}

void set_second_coordinates(HWND& hwnd)
{

    GetCursorPos(&mouse_pos->at(1));
    
}

void on_mouse_move(HWND& hwnd, DWORD flags)
{
}