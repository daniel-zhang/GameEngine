#include "LightHelper.fx"

// VS input
struct PosNormalTanTex
{
    float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float3 TanL    : TANGENT;
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
    float3 gEyePosW <string tag="cam_pos";>; 
    DirectionalLight gMainLight <string tag="main_light";>;
};

cbuffer PerObj
{
    float4x4 gLocalToWorld          <string tag="local_to_world";>;
    float4x4 gWorldToView           <string tag="world_to_view";>;
    float4x4 gViewToProj            <string tag="view_to_proj";>;
    //float4x4 gWorldViewProj         <string tag="world_view_proj";>;

    float4x4 gTexTransform          <string tag="tex_transform";>;
    float4x4 gWorldInvTranspose     <string tag="world_inv_transpose";>;

    Material gMaterial              <string tag="mesh_material";>;
};

//
// Texture states
//
//Texture2D gDiffuseMap <string tag="texture";>;

SamplerState AnisotropicSamplerState <string tag="anisotropic_sampler";>
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

//
// Shaders
//
VertexOut VS(PosNormalTanTex vin)
{
    VertexOut vout;
    
    // Transform to world space space.
    //vout.PosW    = mul(float4(vin.PosL, 1.0f), gLocalToWorld).xyz;
    //vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
    vout.PosW    = mul(gLocalToWorld, float4(vin.PosL, 1.0f)).xyz;
    vout.NormalW = mul((float3x3)gWorldInvTranspose, vin.NormalL);
        
    // Transform to homogeneous clip space.
    //float4x4 wvp = mul(gViewToProj, mul(gWorldToView, gLocalToWorld));
    float4x4 wvp = mul(mul(gViewToProj, gWorldToView), gLocalToWorld);

    vout.PosH = mul(wvp, float4(vin.PosL, 1.0f));
    //vout.PosH = mul(float4(vin.PosL, 1.0f), wvp);

    // Output vertex attributes for interpolation across triangle.
    //vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    vout.Tex = vin.Tex;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_TARGET
{
    pin.NormalW = normalize(pin.NormalW);
    float3 toEye = gEyePosW - pin.PosW;
    float distToEye = length(toEye); 
    toEye /= distToEye;

    // No tex for now, texColor default to be an identity vector if gUseTexture == false
    float4 texColor = float4(1.f, 1.f, 1.f, 1.f);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 A, D, S;
    ComputeDirectionalLight(gMaterial, gMainLight, pin.NormalW, toEye, A, D, S);
    ambient += A;
    diffuse += D;
    spec    += S;

    // Modulate with late add
    float4 litColor = texColor*(ambient + diffuse) + spec;

    // Common to take alpha from diffuse material and texture.
    litColor.a = gMaterial.Diffuse.a * texColor.a;
    return litColor;
}

technique11 MainTech<string VertexFormat="PosNormalTanTex";>
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

 

