#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "CTotalSingleton.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "UI.h"
#include "UI_Changer.h"
#include"UI_PlayerIcon.h"
#include"UI_PlayerSkill.h"

#include "Shader.h"
#include"Fade_In_Out.h"

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::GetInstance() }
	, m_pPotalSingleton{ CTotalSingleton::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pPotalSingleton);

	// D3D11_SAMPLER_DESC

	
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowed = true;

	/* 엔진 초기화과정을 거친다. ( 그래픽디바이스 초기화과정 + 레벨매니져를 사용할 준비를 한다. ) */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;	
	
	if(FAILED(Ready_Fonts()))
		return E_FAIL;


	if (FAILED(Ready_Navigation()))
			return E_FAIL;
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;


	

	

	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
	
	m_pGameInstance->Tick_Engine(fTimeDelta);





}

HRESULT CMainApp::Render()
{


	///* 그린다. */
	//if (FAILED(m_pGameInstance->Draw(_float4(0.f, 0.f, 1.f, 1.f))))
	//	return E_FAIL;
	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Draw()))
		return E_FAIL;

	//m_pGameInstance->Render_Font(TEXT("Font_Default"), TEXT("Hp"), _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;

	return S_OK;
}


HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMainApp::Ready_Navigation()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	



	_float3		vPoints[3] = {};

	_ulong		dwByte = {};
	HANDLE		hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;


	//1픽셀씩 작게 찍기 

	vPoints[0] = _float3(0.f, 0.f, 34.f);
	vPoints[1] = _float3(34.f, 0.f, 0.f);
	vPoints[2] = _float3(0.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.f, 0.f, 34.f);
	vPoints[1] = _float3(34.f, 0.f, 34.f);
	vPoints[2] = _float3(34.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);


	//vPoints[0] = _float3(0.f, 0.f, 10.f);
	//vPoints[1] = _float3(10.f, 0.f, 0.f);
	//vPoints[2] = _float3(0.f, 0.f, 0.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	//
	//vPoints[0] = _float3(0.f, 0.f, 10.f);
	//vPoints[1] = _float3(10.f, 0.f, 10.f);
	//vPoints[2] = _float3(10.f, 0.f, 0.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	//
	//vPoints[0] = _float3(0.f, 0.f, 20.f);
	//vPoints[1] = _float3(10.f, 0.f, 10.f);
	//vPoints[2] = _float3(0.f, 0.f, 10.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	//
	//vPoints[0] = _float3(10.f, 0.f, 10.f);
	//vPoints[1] = _float3(20.f, 0.f, 0.f);
	//vPoints[2] = _float3(10.f, 0.f, 0.0f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	//
	//vPoints[0] = _float3(0.f, 0.f, 20.f);
	//vPoints[1] = _float3(10.f, 0.f, 20.f);
	//vPoints[2] = _float3(10.f, 0.f, 10.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	//
	//vPoints[0] = _float3(10.f, 0.f, 20.f);
	//vPoints[1] = _float3(20.f, 0.f, 20.f);
	//vPoints[2] = _float3(10.f, 0.f, 10.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	//
	//vPoints[0] = _float3(20.f, 0.f, 20.f);
	//vPoints[1] = _float3(20.f, 0.f, 10.f);
	//vPoints[2] = _float3(10.f, 0.f, 10.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	//
	//vPoints[0] = _float3(20.f, 0.f, 10.f);
	//vPoints[1] = _float3(20.f, 0.f, 0.f);
	//vPoints[2] = _float3(10.f, 0.f, 10.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);




	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Default"), TEXT("../Bin/Resources/Fonts/FuckDem_20.spritefont"))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Changer"), UI_Changer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fade_In_Out"), Fade_In_Out::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerIcon"), UI_PlayerIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerSkill"), UI_PlayerSkill::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), 
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/HeungDolLogo_v3.jpg"),1))))
		return E_FAIL;






	/* For.Prototype_Component_Texture_Loading */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
	//	return E_FAIL;


	return S_OK;
}




CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pPotalSingleton);
	Safe_Release(m_pPotalSingleton);

	/* 레퍼런스 카운트를 0으로만든다. */
	Safe_Release(m_pGameInstance);
	CGameInstance::Release_Engine();
}
