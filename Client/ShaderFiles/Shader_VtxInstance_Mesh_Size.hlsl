#include "../Bin/ShaderFiles/Engine_Shader_Defines.hlsli"

/* ������Ʈ ���̺�(������̺�) */
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_Texture;
vector g_vColor;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;

	float2		vLifeTime : COLOR0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
    float4		vLocalPos : TEXCOORD1;
	float2		vLifeTime : COLOR0;
};

/* ���� ���̴� :  /* 
/* 1. ������ ��ġ ��ȯ(����, ��, ����).*/
/* 2. ������ ���������� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		TransformMatrix = matrix(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector		vPosition = mul(float4(In.vPosition, 1.f), TransformMatrix);

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexcoord = In.vTexcoord;

	Out.vLifeTime = In.vLifeTime;
    Out.vLocalPos = float4(In.vPosition, 1.f);

	return Out;
}

/* TriangleList�� ��� : ���� ������ �޾Ƽ� w�����⸦ �� ������ ���ؼ� �����Ѵ�. */
/* ����Ʈ(��������ǥ��) ��ȯ. */
/* �����Ͷ����� : �������� �ѷ��׿��� �ȼ��� ������, ������ ���������Ͽ� �����. -> �ȼ��� ���������!!!!!!!!!!!! */


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float2		vLifeTime : COLOR0;
    float4		vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_SIZEUP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  

    Out.vColor.rgb = float3(1.0f, 0.2f, 0.2f);
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	
 
}

PS_OUT PS_SIZEDOWN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  

    Out.vColor.rgb = float3(1.0f, 0.0f, 0.0f);
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	
 
}







technique11 DefaultTechnique
{
	
	/* Ư�� �������� ������ �� �����ؾ��� ���̴� ����� ��Ʈ���� ���̰� �ִ�. */
	pass SizeUpPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SIZEUP();
	}
	pass SizeDownPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SIZEDOWN();
	}


}

