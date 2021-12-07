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
//Доп задания
//1. Поворот вокруг центра пакмана
//   создать функцию rotation (которая принимает угол иск и игрек), а в этой функции перемножаешь
//   три матрицы: переноса, поворота, обратного переноса
//2. Масштабирование в доль осей самого пакмана (чтобы при уменьшении оставался в той-же точке)
//   Создать скейлинг, принимающий kx ky, а также x0, y0 - координаты точки, которая ост. неизм.
//   при масштабировании
//   c и s  
//

#include "framework.h"
#include <Windows.h>
#include <windowsx.h>
#include "Lab2.h"

#include <fstream>
#include <vector>

#include "axis_rotation_type.h"
#include "camera_2d.h"
#include "direction.h"
#include "graph_2d.h"
#include "model.h"
#include "model_2d.h"
#include "PointDouble.h"

#define MAX_LOADSTRING 100

class camera_2d;
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
const double RESIZE_K = 0.1;

//Объекты сцены
//std::vector<graph_2d*> graphs_vector;
//Объект фокуса
//graph_2d* focused;
//Камера
camera_2d* camera2d;
std::vector<model*> scene_models;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int draw_scene_objects(HDC dc);
int load_camera(HDC dc);
//int add_new_graph(const POINT& pos0, const std::string& graph_type, const PointDouble& params);
void load_scene_objects(std::string filename);
void rotate_scene_model(HDC dc, model& obj, axis_rotation_type type);
void rotate_scene_model(HDC dc, model& obj, double radians);
void rotate_scene_model(HDC dc, model& obj, double cos, double sin);
void resize_scene_model(HDC dc, model& obj, double k);
void resize_scene_model(HDC dc, model& obj, point_double k);
void move_scene_model(HDC dc, model& obj, double distance, direction _direction);

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
   load_scene_objects("objects.txt");
   //Загружаем график для отрисовки (в следующей версии предположительно данный функционал будет по нажатию клавиши)
   //add_new_graph(POINT{ 0,0 }, "rose", PointDouble{1,1});

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
	            point_double{
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
                    point_double{
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

    case WM_KEYDOWN:
	    {

			auto dc = GetDC(hWnd);

	        switch (wParam)
		        {
                //Поворот относительно начала координат на угол фи
					//левый поворот
			        case VK_LEFT:
			        {
				        for (auto& scene_model: scene_models)
				        {
                            rotate_scene_model(dc, *scene_model, -6);
				        }	                    
			        }
			        break;
					//правый поворот
			        case VK_RIGHT:
	                {
                        for (auto& scene_model : scene_models)
                        {
                            rotate_scene_model(dc, *scene_model, 6);
                        }
	                }
	        		break;
					//Перенос на вектор а
	                //клавиша A, а = (-1,0)
	                case 0x41:
	                {
                        for (auto& scene_model : scene_models)
                        {
                            move_scene_model(
                                dc,
                                *scene_model,
                                1.0,
                                direction::LEFT
                            );
                        }
                        
	                }
	        		break;
	                //D, а = (1, 0)
	                case 0x44:
                    {
                        for (auto& scene_model : scene_models)
                        {
                            move_scene_model(
                                dc,
                                *scene_model,
                                1.0,
                                direction::RIGHT
                            );
                        }
	                }
	        		break;
	                //W, а = (0, 1)
	                case 0x57:
	                {
                        for (auto& scene_model : scene_models)
                        {
							move_scene_model(
								dc,
								*scene_model,
                                1.0,
                                direction::UP
                                );
                        }
	                }
	        		break;
	                //S, а = (0, -1)
	                case 0x53:
		            {
						for (auto& scene_model : scene_models)
                        {
							move_scene_model(
								dc,
                                *scene_model,
                                1.0,
                                direction::DOWN
                            );
                        }
		            }
	                break;
                //Отражение относительно координатных осей
					//J
                    case 0x4A:
                    {
                    	for (auto& scene_model : scene_models)
		                {
                            rotate_scene_model(
                                dc,
                                *scene_model,
                                axis_rotation_type::ABSCISSA
                            );
		                }                        
                    }
	        		break;
					//K
                    case 0x4B:
                    {
                        for (auto& scene_model : scene_models)
                        {
                            rotate_scene_model(
                                dc,
                                *scene_model,
                                axis_rotation_type::ORDINATE
                            );
                        }
                    }
	        		break;
					//L
                    case 0x4C:
	                {
                        for (auto& scene_model : scene_models)
                        {
                            rotate_scene_model(
                                dc,
                                *scene_model,
                                axis_rotation_type::ZERO
                            );
                        }
	                }
                    break;
                //Масштабирование на k
					//-
                    case VK_OEM_MINUS:
	                {
                        for (auto& scene_model : scene_models)
                        {
                            resize_scene_model(
                                dc,
                                *scene_model,
                                /*0.9*/
                                point_double{1/1.5, 1/0.9}
                            );
                        }
	                }
	        		break;
					//+
                    case VK_OEM_PLUS:
                    {
                        for (auto& scene_model : scene_models)
                        {
                            resize_scene_model(
                                dc,
                                *scene_model,
                                /*1.2*/
                                point_double{1.5, 0.9}
                            );
                        }
                    }
                    break;
	        }
            InvalidateRect(hWnd, nullptr, false);
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
    default: ;
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
        -4,
        4,
        2.5,
        -2.5,
        POINT{0,0}
    );

    return 1;

}

void load_scene_objects(std::string filename)
{

    //В данный файл записывается имя модели и количество вершин m
    std::ifstream file;
    file.open(filename);

    std::string vertices_filename;
    std::string edges_filename;

    std::ifstream vertices_file;
    std::ifstream edges_file;
    file >> vertices_filename;
    file >> edges_filename;

    vertices_file.open(vertices_filename);
    edges_file.open(edges_filename);

    while(!file.eof())
    {
        std::string model_name;
        std::string buff1;
        std::string buff2;
        file >> model_name;

        edges_file.seekg(0, std::ios::beg);
        vertices_file.seekg(0, std::ios::beg);

        while (!vertices_file.eof() && buff1 != model_name)
        {
            vertices_file >> buff1;
        }

        while (!edges_file.eof() && buff2 != model_name)
        {
            edges_file >> buff2;
        }

        if (!vertices_file.eof() && !edges_file.eof())
        {

            size_t m;

            file >> m;

            std::vector<std::vector<double>> verticies(m);
            for (auto& vector : verticies)
            {
                vector.resize(3);
            }

            //добавляем вершины
            for (size_t i = 0; i < m; i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    vertices_file >> verticies[i][j];
                }
            }

            //Добавляем грани
            std::vector<std::vector<int>> edges(m);
            for (auto& vector : edges)
            {
                vector.resize(m);
            }

            for (size_t i = 0; i < m; i++)
            {
                for (size_t j = 0; j < m; j++)
                {
                    edges_file >> edges[i][j];
                }
            }

            scene_models.push_back(
                new model_2d(verticies, edges)
            );

        }

    }

    file.close();
    vertices_file.close();
    edges_file.close();

}

