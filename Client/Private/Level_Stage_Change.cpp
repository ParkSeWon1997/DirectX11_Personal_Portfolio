#include "stdafx.h"
#include "..\Public\Level_Stage_Change.h"
#include "GameInstance.h"

#include "FreeCamera.h"
#include"Monster.h"
#include"UpgradeMachine.h"
#include"CStage.h"
#include"Level_Loading.h"
#include"EnvironmentObj.h"
#include"CHandBoss.h"
#include"Particle_Mesh.h"
#include"CHandBullet.h"
#include"CTotalSingleton.h"
#include"Changer.h"


#include"UI.h"
#include"Fade_In_Out.h"
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

	if (FAILED(Ready_UI(TEXT("Layer_Ui"))))
		return E_FAIL;
	if (FAILED(Ready_Player_UI(TEXT("Layer_Player_Ui"))))
		return E_FAIL;
	if (FAILED(Ready_Player_SKill_UI(TEXT("Layer_Player_SKill_Ui"))))
		return E_FAIL;

	if (FAILED(Ready_UI_Changer(TEXT("Layer_Ui_Changer"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_FadeIn_Out(TEXT("Layer_Fade_In_Out"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Chnager(TEXT("Layer_Changer"))))
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

	m_pGameInstance->Stop_All_Sound();
	m_pGameInstance->PlayBGM(TEXT("BGM_Shop [1].wav"), 0.7f);
	
	CTotalSingleton::GetInstance()->ResetTimer();
	return S_OK;
}

void CLevel_Stage_Change::Tick(_float fTimeDelta)
{
	static _bool  bIsPotalCreated = false;
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	if (bIsPotalCreated != true)
	{
		bIsPotalCreated = true;
		CEnvironmentObj::CEnvironmentObj_DESC desc;
		desc.strModelName = TEXT("Ventilator");
		desc.vPosition = _float4(0.1f, 0.f, 38.f, 1.0f);
		desc.vScale = _float3(1.f, 1.f, 1.f);
		desc.vRotation = _float3(0.f, 1.f, 0.f);
		desc.vRotationAngle = 45;
		if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, TEXT("Layer_Potal"), TEXT("Prototype_Potal"), &desc)))
			return;

		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y_NONE_DISOLVE,TEXT("Potal_Effect_hitRing_Rotate_Y"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y_NONE_DISOLVE,TEXT("Potal_Effect_LowpolyCylinder6_Rotate_Y"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });

		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Potal_Effect_atomTri_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Potal_Effect_atomTri.001_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Potal_Effect_atomTri.002_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,true });
		CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(desc.vPosition.x, desc.vPosition.y + 1.0f, desc.vPosition.z, 1.0f));
	}

	if (CTotalSingleton::GetInstance()->GetPotalOn())
	{

		 _bool bFadeStart = false;
		_uint iFadeLayerSize = m_pGameInstance->Get_LayerSize(LEVEL_CHANGE_STAGE, TEXT("Layer_Fade_In_Out"));
		Fade_In_Out* pFadeInOut = nullptr;

		for (_uint i = 0; i < iFadeLayerSize; ++i)
		{

			pFadeInOut = static_cast<Fade_In_Out*>(m_pGameInstance->Get_Object(LEVEL_CHANGE_STAGE, TEXT("Layer_Fade_In_Out"), i));
			pFadeInOut->Set_FadeDuration(1.f);
			if (!bFadeStart)
			{
				
				pFadeInOut->Start_FadeIn();
				bFadeStart = true;
			}
		}


		//1차 보스 
		LEVEL eLevel = LEVEL_STAGE_1;
		
		
		if (CTotalSingleton::GetInstance()->GetVisitLevel(eLevel))
		{
			//2차 필드
			eLevel = LEVEL_STAGE_2;
			if (CTotalSingleton::GetInstance()->GetVisitLevel(eLevel))
			{
				//2차 보스
				eLevel = LEVEL_STAGE_3;
			}

		}

		if (pFadeInOut->IsFade())
		{
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Portal_Off001 [1].wav"), SOUND_EFFECT, 0.5f);
			if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevel))))
				return;
			CTotalSingleton::GetInstance()->SetVisitLevel(eLevel);
			bIsPotalCreated= false;
		}
			CTotalSingleton::GetInstance()->SetPotalOn(false);

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

