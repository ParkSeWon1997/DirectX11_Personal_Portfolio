#include "../Bin/ShaderFiles/Engine_Shader_Defines.hlsli"

/* ������Ʈ ���̺�(������̺�) */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_DepthTexture;

// ���̵� ȿ�� ���� �ð� (�� ����)
float g_fFadeDuration;

// ���̵� ȿ�� ���� �ð�
float g_fFadeStartTime;

// ���� �ð� (Update���� ����Ͽ� ����)
float g_fCurrentTime;

float g_fAlphaValue;

int g_iIsSelected;
float g_fHealth;
float g_fMaxHealth;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

/* ���� ���̴� :  /* 
/* 1. ������ ��ġ ��ȯ(����, ��, ����).*/
/* 2. ������ ���������� �����Ѵ�. */
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct VS_OUT_SOFTEFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_SOFTEFFECT VS_MAIN_SOFTEFFECT(VS_IN In)
{
    VS_OUT_SOFTEFFECT Out = (VS_OUT_SOFTEFFECT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

/* TriangleList�� ��� : ���� ������ �޾Ƽ� w�����⸦ �� ������ ���ؼ� �����Ѵ�. */
/* ����Ʈ(��������ǥ��) ��ȯ. */
/* �����Ͷ����� : �������� �ѷ��׿��� �ȼ��� ������, ������ ���������Ͽ� �����. -> �ȼ��� ���������!!!!!!!!!!!! */


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a < 0.1f)
        discard;

	// Out.vColor = tex2D(DefaultSampler, In.vTexcoord);

    return Out;
}

struct PS_IN_SOFTEFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFTEFFECT(PS_IN_SOFTEFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	/* ȭ�� ��ü������ ���� �ȼ��� �׷������ϴ� �ؽ��� ��ǥ. */
    float2 vTexcoord;

    vTexcoord.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexcoord.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

    vector vOldDepthDesc = g_DepthTexture.Sample(PointSampler, vTexcoord);

    Out.vColor.a = Out.vColor.a * saturate(vOldDepthDesc.y * 3000.f - In.vProjPos.w);

    return Out;
}

float CalculateFadeAlpha(float fCurrentTime, float fStartTime, float fDuration)
{
    float fAlpha = 0.0f;
    float fElapsedTime = fCurrentTime - fStartTime;

    if (fElapsedTime < 0.0f)
        return 0.0f; // ���̵� ���� ��
    else if (fElapsedTime < fDuration)
        fAlpha = fElapsedTime / fDuration; // ���̵� ��
    else if (fElapsedTime < fDuration * 2.0f)
        fAlpha = 1.0f; // ���� ����
    else
        fAlpha = 1.0f - (fElapsedTime - fDuration * 2.0f) / fDuration; // ���̵� �ƿ�

    return fAlpha;
}











PS_OUT FadeIn_And_Out(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a < 0.1f)
        discard;
    // ���̵� ���� �� ���
    float fAlpha = CalculateFadeAlpha(g_fCurrentTime, g_fFadeStartTime, g_fFadeDuration);

    // ���̵� ȿ�� ����
    Out.vColor.a *= fAlpha + g_fAlphaValue;

    return Out;
}




PS_OUT Blend_Ui(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 fGrayColor = float4(0.0f, 0.0f, 0.0f, 0.4f);
    
    
    Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    if (In.vTexcoord.x < 0.001f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }
    else if (In.vTexcoord.x > 0.99f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }
    else if (In.vTexcoord.y < 0.01f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }
    else if (In.vTexcoord.y > 0.99f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }

    
    
    
    if (g_iIsSelected == 0)
    {
        Out.vColor = lerp(Out.vColor, fGrayColor, fGrayColor.a);

    }
    
    
    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
}


PS_OUT Hp_bar(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    Out.vColor.a = 0.7f;
    
    float healthRatio = g_fHealth / g_fMaxHealth;
    
    
    if (In.vTexcoord.x < 0.01f || In.vTexcoord.x > 0.99f ||
        In.vTexcoord.y < 0.2f || In.vTexcoord.y > 0.8f)
    {
        Out.vColor = float4(0.f, 0.f, 0.f, 1.0f);
    }
    else
    {
        if (In.vTexcoord.x < healthRatio)
        {
            Out.vColor = float4(1.0f, 0.0f, 0.0f, 0.7f); // ������
        }
       
    }
    
    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
}



PS_OUT Player_Icon(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float4 colorTint = float4(1.0f, 0.0f, 0.0f, 1.0f); // ������
    if (g_iIsSelected == 1)
    {
        colorTint = float4(1.0f, 0.0f, 0.0f, 0.5f);
    }
    if (g_iIsSelected == 0)
    {
        colorTint = float4(0.0f, 0.0f, 1.0f, 0.5f);
    }
    
    // �ؽ�ó ���� ������ ����
    Out.vColor *= colorTint;
    
    
    //if (In.vTexcoord.x < 0.1f)
    //{
    //    Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    //}
    //else if (In.vTexcoord.x > 0.99f)
    //{
    //    Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    //}
    //else if (In.vTexcoord.y < 0.1f)
    //{
    //    Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    //}
    //else if (In.vTexcoord.y > 0.99f)
    //{
    //    Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    //}
    //
    
 
    
    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
}


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

    pass SoftEffect
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* � ���̴��� ��������. ���̴��� �� �������� ����������. �������Լ��� ��������. */
        VertexShader = compile vs_5_0 VS_MAIN_SOFTEFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFTEFFECT();
    }

    pass FadeIn_And_Out
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 FadeIn_And_Out();
    }
    
    pass Blend_Ui
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 Blend_Ui();
    }

    pass Hp_bar
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 Hp_bar();

    }
    pass Player_Icon
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 Player_Icon();

    }
}
