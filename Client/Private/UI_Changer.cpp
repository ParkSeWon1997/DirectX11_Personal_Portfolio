#include "stdafx.h"
#include "..\Public\UI_Changer.h"

#include "GameInstance.h"
#include"CTotalSingleton.h"

#include"Loader.h"


#include"Fade_In_Out.h"

#include"Changer.h"
#include"Player.h"
UI_Changer::UI_Changer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: UI(pDevice, pContext)
{
}

UI_Changer::UI_Changer(const UI_Changer & rhs)
	: UI(rhs)
{
}

HRESULT UI_Changer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT UI_Changer::Initialize(void * pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		i_ChreaterType= static_cast<UI_Changer_DESC*>(pArg)->iChangerType;
	}



	return S_OK;
}

void UI_Changer::Priority_Tick(_float fTimeDelta)
{
	
}

void UI_Changer::Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	
	Fade_In_Out* pFade = dynamic_cast<Fade_In_Out*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Fade_In_Out")));
	pFade->Set_UiOpen(true);
	



	if (KEY_TAP(DIK_LEFT))
	{
		if (i_ChreaterType == 1)
		{
			if (m_iUiGunIndex > 1)
				m_iUiGunIndex--;
		}
		if (i_ChreaterType == 0)
		{
			if (m_iUiSwordIndex > 1)
				m_iUiSwordIndex--;
		}

	}

	if (KEY_TAP(DIK_RIGHT))
	{
		if (i_ChreaterType == 1)
		{
			if (m_iUiGunIndex < 4)
				m_iUiGunIndex++;
		}
		if (i_ChreaterType == 0)
		{
			if (m_iUiSwordIndex < 4)
				m_iUiSwordIndex++;
		}
	}

	for (int i = 2; i <= 13; ++i)
	{
		UI* pUi = dynamic_cast<UI*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Ui_Changer_Icon"), i));

		if (pUi)
		{
			if (i_ChreaterType == 1)
			{
				if (m_iUiGunIndex == 1 && i >= 2 && i <= 4)
					pUi->Set_IsSelect(1);
				else if (m_iUiGunIndex == 2 && i >= 5 && i <= 7)
					pUi->Set_IsSelect(1);
				else if (m_iUiGunIndex == 3 && i >= 8 && i <= 10)
					pUi->Set_IsSelect(1);
				else if (m_iUiGunIndex == 4 && i >= 11 && i <= 13)
					pUi->Set_IsSelect(1);
				else
					pUi->Set_IsSelect(0);
			}
			if (i_ChreaterType == 0)
			{
				if (m_iUiSwordIndex == 1 && i >= 2 && i <= 4)
					pUi->Set_IsSelect(1);
				else if (m_iUiSwordIndex == 2 && i >= 5 && i <= 7)
					pUi->Set_IsSelect(1);
				else if (m_iUiSwordIndex == 3 && i >= 8 && i <= 10)
					pUi->Set_IsSelect(1);
				else if (m_iUiSwordIndex == 4 && i >= 11 && i <= 13)
					pUi->Set_IsSelect(1);
				else
					pUi->Set_IsSelect(0);
			}
		}

	}

	if (KEY_TAP(DIK_SPACE))
	{
		
		if (i_ChreaterType == 0)
		{
			pPlayer->SetPlayerSwordType(static_cast<CPlayer::SWORD_MASTER_TYPE>(m_iUiSwordIndex - 1));

		}
		
		if (i_ChreaterType == 1)
		{
			
			pPlayer->SetPlayerGunType(static_cast<CPlayer::GUN_SLINGER_TYPE>(m_iUiGunIndex -1));
		}
		
		
		b_bIsOpen = false;
		b_IsCreate = false;
		pFade->Set_UiOpen(false);

		m_bIsDead= true;
		CTotalSingleton::GetInstance()->SetUiOpen(false);
	}


	//한 번만 만드는 작업

		if (b_IsCreate==false)
		{
			CTotalSingleton::GetInstance()->SetUiOpen(true);
			Make_Changer_Ui();
			b_IsCreate = true;
		}
}

