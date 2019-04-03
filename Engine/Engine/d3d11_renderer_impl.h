#pragma once

#include "renderer.h"
#include "view.h"
#include <d3dcompiler.h>
#include "debug_renderer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dxgi1_2.h>
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>
#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3DCompiler.lib")
// NOTE: This header file must ONLY be included by renderer.cpp
#define triangle 1
#define mesh 0
#define particle 2
#define onMesh 0
#define onTriangle 0
#define onLine 1

namespace end
{
	struct renderer::impl
	{
#pragma region VARIABLES
		// platform/api specific members, functions, etc.
		// Device, swapchain, resource views, states, etc. can be members here
		ID3D11Device *device = nullptr;
		ID3D11DeviceContext *context = nullptr;
		IDXGISwapChain *swapchain = nullptr;

		struct VIEW_RENDER_TARGET {
			enum { DEFAULT = 0, COUNT };
		};

		ID3D11RenderTargetView*		render_target[VIEW_RENDER_TARGET::COUNT]{};

		struct VIEW_DEPTH_STENCIL {
			enum { DEFAULT = 0, COUNT };
		};

		ID3D11DepthStencilView*		depthStencilView[VIEW_DEPTH_STENCIL::COUNT]{};

		struct STATE_DEPTH_STENCIL {
			enum { DEFAULT = 0, COUNT };
		};

		ID3D11DepthStencilState*	depthStencilState[STATE_DEPTH_STENCIL::COUNT]{};

		struct DEPTH_BUFFER
		{
			enum { DEFAULT = 0, COUNT };
		};

		ID3D11Texture2D*			depthBuffer[DEPTH_BUFFER::COUNT]{};

		struct VERTEX_BUFFER {
			enum {
				MESH = 0, TRIANGLE, PARTICLE, COUNT };
		};

		ID3D11Buffer*				vertex_buffer[VERTEX_BUFFER::COUNT]{};

		struct INDEX_BUFFER {
			enum { MESH = 0, TRIANGLE, PARTICLE, COUNT };
		};

		ID3D11Buffer*				index_buffer[INDEX_BUFFER::COUNT]{};

		struct INPUT_LAYOUT {
			enum {
				MESH = 0, TRIANGLE, PARTICLE, COUNT };
		};

		ID3D11InputLayout*			input_layout[INPUT_LAYOUT::COUNT]{};

		struct CONSTANT_BUFFER {
			enum {
				MESH = 0, TRIANGLE, PARTICLE, COUNT };
		};

		ID3D11Buffer*				constant_buffer[CONSTANT_BUFFER::COUNT]{};

		struct PIXEL_BUFFER {
			enum { MESH = 0, TRIANGLE, PARTICLE, COUNT };
		};

		ID3D11Buffer*				pixel_buffer[PIXEL_BUFFER::COUNT]{};

		struct VERTEX_SHADER {
			enum { MESH = 0, TRIANGLE, PARTICLE, COUNT };
		};

		ID3D11VertexShader*			vertex_shader[VERTEX_SHADER::COUNT]{};

		struct PIXEL_SHADER {
			enum {
				MESH = 0, TRIANGLE, PARTICLE, COUNT };
		};

		ID3D11PixelShader*			pixel_shader[PIXEL_SHADER::COUNT]{};

		struct STATE_RASTERIZER {
			enum { UNKNOWN = 0, COUNT };
		};

		ID3D11RasterizerState*		rasterState[STATE_RASTERIZER::COUNT]{};

		struct VIEWPORT {
			enum { DEFAULT = 0, COUNT };
		};

		D3D11_VIEWPORT				view_port[VIEWPORT::COUNT]{};

		struct SAMPLER_STATE {
			enum { DEFAULT = 0, COUNT };
		};

		ID3D11SamplerState*			sampler_state[SAMPLER_STATE::COUNT]{};

		//struct DEVICE {
		//	enum { DEFAULT = 0, COUNT };
		//};

		//ID3D11Device*			device[DEVICE::COUNT]{};

		/* Add more as needed...
		enum STATE_SAMPLER{ COUNT };
		ID3D11SamplerState*			sampler_state[STATE_SAMPLER::COUNT]{};

		enum STATE_BLEND{ COUNT };
		ID3D11BlendState*			blend_state[STATE_BLEND::COUNT]{};
		*/

