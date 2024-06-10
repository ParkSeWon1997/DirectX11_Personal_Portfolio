#include "../Bin/ShaderFiles/Engine_Shader_Defines.hlsli"

/* 컨스턴트 테이블(상수테이블) */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_DepthTexture;

// 페이드 효과 지속 시간 (초 단위)
float g_fFadeDuration;

// 페이드 효과 시작 시간
float g_fFadeStartTime;

// 현재 시간 (Update에서 계산하여 전달)
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

/* 정점 셰이더 :  /* 
/* 1. 정점의 위치 변환(월드, 뷰, 투영).*/
/* 2. 정점의 구성정보를 변경한다. */
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

/* TriangleList인 경우 : 정점 세개를 받아서 w나누기를 각 정점에 대해서 수행한다. */
/* 뷰포트(윈도우좌표로) 변환. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를, 정점을 선형보간하여 만든다. -> 픽셀이 만들어졌다!!!!!!!!!!!! */


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

	/* 화면 전체기준의 현재 픽셀이 그려져야하는 텍스쳐 좌표. */
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
        return 0.0f; // 페이드 시작 전
    else if (fElapsedTime < fDuration)
        fAlpha = fElapsedTime / fDuration; // 페이드 인
    else if (fElapsedTime < fDuration * 2.0f)
        fAlpha = 1.0f; // 유지 상태
    else
        fAlpha = 1.0f - (fElapsedTime - fDuration * 2.0f) / fDuration; // 페이드 아웃

    return fAlpha;
}











PS_OUT FadeIn_And_Out(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a < 0.1f)
        discard;
    // 페이드 알파 값 계산
    float fAlpha = CalculateFadeAlpha(g_fCurrentTime, g_fFadeStartTime, g_fFadeDuration);

    // 페이드 효과 적용
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
            Out.vColor = float4(1.0f, 0.0f, 0.0f, 0.7f); // 빨간색
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
    
    float4 colorTint = float4(1.0f, 0.0f, 0.0f, 1.0f); // 빨간색
    if (g_iIsSelected == 1)
    {
        colorTint = float4(1.0f, 0.0f, 0.0f, 0.5f);
    }
    if (g_iIsSelected == 0)
    {
        colorTint = float4(0.0f, 0.0f, 1.0f, 0.5f);
    }
    
    // 텍스처 색상에 색상을 곱함
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



PS_OUT Player_Skill(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    // 텍스처 색상에 색상을 곱함
    
    
      
    if (In.vTexcoord.x < 0.05f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }
    else if (In.vTexcoord.x > 0.95f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }
    else if (In.vTexcoord.y < 0.05f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }
    else if (In.vTexcoord.y > 0.95f)
    {
        Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    }
    
 
    
    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
}




technique11 DefaultTechnique
{
	
	/* 특정 렌더링을 수행할 때 적용해야할 셰이더 기법의 셋트들의 차이가 있다. */
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
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

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
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
    pass Player_Skill
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None_Test_None_Write, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 Player_Skill();

    }
}
