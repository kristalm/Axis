#pragma once
#include <memory>
#include "view.h"

#define FSGD_END_USE_D3D

namespace end
{
	// APIs/Platforms have their own types for representing a handle to a 'window'
	// They will generally all fit in the 8-bytes of a void*.
	// HWND is actually just a typedef/alias for a void*.
	using native_handle_type = void*;

	/// Interface to the renderer
	class renderer
	{
	public:
		renderer(native_handle_type window_handle);

		~renderer();

		void draw();

		view_t default_view;

	private:

		//TODO: PImpl idiom ("Pointer to implementation")

		/// 'impl' will define the implementation for the renderer elsewhere
		struct impl;

		/// Pointer to the implementation
		impl* p_impl;
	};	
}