		struct pixel_struct
		{
			DirectX::XMFLOAT4 color;
		};
		struct cbuffer_struct
		{
			DirectX::XMMATRIX world, view, proj;
			DirectX::XMFLOAT4 time;
		};
		struct vertex_type
		{
			float4 pos;
			float4 norm;
			float4 color;
			float2 text;
		};
		struct VERTEX
		{
			DirectX::XMFLOAT3 pos; DirectX::XMFLOAT4 Color;
		};

		uint32_t vertex_count;
		uint32_t index_count;

		int* OurIndices;
		vertex_type* verticies;
		VERTEX *OurVertices;

		bool broken = false;

		float window_height;
		float window_width;

#pragma endregion VARIABLES
		bool load_file()
		{
			uint32_t size;

			float f;
			std::vector<float> verts;

			std::ifstream fin("test.bin", std::ios::binary);

			///read in vertex count
			fin.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
			vertex_count = size;

			verticies = new vertex_type[vertex_count];

			for (size_t i = 0; i < vertex_count; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					fin.read(reinterpret_cast<char*>(&f), sizeof(float));
					verticies[i].pos[j] = f;
				}
				for (size_t j = 0; j < 2; j++)
				{
					fin.read(reinterpret_cast<char*>(&f), sizeof(float));
					verticies[i].text[j] = f;
				}
				for (size_t j = 0; j < 3; j++)
				{
					fin.read(reinterpret_cast<char*>(&f), sizeof(float));
					verticies[i].norm[j] = f;
				}
				for (size_t j = 0; j < 3; j++)
				{
					verticies[i].color[j] = 1.0f;
				}
				verticies[i].color.w = 1.0f;
				verticies[i].norm.w = 1.0f;
			}
			fin.read(reinterpret_cast<char*>(&size), sizeof(float));
			index_count = size;

			OurIndices = new int[index_count];
			uint32_t indicy;
			for (size_t i = 0; i < index_count; i++)
			{
				fin.read(reinterpret_cast<char*>(&indicy), sizeof(uint32_t));
				OurIndices[i] = (int)indicy;
			}

			return 0;
		}

		// Constructor for renderer implementation
		//
		impl(native_handle_type window_handle, view_t& default_view)
		{
			ID3D11Texture2D* back_buffer_ptr;
			//***************************************************
				//DONE: Set UP view
				// Setup the default view
				//	default_view should be setup with a perspective projection
				//	default_view should have a view matrix that positions the view at(0, 15, 15) and looks at(0, 0, 0)
				//		IMPORTANT: XMMatrixLookAtLH returns an inverted matrix
			//***************************************************

			RECT window_size;
			GetClientRect((HWND)window_handle, &window_size);
			window_height = ((float)window_size.bottom - (float)window_size.top);   // (float)windowStuffs.top
			window_width = ((float)window_size.right - (float)window_size.left);     // (float)windowStuffs.left

			default_view.set_world_matrix(DirectX::XMMatrixIdentity());

			default_view.set_position({ 15.0f, 15.0f, 15.0f, 1.0f });
			default_view.set_lookat({ 0.0f, 0.0f, 0.0f, 1.0f });
			default_view.set_vertical({ 0.0f, 1.0f, 0.0f, 1.0f });

			DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH({ 15.0f, 15.0f, 15.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

			default_view.set_view_matrix(view);

			DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH((float)DirectX::XMConvertToRadians(45), (float)window_width / window_height, 1.0f, 1000.0f);

			default_view.set_proj_matrix(proj);

			//****************************************************
				//DONE: Inital Setup
				// create device and swap chain
				// create depth-stencil buffer/state/view
				// create rasterizer state
				// create input layout
				// create vertex and pixel shaders
				// load in mesh
				// fill buffers
			//*****************************************************\

			DXGI_SWAP_CHAIN_DESC scd;
			ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
			scd.BufferCount = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;
			scd.Windowed = true;
			scd.BufferDesc.Height = window_height;
			scd.BufferDesc.Width = window_width;
			scd.OutputWindow = (HWND)window_handle;

			/// create device and swapchain
			HRESULT check = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &device, NULL, &context);

			swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);

			check = device->CreateRenderTargetView(back_buffer_ptr, NULL, &render_target[0]);

			back_buffer_ptr->Release();

