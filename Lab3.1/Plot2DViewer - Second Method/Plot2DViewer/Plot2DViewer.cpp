#include <string>
#include <vector>
#include <windows.h>
#include <windowsx.h>

#include "axis_rotation_type.h"
#include "camera_3d.h"
#include "Data.h"
#include "direction.h"
#include "model3d_impl.h"

#include <fstream>

int draw_scene_objects(HDC dc);
int load_camera(HDC dc);
//int add_new_graph(const POINT& pos0, const std::string& graph_type, const PointDouble& params);
void load_scene_objects(const std::string& filename);
void rotate_scene_model(HDC dc, model3d* obj, axis_rotation_type type);
void mirror_scene_model(HDC dc, model3d* obj, mirror_type_3d);
void rotate_scene_model(HDC dc, model3d* obj, rotation_type_3d rotation_type, double radians);
void resize_scene_model(HDC dc, model3d* obj, double k);
void resize_scene_model(HDC dc, model3d* obj, point_double_3d k);
void move_scene_model(HDC dc, model3d* obj, double distance, direction direction);

const double RESIZE_K = 0.1;
camera_3d* camera;
std::vector<model3d*> objects;

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создаётся окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// имя оконного класса, используемое при создании экземпляров окна
	RegisterClass(&wc);								// регистрация класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// имя оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200,200,400,400,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}



LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
    switch (msg)
    {
    case WM_CREATE:
    {

        load_camera(GetDC(hWnd));
        load_scene_objects("objects.txt");

        InvalidateRect(hWnd, nullptr, false);
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
        draw_scene_objects(hdc);

        EndPaint(hWnd, &ps);
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
        InvalidateRect(hWnd, nullptr, false);
    }
    break;

    case WM_MOUSEMOVE:
    {
        if (camera!=nullptr)
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

    }
    break;

    case WM_LBUTTONUP:
    {
        camera->stop_dragging();
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
                rotate_scene_model(dc, object, rotation_type_3d::ordinate, -radian);
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
            for (const auto& object : objects)
            {
                resize_scene_model(
                    dc,
                    object,
                    point_double_3d{ 0.9,0.9,0.9 }
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
	    {
	        return DefWindowProc(hWnd, msg, wParam, lParam);
	    }
    }

    return 0;
}


int load_camera(const HDC dc)
{
    RECT screen_rectangle;
    GetClientRect(WindowFromDC(dc), &screen_rectangle);

    camera = new camera_3d(
        point_double_3d(0, 0, 0),
        vector_3d(0, 0, 1),
        vector_3d(0, 1, 0),
        24.0,
        point_double(200.0 / 25.0, 200.0 / 25.0),
        static_cast<double>(screen_rectangle.right),
        static_cast<double>(screen_rectangle.bottom),
        point_double(175.0, 150.0)
    );

    return 1;

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
        std::string verticies_buffer;
        std::string faces_buffer;
        file >> model_name;
        double x, y, z;
        file >> x;
        file >> y;
        file >> z;

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

            //Вносим новый объект в вектор объектов
            objects.push_back(
                new model3d_impl(
                    verticies,
                    faces,
                    { x,y,z }
                )
            );

            verticies.clear();
            faces.clear();

        }

    }

    file.close();
    verticies_file.close();
    faces_file.close();
}

void mirror_scene_model(HDC dc, model3d* obj, mirror_type_3d mirror_type)
{
    obj->mapping(dc, mirror_type);
}

void rotate_scene_model(HDC dc, model3d* obj, rotation_type_3d type)
{
    const double radian = sqrt(3) / 2;

    obj->rotate(dc, type, radian);
}

void rotate_scene_model(
    HDC dc,
    model3d* obj,
    const rotation_type_3d rotation_type,
    const double radians
)
{
    obj->rotate(dc, rotation_type, radians);
}

void move_scene_model(HDC dc, model3d* obj, double distance, const direction _direction)
{
    if (_direction == direction::DOWN)
    {

        obj->move(dc, { 0.0, -distance, 0.0 });

    }
    if (_direction == direction::UP)
    {

        obj->move(dc, { 0.0, distance, 0.0 });

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
    obj->resize(dc, point_double_3d{ k, k, k });

}

void resize_scene_model(HDC dc, model3d* obj, point_double_3d k)
{
    obj->resize(dc, k);

}

int draw_scene_objects(HDC dc)
{
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

        return 0;

    }

    return -2;//Failure code. Number of objects was zero
}