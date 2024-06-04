#include "../Bin/ShaderFiles/Engine_Shader_Defines.hlsli"

/* 컨스턴트 테이블(상수테이블) */
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_Texture;
Texture2D	g_NoiseTexture;
float       g_DisolveAmount;

float4 g_vColor;

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

/* 정점 셰이더 :  /* 
/* 1. 정점의 위치 변환(월드, 뷰, 투영).*/
/* 2. 정점의 구성정보를 변경한다. */
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

/* TriangleList인 경우 : 정점 세개를 받아서 w나누기를 각 정점에 대해서 수행한다. */
/* 뷰포트(윈도우좌표로) 변환. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를, 정점을 선형보간하여 만든다. -> 픽셀이 만들어졌다!!!!!!!!!!!! */


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

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
  //Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
	
    float2 center = float2(0.5, 0.5);
    float dist = length(In.vTexcoord - center);

    // 검정색일 때 알파값을 할당 (중점으로부터의 거리에 비례하여 알파값 증가)
    //Out.vColor.rgb = float3(0, 0, 0);
    //Out.vColor.a = 0.5f; //smoothstep(0, 0.5, dist) * In.vLifeTime.y; // 중점에서부터 0.5까지 알파값이 0에서 1로 선형적으로 증가하고, 생명시간을 곱함

	
    //Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
	
	
	
	
	//Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);

	//if (Out.vColor.a < 0.1f)
	//	discard;


   Out.vColor.rgb= float3(1.f, 1.f, 1.f);

	

	// Out.vColor = tex2D(DefaultSampler, In.vTexcoord);

	return Out;
}

PS_OUT PS_MAIN_SPREAD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    Out.vColor.rgba = g_vColor.rgba;
    
	/*if (Out.vColor.a < 0.1f || 
		In.vLifeTime.y > In.vLifeTime.x)
		discard;*/

	if (Out.vColor.a < 0.1f)
		discard;

	Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;


	return Out;
}
PS_OUT PS_MAIN_DROP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    Out.vColor.rgba = g_vColor.rgba;
    float noise = g_NoiseTexture.Sample(PointSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
	/*if (Out.vColor.a < 0.1f || 
		In.vLifeTime.y > In.vLifeTime.x)
		discard;*/

	if (Out.vColor.a < 0.1f)
		discard;

	Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;


	return Out;
}

PS_OUT PS_MAIN_UP_AND_DOWN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    
    Out.vColor.rgba = Out.vColor.rgba*g_vColor.rgba;


    /*if (Out.vColor.a < 0.1f || 
        In.vLifeTime.y > In.vLifeTime.x)
        discard;*/

    
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  

    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;

}

PS_OUT PS_MAIN_Explosion(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor.rgba = g_vColor.rgba;

    float noise = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
    //Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
   
   // Out.vColor = Out.vColor * g_vColor.rgba;
    /*if (Out.vColor.a < 0.1f || 
        In.vLifeTime.y > In.vLifeTime.x)
        discard;*/

    
    Out.vColor.a = In.vLifeTime.x -In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  

    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	
	
	
}

PS_OUT PS_MAIN_Pop(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    Out.vColor.rgba = g_vColor.rgba;
    float noise = g_NoiseTexture.Sample(PointSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }

    if (Out.vColor.a < 0.1f)
        discard;

    if(In.vLifeTime.y > In.vLifeTime.x)
		discard;
    
    

    //PS_OUT Out = (PS_OUT) 0;
    //Out.vColor.rgba = g_vColor.xyzw;
    //Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    //if (Out.vColor.a < 0.1f)
    //    discard;

  

    //Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	
	
	
}
PS_OUT PS_MAIN_Random_Pop(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    if(Out.vColor.a == 0.0f)
    {
        Out.vColor.a=0.5f;
    
    }
        
    Out.vColor.rgba = Out.vColor.rgba * g_vColor.rgba;

    if (Out.vColor.a < 0.1f)
        discard;

    if(In.vLifeTime.y > In.vLifeTime.x)
		discard;
    
    

  

    //Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	
	
	
}
PS_OUT PS_MAIN_Pop_Ahlpa(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;
   
    
    
    //Out.vColor.a = 0.5f;
  

    //Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    Out.vColor.rgba=float4(0.f, 0.f, 0.f, 0.5f);
    return Out;
	
	
	
}

