#include "stdafx.h"
#include "..\Public\Level_Stage_Change.h"
#include "GameInstance.h"

#include "FreeCamera.h"
#include"Monster.h"
#include"UpgradeMachine.h"
#include"CStage.h"
#include"Level_Loading.h"

#include"CHandBoss.h"
#include"Particle_Mesh.h"
#include"CHandBullet.h"
CLevel_Stage_Change::CLevel_Stage_Change(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_Change::Initialize()
{

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_0_Stage(TEXT("Layer_0_Stage"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_2_Player(TEXT("Layer_2_Player"))))
		return E_FAIL;

	if(FAILED(Ready_Layer_Environment(TEXT("Layer_Environment"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_1_Segment(TEXT("Layer_1_Segment"))))
		return E_FAIL;



	return S_OK;
}

void CLevel_Stage_Change::Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		//if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_CHANGE_STAGE))))
		//	return;
		//
		//return;
	}
#ifdef _DEBUG
	ostringstream oss;
	oss << std::fixed << std::setprecision(2) << m_pGameInstance->Get_FPS(TEXT("Timer_60"));
	string FpsString = "FPS:" + oss.str();
	//SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
	SetWindowTextA(g_hWnd,FpsString.c_str());
	

#endif
}

HRESULT CLevel_Stage_Change::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	m_pGameInstance->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Layer_Camera(const wstring & strLayerTag)
{
	CFreeCamera::FREE_CAMERA_DESC		CameraDesc{};

	CameraDesc.fSensor = 0.1f;
	CameraDesc.vEye = _float4(0.0f, 10.f, -15.f, 1.f);
	CameraDesc.vAt = _float4(0.0f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 3000.f;
	CameraDesc.fSpeedPerSec = 20.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.f);	

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_FreeCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Layer_Effect(const wstring& strLayerTag)
{
	CParticle_Mesh::PARTICLE_DESC desc;
	desc.strModelName = TEXT("Boss_C_Mass_Falling");
	desc.vStartPos = _float4(0.f, 0.f, 0.f, 1.0f);
	desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN;

	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_Particle"), &desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Layer_BackGround(const wstring & strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Layer_0_Stage(const wstring& strLayerTag)
{

	CStage::CStage_DESC desc;
	//desc.strModelName = TEXT("StageGround_A_57_New_A");
	//desc.strModelName = TEXT("StageGroundAssemble_C_SM_A_66");
	desc.strModelName = TEXT("StageGroundAssemble_E_GS_A_66");
	desc.vPosition = _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_CStage"), &desc)))
		return E_FAIL;



	desc.strModelName = TEXT("StageGroundAssemble_E_SM_A_66");
	desc.vPosition = _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_CStage_SM"), &desc)))
		return E_FAIL;




	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Layer_2_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	
	return S_OK;
}



HRESULT CLevel_Stage_Change::Ready_Layer_Environment(const wstring& strLayerTag)
{
	CUpgradeMachine::CUpgradeMachine_DESC desc;

	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(-17.f, 0.0f, 24.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_HP;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;
	
	
	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(-13.f, 0.0f, 28.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_SHIELD;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;
	
	
	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(-9.f, 0.0f, 32.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_MOVE_SPEED;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;
	
	
	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(-5.f, 0.0f, 36.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_ATTACK_SPEED;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;
	
	
	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(17.f, 0.0f, 24.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_ATTACK_DAMAGE;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;
	
	
	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(13.f, 0.0f, 28.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_DASH_COUNT;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;
	
	
	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(9.f, 0.0f, 32.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_EXP;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Foothold_Shop");
	desc.vPosition = _float4(5.f, 0.0f, 36.f, 1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	desc.eMachineType = CUpgradeMachine::MACHINE_ADD_COOL_TIME;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_UpgradeMachine"), &desc)))
		return E_FAIL;


	



	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Layer_1_Segment(const wstring& strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC SegmentDesc;
	SegmentDesc.strModelName = TEXT("Segment_GS_BG");
	SegmentDesc.vPosition = _float4(50.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &SegmentDesc)))
		return E_FAIL;


	SegmentDesc.strModelName = TEXT("Segment_SM_BG");
	SegmentDesc.vPosition = _float4(-85.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_CSegment_SM"), &SegmentDesc)))
		return E_FAIL;

	//desc.strModelName = TEXT("Segment_002");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &desc)))
	//	return E_FAIL;
	//

	CGameObject::GAMEOBJECT_DESC SegmentLineDesc;
	SegmentLineDesc.vPosition = _float4(0.0f, 0.02f, 21.f, 1.0f);
	SegmentLineDesc.strModelName = TEXT("Segment_GS_Stroke_Default_Line");
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_CSegment_Line"), &SegmentLineDesc)))
		return E_FAIL;

}

CLevel_Stage_Change * CLevel_Stage_Change::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Stage_Change*		pInstance = new CLevel_Stage_Change(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Stage_Change");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage_Change::Free()
{
	__super::Free();

}
