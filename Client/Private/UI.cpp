#include "stdafx.h"
#include "..\Public\UI.h"

#include "GameInstance.h"
#include"Loader.h"

#include"CTotalSingleton.h"
UI::UI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

UI::UI(const UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT UI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT UI::Initialize(void * pArg)
{
	UI_DESC* pDesc = (UI_DESC*)pArg;



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
		m_iPassIndex = pDesc->iPassIndex;
	}


	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	return S_OK;
}

void UI::Priority_Tick(_float fTimeDelta)
{
	
}

void UI::Tick(_float fTimeDelta)
{
	
}

void UI::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT UI::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	
	m_pShaderCom->Bind_RawValue("g_iIsSelected", &m_iIsSelect, sizeof(int));
	m_pShaderCom->Bind_RawValue("g_fHealth", &m_fHp, sizeof(float));
	m_pShaderCom->Bind_RawValue("g_fMaxHealth", &m_fMaxHp, sizeof(float));


	m_pShaderCom->Begin(m_iPassIndex);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();
	wsprintf(m_szCoin, TEXT("RUPEE %d"), CTotalSingleton::GetInstance()->GetCoin());


	m_pGameInstance->Render_Font(TEXT("Font_Default"), m_szCoin, _float2(5.f, 2.5f), XMVectorSet(0.f, 0.f, 0.f, 1.f),0.0f,1.05f);
	m_pGameInstance->Render_Font(TEXT("Font_Default"), m_szCoin, _float2(10.f, 5.f), XMVectorSet(1.f, 0.647f, 0.f, 1.f),0.0f,1.0f);
	


	int hours = CTotalSingleton::GetInstance()->GetHours();
	int minutes = CTotalSingleton::GetInstance()->GetMinutes();
	int seconds = CTotalSingleton::GetInstance()->GetSeconds();

	TCHAR szTime[MAX_PATH];
	wsprintf(szTime, TEXT("TIME %02d:%02d:%02d"), hours, minutes, seconds);

	m_pGameInstance->Render_Font(TEXT("Font_Default"), szTime, _float2(1000.f, 15.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.0f, 1.05f);
	m_pGameInstance->Render_Font(TEXT("Font_Default"), szTime, _float2(1005.f, 17.5f), XMVectorSet(1.f, 0.647f, 0.f, 1.f), 0.0f, 1.0f);




	TCHAR szStage[MAX_PATH];
	_int iStage = 0;
	if (CLoader::m_eNextLevel == LEVEL_GAMEPLAY)
	{
		wsprintf(szStage, TEXT("STAGE 1"));
	}
	else if(CLoader::m_eNextLevel ==LEVEL_STAGE_1)
	{
		wsprintf(szStage, TEXT("BOSS_B"));
	}
	else if (CLoader::m_eNextLevel == LEVEL_STAGE_2)
	{
		wsprintf(szStage, TEXT("STAGE 2"));
	}
	else if (CLoader::m_eNextLevel == LEVEL_STAGE_3)
	{
		wsprintf(szStage, TEXT("BOSS_C"));
	}
	else if (CLoader::m_eNextLevel == LEVEL_CHANGE_STAGE)
	{
		wsprintf(szStage, TEXT("SHOP"));
	}

	

	m_pGameInstance->Render_Font(TEXT("Font_Default"), szStage, _float2(5.f, 20.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.0f, 1.05f);
	m_pGameInstance->Render_Font(TEXT("Font_Default"), szStage, _float2(10.f, 22.5f), XMVectorSet(1.f, 0.647f, 0.f, 1.f), 0.0f, 1.0f);


	if (m_bIsDead)
	{
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Ui"), this);
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Ui_Changer_Icon"), this);
	}



	return S_OK;
}

HRESULT UI::Add_Components()
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
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel,m_strModelName,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;	
	
	return S_OK;
}

HRESULT UI::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

UI * UI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	UI*		pInstance = new UI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * UI::Clone(void * pArg)
{
	UI*		pInstance = new UI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void UI::Free()
{
 	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
