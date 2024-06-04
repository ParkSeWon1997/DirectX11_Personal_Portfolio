#include "../Bin/ShaderFiles/Engine_Shader_Defines.hlsli"


/* ������Ʈ ���̺�(������̺�) */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_SpecularTexture;

int g_MeshNum;
int g_IntParam[4];

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vLocalPos: TEXCOORD2;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    
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
    Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
    Out.vLocalPos = float4(In.vPosition, 1.f);
    Out.vTangent = normalize(mul(vector(In.vTangent.xyz, 0.f), g_WorldMatrix));
    Out.vBinormal = vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f);
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
    float3 vLocalPos : TEXCOORD2;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
    //vector vSpecular : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = mul(vNormal, WorldMatrix);
    float angle = atan2(In.vLocalPos.z, In.vLocalPos.x);

    if (angle < 0)
    {
        angle += 2 * 3.14159265359;
    }
    
    float fOffset = 3.14159265359 / 4;
    angle += fOffset;
    
    if (angle >= 2 * 3.14159265359)
    {
        angle -= 2 * 3.14159265359;
    }

    if (g_MeshNum == 1)
    {
    
        
        
       if (angle < 3.14159265359 / 2)
       {
            if (g_IntParam[2] == 0)
            {
                vDiffuse = float4(0.f, 0.f, 0.f, 1.f); // ������ �Ʒ�
            }
            else
            {
                vDiffuse = float4(1.f, 1.f, 1.f, 1.f); // 
            }
            
            
        
       }
       else if (angle < 3.14159265359)
        {
            if (g_IntParam[1] == 0)
            {
                vDiffuse = float4(0.f, 0.f, 0.f, 1.f); // ������ ��
            }
            else
            {
                vDiffuse = float4(1.f, 1.f, 1.f, 1.f); // 
            }
         
            
         
       }
       else if (angle < 3.14159265359 * 3 / 2)
       {
            if(g_IntParam[0]==0)
            {
                vDiffuse = float4(0.f, 0.f, 0.f, 1.f); // ���� ��
            }
            else
            {
                vDiffuse = float4(1.f, 1.f, 1.f, 1.f); // 
            }
            
       }
       else
       {
            if (g_IntParam[3] == 0)
            {
                vDiffuse = float4(0.f, 0.f, 0.f, 1.f); // ���� �Ʒ�
            }
            else
            {
                vDiffuse = float4(1.f, 1.f, 1.f, 1.f); // 
            }
        }
    }
    if (vDiffuse.a < 0.1f)
        discard;

    
    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 3000.f, 0.0f, 1.f);
 //   Out.vSpecular = vSpecular;
    return Out;
}
PS_OUT PS_Top(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (vDiffuse.a < 0.1f)
        discard;

    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

    vNormal = mul(vNormal, WorldMatrix);

    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 3000.f, 0.0f, 1.f);
    //Out.vSpecular = vSpecular;
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
    pass MachineTop
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Top();
    }

}

