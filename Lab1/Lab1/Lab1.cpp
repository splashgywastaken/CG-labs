// Lab1.cpp : Определяет точку входа для приложения.
//

/*
Вариант 12. Полярная система координат. Параметрический способ задания функции.
Второй способ связи мировых и экранных координатных систем (при помощи параметров X0, Y0, px, py).
Масштабирование с сохранением положения верхней и нижней границ окна.

Задачи из этой лабы:

Построение графиков функций:
• (A) построение графика функции, заданной явно в мировых координатах (система координат – декартова прямоугольная);
• (B) перетаскивание графика функции;
• (B) масштабирование графика функции (с сохранением позиции точки, на которую наведён курсор мыши);
• (C) согласование масштабов по координатным осям;
• (C) построение графика функции, заданной иным способом (параметрически).

*/

//
//Класс камера отвечает за хранение переменных с данными об окне юзера
//Класс граф содержит данные о графе и способы реализации его в окне
//

#include "framework.h"
#include <Windows.h>
#include <windowsx.h>
#include "Lab1.h"
#include <vector>

#include "camera_2d.h"
#include "graph_2d.h"
#include "PointDouble.h"

#define MAX_LOADSTRING 100

class camera_2d;
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
const double RESIZE_K = 0.1;

//Объекты сцены
std::vector<graph_2d*> graphs_vector;
//Объект фокуса
graph_2d* focused;
//Камера
camera_2d* camera2d;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int draw_scene_objects(HDC dc);
int load_camera(HDC dc);
int add_new_graph(const POINT& pos0, const std::string& graph_type, const PointDouble& params);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //Инициализация приложения

    

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);

   // Загружаем объект камеры
   load_camera( GetDC(hWnd) );
   //Загружаем график для отрисовки (в следующей версии предположительно данный функционал будет по нажатию клавиши)
   add_new_graph(POINT{ 0,0 }, "rose", PointDouble{1,1});

   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

            //Очистка экрана
            camera_2d::clear_window(hdc);

            //Функция для прорисовки объектов на сцене
            draw_scene_objects(hdc);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_MOUSEWHEEL:
	    {
        
	        POINT mouse_pos{
	               GET_X_LPARAM(lParam),
	               GET_Y_LPARAM(lParam)
	        };
            ScreenToClient(hWnd, &mouse_pos);
            
	        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
	        {
	            camera2d->resize(1 + RESIZE_K, mouse_pos);
	        }
	        else
	        {
	            camera2d->resize(1 - RESIZE_K, mouse_pos);
	        }
			InvalidateRect(hWnd, nullptr, false);
		        
	    }
    	break;

    case WM_LBUTTONDOWN:
	    {

	        camera2d->start_dragging(
	            PointDouble{
	                GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)
	            }
	        );

	    }
        break;

    case WM_MOUSEMOVE:
	    {

			if (camera2d->is_dragging())
			{

                camera2d->drag(
                    PointDouble{
                    GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)
                    }
                );

			}
            InvalidateRect(hWnd, nullptr, false);

	    }
        break;

    case WM_LBUTTONUP:
	    {

			camera2d->stop_dragging();

	    }
        break;

    case WM_SIZE:
	    {
	        if (camera2d != nullptr) {
	            //Вызывается при изменении размеров окна
	            RECT r;
	            auto dc = GetDC(hWnd);

	            GetClientRect(WindowFromDC(dc), &r);
	            //Функция для изменения параметров класса камеры, хранящих информацию об окне пользователя

	            camera2d->on_window_size_change(
	                r.right,
	                r.bottom
	            );
                ReleaseDC( hWnd, dc);
                InvalidateRect(hWnd, nullptr, false);
	        }

	    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int load_camera(HDC dc)
{
    RECT r;
    GetClientRect(WindowFromDC(dc), &r);
    
    camera2d = new camera_2d(
        r.right,
        r.bottom,
        -8,
        8,
        4.5,
        -4.5,
        POINT{0,0}
    );

    return 1;

}

int add_new_graph(const POINT& pos0, const std::string& graph_type, const PointDouble& params)
{
    
    graphs_vector.push_back(
        new graph_2d(
            pos0.x,
            pos0.y  
        )
    );

    graphs_vector.at(graphs_vector.size() - 1)->set_graph_type(
        graph_type,
        params.x,
        params.y
    );

    return 1;

}

int draw_scene_objects(HDC dc)
{
    if (!graphs_vector.empty())
    {

        for (auto& it : graphs_vector)
        {
            camera2d->draw(dc, *it);
        }

        return 1;

    }
    return -1;//Failure code. Number of objects was zero
}