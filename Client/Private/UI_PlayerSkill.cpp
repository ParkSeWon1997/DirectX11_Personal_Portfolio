#include "stdafx.h"
#include "..\Public\UI_PlayerSkill.h"

#include "GameInstance.h"
#include"CTotalSingleton.h"

#include"Loader.h"


#include"Fade_In_Out.h"

#include"Changer.h"
#include"Player.h"
UI_PlayerSkill::UI_PlayerSkill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: UI(pDevice, pContext)
{
}

UI_PlayerSkill::UI_PlayerSkill(const UI_PlayerSkill & rhs)
	: UI(rhs)
{
}

HRESULT UI_PlayerSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT UI_PlayerSkill::Initialize(void * pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		
	}



	return S_OK;
}

void UI_PlayerSkill::Priority_Tick(_float fTimeDelta)
{
	
}

void UI_PlayerSkill::Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	
	Fade_In_Out* pFade = dynamic_cast<Fade_In_Out*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Fade_In_Out")));

}

void UI_PlayerSkill::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
		
}

HRESULT UI_PlayerSkill::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	
	



	return S_OK;
}

HRESULT UI_PlayerSkill::Add_Components()
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
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT UI_PlayerSkill::Bind_ShaderResources()
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


UI_PlayerSkill * UI_PlayerSkill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	UI_PlayerSkill*		pInstance = new UI_PlayerSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : UI_PlayerSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * UI_PlayerSkill::Clone(void * pArg)
{
	UI_PlayerSkill*		pInstance = new UI_PlayerSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : UI_PlayerSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void UI_PlayerSkill::Free()
{
 	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