HRESULT CLevel_Stage_Change::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//m_pGameInstance->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_UI(const wstring& strLayerTag)
{

	UI::UI_DESC desc;


	//좌상단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Left_Top");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = desc.fSizeX * 0.5f;
	desc.fY = desc.fSizeY * 0.5f;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;

	// 우상단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Right_Top");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = 1280.f - desc.fSizeX * 0.5f;  // X 좌표: 화면 너비 - 크기의 반
	desc.fY = desc.fSizeY * 0.5f;           // Y 좌표: 크기의 반
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;

	// 좌하단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Left_Bottom");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = desc.fSizeX * 0.5f;             // X 좌표: 크기의 반
	desc.fY = 720.f - desc.fSizeY * 0.5f;     // Y 좌표: 화면 높이 - 크기의 반
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;


	// 우하단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Right_Bottom");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = 1280.f - desc.fSizeX * 0.5f;    // X 좌표: 화면 너비 - 크기의 반
	desc.fY = 720.f - desc.fSizeY * 0.5f;     // Y 좌표: 화면 높이 - 크기의 반
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;





	desc.strModelName = TEXT("Icon_Keyboard_X");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 800.f - 31.5f + desc.fSizeX * 0.5f;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Icon_Keyboard_C");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 863.f - 31.5f + desc.fSizeX * 0.5f;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;


	desc.strModelName = TEXT("Icon_Keyboard_X");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 926.f - 31.5f + desc.fSizeX * 0.5f;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;
	desc.strModelName = TEXT("Icon_Sametime");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 926.f - 31.5f + desc.fSizeX * 0.5f + desc.fSizeX;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;
	desc.strModelName = TEXT("Icon_Keyboard_C");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 926.f - 31.5f + desc.fSizeX * 0.5f + desc.fSizeX + desc.fSizeX;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;














	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Player_UI(const wstring& strLayerTag)
{

	UI::UI_DESC desc;



	desc.strModelName = TEXT("HPBar_New_Player_A");
	desc.fSizeX = 500.f * 0.5f;
	desc.fSizeY = 39.f * 0.5f;
	desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX) - desc.fSizeX * 0.3;
	//desc.fX = (g_iWinSizeX * 0.5f);
	desc.fY = (g_iWinSizeY * 0.8f) + (desc.fSizeY);
	desc.iPassIndex = 4;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;




	desc.fSizeX = 512.f * 0.5f;
	desc.fSizeY = 512.f * 0.5f;
	desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX + 300.f);
	desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
	desc.iPassIndex = 4;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerIcon"), &desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Player_SKill_UI(const wstring& strLayerTag)
{
	UI::UI_DESC desc;



	desc.strModelName = TEXT("Amanda_X");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f);
	desc.fY = 580.f;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Amanda_C");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + 0.5f;
	desc.fY = 580.f;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Amanda_XC");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + desc.fSizeX + 1.0f;
	desc.fY = 580.f;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;



	desc.strModelName = TEXT("Matilda_X");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f);
	desc.fY = 580.f + desc.fSizeY;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Matilda_C");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + 0.5f;
	desc.fY = 580.f + desc.fSizeY;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Matilda_XC");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + desc.fSizeX + 1.0f;
	desc.fY = 580.f + desc.fSizeY;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	//desc.strModelName = TEXT("Player_Skill_Dash");
	//desc.fSizeX = 125.f * 0.5f;
	//desc.fSizeY = 200.f * 0.5f;
	//desc.fX = (800.f) + desc.fSizeX + desc.fSizeX + desc.fSizeX + 2.0f;
	//desc.fY = 580.f + 25.f;
	//desc.iPassIndex = 6;
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
	//	return E_FAIL;





	return S_OK;

}

HRESULT CLevel_Stage_Change::Ready_UI_Changer(const wstring& strLayerTag)
{

	UI::UI_DESC desc;


	desc.strModelName = TEXT("PauseFrame_HowtoPlay");
	desc.fSizeX = 1024.f;
	desc.fSizeY = 384.f;
	desc.fX = g_iWinSizeX * 0.5f;
	desc.fY = g_iWinSizeY * 0.5f;
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_UI_Changer"), &desc)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Stage_Change::Ready_Layer_FadeIn_Out(const wstring& strLayerTag)
{
	Fade_In_Out::Fade_In_Out_DESC FadeDesc;
	FadeDesc.fSizeX = g_iWinSizeX;
	FadeDesc.fSizeY = g_iWinSizeY;
	FadeDesc.fX = g_iWinSizeX * 0.5f;
	FadeDesc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_GameObject_Fade_In_Out"), &FadeDesc)))
		return E_FAIL;



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

HRESULT CLevel_Stage_Change::Ready_Layer_Chnager(const wstring& strLayerTag)
{
	CChanger::CChanger_DESC desc;

	desc.strModelName = TEXT("Changer_Icon_Gun");
	desc.vPosition = _float4(-3.f, 1.0f, 5.f, 1.0f);
	desc.vScale = _float3(2.f, 2.f, 2.f);
	desc.ChangerState = &CChanger::Up_And_Down;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_Changer"), &desc)))
		return E_FAIL;


	desc.strModelName = TEXT("Changer_Icon_Sword");
	desc.vPosition = _float4(3.f, 1.0f, 5.f, 1.0f);
	desc.vScale = _float3(2.f, 2.f, 2.f);
	desc.ChangerState = &CChanger::Up_And_Down;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_Changer"), &desc)))
		return E_FAIL;
	
	
	
	CGameObject* pChanger = static_cast<CChanger*>(m_pGameInstance->Get_Object(LEVEL_CHANGE_STAGE, strLayerTag,0));
	
	desc.strModelName = TEXT("Changer_Rot_Gun");
	desc.vPosition = _float4(-3.f, 1.5f, 5.f, 1.0f);
	desc.vScale = _float3(2.f, 2.f, 2.f);
	desc.ChangerState = &CChanger::Rotate;
	desc.fRotationPerSec = 3.f;
	desc.pChanger = pChanger;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_Changer"), &desc)))
		return E_FAIL;
	
	
	
	
	pChanger = static_cast<CChanger*>(m_pGameInstance->Get_Object(LEVEL_CHANGE_STAGE, strLayerTag, 1));
	desc.strModelName = TEXT("Changer_Rot_Sword");
	desc.vPosition = _float4(3.f, 1.5f, 5.f, 1.0f);
	desc.vScale = _float3(2.f, 2.f, 2.f);
	desc.ChangerState = &CChanger::Rotate;
	desc.fRotationPerSec = 3.f;
	desc.pChanger = pChanger;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_CHANGE_STAGE, strLayerTag, TEXT("Prototype_Changer"), &desc)))
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