PS_OUT PS_MAIN_Color_Change(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;
   
    
    
    //Out.vColor.a = 0.5f;
  
    //g_vColor.rgba 
    //Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    Out.vColor.rgba += g_vColor.rgba;
    return Out;
	
	
	
}
PS_OUT PS_MAIN_Texture_Set(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
	float4 Diffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Diffuse.a=0.3f;
    //Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	float4 color = float4(0.0f, 0.0f, 0.3f, 1.f);
	
    Diffuse = lerp(Diffuse, color, 0.9);
    Out.vColor=Diffuse;
	
    if (Out.vColor.a < 0.1f)
        discard;
	
    Out.vColor.a =In.vLocalPos.y * 0.7f;
	
	
    return Out;
	
	
}

PS_OUT PS_SIZEUP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor.rgba = g_vColor.xyzw;
   
    float noise = g_NoiseTexture.Sample(PointSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  
    
   
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	
 
}

PS_OUT PS_SIZEDOWN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float noise = g_NoiseTexture.Sample(PointSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  

    Out.vColor.rgb = g_vColor.rgb;
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	
 
}
PS_OUT SizeDOWn_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor.rgba = g_vColor.xyzw;
    float noise = g_NoiseTexture.Sample(PointSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  
    
   
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	

	
 
}
PS_OUT SizeDOWn_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor.rgba = g_vColor.xyzw;
    float noise = g_NoiseTexture.Sample(PointSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  
    
   
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	

	
 
}
PS_OUT SizeDOWn_Z(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor.rgba = g_vColor.xyzw;
    float noise = g_NoiseTexture.Sample(PointSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
    
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  
    
   
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	

	
 
}
PS_OUT PS_MAIN_Rotate(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor.rgba = g_vColor.xyzw;
    float noise = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord).r;
    if (noise < g_DisolveAmount)
    {
        discard;
    }
    
    
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
	

	
 
}
PS_OUT PS_MAIN_None_Disolve_SizeDOWn_Z(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor.rgba = g_vColor.xyzw;

    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (Out.vColor.a < 0.1f)
        discard;

  
    
   
    //Out.vColor.rgba=float4(1.f, 1.f, 1.f, 1.f);
    return Out;
	


	
 
}
PS_OUT PS_MAIN_None_Disolve_Pop(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    Out.vColor.rgba = g_vColor.rgba;
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
    if (In.vLifeTime.y > In.vLifeTime.x)
        discard;
    
    if (Out.vColor.a < 0.1f)
        discard;

  
    return Out;
	
 
}
PS_OUT PS_MAIN_None_Disolve(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    Out.vColor.rgba = g_vColor.rgba;
    if (In.vLifeTime.y > In.vLifeTime.x)
        discard;
    
    if (Out.vColor.a < 0.1f)
        discard;

   
    return Out;
	
 
}
PS_OUT PS_MAIN_Texture_Up_And_Down(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
 
 
    if (In.vLifeTime.y > In.vLifeTime.x)
        discard;
    
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
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
  
	pass Spread
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SPREAD();
	}	

    pass Drop
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DROP();
    }
    pass Up_And_Down
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UP_AND_DOWN();
    }

    pass Explosion
    {
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Explosion();
    }	
	pass Pop
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Pop();
    }
    pass Random_Pop
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        /* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Random_Pop();
    }
    pass SizeUpPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
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
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SIZEDOWN();
    }
    pass SizeDOWn_X
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 SizeDOWn_X();
    }
    pass SizeDOWn_Y
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 SizeDOWn_Y();
    }
   
    pass SizeDOWn_Z
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 SizeDOWn_Z();
    }
    pass Pop_Ahlpa
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Pop_Ahlpa();
    } 

    pass Color_Change	
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Color_Change();
    }

    pass Texture_Set
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Texture_Set();
    }
    pass Rotate
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Rotate();

    }

    pass None_Disolve_SizeDOWn_Z
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_None_Disolve_SizeDOWn_Z();

    } 
    pass None_Disolve_Pop
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_None_Disolve_Pop();

    }
    pass None_Disolve
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_None_Disolve();

    }
    pass Texture_Up_And_Down
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 어떤 셰이덜르 국동할지. 셰이더를 몇 버젼으로 컴파일할지. 진입점함수가 무엇이찌. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Texture_Up_And_Down();

    }

  
   
    


}

