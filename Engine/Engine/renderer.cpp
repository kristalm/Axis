#include "renderer.h"

#ifdef FSGD_END_USE_D3D
#include "d3d11_renderer_impl.h"
#endif

namespace end
{
	renderer::renderer(native_handle_type window_handle)
	{
		//p_impl = //create and initialize the implementation
	
		p_impl = new impl(window_handle, default_view);
	}

	renderer::~renderer() 
	{
		// Clean up implementation
		if (p_impl != NULL)
		{
			p_impl = 0;
			delete p_impl;
		}
	}

	void renderer::draw()
	{
		// draw views...
		//p_impl->draw_view(default_view);
		p_impl->draw_view(default_view);
		// draw views...
		// draw views...
		debug_renderer::clear_lines();
	}
}