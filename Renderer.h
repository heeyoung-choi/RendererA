#pragma once
#include <d3d11.h>
#include "GraphicsTypes.h"
#include <vector>
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
		if (g_pInputLayout)   g_pInputLayout->Release();   
		if (g_pVertexShader)  g_pVertexShader->Release(); 
		if (g_pPixelShader)   g_pPixelShader->Release();
		if (g_pConstantBuffer) g_pConstantBuffer->Release();
		if (g_pVertexBuffer) g_pVertexBuffer->Release();
		if (g_pIndexBuffer)  g_pIndexBuffer->Release();

		if (g_pDSV)          g_pDSV->Release();

		if (g_pRenderTarget) g_pRenderTarget->Release();
		if (g_pSwapChain)    g_pSwapChain->Release();
		if (g_pContext)      g_pContext->Release();
		if (g_pDevice)       g_pDevice->Release();

	}
	HRESULT InitD3D(HWND hWnd, int height, int width);
	// Helper to set up Device, SwapChain, RTV, and Viewport
	HRESULT InitPipeline(HWND hWnd, int width, int height);

	// Helper to define vertices/indices and create buffers
	HRESULT InitGraphics();

	// Helper to compile and create shaders and input layout
	HRESULT InitShaders();
	void RenderFrame();
private:
	ID3D11Device* g_pDevice;        // The Resource Manager
	ID3D11DeviceContext* g_pContext;       // The Artist
	IDXGISwapChain* g_pSwapChain;     // The Double Buffer Manager
	ID3D11RenderTargetView* g_pRenderTarget;  // The Canvas
	ID3D11Buffer* g_pConstantBuffer;
	ID3D11Buffer* g_pVertexBuffer; // The Vertex Buffer
	ID3D11Buffer* g_pIndexBuffer;
	ID3D11DepthStencilView* g_pDSV; // depth canvas

	ID3D11VertexShader* g_pVertexShader;
	ID3D11PixelShader* g_pPixelShader;
	ID3D11InputLayout* g_pInputLayout;

	std::vector<Vertex> vertices =
	{
		// x, y, z                        // r, g, b
		// FRONT FACE
		{ -0.33f, -0.33f, -0.33f,    1.0f, 0.0f, 0.0f }, // Bottom-Left (Red)
		{ -0.33f,  0.33f, -0.33f,    0.0f, 1.0f, 0.0f }, // Top-Left (Green)
		{  0.33f,  0.33f, -0.33f,    0.0f, 0.0f, 1.0f }, // Top-Right (Blue)
		{  0.33f, -0.33f, -0.33f,    1.0f, 1.0f, 0.0f }, // Bottom-Right (Yellow)

		// BACK FACE
		{ -0.33f, -0.33f,  0.33f,    0.0f, 1.0f, 1.0f }, // Bottom-Left (Cyan)
		{ -0.33f,  0.33f,  0.33f,    1.0f, 0.0f, 1.0f }, // Top-Left (Magenta)
		{  0.33f,  0.33f,  0.33f,    1.0f, 1.0f, 1.0f }, // Top-Right (White)
		{  0.33f, -0.33f,  0.33f,    0.0f, 0.0f, 0.0f }  // Bottom-Right (Black)
	};
	std::vector<unsigned short> indices = {
		// Front Face
		0, 1, 2,
		0, 2, 3,

		// Back Face
		4, 6, 5,
		4, 7, 6,

		// Left Face
		4, 5, 1,
		4, 1, 0,

		// Right Face
		3, 2, 6,
		3, 6, 7,

		// Top Face
		1, 5, 6,
		1, 6, 2,

		// Bottom Face
		4, 0, 3,
		4, 3, 7
	};
	
};