			// create depth-stencil buffer/state/view

			///Create Buffer

			D3D11_TEXTURE2D_DESC depth_buffer_desc;
			ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
			depth_buffer_desc.Width = window_width;
			depth_buffer_desc.Height = window_height;
			depth_buffer_desc.MipLevels = 1;
			depth_buffer_desc.ArraySize = 1;
			depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depth_buffer_desc.SampleDesc.Count = 1;
			depth_buffer_desc.SampleDesc.Quality = 0;
			depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depth_buffer_desc.CPUAccessFlags = 0;
			depth_buffer_desc.MiscFlags = 0;

			device->CreateTexture2D(&depth_buffer_desc, NULL, &depthBuffer[0]);

			///Create Depth Stencil

			D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;

			ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

			depth_stencil_desc.DepthEnable = true;
			depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
			depth_stencil_desc.StencilEnable = true;
			depth_stencil_desc.StencilReadMask = 0xff;
			depth_stencil_desc.StencilWriteMask = 0xff;
			depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			check = device->CreateDepthStencilState(&depth_stencil_desc, &depthStencilState[0]);

			context->OMSetDepthStencilState(depthStencilState[0], 1);

			///Create View

			D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
			ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

			depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depth_stencil_view_desc.Texture2D.MipSlice = 0;
			device->CreateDepthStencilView(depthBuffer[0], &depth_stencil_view_desc, &depthStencilView[0]);

			context->OMSetRenderTargets(1, &render_target[0], depthStencilView[0]);

			///Create rasterstate

			D3D11_RASTERIZER_DESC rasterizer_desc;

			rasterizer_desc.AntialiasedLineEnable = false;
			rasterizer_desc.CullMode = D3D11_CULL_BACK;
			rasterizer_desc.DepthBias = 0;
			rasterizer_desc.DepthBiasClamp = 0.0f;
			rasterizer_desc.DepthClipEnable = true;
			rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizer_desc.FrontCounterClockwise = false;
			rasterizer_desc.MultisampleEnable = false;
			rasterizer_desc.ScissorEnable = false;
			rasterizer_desc.SlopeScaledDepthBias = 0.0f;

			device->CreateRasterizerState(&rasterizer_desc, &rasterState[0]);

			context->RSSetState(rasterState[0]);

			D3D11_VIEWPORT _viewport;

			_viewport.Width = static_cast<float>(window_width);
			_viewport.Height = static_cast<float>(window_height);
			_viewport.MinDepth = 0.0f;
			_viewport.MaxDepth = 1.0f;
			_viewport.TopLeftX = 0.0f;
			_viewport.TopLeftY = 0.0f;

			context->RSSetViewports(1, &_viewport);


#if onMesh
			//DONE: load
			// load your fbx mesh
			// create buffers for drawing your loaded fbx mesh
			load_file();

			OurVertices = new VERTEX[vertex_count];

			for (size_t i = 0; i < vertex_count; i++)
			{
				OurVertices[i].pos.x = verticies[i].pos.x;
				OurVertices[i].pos.y = verticies[i].pos.y;
				OurVertices[i].pos.z = verticies[i].pos.z;

				OurVertices[i].Color.x = verticies[i].color.x;
				OurVertices[i].Color.y = verticies[i].color.y;
				OurVertices[i].Color.z = verticies[i].color.z;
				OurVertices[i].Color.w = verticies[i].color.w;
			}

			//Create buffer descs
			D3D11_BUFFER_DESC vertex_buffer_desc;
			D3D11_BUFFER_DESC index_buffer_desc;

			//Fill out descs
			vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			vertex_buffer_desc.ByteWidth = sizeof(VERTEX) * vertex_count; //sizeof(struct type)
			vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer_desc.CPUAccessFlags = 0;
			vertex_buffer_desc.MiscFlags = 0;
			vertex_buffer_desc.StructureByteStride = 0;

			index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			index_buffer_desc.ByteWidth = sizeof(unsigned int) * index_count;
			index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_buffer_desc.CPUAccessFlags = 0;
			index_buffer_desc.MiscFlags = 0;
			index_buffer_desc.StructureByteStride = 0;

			//Create subresource structure
			D3D11_SUBRESOURCE_DATA vertex_data;
			D3D11_SUBRESOURCE_DATA index_data;

