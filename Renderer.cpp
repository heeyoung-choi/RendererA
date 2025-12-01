#define TINYOBJLOADER_IMPLEMENTATION
#include "Renderer.h"
Renderer::Renderer()
{

}

HRESULT Renderer::InitD3D(HWND hWnd, int height, int  width)
{
	HRESULT hr;

	// Step 1: Get the foundation ready
	hr = InitPipeline(hWnd, width, height);
	if (FAILED(hr)) return hr;

	// Step 2: Load the shapes
	hr = InitGraphics();
	if (FAILED(hr)) return hr;

	// Step 3: Load the programs
	hr = InitShaders();
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT Renderer::InitPipeline(HWND hWnd, int width, int height)
{

	HRESULT hr;

	// A. Describe the Swap Chain
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferCount = 1;                                 // 1 Back Buffer
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 32-bit Color and supported by DirectX 2D
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// B. Create Device, Context, and Swap Chain
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // Helpful debug messages
#endif
	createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT; //to support 2d stuff

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[] = {  D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevelArray,
		1,
		D3D11_SDK_VERSION,
		&sd,
		g_pSwapChain.GetAddressOf(),
		g_pDevice.GetAddressOf(),
		&featureLevel,
		g_pContext.GetAddressOf());

	if (FAILED(hr)) return hr;

	// C. Create Render Target View (RTV)
	ID3D11Texture2D* pBackBuffer = nullptr;
	// Get the pointer to the back buffer texture
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr)) return hr;

	// Create the view
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, g_pRenderTarget.GetAddressOf());
	pBackBuffer->Release(); // We don't need the raw pointer anymore
	if (FAILED(hr)) return hr;
	// ============================================================
	// [NEW] C2. Create Depth Stencil View (DSV) - The Depth Canvas
	// ============================================================
	// 1. Describe the raw depth texture
	D3D11_TEXTURE2D_DESC descDepth = { 0 };
	descDepth.Width = width;  // Must match screen size
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	// D24_UNORM_S8_UINT = 24 bits for Depth, 8 bits for Stencil (standard format)
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// Multisampling must match the swap chain settings (currently 1, 0)
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	// Bind as a Depth Stencil target
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	// 2. Create the texture resource
	ID3D11Texture2D* pDepthStencilTex = nullptr;
	hr = g_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencilTex);
	if (FAILED(hr)) return hr;

	// 3. Create the View (DSV) so the pipeline can use it
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = g_pDevice->CreateDepthStencilView(pDepthStencilTex, &descDSV, g_pDSV.GetAddressOf());
	pDepthStencilTex->Release(); // Done with the raw texture pointer
	if (FAILED(hr)) return hr;
	// D. Bind the View
	g_pContext.Get()->OMSetRenderTargets(1, g_pRenderTarget.GetAddressOf(), g_pDSV.Get());

	// E. Set the Viewport (Tell DirectX how to map coordinates to the window)
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	g_pContext.Get()->RSSetViewports(1, &viewport);


	// 
	hr = InitUIPipeline(hWnd);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT Renderer::InitGraphics()
{
	ImportModel("shape.obj", "./");

	// Define indices to build a hexagon from 6 triangles
	 
	D3D11_BUFFER_DESC vbd = { 0 };
	vbd.ByteWidth = sizeof(Vertex) * vertices.size(); // Use .size()
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vsd = { 0 };
	vsd.pSysMem = vertices.data(); // .data() gives the raw pointer needed by DirectX
	g_pDevice->CreateBuffer(&vbd, &vsd, g_pVertexBuffer.GetAddressOf());

	// Setup Index Buffer
	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof(unsigned short) * indices.size();
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = { 0 };
	isd.pSysMem = indices.data();
	g_pDevice->CreateBuffer(&ibd, &isd, g_pIndexBuffer.GetAddressOf());
	



	// 3. Create Constant Buffer (This stays in Renderer for now as it's global)
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return g_pDevice->CreateBuffer(&cbd, nullptr, g_pConstantBuffer.GetAddressOf());
	return S_OK;
}

