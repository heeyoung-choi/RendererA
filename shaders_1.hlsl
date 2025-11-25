// =================================================================================
// STRUCTURES
// =================================================================================

// Constant Buffer Definitions
// This must match the "ConstantBuffer" struct in your C++ code
cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

// Input for the Vertex Shader
struct VS_INPUT
{
    float3 pos : POSITION; // Matches the C++ Input Layout
    float3 color : COLOR; // Matches the C++ Input Layout
};

// Output from Vertex Shader -> Input to Pixel Shader
struct PS_INPUT
{
    float4 pos : SV_POSITION; // System Value Position
    float4 color : COLOR; // Pass through color (not used in this example)
};


// =================================================================================
// VERTEX SHADER
// Function Name: VSMain
// =================================================================================
PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;
    
    // 1. Convert float3 to float4 (adding W=1.0f)
    // We use "input.pos" because that is what you named it in VS_INPUT above
    float4 pos4 = float4(input.pos, 1.0f);
    
    // 2. Apply Transformations using mul() to avoid ambiguity
    // Order: Position -> World -> View -> Projection
    
    output.pos = mul(pos4, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.color = float4(input.color, 1.0f);
    
    return output;
}


// =================================================================================
// PIXEL SHADER
// Function Name: PSMain
// =================================================================================
float4 PSMain(PS_INPUT input) : SV_TARGET
{
    // Return bright YELLOW
    return input.color;
}