			//fill out subresource data
			vertex_data.pSysMem = OurVertices;
			vertex_data.SysMemPitch = 0;
			vertex_data.SysMemSlicePitch = 0;

			index_data.pSysMem = OurIndices;
			index_data.SysMemPitch = 0;
			index_data.SysMemSlicePitch = 0;

			//Create buffers
			check = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer[mesh]);

			check = device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer[mesh]);

			///Create input layout
			ID3D10Blob *vertex_shader_blob, *pixel_shader_blob, *error_message;
			vertex_shader_blob = { 0 };
			pixel_shader_blob = { 0 };
			error_message = { 0 };
			check = D3DCompileFromFile(L"basic_shader.hlsl", 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex_shader_blob, &error_message);
			if (FAILED(check))
			{
				// If the shader failed to compile it should have written something to the error message.
				if (error_message)
				{
					auto error = (char*)error_message->GetBufferPointer();
				}
			}

			check = D3DCompileFromFile(L"basic_shader.hlsl", 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel_shader_blob, &error_message);
			if (FAILED(check))
			{
				// If the shader failed to compile it should have written something to the error message.
				if (error_message)
				{
					auto error = (char*)error_message->GetBufferPointer();
				}
			}

			device->CreateVertexShader(vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), NULL, &vertex_shader[mesh]);
			device->CreatePixelShader(pixel_shader_blob->GetBufferPointer(), pixel_shader_blob->GetBufferSize(), NULL, &pixel_shader[mesh]);

			// set the shader objects
			context->VSSetShader(vertex_shader[mesh], 0, 0);
			context->PSSetShader(pixel_shader[mesh], 0, 0);

			// create the input layout object
			D3D11_INPUT_ELEMENT_DESC ied[] =
			{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			device->CreateInputLayout(ied, 2, vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), &input_layout[mesh]);
			context->IASetInputLayout(input_layout[mesh]);

			D3D11_BUFFER_DESC bd1;
			ZeroMemory(&bd1, sizeof(bd1));

			bd1.Usage = D3D11_USAGE_DYNAMIC;
			bd1.ByteWidth = sizeof(DirectX::XMMATRIX);
			bd1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			device->CreateBuffer(&bd1, NULL, &constant_buffer[mesh]);
#endif

