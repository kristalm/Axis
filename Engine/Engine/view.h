#pragma once

#include <DirectXMath.h>
#include "math_types.h"

namespace end
{
	class view_t
	{
		// stores properties of a view
		//
		//	type information 				(optional) (Orthographic/Perspective/Cubemap/Shadowmap/etc)
		//	render target id(s)				(optional)
		//	viewport id						(optional)
		//	view and projection matrices	(REQUIRED)
		//		IMPORTANT: 
		//			Do not store an inverted view matrix.
		//			It will be much easier on you, me, and the CPU this way.
		//			When updating your constant buffers, send an inverted copy of the stored view matrix.
		


		// maintains a visible-set of renderable objects in view (implemented in a future assignment)
	public:
		DirectX::XMMATRIX get_view_matrix();
		DirectX::XMMATRIX get_world_matrix();
		DirectX::XMMATRIX get_proj_matrix();
		void set_r_matrix(DirectX::XMVECTOR vector, int i);

		void set_view_matrix(DirectX::XMMATRIX _view);
		void set_world_matrix(DirectX::XMMATRIX _world);
		void set_proj_matrix(DirectX::XMMATRIX _proj);

		DirectX::XMFLOAT4 get_position();
		DirectX::XMFLOAT4 get_lookat();
		DirectX::XMFLOAT4 get_vertical();

		void set_position(DirectX::XMFLOAT4 _position);
		void set_lookat(DirectX::XMFLOAT4 _lookat);
		void set_vertical(DirectX::XMFLOAT4 _vertical);
		view_t() {}
	private:
		//Private variables
		DirectX::XMMATRIX world, view, proj;
		DirectX::XMFLOAT4 position, lookat, vertical;
	};
}