#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"

#include "FreeCamera.h"
#include"Monster.h"
#include"EnvironmentObj.h"
#include"CStage.h"
#include"Level_Loading.h"
#include"CPotalSingleton.h"
#include"CHandBoss.h"
#include"Particle_Mesh.h"
#include"CHandBullet.h"
#include"UpgradeMachine.h"
#include"UpgradeMachineTop.h"
#include"Particle_Mesh.h"
CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;





	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_UI(TEXT("Layer_Ui"))))
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

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	static _bool   bisMosterClear = false;
	_uint iLayerSize= m_pGameInstance->Get_LayerSize(LEVEL_GAMEPLAY, TEXT("Layer_2_Monster"));
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	if (iLayerSize == 0)
	{
		if (bisMosterClear == false)
		{
			bisMosterClear = true;
			CEnvironmentObj::CEnvironmentObj_DESC desc;
			desc.strModelName = TEXT("Ventilator");
			desc.vPosition = _float4(0.1f, 0.f, 38.f, 1.0f);
			desc.vScale = _float3(1.f, 1.f, 1.f);
			desc.vRotation = _float3(0.f, 1.f, 0.f);
			desc.vRotationAngle = 45.f;
			

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y_NONE_DISOLVE,TEXT("Potal_Effect_hitRing_Rotate_Y"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y_NONE_DISOLVE,TEXT("Potal_Effect_LowpolyCylinder6_Rotate_Y"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Potal_Effect_atomTri_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Potal_Effect_atomTri.001_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Potal_Effect_atomTri.002_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(desc.vPosition.x, desc.vPosition.y + 1.0f, desc.vPosition.z, 1.0f));

			if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Potal"), TEXT("Prototype_Potal"), &desc)))
				return;
			//if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_CHANGE_STAGE))))
			//	return;
		}
	}
	else
	{
		bisMosterClear = false;
	}


	if (CPotalSingleton::GetInstance()->GetPotalOn())
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_CHANGE_STAGE))))
			return;
		CPotalSingleton::GetInstance()->SetPotalOn(false);

		return;
	}



#ifdef _DEBUG
	ostringstream oss;
	oss << std::fixed << std::setprecision(2) << m_pGameInstance->Get_FPS(TEXT("Timer_60"));
	string FpsString = "FPS:" + oss.str();
	//SetWindowText(g_hWnd, TEXT("�����÷��̷�����"));
	SetWindowTextA(g_hWnd,FpsString.c_str());
	

