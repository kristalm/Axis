#include <chrono>
#include "main_window.h"
#include "debug_renderer.h"
#include <time.h>
#include "pools.h"
#include <windowsx.h>
namespace
{
	HWND create_window(int width, int height, void* user_data);
}

namespace end
{
#define particle_size 1000

	
	int main_window_t::launch(main_window_t& main_window)
	{
		main_window.hwnd = create_window(main_window.width, main_window.height, &main_window);

		if (main_window.hwnd == nullptr)
			return -1;
		//time_t start = time(0);
		auto last_time = std::chrono::high_resolution_clock::now();

		main_window.initialize();
		ShowWindow(main_window.hwnd, SW_SHOW);

		MSG msg = { nullptr };
		auto start = clock();
		

		
		main_window.render = new renderer(main_window.hwnd);
		while (true)
		{
			BOOL message_recieved = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);

			if (message_recieved)
			{
				if (msg.message == WM_QUIT)
				{
					main_window.shutdown();
					return (int)msg.wParam;
				}
				if(msg.message == WM_KEYDOWN)
				{
					main_window.Move(msg.wParam);
				}
				if (msg.message == WM_MOUSEMOVE)
				{
					int x = LOWORD(msg.lParam);
					int y = HIWORD(msg.lParam);
					main_window.MouseMove(x, y);
				}
				if (msg.message == WM_LBUTTONDOWN)
				{
					main_window.l_isPressed = true;
				}
				if (msg.message == WM_LBUTTONUP)
				{
					main_window.l_isPressed = false;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				auto current_time = std::chrono::high_resolution_clock::now();
				auto delta_us = std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_time);
				last_time = current_time;
				
				main_window.update(delta_us.count());
				
				main_window.render->draw();
			}
		}

		return 0;
	}



	//HWND main_window_t::window_hwnd()
	//{
	//	return hwnd;
	//}

	void main_window_t::Move(WPARAM wparam)
	{
		switch (wparam)
		{
		case 0x57:
			z_move -= 1.0f;
			break;
		case 0x53:
			z_move += 1.0f;
			break;
		case 0x41:
			x_move -= 1.0f;
			break;
		case 0x44:
			x_move += 1.0f;
			break;
		}
	}

	void main_window_t::MouseMove(int _x, int _y)
	{
		
		float delta_x, delta_y;
		XMMATRIX rotation_x, rotation_y;
		if (l_isPressed)
		{
			XMVECTOR temp = render->default_view.get_view_matrix().r[3];
			render->default_view.set_r_matrix({ 0,0,0,1 }, 3);
			delta_x = x - _x;
			delta_y = y - _y;

			rotation_y = XMMatrixRotationY(delta_y * 0.00005f);
			rotation_x = XMMatrixRotationX(delta_x * 0.00005f);

			render->default_view.set_view_matrix(XMMatrixMultiply(rotation_x, render->default_view.get_view_matrix()));
			render->default_view.set_view_matrix(XMMatrixMultiply(rotation_y, render->default_view.get_view_matrix()));

			render->default_view.set_r_matrix(temp, 3);
		}
		else
		{
			x = _x;
			y = _y;
		}
	}

	LRESULT main_window_t::on_message(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		switch (uMsg)
		{
		case WM_CREATE:
		{
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
			break;
		}

		if (main_window_t* main_window = (main_window_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA))
			return main_window->on_message(uMsg, wParam, lParam);

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

namespace
{
	HWND create_window(int width, int height, void* user_data)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSEX wcex;

		ZeroMemory(&wcex, sizeof(WNDCLASSEX));

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = end::WindowProc;
		wcex.hInstance = hInstance;
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszClassName = L"main_window_class";

		if (size_t wnd_class_id = RegisterClassEx(&wcex))
		{
			DWORD wnd_style = WS_POPUPWINDOW | WS_CAPTION;
			DWORD wnd_exstyle = 0;
			LPCTSTR wnd_title = L"main_window";

			return CreateWindowEx
			(
				wnd_exstyle, 
				(LPCWSTR)wnd_class_id, 
				wnd_title,
				wnd_style, 
				CW_USEDEFAULT, 
				0, 
				width, 
				height, 
				0, 
				0, 
				hInstance, 
				user_data
			);
		}

		return 0;
	}
}