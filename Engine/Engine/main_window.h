#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <cstdint>
#include "math_types.h"
#include "debug_renderer.h"
#include "pools.h"
#pragma comment(lib, "d3d11.lib")
#include <DirectXMath.h>
#include "renderer.h"

using namespace DirectX;

#define PARTICLE 0
#define GRID 1
int main(int argc, char* argv[]);

namespace end
{
	class main_window_t
	{
	public:
		main_window_t() = default;
		HWND window_hwnd() { return hwnd; }
		void Move(WPARAM wparam);
		void MouseMove(int x, int y);
		bool l_isPressed = false;
		renderer *render;
		int x, y;

	protected:
		HWND hwnd = 0;
		int width = 1280;
		int height = 720;

		//grid
		float sec = 0;
		float seconds = 0;
		float seconds1 = 0;

		float x_move = 5.0f;
		float y_move = 0.0f;
		float z_move = 5.0f;

		DirectX::XMMATRIX matrix_1;
		DirectX::XMMATRIX matrix_2;
		DirectX::XMMATRIX matrix_3;

		XMMATRIX translation;
	private:
#pragma region PARTICLE
		struct particle
		{
			float3 pos, prev_pos;
			float4 color;

			particle()
			{
				pos = { 0,0,0 };

				prev_pos = { rand() % 10 * -0.0015f , rand() % 10 * -0.005f , rand() % 10 * -0.0015f };

				color = { rand() % 10 * 0.10f, rand() % 10 * 0.10f, rand() % 10 * 0.10f };
				color.w = 1.0f;

			}
		};
		struct particle2
		{
			float3 pos;
			float3 prev_pos;
			float4 color;

			particle2()
			{
				pos = { 0,0,0 };

				prev_pos = { rand() % 10 * 0.0015f, rand() % 10 * -0.005f, rand() % 10 * 0.0015f };

				color = { rand() % 10 * 0.10f, rand() % 10 * 0.10f, rand() % 10 * 0.10f };
				color.w = 1.0f;

			}
		};

		pool <particle, 800> free_list;
		sorted_pool <particle2, 800> sorted_pool;
		particle* particle_array[800];
		particle temp_particle;
#pragma endregion PARTICLE
		inline void matrices(float3 x_axis_color, float3 y_axis_color, float3 z_axis_color, float3 x1, float3 x2, float3 y1, float3 y2, float3 z1, float3 z2)
		{
			debug_renderer::add_line(x1, x2, x_axis_color);
			debug_renderer::add_line(y1, y2, y_axis_color);
			debug_renderer::add_line(z1, z2, z_axis_color);
		}
		inline XMMATRIX look_at(XMVECTOR cam_pos, XMVECTOR target_pos, XMVECTOR up)
		{
			float3 color = { 0.9137f, 1.0f, 0.0f };

			debug_renderer::add_line(*reinterpret_cast<float3*>(&cam_pos), *reinterpret_cast<float3*>(&target_pos), color);

			XMVECTOR z = target_pos - cam_pos;
			z = XMVector3Normalize(z);

			XMVECTOR x = XMVector3Cross(up, z);
			x = XMVector3Normalize(x);

			XMVECTOR y = XMVector3Cross(z, x);
			y = XMVector3Normalize(y);

			XMMATRIX temp = XMMatrixIdentity();

			temp.r[0] = x;
			temp.r[1] = y;
			temp.r[2] = z;
			temp.r[3] = cam_pos;

			return temp;
		}

