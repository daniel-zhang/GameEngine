//
// Data structures
//
struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};

struct VertexIn
{
    float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex     : TEXCOORD;
};

struct VertexOut
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
};

//
// Constant states
//
cbuffer PerFrame
{
	float3 gEyePosW;
};

cbuffer PerObj
{
    float4x4 gLocalToWorld;
    float4x4 gWorldToView;
    float4x4 gViewToProj;
    float4x4 gWorldViewProj;

    float4x4 gTexTransform;
    float4x4 gWorldInvTranspose;

	Material gMaterial;
};

//
// Texture states
//
Texture2D gDiffuseMap;

SamplerState AnisotropicSamplerState
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

//
// Shaders
//
VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    // Transform to world space space.
    vout.PosW    = mul(float4(vin.PosL, 1.0f), gLocalToWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
        
    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    // Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_TARGET
{
	return float4(0.3f, 0.3f, 0.3f, 1.f);
}

technique11 BaseTech
{
	pass p0
	{
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
	}
}
 

