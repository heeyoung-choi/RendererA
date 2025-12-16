#pragma once
#include <DirectXMath.h>
namespace RosaEngine
{
    struct Vertex
    {
        float x, y, z;    // Position
        float r, g, b;    // Color
        float nx, ny, nz; // Normal
        float u, v;
    };

    struct ConstantBuffer
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
        DirectX::XMFLOAT4 materialDiffuseColor;
    };
}
