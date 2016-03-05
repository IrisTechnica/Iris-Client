#pragma once

#include <boost/utility.hpp>
#include "../../win32/window.h"
#include "../../singleton.hpp"

#include <D3D11.h>
#include <DXGI.h>
#include <DirectXMath.h>

#include <memory>
#include <vector>
#include "../../com_ptr.h"

#include "../../filesystem/cache.h"
#include <boost/thread.hpp>

#include <mutex>

namespace dx11 //shortcut namespace
{
typedef ID3D11Device				idevice;
typedef ID3D11DeviceContext			idevicecontext;
typedef IDXGIDevice					idxgidevice;
typedef IDXGIAdapter				idxgiadapter;
typedef IDXGIFactory				idxgifactory;
typedef IDXGISwapChain				idxgiswapchain;

typedef ID3D11Texture1D				itexture1d;
typedef ID3D11Texture2D				itexture2d;
typedef ID3D11Texture3D				itexture3d;

typedef ID3D11Buffer				ibuffer;

typedef ID3D11RasterizerState		irasterizerstate;
typedef ID3D11DepthStencilState		idepthstencilstate;

typedef ID3D11ShaderResourceView	ishaderresourceview;

typedef ID3D11VertexShader			ivertexshader;
typedef	ID3D11PixelShader			ipixelshader;
typedef	ID3D11GeometryShader		igeometryshader;
typedef ID3D11ComputeShader			icomputeshader;

typedef ID3D11InputLayout			iinputlayout;

typedef ID3D11CommandList			icommandlist;

//インターフェイスは基本スマートポインタに包括されているものとする
typedef stx::com_ptr<ID3D11Device>				device;
typedef stx::com_ptr<ID3D11DeviceContext>		devicecontext;
typedef stx::com_ptr<IDXGIDevice>				dxgidevice;
typedef stx::com_ptr<IDXGIAdapter>				dxgiadapter;
typedef stx::com_ptr<IDXGIFactory>				dxgifactory;
typedef stx::com_ptr<IDXGISwapChain>			dxgiswapchain;

typedef stx::com_ptr<ID3D11Texture1D>			texture1d;
typedef stx::com_ptr<ID3D11Texture2D>			texture2d;
typedef stx::com_ptr<ID3D11Texture3D>			texture3d;

typedef stx::com_ptr<ID3D11Buffer>				buffer;

typedef stx::com_ptr<ID3D11BlendState>			blendstate;

typedef stx::com_ptr<ID3D11SamplerState>		samplerstate;
typedef stx::com_ptr<ID3D11RasterizerState>		rasterizerstate;
typedef stx::com_ptr<ID3D11DepthStencilState>	depthstencilstate;

typedef stx::com_ptr<ID3D11RenderTargetView>	rendertargetview;
typedef stx::com_ptr<ID3D11DepthStencilView>	depthstencilview;
typedef stx::com_ptr<ID3D11ShaderResourceView>	shaderresourceview;

typedef stx::com_ptr<ID3D11VertexShader>		vertexshader;
typedef	stx::com_ptr<ID3D11PixelShader>			pixelshader;
typedef	stx::com_ptr<ID3D11GeometryShader>		geometryshader;
typedef stx::com_ptr<ID3D11ComputeShader>		computeshader;

typedef stx::com_ptr<ID3D11InputLayout>			inputlayout;

typedef stx::com_ptr<ID3D11CommandList>			commandlist;

typedef DXGI_MODE_DESC							displaymode_desc;
typedef DXGI_ADAPTER_DESC						adapter_desc;
typedef DXGI_SWAP_CHAIN_DESC					swapchain_desc;

typedef D3D11_TEXTURE1D_DESC					texture1d_desc;
typedef D3D11_TEXTURE2D_DESC					texture2d_desc;
typedef D3D11_TEXTURE3D_DESC					texture3d_desc;

typedef D3D11_BUFFER_DESC						buffer_desc;

typedef D3D11_BLEND_DESC						blend_desc;
typedef D3D11_RENDER_TARGET_BLEND_DESC			rendertarget_blend_desc;

typedef D3D11_SAMPLER_DESC						sampler_desc;
typedef D3D11_RASTERIZER_DESC					rasterizer_desc;
typedef D3D11_DEPTH_STENCIL_DESC				depthstencil_desc;

typedef D3D11_RENDER_TARGET_VIEW_DESC			rendertargetview_desc;
typedef D3D11_DEPTH_STENCIL_VIEW_DESC			depthstencilview_desc;
typedef D3D11_SHADER_RESOURCE_VIEW_DESC			shaderresourceview_desc;

typedef D3D11_INPUT_ELEMENT_DESC				inputelement_desc;

typedef std::vector<dx11::inputelement_desc>	inputelement_desclist;

typedef std::shared_ptr<inputelement_desclist>	inputelement_desclist_ptr;


typedef D3D11_SUBRESOURCE_DATA					subresource_data;

typedef D3D11_MAPPED_SUBRESOURCE				mapped_subresource;

typedef std::vector<DXGI_MODE_DESC>				displaymode_desclist;

typedef D3D11_VIEWPORT							viewport;

typedef DXGI_FORMAT								display_format;
typedef D3D_FEATURE_LEVEL						feature_level;

//DirectX Math
typedef DirectX::XMVECTOR						vector;
typedef DirectX::XMFLOAT2						mfloat2;
typedef DirectX::XMFLOAT3						mfloat3;
typedef DirectX::XMFLOAT4						mfloat4;
typedef DirectX::XMMATRIX						matrix;
typedef DirectX::FXMMATRIX						fmatrix;
typedef DirectX::XMFLOAT4X4						float4x4;


//必要なものをまとめた構造体
typedef struct
{
	device				device;				//デバイス
	devicecontext		devicecontext;		//コンテキスト
	dxgiadapter			adapter;			//アダプタ
	dxgiswapchain		swapchain;			//スワップチェイン
	rendertargetview	rendertargetview;	//ターゲットビュー
	depthstencilview	depthstencilview;	//深度&ステンシルビュー
}igraphics_essential;

typedef std::shared_ptr<igraphics_essential> graphics_essential;

typedef struct
{
	dx11::texture2d				tex;
	dx11::rendertargetview		rendertarget;
	dx11::shaderresourceview	shaderresourceview;
	dx11::samplerstate			samplerstate;
} irendable_texture2d;
typedef std::shared_ptr<irendable_texture2d> rendable_texture2d;

//いちいち取り出しがだるいからsize追加
typedef struct
{
	uint2			size;
	dx11::texture2d	tex;
} extend_texture2d;

//ワールドとか必須系物
typedef struct
{
	dx11::float4x4 world;
	dx11::float4x4 view;
	dx11::float4x4 projection;
} camera_elements;

typedef struct
{
	dx11::matrix world;
	dx11::matrix view;
	dx11::matrix projection;
} camera_elements_aligned;


typedef struct
{
	dx11::float4x4 local;
} extend_elements;

typedef stx::filesystem::raw_ptr CompiledShaderObject;

typedef enum
{
	decal,
	bump,
	diffuse,
	specular,
	illumination,
	reflective,
}texture_types;

typedef enum
{
	none,
	alpha,
	alpha_src,
	mul,
	add,
	sub
}blend_types;

};