//int add_new_graph(const POINT& pos0, const std::string& graph_type, const PointDouble& params)
//{
//    
//    graphs_vector.push_back(
//        new graph_2d(
//            pos0.x,
//            pos0.y  
//        )
//    );
//
//    graphs_vector.at(graphs_vector.size() - 1)->set_graph_type(
//        graph_type,
//        params.x,
//        params.y
//    );
//
//    return 1;
//
//}

void rotate_scene_model(HDC dc, model& obj, axis_rotation_type type)
{
    if (type == axis_rotation_type::ABSCISSA)
    {
        obj.rotate(dc, POINT{1, -1});
    } else if (type == axis_rotation_type::ORDINATE)
    {
        obj.rotate(dc, POINT{ -1, 1 });
    } else if (type == axis_rotation_type::ZERO)
    {
        obj.rotate(dc, POINT{-1, -1});
    }
}

void rotate_scene_model(HDC dc, model& obj, double radians)
{

    obj.rotate(dc, radians);

}

void rotate_scene_model(HDC dc, model& obj, double cos, double sin)
{

    obj.rotate(dc, cos, sin);

}

void move_scene_model(HDC dc, model& obj, double distance, direction _direction)
{

    if (_direction == direction::UP)
    {

        obj.move(dc, {0.0, distance});

    }
    if (_direction == direction::DOWN)
    {

        obj.move(dc, { 0.0, -1 * distance });

    }
    if (_direction == direction::LEFT)
    {

        obj.move(dc, { -distance, 0.0 });

    }
    if (_direction == direction::RIGHT)
    {

        obj.move(dc, { distance, 0.0 });

    }

}

void resize_scene_model(HDC dc, model& obj, double k)
{

    obj.resize(dc, point_double{ k, k});

}

void resize_scene_model(HDC dc, model& obj, point_double k)
{

    obj.resize(dc, k);

}

int draw_scene_objects(HDC dc)
{

    if (!scene_models.empty())
    {
	    for (const auto& scene_model : scene_models)
	    {
            camera2d->draw(dc, scene_model);
	    }
    }

    return -1;//Failure code. Number of objects was zero
}
