#include "debug_renderer.h"
#include <array>
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

struct VERTEX
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 UnVnW;
	XMFLOAT3 normalz;
};

namespace end
{
	constexpr size_t MAX_LINE_VERTS = 2048; // Maximum number of debug lines at one time (i.e: Capacity)

	size_t line_vert_count = 0;
	std::array< end::colored_vertex, MAX_LINE_VERTS> line_verts;
}

namespace end
{
	namespace debug_renderer

	{
		void add_line(float3 p, float3 q, float3 color)
		{
			// Add points to debug_verts, increments debug_vert_count

			colored_vertex line1;
			colored_vertex line2;

			line1.pos.x = p.x;
			line1.pos.y = p.y;
			line1.pos.z = p.z;

			line1.color.x = color.x;
			line1.color.y = color.y;
			line1.color.z = color.z;

			line2.pos.x = q.x;
			line2.pos.y = q.y;
			line2.pos.z = q.z;

			line2.color.x = color.x;
			line2.color.y = color.y;
			line2.color.z = color.z;

			if (line_vert_count < MAX_LINE_VERTS)
			{
				line_verts[line_vert_count].pos = line1.pos;
				line_verts[line_vert_count].color.xyz = line1.color.xyz;

				line_vert_count++;

				line_verts[line_vert_count].pos = line2.pos;
				line_verts[line_vert_count].color.xyz = line2.color.xyz;

				line_vert_count++;
			}
			else if (line_vert_count == MAX_LINE_VERTS)
			{
				return;
			}
		}

		void clear_lines()
		{
			// Resets debug_vert_count
			line_vert_count = 0;
		}

		const end::colored_vertex* get_line_verts()
		{
			return line_verts.data();
		}

		size_t get_line_vert_count()
		{
			return line_vert_count;
		}

		size_t get_line_vert_capacity()
		{
			return line_verts.size();
		}
		DirectX::XMMATRIX lookatfcn(DirectX::XMVECTOR cameraPositon, DirectX::XMVECTOR targetPosition, DirectX::XMVECTOR localUP)
		{
			float3 color;

			color.x = 0.9137f;
			color.y = 1.0f;
			color.z = 0.0f;

			debug_renderer::add_line(*(float3*)&cameraPositon, *(float3*)&targetPosition, color);

			DirectX::XMMATRIX tempMATRIX;

			DirectX::XMVECTOR zValue;

			zValue = DirectX::XMVectorSubtract(targetPosition, targetPosition);

			zValue = DirectX::XMVector3Normalize(zValue);

			DirectX::XMVECTOR xValue;

			xValue = DirectX::XMVector3Cross(localUP, zValue);

			xValue = DirectX::XMVector3Normalize(xValue);

			DirectX::XMVECTOR yValue;

			yValue = DirectX::XMVector3Cross(zValue, xValue);

			yValue = DirectX::XMVector3Normalize(yValue);

			tempMATRIX = DirectX::XMMatrixIdentity();

			tempMATRIX.r[0] = xValue;
			tempMATRIX.r[1] = yValue;
			tempMATRIX.r[2] = zValue;
			tempMATRIX.r[3] = cameraPositon;

			return tempMATRIX;
		}

	}
}