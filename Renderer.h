#pragma once
#include <d3d11.h>
#include "GraphicsTypes.h"
#include <vector>
#include <string>
#include <d2d1.h>
#include <d3dcompiler.h>
#include <dwrite.h>
#include "tiny_object_loader.h"

#include "Common.h"




// Link the Direct3D library automatically
// (Alternatively, you can add d3d11.lib in Project Properties -> Linker -> Input)
using namespace DirectX;
#pragma comment(lib, "d3d11.lib")
class Renderer
{
friend class Window;
public:
	Renderer();
	~Renderer()
	{
		

	}
private:
	HRESULT InitD3D(HWND hWnd, int height, int width);
	// Helper to set up Device, SwapChain, RTV, and Viewport
	HRESULT InitPipeline(HWND hWnd, int width, int height);

	// Helper to define vertices/indices and create buffers
	HRESULT InitGraphics();

	// Helper to compile and create shaders and input layout
	HRESULT InitShaders();
	void RenderFrame();
	void Present();
	bool ImportModel(const std::string& fileName, const std::string& searchPath);
	HRESULT InitUIPipeline(HWND hWnd);
	HRESULT InitTextResources();
	HRESULT CreateBrushes();
	HRESULT DrawUI();
	void DrawText();
private:
	ComPtr<ID3D11Device> g_pDevice;        // The Resource Manager
	ComPtr<ID3D11DeviceContext> g_pContext;       // The Artist]]]]]]]]]]p]
	ComPtr<IDXGISwapChain> g_pSwapChain;     // The Double Buffer Manager
	ComPtr<ID3D11RenderTargetView> g_pRenderTarget;  // The Canvas
	ComPtr<ID3D11Buffer> g_pConstantBuffer;
	ComPtr<ID3D11Buffer> g_pVertexBuffer; // The Vertex Buffer
	ComPtr<ID3D11Buffer> g_pIndexBuffer;
	ComPtr<ID3D11DepthStencilView> g_pDSV; // depth canvas
	ComPtr<IDXGISurface> g_pBackBuffer; // surface 
	ComPtr<ID2D1RenderTarget> g_pBackBufferRT; // Direct2D Render Target
	ComPtr<ID2D1Factory> g_pD2DFactory; //FACTOry

	ComPtr<ID2D1SolidColorBrush> g_pBrushBlack;
	ComPtr<ID2D1SolidColorBrush> g_pBrushWhite;
	ComPtr<ID2D1SolidColorBrush> g_pBrushUIMain;
	ComPtr<IDWriteTextFormat> g_pTextFormat;
	ComPtr<IDWriteFactory> g_pDWriteFactory;


	ComPtr<ID3D11VertexShader> g_pVertexShader;
	ComPtr<ID3D11PixelShader> g_pPixelShader;
	ComPtr<ID3D11InputLayout> g_pInputLayout;
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;


	
};