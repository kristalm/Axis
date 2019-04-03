#include "view.h"

DirectX::XMMATRIX end::view_t::get_view_matrix()
{
	return view;
}

DirectX::XMMATRIX end::view_t::get_world_matrix()
{
	return world;
}

DirectX::XMMATRIX end::view_t::get_proj_matrix()
{
	return proj;
}
void end::view_t::set_r_matrix(DirectX::XMVECTOR vector, int i)
{
	view.r[i] = vector;
}
void end::view_t::set_view_matrix(DirectX::XMMATRIX _view)
{
	view = _view;
}

void end::view_t::set_world_matrix(DirectX::XMMATRIX _world)
{
	world = _world;
}

void end::view_t::set_proj_matrix(DirectX::XMMATRIX _proj)
{
	proj = _proj;
}

DirectX::XMFLOAT4 end::view_t::get_position()
{
	return position;
}

DirectX::XMFLOAT4 end::view_t::get_lookat()
{
	return lookat;
}

DirectX::XMFLOAT4 end::view_t::get_vertical()
{
	return vertical;
}

void end::view_t::set_position(DirectX::XMFLOAT4 _position)
{
	position = _position;
}

void end::view_t::set_lookat(DirectX::XMFLOAT4 _lookat)
{
	lookat = _lookat;
}

void end::view_t::set_vertical(DirectX::XMFLOAT4 _vertical)
{
	vertical = _vertical;
}
