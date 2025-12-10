#pragma once

#include<Windows.h>
#include <d2d1.h>
#include <dcommon.h> //to use D2D1_RECT_F (OR D2D_RECT_F)
typedef D2D1_RECT_F RectD;
#include <D3D9Types.h>
typedef D2D1_COLOR_F ColorD;



#include <wrl/client.h>
using Microsoft::WRL::ComPtr; //for com pointer

#include <string>

//link stuff
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")        // Fixes "unresolved external symbol D2D1CreateFactory"
#pragma comment(lib, "dxgi.lib")        // Needed for DXGI Surface interfaces
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "dwrite.lib")