#endif
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	m_pGameInstance->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_UI(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_UI"))))
		return E_FAIL;





	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring & strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_FreeCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const wstring& strLayerTag)
{
	CParticle_Mesh::PARTICLE_DESC desc;
	desc.strModelName = TEXT("Boss_C_Mass_Falling");
	desc.vStartPos = _float4(0.f, 0.f, 0.f, 1.0f);
	desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN;

	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Particle"), &desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring & strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_0_Stage(const wstring& strLayerTag)
{

	CStage::CStage_DESC desc;

	desc.strModelName = TEXT("StageGroundAssemble_A_GS_A_66");
	desc.vPosition= _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CStage"),&desc)))
		return E_FAIL;
	
	desc.strModelName = TEXT("StageGroundAssemble_A_SM_A_66");
	desc.vPosition = _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CStage_SM"),&desc)))
		return E_FAIL;





	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_2_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_2_Monster(const wstring & strLayerTag)
{
	random_device randomDevice;
	mt19937_64 Gen(randomDevice());


	uniform_real_distribution<float> fRandomX(0.f, 30.f);
	uniform_real_distribution<float> fRandomZ(0.f, 30.f);

	//decltype(auto) loadedData = Load_Data<_matrix, wstring, _matrix, wstring>(strLayerTag);	//Ÿ�԰� ������ �����������
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
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Boss_D"), &desc)))
	//	return E_FAIL;

	//desc.strModelName = TEXT("Boss_D_Picker_New");
	//desc.vPosition = _float4(0.f, 0.f, 20.f, 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CBoss_D_Picker"), &desc)))
	//	return E_FAIL;

	//desc.strModelName = TEXT("Boss_B");
	//desc.vPosition = _float4(0.0f, 0.f, 20.f, 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Boss_B"), &desc)))
	//	return E_FAIL;

	//
	//
	
	for (int i = 0; i < 1; i++)
	{
		desc.strModelName = TEXT("NewMoldTest");
		desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
		desc.strDeconModelTag = TEXT("NewMold_Deco_Segment");
		desc.fHp= 50.f;
		if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_NewMold"), &desc)))
			return E_FAIL;
	}
	
	//desc.strModelName = TEXT("NewMoldTest");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//desc.strDeconModelTag = TEXT("NewMold_Deco_Spike");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_NewMold"), &desc)))
	//	return E_FAIL;
	//
	//desc.strModelName = TEXT("NewMoldMobile_C");
	//desc.vPosition = _float4( fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_NewMold_C"), &desc)))
	//	return E_FAIL;
	//	
	//desc.strModelName = TEXT("NewMoldMobile_D");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_NewMold_D"), &desc)))
	//	return E_FAIL;
	//
	//desc.strModelName = TEXT("Miniboss_A");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_MiniBoss_A"), &desc)))
	//	return E_FAIL;
	//
	//desc.strModelName = TEXT("Miniboss_B");
	//desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_MiniBoss_B"), &desc)))
	//	return E_FAIL;
	//
	//Prototype_GameObject_Hand_Boss
	//Boss_C_Right_Hand

	//CHandBoss::CHandBoss_DESC HandDesc;
	//HandDesc.strModelName = TEXT("Boss_C_Right_Hand");
	//HandDesc.vPosition = _float4(0.0f, 0.f, 20.f, 1.0f);
	//HandDesc.ePart = CHandBoss::PARTS_HAND_R;
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Hand_Boss"), &HandDesc)))
	// return E_FAIL;
	//
	//HandDesc.strModelName = TEXT("Boss_C_Left_Hand");
	//HandDesc.vPosition = _float4(0.0f, 0.f, 20.f, 1.0f);
	//HandDesc.ePart = CHandBoss::PARTS_HAND_L;
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Hand_Boss"), &HandDesc)))
	//	return E_FAIL;
	

	
	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Environment(const wstring& strLayerTag)
{
	CEnvironmentObj::CEnvironmentObj_DESC desc;
	desc.strModelName = TEXT("Ventilator");
	desc.vPosition = _float4(5.f, 0.f, 20.f, 1.0f);
	desc.vScale = _float3(1.f, 1.f, 1.f);
	desc.vRotation= _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 45.f;
	
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_EnvironmentObj"), &desc)))
	//	return E_FAIL;




	//CUpgradeMachine::CUpgradeMachine_DESC tttdesc;
	//tttdesc.strModelName = TEXT("Foothold_Shop");
	//tttdesc.vPosition = _float4(-17.f, 0.0f, 24.f, 1.0f);
	//tttdesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	//tttdesc.vRotation = _float3(0.f, 1.f, 0.f);
	//tttdesc.vRotationAngle = 45.f;
	//tttdesc.eMachineType = CUpgradeMachine::MACHINE_ADD_HP;
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_UpgradeMachine"), &tttdesc)))
	//	return E_FAIL;




	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_1_Segment(const wstring& strLayerTag)
{

	CGameObject::GAMEOBJECT_DESC SegmentDesc;
	SegmentDesc.strModelName = TEXT("Segment_GS_Deco");
	SegmentDesc.vPosition = _float4(20.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &SegmentDesc)))
		return E_FAIL;
	

	SegmentDesc.strModelName = TEXT("Segment_SM_Deco");
	SegmentDesc.vPosition = _float4(20.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CSegment_SM"), &SegmentDesc)))
		return E_FAIL;

	//desc.strModelName = TEXT("Segment_002");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &desc)))
	//	return E_FAIL;
	//

	CGameObject::GAMEOBJECT_DESC SegmentLineDesc;
	SegmentLineDesc.vPosition = _float4(20.f, 0.02f, 24.f, 1.0f);
	SegmentLineDesc.strModelName = TEXT("Segment_GS_Stroke");
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CSegment_Line"),&SegmentLineDesc)))
		return E_FAIL;




	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}