#if onTriangle
			VERTEX *TriangleVerts = new VERTEX[5];

			TriangleVerts[0].pos = { -1.0f, -1.0f, 1.0f };
			TriangleVerts[1].pos = { 1.0f, -1.0f, 1.0f };
			TriangleVerts[2].pos = { -1.0f, -1.0f, -1.0f };
			TriangleVerts[3].pos = { 1.0f, -1.0f, -1.0f };
			TriangleVerts[4].pos = { 0.0f, 1.0f, 0.0f };

			TriangleVerts[0].Color = { 0.0f, 1.0f, 0.0f, 1.0f };
			TriangleVerts[1].Color = { 0.0f, 0.0f, 1.0f, 1.0f };
			TriangleVerts[2].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
			TriangleVerts[3].Color = { 0.0f, 1.0f, 1.0f, 1.0f };
			TriangleVerts[4].Color = { 0.0f, 1.0f, 0.0f, 1.0f };

			// create the index buffer out of DWORDs
			DWORD TriangleIndices[] =
			{
				0, 2, 1,    // base
				1, 2, 3,
				0, 1, 4,    // sides
				1, 3, 4,
				3, 2, 4,
				2, 0, 4,
			};

			//Create buffer descs
			D3D11_BUFFER_DESC vertex_buffer_desc1;
			D3D11_BUFFER_DESC index_buffer_desc1;

			//Fill out descs
			vertex_buffer_desc1.Usage = D3D11_USAGE_DEFAULT;
			vertex_buffer_desc1.ByteWidth = sizeof(VERTEX) * 10; //sizeof(struct type)
			vertex_buffer_desc1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer_desc1.CPUAccessFlags = 0;
			vertex_buffer_desc1.MiscFlags = 0;
			vertex_buffer_desc1.StructureByteStride = 0;

			index_buffer_desc1.Usage = D3D11_USAGE_DEFAULT;
			index_buffer_desc1.ByteWidth = sizeof(unsigned int) * 18;
			index_buffer_desc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_buffer_desc1.CPUAccessFlags = 0;
			index_buffer_desc1.MiscFlags = 0;
			index_buffer_desc1.StructureByteStride = 0;

			//Create subresource structure
			D3D11_SUBRESOURCE_DATA vertex_data1;
			D3D11_SUBRESOURCE_DATA index_data1;

			//fill out subresource data
			vertex_data1.pSysMem = TriangleVerts;
			vertex_data1.SysMemPitch = 0;
			vertex_data1.SysMemSlicePitch = 0;

			index_data1.pSysMem = TriangleIndices;
			index_data1.SysMemPitch = 0;
			index_data1.SysMemSlicePitch = 0;

			//Create buffers
			check = device->CreateBuffer(&vertex_buffer_desc1, &vertex_data1, &vertex_buffer[triangle]);


			check = device->CreateBuffer(&index_buffer_desc1, &index_data1, &index_buffer[triangle]);


			///Create input layout
			ID3D10Blob *vertex_shader_blob1, *pixel_shader_blob1, *error_message1;
			vertex_shader_blob1 = { 0 };
			pixel_shader_blob1 = { 0 };
			error_message1 = { 0 };
			check = D3DCompileFromFile(L"basic_shader.hlsl", 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex_shader_blob1, &error_message1);
			if (FAILED(check))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (error_message1)
				{
					auto error = (char*)error_message1->GetBufferPointer();
				}

			}

			check = D3DCompileFromFile(L"basic_shader.hlsl", 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel_shader_blob1, &error_message1);
			if (FAILED(check))
			{
				// If the shader failed to compile it should have writen something to the error message.
				if (error_message1)
				{
					auto error = (char*)error_message1->GetBufferPointer();
				}
			}

			device->CreateVertexShader(vertex_shader_blob1->GetBufferPointer(), vertex_shader_blob1->GetBufferSize(), NULL, &vertex_shader[triangle]);
			device->CreatePixelShader(pixel_shader_blob1->GetBufferPointer(), pixel_shader_blob1->GetBufferSize(), NULL, &pixel_shader[triangle]);

			// set the shader objects
			context->VSSetShader(vertex_shader[triangle], 0, 0);
			context->PSSetShader(pixel_shader[triangle], 0, 0);

			// create the input layout object
			D3D11_INPUT_ELEMENT_DESC ied1[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			device->CreateInputLayout(ied1, ARRAYSIZE(ied1), vertex_shader_blob1->GetBufferPointer(), vertex_shader_blob1->GetBufferSize(), &input_layout[triangle]);
			context->IASetInputLayout(input_layout[triangle]);

			D3D11_BUFFER_DESC bd2;
			ZeroMemory(&bd2, sizeof(bd2));

			bd2.Usage = D3D11_USAGE_DEFAULT;
			bd2.ByteWidth = sizeof(DirectX::XMMATRIX);
			bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			device->CreateBuffer(&bd2, NULL, &constant_buffer[triangle]);
#endif
#if onLine
			
			//Create buffer descs
			D3D11_BUFFER_DESC vertex_buffer_desc1;
			D3D11_BUFFER_DESC index_buffer_desc1;

			//Fill out descs
			vertex_buffer_desc1.Usage = D3D11_USAGE_DYNAMIC;
			vertex_buffer_desc1.ByteWidth = sizeof(colored_vertex) * debug_renderer::get_line_vert_capacity(); //sizeof(struct type)
			vertex_buffer_desc1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer_desc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertex_buffer_desc1.MiscFlags = 0;
			vertex_buffer_desc1.StructureByteStride = sizeof(colored_vertex);

			//Create buffers
			check = device->CreateBuffer(&vertex_buffer_desc1, NULL, &vertex_buffer[particle]);


			///Create input layout
			ID3D10Blob *vertex_shader_blob1, *pixel_shader_blob1, *error_message1;
			vertex_shader_blob1 = { 0 };
			pixel_shader_blob1 = { 0 };
			error_message1 = { 0 };
			check = D3DCompileFromFile(L"basic_shader.hlsl", 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex_shader_blob1, &error_message1);
			if (FAILED(check))
			{
				// If the shader failed to compile it should have written something to the error message.
				if (error_message1)
				{
					auto error = (char*)error_message1->GetBufferPointer();
				}

			}

			check = D3DCompileFromFile(L"basic_shader.hlsl", 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel_shader_blob1, &error_message1);
			if (FAILED(check))
			{
				// If the shader failed to compile it should have written something to the error message.
				if (error_message1)
				{
					auto error = (char*)error_message1->GetBufferPointer();
				}
			}

			device->CreateVertexShader(vertex_shader_blob1->GetBufferPointer(), vertex_shader_blob1->GetBufferSize(), NULL, &vertex_shader[particle]);
			device->CreatePixelShader(pixel_shader_blob1->GetBufferPointer(), pixel_shader_blob1->GetBufferSize(), NULL, &pixel_shader[particle]);

			// set the shader objects
			context->VSSetShader(vertex_shader[particle], 0, 0);
			context->PSSetShader(pixel_shader[particle], 0, 0);

			// create the input layout object
			D3D11_INPUT_ELEMENT_DESC ied1[] =
			{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			device->CreateInputLayout(ied1, _ARRAYSIZE(ied1), vertex_shader_blob1->GetBufferPointer(), vertex_shader_blob1->GetBufferSize(), &input_layout[particle]);
			context->IASetInputLayout(input_layout[particle]);

			D3D11_BUFFER_DESC bd2;
			ZeroMemory(&bd2, sizeof(bd2));

			bd2.Usage = D3D11_USAGE_DEFAULT;
			bd2.ByteWidth = sizeof(DirectX::XMMATRIX);
			bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			device->CreateBuffer(&bd2, NULL, &constant_buffer[particle]);
#endif
#pragma region TESTOPERATORS
			float4 x = { 0,0,0,1 };
			float4 y = { 1, 0, 0, 1 };
			float4 z = x + y;

			float3 x1 = { 0,0,0 };
			float3 y1 = { 1, 0, 0 };
			float3 z1 = x1 + y1;

			float4 x2 = { 0,1,1,0 };
			float4 y2 = { 1, 0,0,1 };
			float4 z2 = x2 * y2;

			z2 += x;

			float4 crs = cross(x, y);
			float4 norm = normalize(x, y);
			float doa = dot(x, y);

			z += x;

			auto m = lerp(z, x, 5.0f);

			auto s = x[3];
#pragma endregion TESTOPERATORS
		}

		void draw_view(view_t& v)
		{
			//UNDONE: draw view
			/// Apply view properties
			///	Set and clear render targets
			///	Set viewport(s), etc

			float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };

			context->ClearRenderTargetView(render_target[0], color);

			//// clear the depth buffer
			context->ClearDepthStencilView(depthStencilView[0], D3D11_CLEAR_DEPTH, 1.0f, 0);

#if onMesh
			DirectX::XMMATRIX final = v.get_view_matrix() * v.get_proj_matrix();

			UINT stride = sizeof(VERTEX);
			UINT offset = 0;

			context->IASetVertexBuffers(0, 1, &vertex_buffer[mesh], &stride, &offset);
			context->IASetIndexBuffer(index_buffer[mesh], DXGI_FORMAT_R32_UINT, 0);

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//context->UpdateSubresource(constant_buffer[0], 0, 0, &final, 0, 0);
			D3D11_MAPPED_SUBRESOURCE d1;
			context->Map(constant_buffer[mesh], 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &d1);
			memcpy(d1.pData, &final, sizeof(DirectX::XMMATRIX));
			context->Unmap(constant_buffer[mesh], 0);
			context->VSSetConstantBuffers(0, 1, &constant_buffer[mesh]);

			context->DrawIndexed(vertex_count, 0, 0);
#endif

#if onTriangle
			DirectX::XMMATRIX matRotate1, matView1, matProjection1, matTrans1;
			DirectX::XMMATRIX matFinal1;

			static float Time1 = 0.0f; Time1 += 0.001f;

			// create a world matrices
			matRotate1 = DirectX::XMMatrixRotationY(Time1);
			
			matTrans1 = DirectX::XMMatrixTranslation(-1.0f, -2.0f, 0.0f);

			// create a view matrix
			matView1 = DirectX::XMMatrixLookAtLH({ 0.0f, 9.0f, 24.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

			matProjection1 = DirectX::XMMatrixPerspectiveFovLH((float)DirectX::XMConvertToRadians(45), (float)window_width / window_height, 1.0f, 1000.0f);

			matFinal1 = matTrans1 * matRotate1 * matView1 * matProjection1;
			//matFinal1 = matRotate1 * v.get_view_matrix() * v.get_proj_matrix();

			// select which vertex buffer to display
		    UINT stride1 = sizeof(VERTEX);
			UINT offset1 = 0;
			context->IASetVertexBuffers(0, 1, &vertex_buffer[triangle], &stride1, &offset1);
			context->IASetIndexBuffer(index_buffer[triangle], DXGI_FORMAT_R32_UINT, 0);

			// select which primitive type we are using
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			context->UpdateSubresource(constant_buffer[triangle], 0, 0, &matFinal1, 0, 0);
			context->VSSetConstantBuffers(0, 1, &constant_buffer[triangle]);

			context->DrawIndexed(18, 0, 0);
#endif
#if onLine
			DirectX::XMMATRIX  matView1, matProjection1;
			DirectX::XMMATRIX matFinal1;

			// create a world matrices
			// create a view matrix
			matView1 = DirectX::XMMatrixLookAtLH({ 0.0f, 5.0f, 15.0f }, { 0.0f,0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

			//matView1 = DirectX::XMMatrixInverse(nullptr, matView1);

			matProjection1 = DirectX::XMMatrixPerspectiveFovLH((float)DirectX::XMConvertToRadians(90.0f), (float)window_width / window_height, 0.1f, 1000.0f);

			matFinal1 =  matView1 * matProjection1;
			//matFinal1 = matRotate1 * v.get_view_matrix() * v.get_proj_matrix();

			// select which vertex buffer to display
			UINT stride1 = sizeof(colored_vertex);
			UINT offset1 = 0;

			D3D11_MAPPED_SUBRESOURCE d1;
			context->Map(vertex_buffer[particle], 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &d1);
			memcpy(d1.pData, debug_renderer::get_line_verts(), sizeof(VERTEX) * debug_renderer::get_line_vert_count());
			context->Unmap(vertex_buffer[particle], 0);

			context->IASetVertexBuffers(0, 1, &vertex_buffer[particle], &stride1, &offset1);\

			// select which primitive type using
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			context->UpdateSubresource(constant_buffer[particle], 0, 0, &matFinal1, 0, 0);
			context->VSSetConstantBuffers(0, 1, &constant_buffer[particle]);

			context->Draw(debug_renderer::get_line_vert_count(), 0);
			//context->Draw(2, 0);
#endif
			swapchain->Present(0, 0);

		}

		~impl()
		{
			//UNDONE: Clean-up
			//
			// In general, release objects in reverse order of creation
			if (constant_buffer != nullptr)
			{
				constant_buffer[0]->Release();
				delete[] & constant_buffer;
			}
			if (sampler_state != nullptr)
			{
				sampler_state[0]->Release();
				delete[] & sampler_state;
			}
			if (input_layout != nullptr)
			{
				input_layout[0]->Release();
				delete[] & input_layout;
			}
			if (pixel_buffer != nullptr)
			{
				pixel_buffer[0]->Release();
				delete[] & pixel_buffer;
			}
			if (vertex_buffer[0] != nullptr)
			{
				vertex_buffer[0]->Release();
				delete[] & vertex_buffer;
			}
			if (pixel_shader != nullptr)
			{
				pixel_shader[0]->Release();
				delete[] & pixel_shader;
			}
			if (vertex_shader != nullptr)
			{
				vertex_shader[0]->Release();
				delete[] & vertex_shader;
			}
			if (rasterState != nullptr)
			{
				rasterState[0]->Release();
				delete[] & rasterState;
			}
			if (depthStencilView != nullptr)
			{
				depthStencilState[0]->Release();
				delete[] & depthStencilView;
			}
			if (depthBuffer != nullptr)
			{
				depthBuffer[0]->Release();
				delete[] & depthBuffer;
			}
			if (depthStencilState != nullptr)
			{
				depthStencilState[0]->Release();
				delete[] & depthStencilState;
			}
			if (context != nullptr)
			{
				context->Release();
				context = 0;
			}
			if (device != nullptr)
			{
				device->Release();
				device = 0;
			}
			if (swapchain != nullptr)
			{
				swapchain->Release();
				swapchain = 0;
			}
		}
	};
}