HRESULT Renderer::InitShaders()
{
	HRESULT hr;



	// ============================================================
	// SHADER COMPILATION & INPUT LAYOUT
	// ============================================================

	// Blobs store the compiled bytecode or error messages
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG; // Add debug info to shaders
#endif

	// 1. COMPILE VERTEX SHADER
	// We compile the file "shaders_1.hlsl", looking for the function "VSMain".
	// We use "vs_5_0" target (Shader Model 5, standard for DX11).
	hr = D3DCompileFromFile(
		L"shaders_1.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VSMain", "vs_5_0",
		flags, 0, &pVSBlob, &pErrorBlob);

	if (FAILED(hr)) {
		if (pErrorBlob) {
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return hr;
	}

	// 2. CREATE VERTEX SHADER OBJECT
	hr = g_pDevice->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		nullptr, g_pVertexShader.GetAddressOf());
	if (FAILED(hr)) return hr;


	// 3. CREATE INPUT LAYOUT
	// This is the bridge map between C++ Vertex struct and HLSL VS_INPUT struct.
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		// 1. Position (Offset 0)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// 2. Color (Offset 12 - after 3 floats)
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// 3. Normal (Offset 24 - after Pos(12) + Col(12))
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// We need the VS signature (bytecode) to validate the layout
	hr = g_pDevice->CreateInputLayout(
		ied, ARRAYSIZE(ied),
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		g_pInputLayout.GetAddressOf());

	pVSBlob->Release(); // Done with the vertex shader bytecode blob
	if (FAILED(hr)) return hr;


	// 4. COMPILE PIXEL SHADER
	// Look for function "PSMain", target "ps_5_0".
	hr = D3DCompileFromFile(
		L"shaders_1.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain", "ps_5_0",
		flags, 0, &pPSBlob, &pErrorBlob);

	if (FAILED(hr)) {
		if (pErrorBlob) {
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return hr;
	}

	// 5. CREATE PIXEL SHADER OBJECT
	hr = g_pDevice->CreatePixelShader(
		pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		nullptr, g_pPixelShader.GetAddressOf());

	pPSBlob->Release(); // Done with pixel shader blob
	if (FAILED(hr)) return hr;

	return S_OK;

}

void Renderer::RenderFrame()
{
	// 1. Clear screen (same as before)
	const float color[] = { 0.392f, 0.584f, 0.929f, 1.0f };
	g_pContext.Get()->ClearRenderTargetView(g_pRenderTarget.Get(), color);
	g_pContext.Get()->ClearDepthStencilView(g_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	// ============================================================
	// NEW CODE: DRAW THE TRIANGLE
	// ============================================================

	//update the constant buffer

	//rotate overtime 
	static float t = 0.0f;
	t += 0.01f;
	XMMATRIX scale_mat = XMMatrixScaling(0.25f, 0.25f, 0.25f);
	XMMATRIX world = XMMatrixRotationY(t) * scale_mat;
	// 2. View: The "Camera"
	// Eye: Where the camera is (0, 1, -2) -> Back 2 units, up 1 unit
	// At:  What it is looking at (0, 0, 0) -> The center of the world
	// Up:  Which way is "up" (0, 1, 0) -> Y-axis is up
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(eye, at, up);

	// FOV (45 degrees), Aspect Ratio (800/600), Near Z (0.1), Far Z (100)
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1000.0f / 1000.0f, 0.1f, 100.0f);

	// C. PREPARE DATA FOR GPU
	// Important: We must TRANSPOSE matrices because HLSL stores them differently than C++
	ConstantBuffer cb;
	cb.world = XMMatrixTranspose(world);
	cb.view = XMMatrixTranspose(view);
	cb.projection = XMMatrixTranspose(proj);

	// D. MAP & UPLOAD
	// Lock the constant buffer so we can write to it
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	g_pContext.Get()->Map(g_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);

	// Copy the data
	memcpy(mappedRes.pData, &cb, sizeof(ConstantBuffer));

	// Unlock it
	g_pContext.Get()->Unmap(g_pConstantBuffer.Get(), 0);





	// A. Bind the Input Layout
	g_pContext.Get()->IASetInputLayout(g_pInputLayout.Get());



	// C. Tell IA how to interpret the data (a list of triangles)
	g_pContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// [NEW] Bind the Constant Buffer to the Vertex Shader (Slot 0)
	g_pContext.Get()->VSSetConstantBuffers(0, 1, g_pConstantBuffer.GetAddressOf());
	// D. Bind the Shaders
	g_pContext.Get()->VSSetShader(g_pVertexShader.Get(), nullptr, 0);
	g_pContext.Get()->PSSetShader(g_pPixelShader.Get(), nullptr, 0);

	// E. DRAW!
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext.Get()->IASetVertexBuffers(0, 1, g_pVertexBuffer.GetAddressOf(), &stride, &offset);
	g_pContext.Get()->IASetIndexBuffer(g_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	g_pContext.Get()->DrawIndexed(indices.size(), 0, 0);
	// ============================================================
	DrawUI();
	// 2. Present (same as before)
	g_pSwapChain->Present(1, 0);
}

bool Renderer::ImportModel(const std::string& fileName, const std::string& searchPath)
{
	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = searchPath; // Path to material files

	if (!reader.ParseFromFile(fileName, reader_config)) {
		/*if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}*/
		return false;
	}
	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				Vertex vertex = {};
				vertex.x = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.y = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.z = attrib.vertices[3 * idx.vertex_index + 2];
				if (idx.normal_index >= 0) {
					vertex.nx = attrib.normals[3 * idx.normal_index + 0];
					vertex.ny = attrib.normals[3 * idx.normal_index + 1];
					vertex.nz = attrib.normals[3 * idx.normal_index + 2];
				}
				if (idx.texcoord_index >= 0) {
					vertex.u = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					vertex.v = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}
				vertex.r = 0.0f;
				vertex.g = 0.8f;
				vertex.b = 0.8f;
				vertices.push_back(vertex);
				indices.push_back(static_cast<unsigned short>(indices.size()));
			}
			index_offset += fv;
		}
	}
	return true;
}
HRESULT Renderer::InitUIPipeline(HWND hWnd)
{
	HRESULT hr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),(void**) (g_pBackBuffer.GetAddressOf()));
	if (FAILED(hr)) return hr;

	float dpi = GetDpiForWindow(hWnd);

	const D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi,
			dpi);

	// Create a Direct2D render target that can draw into the surface in the swap chain
	D2D1_FACTORY_OPTIONS options;
	#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	#endif
	D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory),
		&options,
		(void**)g_pD2DFactory.GetAddressOf()
		);


	hr = g_pD2DFactory.Get()->CreateDxgiSurfaceRenderTarget(
		g_pBackBuffer.Get(),
		&props,
		g_pBackBufferRT.GetAddressOf());

	if (FAILED(hr)) return hr;

	hr = InitTextResources();
	
	if (FAILED(hr)) return hr;

	hr = CreateBrushes();
	

	return hr;
}
HRESULT Renderer::InitTextResources()
{
	HRESULT hr = S_OK;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(g_pDWriteFactory.GetAddressOf()));
	if (FAILED(hr)) return hr;
	//create text format
	hr = g_pDWriteFactory.Get()->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		10.0f * 96.0f / 72.0f,
		L"en-US",
		g_pTextFormat.GetAddressOf()
	);
	return hr;
}
HRESULT Renderer::CreateBrushes()
{
	HRESULT hr = S_OK;
	hr = g_pBackBufferRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		g_pBrushBlack.GetAddressOf());
	if (FAILED(hr)) return hr;
	hr = g_pBackBufferRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		g_pBrushWhite.GetAddressOf());
	return hr;
}
HRESULT Renderer::DrawUI()
{
	HRESULT hr = S_OK;
	if (g_pBackBufferRT)
	{
		D2D1_SIZE_F targetSize = g_pBackBufferRT->GetSize();

		g_pBackBufferRT->BeginDraw();

		g_pBrushBlack->SetTransform(
			D2D1::Matrix3x2F::Scale(targetSize)
		);

		D2D1_RECT_F rect = D2D1::RectF(
			0.0f,
			0.0f,
			100.0f,
			100.0f
		);

		g_pBackBufferRT.Get()->FillRectangle(&rect, g_pBrushBlack.Get());
		DrawText();
		hr = g_pBackBufferRT.Get()->EndDraw();
	}
	return hr;
}
void Renderer::DrawText()
{
	D2D1_RECT_F layoutRect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);
	 g_pBackBufferRT.Get()->DrawText(
		L"Hello World",
		wcslen(L"Hello World"),
		g_pTextFormat.Get(),
		layoutRect,
		g_pBrushWhite.Get()
	);
}
//////