		static inline XMMATRIX turn_too(XMMATRIX view_matrix, XMVECTOR target_pos, float speed)
		{
			XMMATRIX temp_matrix{};
			XMVECTOR x, y, z;
			const float3 color = { 1.0f, 0.349f, 0.0f };

			debug_renderer::add_line(*reinterpret_cast<float3*>(&view_matrix.r[3]), *reinterpret_cast<float3*>(&target_pos), color);

			const auto view = view_matrix.r[3];
			auto v = (target_pos - view);
			v = XMVector3Normalize(v);

			auto check = view_matrix.r[0];
			check = XMVector3Normalize(check);
			const auto temp_vec = XMVector3Dot(v, check);

			const auto dot = XMVectorGetY(temp_vec);

			if (dot > 0)
			{
				const auto rotate_left = XMMatrixRotationY(dot * speed);
				temp_matrix = XMMatrixMultiply(rotate_left, view_matrix);

				return temp_matrix;
			}
			else
			{
				const auto rotate_right = XMMatrixRotationY(-dot * speed);
				temp_matrix = XMMatrixMultiply(rotate_right, view_matrix);
				return temp_matrix;
			}
		}
		friend int ::main(int argc, char* argv[]);

		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		inline virtual void update(int64_t us) 
		{
			float3 red = { 1.0, 0.0f, 0.0f };
			float3 green = { 0.0f, 1.0f, 0.0f };
			float3 blue = { 0.0f, 0.0f, 1.0f };
#pragma region GRID
#if GRID

			auto ratio = (sec / 10) * 5;

			float3 c;

			if (sec >= 0 && sec < 3)
			{
				c = lerp(red, green, ratio);
			}
			if (sec >= 3 && sec < 8)
			{
				ratio = ((sec - 3) / 10) * 5;
				c = lerp(green, blue, ratio);
			}
			if (sec >= 8 && sec < 11)
			{
				ratio = ((sec - 8) / 10) * 5;
				c = lerp(blue, red, ratio);
			}
			if (sec >= 11)
			{
				sec = 0;
				c = lerp(red, green, ratio);
			}

			for (float i = -20.0f; i <= 20.0f; i += 1.0f)
			{
				float3 point1 = { 0 };
				point1.x = i;
				point1.z = -20.0f;

				float3 point2 = { 0 };
				point2.x = i;
				point2.z = 20.0f;

				float3 point3 = { 0 };
				point3.x = -20.0f;
				point3.z = i;

				float3 point4 = { 0 };
				point4.x = 20.0f;
				point4.z = i;
				debug_renderer::add_line(point1, point2, c);
				debug_renderer::add_line(point3, point4, c);
			}
#endif
#pragma endregion GRID
#pragma region PARTICLE
#if PARTICLE
			seconds = 0;
			seconds += (us * 0.000001f);
			float t = (us * 0.000001f);

			float3 acc = { 0.0f, -1.0f, 0.0f };
			float3 pos, prev_pos, delta;

			if (seconds < 0.8f)
			{
				for (auto & i : particle_array)
				{
					if (i == nullptr)
					{
						particle _particle{};
						i = free_list.allocate(_particle);
					}
				}

			}

			if (seconds < 0.8f)
			{
				for (int i = 0; i < 800; i++)
				{

					if (sorted_pool.allocate() == nullptr)
						break;
				}
			}

			for (auto & i : particle_array)
			{
				if (i == nullptr)
					continue;

				debug_renderer::add_line(i->pos, i->prev_pos, i->color.xyz);
			}

			for (int i = 0; i < sorted_pool.activeParticle(); i++)
			{
				debug_renderer::add_line(sorted_pool[i].pos, sorted_pool[i].prev_pos, sorted_pool[i].color.xyz);
			}

			for (auto & i : particle_array)
			{
				if (i == nullptr)
				{
					particle Part{};

					i = free_list.allocate(Part);
					if (i == nullptr)
						continue;
				}


				pos = { i->pos.x, i->pos.y, i->pos.z };

				prev_pos = { i->prev_pos.x, i->prev_pos.y, i->prev_pos.z };

				delta = { pos.x - prev_pos.x, pos.y - prev_pos.y, pos.z - prev_pos.z };

				prev_pos = { pos.x, pos.y, pos.z };

				pos.x += (delta.x) + acc.x * (t);
				pos.y += (delta.y) + acc.y * (t);
				pos.z += (delta.z) + acc.z * (t);

				i->pos = { pos.x, pos.y, pos.z };

				i->prev_pos = { prev_pos.x, prev_pos.y, prev_pos.z };

				if (pos.y < 0 || pos.y > 100)
				{
					free_list.free(i);
					i = nullptr;
				}

			}

			for (int i = 0; i < sorted_pool.activeParticle(); i++)
			{
				pos = { sorted_pool[i].pos.x, sorted_pool[i].pos.y, sorted_pool[i].pos.z };

				prev_pos = { sorted_pool[i].prev_pos.x, sorted_pool[i].prev_pos.y, sorted_pool[i].prev_pos.z };

				delta = { pos.x - prev_pos.x, pos.y - prev_pos.y, pos.z - prev_pos.z };

				prev_pos = { pos.x, pos.y, pos.z };

				pos.x += (delta.x) + acc.x * (t);
				pos.y += (delta.y) + acc.y * (t);
				pos.z += (delta.z) + acc.z * (t);

				sorted_pool[i].pos = { pos.x, pos.y, pos.z };
				sorted_pool[i].prev_pos = { prev_pos.x, prev_pos.y, prev_pos.z };

				if (pos.y < 0 || pos.y > 100)
				{
					sorted_pool.free(&sorted_pool[i]);

					sorted_pool.allocate();
				}

			}
#endif 
#pragma endregion PARTICLE
			

			float3 x1 = { 0.0f, 0.0f, 0.0f };
			float3 x2 = { 2.0f, 0.0f, 0.0f };
			float3 y1 = { 0.0f, 0.0f, 0.0f };
			float3 y2 = { 0.0f, 2.0f, 0.0f };
			float3 z1 = { 0.0f, 0.0f, 0.0f };
			float3 z2 = { 0.0f, 0.0f, 2.0f };

			matrices(red, green, blue, x1, x2, y1, y2, z1, z2);

			//x1 = { -15.0f, 0.0f, 0.0f };
			DirectX::XMVECTOR x_vec1_1 = { 0, 0, 0, 1 };
			x_vec1_1 = DirectX::XMVector4Transform(x_vec1_1, matrix_1);

			//x2 = { -17.0f, 0.0f, 0.0f };
			DirectX::XMVECTOR x_vec2_1 = { 2, 0, 0, 1 };
			x_vec2_1 = DirectX::XMVector4Transform(x_vec2_1, matrix_1);

			//y1 = { -15.0f, 0.0f, 0.0f };
			DirectX::XMVECTOR y_vec1_1 = { 0, 0, 0, 1 };
			y_vec1_1 = DirectX::XMVector4Transform(y_vec1_1, matrix_1);

			//y2 = { -15.0f, 2.0f, 0.0f };
			DirectX::XMVECTOR y_vec2_1 = { 0, 2, 0, 1 };
			y_vec2_1 = DirectX::XMVector4Transform(y_vec2_1, matrix_1);

			//z1 = { -15.0f, 0.0f, 0.0f };
			DirectX::XMVECTOR z_vec1_1 = { 0, 0, 0, 1 };
			z_vec1_1 = DirectX::XMVector4Transform(z_vec1_1, matrix_1);

			//z2 = { -15.0f, 0.0f, 2.0f };
			DirectX::XMVECTOR z_vec2_1 = { 0, 0, 2, 1 };
			z_vec2_1 = DirectX::XMVector4Transform(z_vec2_1, matrix_1);

			matrices(red, green, blue, *reinterpret_cast<float3*>(&x_vec1_1), *reinterpret_cast<float3*>(&x_vec2_1),
			        *reinterpret_cast<float3*>(&y_vec1_1), *reinterpret_cast<float3*>(&y_vec2_1),
			        *reinterpret_cast<float3*>(&z_vec1_1), *reinterpret_cast<float3*>(&z_vec2_1));

			//x1 = { -25.0f, 0.0f, -15.0f };
			DirectX::XMVECTOR x_vec1_2 = { 0, 0, 0, 1 };
			x_vec1_2 = DirectX::XMVector4Transform(x_vec1_2, matrix_2);

			//x2 = { -23.0f, 0.0f, -15.0f };
			DirectX::XMVECTOR x_vec2_2 = { 4, 0, 0, 1 };
			x_vec2_2 = DirectX::XMVector4Transform(x_vec2_2, matrix_2);

			//y1 = { 10.0f, 0.0f, -15.0f };
			DirectX::XMVECTOR y_vec1_2 = { 0, 0, 0, 1 };
			y_vec1_2 = DirectX::XMVector4Transform(y_vec1_2, matrix_2);

			//y2 = { 10.0f, 2.0f, 15.0f };
			DirectX::XMVECTOR y_vec2_2 = { 0, 4, 0, 1 };
			y_vec2_2 = DirectX::XMVector4Transform(y_vec2_2, matrix_2);

			//z1 = { 10.0f, 0.0f, -15.0f };
			DirectX::XMVECTOR z_vec1_2 = { 0, 0, 0, 1 };
			z_vec1_2 = DirectX::XMVector4Transform(z_vec1_2, matrix_2);

			//z2 = { 10.0f, 0.0f, -17.0f };
			DirectX::XMVECTOR z_vec2_2 = { 0, 0, 2, 1 };
			z_vec2_2 = DirectX::XMVector4Transform(z_vec2_2, matrix_2);

			matrices(red, green, blue, *reinterpret_cast<float3*>(&x_vec1_2), *reinterpret_cast<float3*>(&x_vec2_2),
				*reinterpret_cast<float3*>(&y_vec1_2), *reinterpret_cast<float3*>(&y_vec2_2),
				*reinterpret_cast<float3*>(&z_vec1_2), *reinterpret_cast<float3*>(&z_vec2_2));

			translation = XMMatrixTranslation(x_move, y_move, z_move);
			matrix_3 = translation;

			float3 x_move_1 = { x_move, y_move, z_move };
			float3 x_move_2 = { x_move + 2, y_move, z_move };

			float3 y_move_1 = { x_move, y_move, z_move };
			float3 y_move_2 = { x_move, y_move + 2, z_move };

			float3 z_move_1= { x_move, y_move, z_move };
			float3 z_move_2 = { x_move, y_move, z_move + 2 };

			matrices(red, green, blue, x_move_1, x_move_2, y_move_1, y_move_2, z_move_1, z_move_2);

			XMVECTOR camera_pos = matrix_1.r[3];
			XMVECTOR target_pos = matrix_3.r[3];
			XMVECTOR local_up = { 0.0f, 1.0f, 0.0f, 0.0f };
			XMMATRIX view = matrix_2;
			XMVECTOR target_pos_2 = matrix_3.r[3];
			float speed = 0.1f;

			matrix_1 = look_at(camera_pos, target_pos, local_up);

			matrix_2 = turn_too(view, target_pos, speed);



		};
		inline virtual void initialize() 
		{
			for (auto & i : particle_array)
			{
				i = nullptr;
			}
			matrix_1 = DirectX::XMMatrixTranslation(-15.0f, 0.0f, 0.0f);
			matrix_2 = DirectX::XMMatrixTranslation(25.0f, 0.0f, -15.0f);
			matrix_3 = DirectX::XMMatrixTranslation(5.0f, 0.0f, 5.0f);
		};

		inline virtual void shutdown() {
			delete render;
		};

		virtual LRESULT on_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

		static int launch(main_window_t& main_window);

		
	};
};