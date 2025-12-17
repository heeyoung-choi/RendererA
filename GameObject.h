#pragma once
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>
#include "GraphicsTypes.h" // Where your Vertex struct is defined

using namespace Microsoft::WRL;
using namespace DirectX;

class Model
{
public:
    Model() = default;

    // Transform Data
    XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f }; // In Radians
    XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

    // Mesh Data
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;
    UINT indexCount = 0;

    // Helper to get the World Matrix for this specific object
    XMMATRIX GetWorldMatrix() const
    {
        XMMATRIX matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
        XMMATRIX matRot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
        XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);

        return matScale * matRot * matTrans;
    }

    
};