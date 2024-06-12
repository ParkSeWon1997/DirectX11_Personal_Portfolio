
#include "Engine_Shader_Defines.hlsli"

/* ������Ʈ ���̺�(������̺�) */
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ViewMatrixInv, g_ProjMatrixInv;
matrix		g_LightViewMatrix, g_LightProjMatrix;
vector		g_vLightDir;
vector		g_vLightPos;
float		g_fLightRange;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;
vector      g_vRampTexture;
vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector		g_vCamPosition;

texture2D	g_Texture;
texture2D	g_NormalTexture;
texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;
texture2D	g_DepthTexture;
texture2D	g_LightDepthTexture;
texture2D	g_SpecularTexture;
texture2D	g_SpecularMapTexture;
texture2D   g_MapMaskTexture;





float g_fTexW = 1280.0f;
float g_fTexH = 720.0f;



texture2D g_EffectTexture;
texture2D g_BlurTexture;
texture2D g_ResultTexture;



static const float g_fWeight[13] =
{
    0.0044, 0.0175, 0.0540, 0.1295, 0.2420, 0.3521, 0.3989, 0.3521, 0.2420, 0.1295, 0.0540, 0.0175, 0.0044
	/*0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1, 
	0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561*/
};



static const float g_fTotal = 4.0f;



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
	//vector		vSpecular : SV_TARGET1;
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

	//vector	vSpecularMap = g_SpecularMapTexture.Sample(LinearSampler, In.vTexcoord);
	
    vector vMaskMap = g_MapMaskTexture.Sample(PointSampler, In.vTexcoord);

    //Out.vSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);// * (1.f - vSpecularMap.r);

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

	//Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f) * fAtt;

	return Out;
}

PS_OUT PS_MAIN_DEFERRED_RESULT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector			vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

	if (0.0f == vDiffuse.a)
		discard;

	vector			vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    vShade = floor(vShade * 2.0) / 2.0;
	
   // vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vNormal = vector(vNormalDesc.xyz * 2.0 - 1.0, 0.0);
	//
	//
   // float2 offsets[8] =
   // {
   //     float2(-1, -1), float2(0, -1), float2(1, -1),
   //     float2(-1, 0), float2(1, 0),
   //     float2(-1, 1), float2(0, 1), float2(1, 1)
   // };
	//
   // float edgeValue = 0;
   // for (int i = 0; i < 8; i++)
   //     edgeValue += dot(vNormal, g_NormalTexture.Sample(LinearSampler, In.vTexcoord + offsets[i]).xyz * 2.0 - 1.0);
   // 
   // edgeValue = saturate(abs(edgeValue))*2; // 0~1 ������ ����ȭ
   //
   // vector edgeColor = vector(1, 0, 0, 1); // �ܰ��� ���� (��: ������)
   // Out.vColor = lerp(vDiffuse * vShade, edgeColor, edgeValue); // �ܰ��� ȥ��
	
	
	
	//vector			vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

	//vector          vMaskmap = g_MapMaskTexture.Sample(PointSampler, In.vTexcoord);
	
    Out.vColor = vDiffuse * vShade;//+vSpecular;
	
	
    vector vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
    vector vWorldPos;
	
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x; /* 0 ~ 1 */
    vWorldPos.w = 1.f;
	
    vWorldPos = vWorldPos * (vDepthDesc.y * 3000.f);
	
	/* �佺���̽� ���� ��ġ�� ���Ѵ�. */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	
	/* ���彺���̽� ���� ��ġ�� ���Ѵ�. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    vector vLightPos = mul(vWorldPos, g_LightViewMatrix);
    vLightPos = mul(vLightPos, g_LightProjMatrix);
	
    float2 vTexcoord;
    vTexcoord.x = (vLightPos.x / vLightPos.w) * 0.5f + 0.5f;
    vTexcoord.y = (vLightPos.y / vLightPos.w) * -0.5f + 0.5f;
	
    vector vLightDepthDesc = g_LightDepthTexture.Sample(PointSampler, vTexcoord);
	
    float fLightOldDepth = vLightDepthDesc.x * 1000.f;
	
    if (fLightOldDepth + 0.8f < vLightPos.w)
        Out.vColor = vector(Out.vColor.rgb * 0.5f, 1.f);

	return Out;
}





PS_OUT PS_MAIN_RESULT(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_ResultTexture.Sample(LinearSampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);
    vector vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);

   Out.vColor = vResult + vBlur + vEffect;

    return Out;


}



float4 Blur_X(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(1.f / g_fTexW * i, 0);
        vOut += g_fWeight[6 + i] * g_EffectTexture.Sample(LinearSampler, vUV);
    }

    vOut /= g_fTotal;

    return vOut;
}

float4 Blur_Y(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(0, 1.f / g_fTexH * i);
        vOut += g_fWeight[6 + i] * g_EffectTexture.Sample(ClampSampler, vUV);
    }

    vOut /= g_fTotal;

    return vOut;
}

PS_OUT PS_MAIN_BLUR_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_X(In.vTexcoord);
	

    return Out;
}

PS_OUT PS_MAIN_BLUR_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_Y(In.vTexcoord);
	

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
    pass Blur_X
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_X();
    }
    pass Blur_Y
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_Y();
    }

    pass Result
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT();
    }






}

