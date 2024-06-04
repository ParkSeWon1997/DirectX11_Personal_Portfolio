#include "../Bin/ShaderFiles/Engine_Shader_Defines.hlsli"


/* ������Ʈ ���̺�(������̺�) */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_BrushTexture;
texture2D g_MaskTexture; /* �ȼ��� ����ǻ�� */
texture2D g_DiffuseTexture[2]; /* �ȼ��� ����ǻ�� */

vector g_vBrushPos = float4(30.f, 0.0f, 20.f, 1.f);
float g_fBrushRange = 5.f;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float4 vWorldPos : TEXCOORD2;
};

/* ���� ���̴� : */
/* 1. ������ ��ġ ��ȯ(����, ��, ����).*/
/* 2. ������ ���������� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    return Out;
}

/* TriangleList�� ��� : ���� ������ �޾Ƽ� w�����⸦ �� ������ ���ؼ� �����Ѵ�. */
/* ����Ʈ(��������ǥ��) ��ȯ. */
/* �����Ͷ����� : �������� �ѷ��׿��� �ȼ��� ������, ������ ���������Ͽ� �����. -> �ȼ��� ���������!!!!!!!!!!!! */


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float4 vWorldPos : TEXCOORD2;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;


    vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vBrush = float4(0.0f, 0.f, 0.f, 1.f);

    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);

    if (g_vBrushPos.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fBrushRange &&
		g_vBrushPos.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fBrushRange)
    {
        float2 vTexcoord = (float2) 0.f;

		/* (g_vBrushPos.x - g_fBrushRange) : ������ ������ ���� x */
        vTexcoord.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
        vTexcoord.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

        vBrush = g_BrushTexture.Sample(LinearSampler, vTexcoord);
    }

    vector vMtrlDiffuse = vDestDiffuse * vMask + vSourDiffuse * (1.f - vMask) + vBrush;
	
    Out.vDiffuse = vMtrlDiffuse;

	/* -1 -> 0 */
	/* 1 -> 1 */
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	/* In.vProjPos.z = 0 ~ far */
	/* In.vProjPos.w = near ~ far */
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 3000.f, 0.0f, 1.f);

    return Out;
}
//
//PS_OUT PS_MAIN_POINT(PS_IN In)
//{
//	PS_OUT			Out = (PS_OUT)0;
//
//	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * 30.f);
//
//	vector		vLightDir = In.vWorldPos - g_vLightPos;
//
//	float		fDistance = length(vLightDir);
//
//	vector		fShade = min(max(dot(normalize(vLightDir) * -1.f, In.vNormal), 0.f) + 
//		(g_vLightAmbient * g_vMtrlAmbient), 1.f);
//
//	vector		vReflect = reflect(normalize(vLightDir), In.vNormal);
//	vector		vLook = In.vWorldPos - g_vCamPosition;
//
//	float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);
//	
//	float		fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);
//
//	Out.vColor = ((g_vLightDiffuse * vMtrlDiffuse) * fShade + 
//		(g_vLightSpecular * g_vMtrlSpecular) * fSpecular) * fAtt;
//
//	return Out;
//}


technique11 DefaultTechnique
{
	/* Ư�� �������� ������ �� �����ؾ��� ���̴� ����� ��Ʈ���� ���̰� �ִ�. */
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	//pass Light_Point
	//{
	//	/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
	//	VertexShader = compile vs_5_0 VS_MAIN();
	//	GeometryShader = NULL;
	//	HullShader = NULL;
	//	DomainShader = NULL;
	//	PixelShader = compile ps_5_0 PS_MAIN_POINT();
	//}
}

