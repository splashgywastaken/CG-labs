#include "framework.h"
#include <Windows.h>
#include <windowsx.h>
#include "Lab3.2.h"

#include <fstream>
#include <vector>

#include "axis_rotation_type.h"
#include "point_double_3d.h"
#include "camera_3d.h"
#include "direction.h"
#include "model3d_impl.h"
#include "PointDouble.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

class camera_2d;
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

const double RESIZE_K = 0.1;
camera_3d* camera;
std::vector<model3d*> objects;
int frame = 0;
bool is_there_input = true;
bool mouse_button_pressed = false;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int draw_scene_objects(HDC dc);
int load_camera(HDC dc);
//int add_new_graph(const POINT& pos0, const std::string& graph_type, const PointDouble& params);
void animate(HDC dc);
void load_scene_objects(const std::string& filename);
void rotate_scene_model(HDC dc, model3d* obj, axis_rotation_type type);
void mirror_scene_model(HDC dc, model3d* obj, mirror_type_3d);
void rotate_scene_model(HDC dc, model3d* obj, rotation_type_3d rotation_type, double radians);
void resize_scene_model(HDC dc, model3d* obj, double k);
void resize_scene_model(HDC dc, model3d* obj, point_double_3d k);
void move_scene_model(HDC dc, model3d* obj, double distance, direction direction);

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
    LoadStringW(hInstance, IDC_LAB32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB32));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB32);
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

   load_camera( GetDC(hWnd) );
   load_scene_objects("objects.txt");

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
LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
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

            /*if (is_there_input) {*/
                //Очистка экрана
                camera_3d::clear_window(hdc);
                //Функция для прорисовки объектов на сцене
                //animate(hdc);
                draw_scene_objects(hdc);
                //InvalidateRect(hWnd, nullptr, false);

                EndPaint(hWnd, &ps);
                is_there_input = false;
            /*}*/
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
			camera->resize(1 + RESIZE_K, mouse_pos);
        }
        else
        {
			camera->resize(1 - RESIZE_K, mouse_pos);
        }
        is_there_input = true;
        InvalidateRect(hWnd, nullptr, false);
    }
    break;

    case WM_LBUTTONDOWN:
    {

        camera->start_dragging(
            point_double{
                GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)
            }
        );
    }
    break;

    case WM_MOUSEMOVE:
    {
            if (camera->is_dragging())
            {
                camera->drag(
                    point_double{
                        GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)
                    }
                );
                InvalidateRect(hWnd, nullptr, false);
            }
            
    }
    break;

    case WM_LBUTTONUP:
    {
        mouse_button_pressed = false;
        camera->stop_dragging();
        is_there_input = true;

    }
    break;

    case WM_KEYDOWN:
	    {

			auto dc = GetDC(hWnd);
            constexpr double distance = 5.0;
            const double radian = 1.0 - sqrt(3) / 2;

	        switch (wParam)
		        {

                //Поворот относительно начала координат на угол фи
					//левый поворот
			        case VK_LEFT:
			        {
				        for (const auto& object : objects)
				        {
                            rotate_scene_model(dc, object, rotation_type_3d::ordinate, radian);
				        }	                    
			        }
			        break;
					//правый поворот
			        case VK_RIGHT:
	                {
                        for (const auto& object : objects)
                        {
                            rotate_scene_model(dc, object, rotation_type_3d::ordinate, - radian);
                        }
	                }
                    break;
                    //левый поворот
                    case VK_UP:
                    {
                        for (const auto& object : objects)
                        {
                            rotate_scene_model(dc, object, rotation_type_3d::abscissa, radian);
                        }
                    }
                    break;
                    //правый поворот
                    case VK_DOWN:
                    {
                        for (const auto& object : objects)
                        {
                            rotate_scene_model(dc, object, rotation_type_3d::abscissa, -radian);
                        }
                    }
	        		break;
					//Перенос на вектор а
	                //клавиша A, а = (-1,0)
	                case 0x41:
	                {
                        for (const auto& object : objects)
                        {
                            move_scene_model(
                                dc,
                                object,
                                distance,
                                direction::LEFT
                            );
                        }
                        
	                }
	        		break;
					
	                //D, а = (1, 0)
	                case 0x44:
                    {
                        for (const auto& object : objects)
                        {
                            move_scene_model(
                                dc,
                                object,
                                distance,
                                direction::RIGHT
                            );
                        }
	                }
	        		break;
	                //W, а = (0, 1)
	                case 0x57:
	                {
                        for (const auto& object : objects)
                        {
							move_scene_model(
								dc,
                                object,
                                distance,
                                direction::DOWN
                                );
                        }
	                }
	        		break;
	                //S, а = (0, -1)
	                case 0x53:
		            {
						for (const auto& object : objects)
                        {
							move_scene_model(
								dc,
                                object,
                                distance,
                                direction::UP
                            );
                        }
		            }
	                break;
					//Отражение относительно координатных осей
					//J
                    case 0x4A:
                    {
                    	for (const auto& object : objects)
		                {
                            mirror_scene_model(dc, object, mirror_type_3d::abscissa);
		                }                        
                    }
	        		break;
					//K
                    case 0x4B:
                    {
                        for (const auto& object : objects)
                        {
                            mirror_scene_model(dc, object, mirror_type_3d::ordinate);
                        }
                    }
	        		break;
					//L
                    case 0x4C:
	                {
                        for (const auto& object : objects)
                        {
                            mirror_scene_model(dc, object, mirror_type_3d::applicate);
                        }
	                }
                    break;
					//Масштабирование на k
					//-
                    case VK_OEM_MINUS:
	                {
                        for (const auto& object: objects)
                        {
                            resize_scene_model(
                                dc,
                                object,
                                point_double_3d{0.9,0.9,0.9}
                            );
                        }
	                }
	        		break;
					//+
                    case VK_OEM_PLUS:
                    {
                        for (const auto& object : objects)
                        {
                            resize_scene_model(
                                dc,
                                object,
                                point_double_3d{ 1.1,1.1,1.1 }
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
	        if (camera != nullptr) 
            {
	            //Вызывается при изменении размеров окна
	            RECT r;
	            const auto dc = GetDC(hWnd);

	            GetClientRect(WindowFromDC(dc), &r);
	            //Функция для изменения параметров класса камеры, хранящих информацию об окне пользователя

	            camera->on_window_size_change(
	                r.right,
	                r.bottom
	            );
	            ReleaseDC(hWnd, dc);
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

int load_camera(const HDC dc)
{
    RECT screen_rectangle;
    GetClientRect(WindowFromDC(dc), &screen_rectangle);
    
    camera = new camera_3d(
        point_double_3d(0,0,0),
        vector_3d(0,0, 1),
        vector_3d(0,1,0),
        24.0,
         point_double(0.0, 0.0),
        static_cast<double>(screen_rectangle.right),
        static_cast<double>(screen_rectangle.bottom),
        point_double(150.0, 150.0)
    );
    
    return 1;

}

void animate(HDC dc)
{

    if (frame % 2 == 0)
    {
	   /* for (model3d* object : objects)
	    {
            object->resize(dc, point_double_3d{ 1.0-sin(frame), 1.0 - sin(frame), 1.0 - sin(frame)});
	    }*/
    } else
    {
        for (model3d* object : objects)
        {
            object->rotate(dc, rotation_type_3d::applicate, sin(frame/10000));
        }
    }
    frame++;
}

void load_scene_objects(const std::string& filename)
{
    
    //В данный файл записывается имя модели и количество вершин m
    std::ifstream file;
    file.open(filename);

    std::string vertices_filename;
    std::string faces_filename;

    std::ifstream verticies_file;
    std::ifstream faces_file;
    file >> vertices_filename;
    file >> faces_filename;

    verticies_file.open(vertices_filename);
    faces_file.open(faces_filename);

    while (!file.eof())
    {
        std::string model_name;
        int number_of_duplicates;
        std::string verticies_buffer;
        std::string faces_buffer;
        file >> model_name;
        file >> number_of_duplicates;
        double x, y, z;
        file >> x;
        file >> y;
        file >> z;
        double alpha, beta, gamma;
        file >> alpha;
        file >> beta;
        file >> gamma;

        faces_file.seekg(0, std::ios::beg);
        verticies_file.seekg(0, std::ios::beg);

        while (!verticies_file.eof() && verticies_buffer != model_name)
        {
            verticies_file >> verticies_buffer;
        }

        while (!faces_file.eof() && faces_buffer != model_name)
        {
            faces_file >> faces_buffer;
        }

        if (!verticies_file.eof() && !faces_file.eof())
        {

            size_t l;
            size_t m;

            faces_file >> l;
            verticies_file >> m;

            std::vector<std::vector<double>> verticies(4);
            for (auto& vector : verticies)
            {
                vector.resize(m);
            }

            //добавляем вершины
            for (size_t i = 0; i < 4; i++)
            {
                for (size_t j = 0; j < m; j++)
                {
                    verticies_file >> verticies[i][j];
                }
            }

            //Добавляем грани
            std::vector<std::vector<int>> faces(l);
            for (auto& vector : faces)
            {
                vector.resize(3);
            }

            for (size_t i = 0; i < l; i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    faces_file >> faces[i][j];
                }
            }

            for (int i = 0; i < number_of_duplicates; i++)
            {
                const auto init_position = new point_double_3d{x,y,z};
                const auto init_rotation = new point_double_3d{ alpha,beta,gamma };

	            //Вносим новый объект в вектор объектов
                objects.push_back(
                    new model3d_impl(
	                    verticies,
	                    faces,
	                    *init_position,
	                    *init_rotation
                    ));
                file >> x;
                file >> y;
                file >> z;
                file >> alpha;
                file >> beta;
                file >> gamma;
                delete init_position;
                delete init_rotation;
            }

            verticies.clear();
            faces.clear();
                
        }

    }

    file.close();
    verticies_file.close();
    faces_file.close();
    is_there_input = true;
}

void mirror_scene_model(HDC dc, model3d* obj, mirror_type_3d mirror_type)
{
    is_there_input = true;

    obj->mapping(dc, mirror_type);

}

void rotate_scene_model(HDC dc, model3d* obj, rotation_type_3d type)
{
	const double radian = sqrt(3)/2;
    is_there_input = true;

    obj->rotate(dc, type, radian);
}

void rotate_scene_model(
    HDC dc, 
    model3d* obj, 
    const rotation_type_3d rotation_type, 
    const double radians
)
{
    is_there_input = true;
    obj->rotate(dc, rotation_type, radians);
}

void move_scene_model(HDC dc, model3d* obj, double distance, const direction _direction)
{
    is_there_input = true;

    if (_direction == direction::DOWN)
    {

        obj->move(dc, {0.0, distance, 0.0});

    }
    if (_direction == direction::UP)
    {

        obj->move(dc, { 0.0, -distance, 0.0 });

    }
    if (_direction == direction::LEFT)
    {

        obj->move(dc, { -distance, 0.0, 0.0 });

    }
    if (_direction == direction::RIGHT)
    {

        obj->move(dc, { distance, 0.0, 0.0 });

    }

}

void resize_scene_model(HDC dc, model3d* obj, double k)
{
    is_there_input = true;

    obj->resize(dc, point_double_3d{ k, k, k});

}

void resize_scene_model(HDC dc, model3d* obj, point_double_3d k)
{
    is_there_input = true;

    obj->resize(dc, k);

}

int draw_scene_objects(HDC dc)
{
    is_there_input = true;

    if (camera == nullptr)
    {
        return -1;
    }
    
    if (!objects.empty())
    {
	    for (const auto& object : objects)
	    {
            camera->draw(dc, object);
	    }
    }

    return -2;//Failure code. Number of objects was zero
}