void UI_Changer::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
		
}

HRESULT UI_Changer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	if (i_ChreaterType == 0)
	{
		//m_pGameInstance->Render_Font(TEXT("Font_Default"), TEXT("Gun"), _float2(m_fX, 200.f), XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}
	if (i_ChreaterType == 1)
	{
		//m_pGameInstance->Render_Font(TEXT("Font_Default"), TEXT("담배 피러 가자"), _float2(m_fX, 200.f), XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}



	if (m_bIsDead)
	{

		Fade_In_Out* pFade = dynamic_cast<Fade_In_Out*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Fade_In_Out")));
		pFade->Set_UiOpen(false);



		_uint iChangerIconLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_Ui_Changer_Icon"));

		for (int i = 0; i < iChangerIconLayerSize; ++i)
		{
			UI* pUi = dynamic_cast<UI*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Ui_Changer_Icon"), i));
			pUi->Set_Dead(true);
		}




		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Ui_Changer"), this);
		CTotalSingleton::GetInstance()->SetUiOpen(false);
	}




	return S_OK;
}

HRESULT UI_Changer::Add_Components()
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

HRESULT UI_Changer::Bind_ShaderResources()
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

void UI_Changer::Make_Changer_Ui()
{
	UI::UI_DESC desc;


	desc.strModelName = TEXT("Icon_Keyboard_Left");
	desc.fSizeX = 64.f;
	desc.fSizeY = 64.f;
	desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 6.f);
	desc.fY = g_iWinSizeY * 0.5f;
	m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);


	desc.strModelName = TEXT("Icon_Keyboard_Right");
	desc.fSizeX = 64.f;
	desc.fSizeY = 64.f;
	desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 6.f);
	desc.fY = g_iWinSizeY * 0.5f;
	m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);




	if (i_ChreaterType == 1)
	{
		
		desc.strModelName = TEXT("Amanda_Skill_Balance_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f)- (desc.fSizeX*2.f);
		desc.fY = (g_iWinSizeY * 0.5f)- (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Amanda_Skill_Balance_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
		
		desc.strModelName = TEXT("Amanda_Skill_Balance_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
		//--------------------------------------------------------------


		desc.strModelName = TEXT("Amanda_Skill_Technic_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) - (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Amanda_Skill_Technic_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Amanda_Skill_Technic_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
		//--------------------------------------------------------------


		desc.strModelName = TEXT("Amanda_Skill_Power_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) - (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Amanda_Skill_Power_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Amanda_Skill_Power_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
		//--------------------------------------------------------------


		desc.strModelName = TEXT("Amanda_Skill_Reverse_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f) - (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Amanda_Skill_Reverse_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Amanda_Skill_Reverse_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
	}



	if (i_ChreaterType == 0)
	{
		UI::UI_DESC desc;
		desc.strModelName = TEXT("Matilda_Skill_Balance_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f) - (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Balance_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Balance_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
		//--------------------------------------------------------------


		desc.strModelName = TEXT("Matilda_Skill_Technic_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) - (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Technic_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Technic_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
		//--------------------------------------------------------------


		desc.strModelName = TEXT("Matilda_Skill_Power_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) - (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Power_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Power_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 0.7f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
		//--------------------------------------------------------------


		desc.strModelName = TEXT("Matilda_Skill_Reverse_X");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f) - (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Reverse_C");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);

		desc.strModelName = TEXT("Matilda_Skill_Reverse_XC");
		desc.fSizeX = 125.f;
		desc.fSizeY = 100.f;
		desc.fX = (g_iWinSizeX * 0.5f) + (desc.fSizeX * 2.f);
		desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
		desc.iPassIndex = 3;
		m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Ui_Changer_Icon"), TEXT("Prototype_GameObject_UI"), &desc);
	}








}

UI_Changer * UI_Changer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	UI_Changer*		pInstance = new UI_Changer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : UI_Changer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * UI_Changer::Clone(void * pArg)
{
	UI_Changer*		pInstance = new UI_Changer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : UI_Changer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void UI_Changer::Free()
{
 	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
