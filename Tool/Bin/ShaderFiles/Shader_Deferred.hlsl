
#include "Engine_Shader_Defines.hlsli"

/* ������Ʈ ���̺�(������̺�) */
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ViewMatrixInv, g_ProjMatrixInv;
vector		g_vLightDir;
vector		g_vLightPos;
float		g_fLightRange;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector		g_vCamPosition;

texture2D	g_Texture;
texture2D	g_NormalTexture;
texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;
texture2D	g_DepthTexture;
texture2D	g_SpecularTexture;
texture2D	g_SpecularMapTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;	
	float2		vTexcoord : TEXCOORD0;
};

/* ���� ���̴� :  /*
/* 1. ������ ��ġ ��ȯ(����, ��, ����).*/
/* 2. ������ ���������� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);	
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

/* TriangleList�� ��� : ���� ������ �޾Ƽ� w�����⸦ �� ������ ���ؼ� �����Ѵ�. */
/* ����Ʈ(��������ǥ��) ��ȯ. */
/* �����Ͷ����� : �������� �ѷ��׿��� �ȼ��� ������, ������ ���������Ͽ� �����. -> �ȼ��� ���������!!!!!!!!!!!! */
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);


	return Out;
}

struct PS_OUT_LIGHT
{
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector	vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.0f);	 

	Out.vShade = g_vLightDiffuse * 
		saturate(max(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));


	/* vWorldPos:ȭ�鿡 �׷����� �ִ� �ȼ����� ���� ���� ��ġ�� �޾ƿ;��ϳ�.  */
	/* 1. ����Ÿ�ٿ� ��ü���� �׸��� �ȼ��� ������ġ�� �����ϴ� ���.(���� �������̶�� ���Ƕ����� ������ �����) */
	/* 2. ���� ��Ȳ���� �ȼ��� ������ġ(x, y)���� �������ϴ� �۾��� ���� -> z�� ���⶧���� ��������� ����ȯ�� �����.-> ���� z( 0 ~ 1), ViewSpace`s Pixel`s Z��(near ~ far) �޾ƿ���.(�������̾ƴϴ�.)  */

	vector		vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	

	vector  vWorldPos;

	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x; /* 0 ~ 1 */
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * (vDepthDesc.y * 3000.f);

	/* �佺���̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ���彺���̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
	vector	vLook = vWorldPos - g_vCamPosition;

	vector	vSpecularMap = g_SpecularMapTexture.Sample(LinearSampler, In.vTexcoord);

	Out.vSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f) * (1.f - vSpecularMap.r);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector	vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.0f);
	vector	vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	vector  vWorldPos;

	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x; /* 0 ~ 1 */
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * (vDepthDesc.y * 3000.f);

	/* �佺���̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ���彺���̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);


	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);

	float		fAtt = max((g_fLightRange - fDistance), 0.f) / g_fLightRange;

	Out.vShade = g_vLightDiffuse *
		saturate(max(dot(normalize(vLightDir) * -1.f, normalize(vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;


	/* vWorldPos:ȭ�鿡 �׷����� �ִ� �ȼ����� ���� ���� ��ġ�� �޾ƿ;��ϳ�.  */
	/* 1. ����Ÿ�ٿ� ��ü���� �׸��� �ȼ��� ������ġ�� �����ϴ� ���.(���� �������̶�� ���Ƕ����� ������ �����) */
	/* 2. ���� ��Ȳ���� �ȼ��� ������ġ(x, y)���� �������ϴ� �۾��� ���� -> z�� ���⶧���� ��������� ����ȯ�� �����.-> ���� z( 0 ~ 1), ViewSpace`s Pixel`s Z��(near ~ far) �޾ƿ���.(�������̾ƴϴ�.)  */
	vector	vReflect = reflect(normalize(vLightDir), normalize(vNormal));
	vector	vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f) * fAtt;

	return Out;
}

PS_OUT PS_MAIN_DEFERRED_RESULT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector			vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	if (0.0f == vDiffuse.a)
		discard;

	vector			vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
	vector			vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

	Out.vColor = vDiffuse * vShade + vSpecular;

	return Out;
}




technique11 DefaultTechnique
{
	/* Ư�� �������� ������ �� �����ؾ��� ���̴� ����� ��Ʈ���� ���̰� �ִ�. */
	pass DebugRender
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None_Test_None_Write, 0);
		SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None_Test_None_Write, 0);
		SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	pass Deferred_Result
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None_Test_None_Write, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_RESULT();
	}
}

