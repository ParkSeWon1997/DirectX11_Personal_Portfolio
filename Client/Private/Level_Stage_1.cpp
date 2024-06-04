#include "stdafx.h"
#include "..\Public\Level_Stage_1.h"
#include "GameInstance.h"

#include "FreeCamera.h"
#include"Monster.h"
#include"EnvironmentObj.h"
#include"CStage.h"
#include"Level_Loading.h"

#include"CHandBoss.h"
#include"Particle_Mesh.h"
#include"CHandBullet.h"
CLevel_Stage_1::CLevel_Stage_1(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_1::Initialize()
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

	if (FAILED(Ready_Layer_2_Monster(TEXT("Layer_2_Monster"))))
		return E_FAIL;

	if(FAILED(Ready_Layer_Environment(TEXT("Layer_Environment"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_1_Segment(TEXT("Layer_1_Segment"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Stage_1::Tick(_float fTimeDelta)
{
	if (KEY_TAP(DIK_5))
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_CHANGE_STAGE))))
			return;
		
		return;
	}
#ifdef _DEBUG
	ostringstream oss;
	oss << std::fixed << std::setprecision(2) << m_pGameInstance->Get_FPS(TEXT("Timer_60"));
	string FpsString = "FPS:" + oss.str();
	//SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
	SetWindowTextA(g_hWnd,FpsString.c_str());
	

#endif
}

HRESULT CLevel_Stage_1::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	m_pGameInstance->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_Camera(const wstring & strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_FreeCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_Effect(const wstring& strLayerTag)
{
	CParticle_Mesh::PARTICLE_DESC desc;
	desc.strModelName = TEXT("Boss_C_Mass_Falling");
	desc.vStartPos = _float4(0.f, 0.f, 0.f, 1.0f);
	desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN;

	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Particle"), &desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_BackGround(const wstring & strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_0_Stage(const wstring& strLayerTag)
{

	CStage::CStage_DESC desc;
	//desc.strModelName = TEXT("StageGround_A_57_New_A");
	//desc.strModelName = TEXT("StageGroundAssemble_C_SM_A_66");
	desc.strModelName = TEXT("StageGroundAssemble_C_GS_A_66");
	desc.vPosition = _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_CStage"),&desc)))
		return E_FAIL;



	desc.strModelName = TEXT("StageGroundAssemble_C_SM_A_66");
	desc.vPosition = _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_CStage_SM"), &desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_2_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_2_Monster(const wstring & strLayerTag)
{
	random_device randomDevice;
	mt19937_64 Gen(randomDevice());


	uniform_real_distribution<float> fRandomX(0.f, 30.f);
	uniform_real_distribution<float> fRandomZ(0.f, 30.f);

	//decltype(auto) loadedData = Load_Data<_matrix, wstring, _matrix, wstring>(strLayerTag);	//타입과 갯수를 지정해줘야함
	//_matrix pWorldMatrix = get<0>(loadedData);
	//wstring pObjname = get<1>(loadedData);
	//_matrix pWorldMatri1 = get<2>(loadedData);
	//wstring pObjname1 = get<3>(loadedData);
	_matrix	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(-45.0f));
	_matrix pWorldMatrix = {};

	
	CMonster::CMonster_DESC desc;
	desc.m_pWorldMatrix = pWorldMatrix;
	desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen),1.0f);
	
	
	//desc.strModelName = TEXT("Boss_D");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Boss_D"), &desc)))
	//	return E_FAIL;

	//desc.strModelName = TEXT("Boss_D_Picker_New");
	//desc.vPosition = _float4(0.f, 0.f, 20.f, 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_CBoss_D_Picker"), &desc)))
	//	return E_FAIL;

	//desc.strModelName = TEXT("Boss_B");
	//desc.vPosition = _float4(0.0f, 0.f, 20.f, 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Boss_B"), &desc)))
	//	return E_FAIL;

	//
	//
	

	//desc.strModelName = TEXT("NewMoldTest");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//desc.strDeconModelTag = TEXT("NewMold_Deco_Segment");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_NewMold"), &desc)))
	//	return E_FAIL;
	////
	//desc.strModelName = TEXT("NewMoldTest");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//desc.strDeconModelTag = TEXT("NewMold_Deco_Spike");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_NewMold"), &desc)))
	//	return E_FAIL;
	//
	//desc.strModelName = TEXT("NewMoldMobile_C");
	//desc.vPosition = _float4( fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_NewMold_C"), &desc)))
	//	return E_FAIL;
	//	
	//desc.strModelName = TEXT("NewMoldMobile_D");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_NewMold_D"), &desc)))
	//	return E_FAIL;
	//
	//desc.strModelName = TEXT("Miniboss_A");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_MiniBoss_A"), &desc)))
	//	return E_FAIL;
	//
	//desc.strModelName = TEXT("Miniboss_B");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_MiniBoss_B"), &desc)))
	//	return E_FAIL;
	//
	//Prototype_GameObject_Hand_Boss
	//Boss_C_Right_Hand

	CHandBoss::CHandBoss_DESC HandDesc;
	HandDesc.strModelName = TEXT("Boss_C_Right_Hand");
	HandDesc.vPosition = _float4(0.0f, 0.f, 20.f, 1.0f);
	HandDesc.ePart = CHandBoss::PARTS_HAND_R;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Hand_Boss"), &HandDesc)))
	 return E_FAIL;
	
	HandDesc.strModelName = TEXT("Boss_C_Left_Hand");
	HandDesc.vPosition = _float4(0.0f, 0.f, 20.f, 1.0f);
	HandDesc.ePart = CHandBoss::PARTS_HAND_L;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_Hand_Boss"), &HandDesc)))
		return E_FAIL;
	

	
	

	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_Environment(const wstring& strLayerTag)
{
	//CEnvironmentObj::CEnvironmentObj_DESC desc;
	//desc.strModelName = TEXT("Ventilator");
	//desc.vPosition = _float4(0.f, 0.f, 0.f, 1.0f);
	//desc.vScale = _float3(1.f, 1.f, 1.f);
	//desc.vRotation= _float3(0.f, 1.f, 0.f);
	//desc.vRotationAngle = 45.f;
	//
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_EnvironmentObj"), &desc)))
	//	return E_FAIL;




	



	return S_OK;
}

HRESULT CLevel_Stage_1::Ready_Layer_1_Segment(const wstring& strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC SegmentDesc;
	SegmentDesc.strModelName = TEXT("Segment_GS_Deco");
	SegmentDesc.vPosition = _float4(0.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &SegmentDesc)))
		return E_FAIL;


	SegmentDesc.strModelName = TEXT("Segment_SM_Deco");
	SegmentDesc.vPosition = _float4(0.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_CSegment_SM"), &SegmentDesc)))
		return E_FAIL;

	//desc.strModelName = TEXT("Segment_002");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &desc)))
	//	return E_FAIL;
	//

	CGameObject::GAMEOBJECT_DESC SegmentLineDesc;
	SegmentLineDesc.vPosition = _float4(0.f, 0.f, 24.f, 1.0f);
	SegmentLineDesc.strModelName = TEXT("Segment_GS_Stroke");
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_1, strLayerTag, TEXT("Prototype_GameObject_CSegment_Line"), &SegmentLineDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Stage_1 * CLevel_Stage_1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Stage_1*		pInstance = new CLevel_Stage_1(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Stage_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage_1::Free()
{
	__super::Free();

}
