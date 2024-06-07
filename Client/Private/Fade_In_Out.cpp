#include "stdafx.h"
#include "..\Public\Fade_In_Out.h"

#include "GameInstance.h"
#include"Loader.h"

Fade_In_Out::Fade_In_Out(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

Fade_In_Out::Fade_In_Out(const Fade_In_Out & rhs)
	: CGameObject(rhs)
{
}

HRESULT Fade_In_Out::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Fade_In_Out::Initialize(void * pArg)
{
	Fade_In_Out_DESC* pDesc = (Fade_In_Out_DESC*)pArg;



	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_fSizeX = pDesc->fSizeX;
		m_fSizeY = pDesc->fSizeY;
		m_fX = pDesc->fX;
		m_fY = pDesc->fY;
	}


	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	return S_OK;
}

void Fade_In_Out::Priority_Tick(_float fTimeDelta)
{
	
}

void Fade_In_Out::Tick(_float fTimeDelta)
{

	if (m_bIsFadeStart)
	{
		m_fCurrentTime += fTimeDelta;

		//원상태로 돌아 왔을 떼
		if (m_fCurrentTime >= m_fFadeStartTime + 2 * m_fFadeDuration)
		{
			m_bIsFadeStart = false;
			
			m_fCurrentTime = 0.f;
		}
	}
	if (m_fCurrentTime >= m_fFadeStartTime + m_fFadeDuration)
	{
		m_bIsFadeEnd = true;
	}
	//if (m_bIsFadeStart)
	//{
	//	m_fCurrentTime = 0.f;
	//	
	//	m_bIsFade = true;
	//	
	//}

	
}

void Fade_In_Out::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_POST, this);
}

HRESULT Fade_In_Out::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;






	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT Fade_In_Out::Add_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), 
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Fade_In_Out"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;	
	
	return S_OK;
}

HRESULT Fade_In_Out::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFadeDuration", &m_fFadeDuration, sizeof(float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFadeStartTime", &m_fFadeStartTime, sizeof(float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fCurrentTime", &m_fCurrentTime, sizeof(float))))
		return E_FAIL;




	return S_OK;
}

Fade_In_Out * Fade_In_Out::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	Fade_In_Out*		pInstance = new Fade_In_Out(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Fade_In_Out");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * Fade_In_Out::Clone(void * pArg)
{
	Fade_In_Out*		pInstance = new Fade_In_Out(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Fade_In_Out");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Fade_In_Out::Free()
{
 	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
