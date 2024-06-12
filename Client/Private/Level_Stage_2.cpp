#include "stdafx.h"
#include "..\Public\Level_Stage_2.h"
#include "GameInstance.h"

#include "FreeCamera.h"
#include"Monster.h"
#include"EnvironmentObj.h"
#include"CStage.h"
#include"Level_Loading.h"

#include"CHandBoss.h"
#include"Particle_Mesh.h"
#include"CHandBullet.h"
#include"CTotalSingleton.h"


#include"UI.h"
#include"Fade_In_Out.h"
CLevel_Stage_2::CLevel_Stage_2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_2::Initialize()
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

	if (FAILED(Ready_Layer_FadeIn_Out(TEXT("Layer_Fade_In_Out"))))
		return E_FAIL;



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

	m_pGameInstance->Stop_All_Sound();
	m_pGameInstance->PlayBGM(TEXT("BGM_SnowyDesert [1].wav"), 0.7f);

	CTotalSingleton::GetInstance()->ResetTimer();
	return S_OK;
}

void CLevel_Stage_2::Tick(_float fTimeDelta)
{

	if (!CTotalSingleton::GetInstance()->IsTimerPaused())
	{
		CTotalSingleton::GetInstance()->EndTime();
		CTotalSingleton::GetInstance()->UpdateElapsedTime();
	}
	int hours = CTotalSingleton::GetInstance()->GetHours();
	int minutes = CTotalSingleton::GetInstance()->GetMinutes();
	int seconds = CTotalSingleton::GetInstance()->GetSeconds();
	cout << "Hours: " << hours << " Minutes: " << minutes << " Seconds: " << seconds << endl;







	static _bool   bisMosterClear = false;
	_uint iLayerSize = m_pGameInstance->Get_LayerSize(LEVEL_STAGE_2, TEXT("Layer_2_Monster"));


	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	CGameObject::GAMEOBJECT_DESC Rankdesc;



	if (iLayerSize == 0)
	{

		CTotalSingleton::GetInstance()->PauseTimer();


		if (bisMosterClear == false)
		{

			if (minutes >= 1)
			{
				Rankdesc.strModelName = TEXT("Interactor_RankBonus_Game_D");

			}

			else if (seconds <= 20)
			{
				Rankdesc.strModelName = TEXT("Interactor_RankBonus_Game_S");

			}
			else if (seconds <= 30)
			{
				Rankdesc.strModelName = TEXT("Interactor_RankBonus_Game_A");

			}
			else if (seconds <= 40)
			{
				Rankdesc.strModelName = TEXT("Interactor_RankBonus_Game_B");

			}
			else if (seconds <= 50)
			{
				Rankdesc.strModelName = TEXT("Interactor_RankBonus_Game_C");

			}
			else
			{
				Rankdesc.strModelName = TEXT("Interactor_RankBonus_Game_D");

			}

			Rankdesc.vPosition = _float4(0.1f, 5.f, 28.f, 1.0f);
			Rankdesc.vScale = _float3(1.f, 1.f, 1.f);
			Rankdesc.vRotation = _float3(0.f, 1.f, 0.f);
			Rankdesc.vRotationAngle = 0.f;


			if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, TEXT("Layer_Environment"), TEXT("Prototype_Rank"), &Rankdesc)))
				return;




























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

			if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, TEXT("Layer_Potal"), TEXT("Prototype_Potal"), &desc)))
				return;
			//if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_CHANGE_STAGE))))
			//	return;
		}
	}
	else
	{
		bisMosterClear = false;
	}


	if (CTotalSingleton::GetInstance()->GetPotalOn())
	{

		static _bool bFadeStart = false;

		_uint iFadeLayerSize = m_pGameInstance->Get_LayerSize(LEVEL_STAGE_2, TEXT("Layer_Fade_In_Out"));
		Fade_In_Out* pFadeInOut = nullptr;

		for (_uint i = 0; i < iFadeLayerSize; ++i)
		{

			pFadeInOut = static_cast<Fade_In_Out*>(m_pGameInstance->Get_Object(LEVEL_STAGE_2, TEXT("Layer_Fade_In_Out"), i));
			pFadeInOut->Set_FadeDuration(1.f);
			if (!bFadeStart)
			{
				pFadeInOut->Start_FadeIn();
				bFadeStart = true;
			}
		}


		if (pFadeInOut->IsFade())
		{
			if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_CHANGE_STAGE))))
				return;
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

HRESULT CLevel_Stage_2::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//m_pGameInstance->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_UI(const wstring& strLayerTag)
{
	UI::UI_DESC desc;


	//좌상단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Left_Top");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = desc.fSizeX * 0.5f;
	desc.fY = desc.fSizeY * 0.5f;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;

	// 우상단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Right_Top");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = 1280.f - desc.fSizeX * 0.5f;  // X 좌표: 화면 너비 - 크기의 반
	desc.fY = desc.fSizeY * 0.5f;           // Y 좌표: 크기의 반
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;

	// 좌하단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Left_Bottom");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = desc.fSizeX * 0.5f;             // X 좌표: 크기의 반
	desc.fY = 720.f - desc.fSizeY * 0.5f;     // Y 좌표: 화면 높이 - 크기의 반
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;


	// 우하단
	desc.strModelName = TEXT("Panel_Frame_InGame_Deco_Right_Bottom");
	desc.fSizeX = 256.f;
	desc.fSizeY = 128.f;
	desc.fX = 1280.f - desc.fSizeX * 0.5f;    // X 좌표: 화면 너비 - 크기의 반
	desc.fY = 720.f - desc.fSizeY * 0.5f;     // Y 좌표: 화면 높이 - 크기의 반
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;






	desc.strModelName = TEXT("Icon_Keyboard_X");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 800.f - 31.5f + desc.fSizeX * 0.5f;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Icon_Keyboard_C");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 863.f - 31.5f + desc.fSizeX * 0.5f;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;


	desc.strModelName = TEXT("Icon_Keyboard_X");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 926.f - 31.5f + desc.fSizeX * 0.5f;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;
	desc.strModelName = TEXT("Icon_Sametime");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 926.f - 31.5f + desc.fSizeX * 0.5f + desc.fSizeX;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;
	desc.strModelName = TEXT("Icon_Keyboard_C");
	desc.fSizeX = 64.f * 0.25f;
	desc.fSizeY = 64.f * 0.25f;
	desc.fX = 926.f - 31.5f + desc.fSizeX * 0.5f + desc.fSizeX + desc.fSizeX;
	desc.fY = 530.f + desc.fSizeX;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;












	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Player_UI(const wstring& strLayerTag)
{
	UI::UI_DESC desc;



	desc.strModelName = TEXT("HPBar_New_Player_A");
	desc.fSizeX = 500.f * 0.5f;
	desc.fSizeY = 39.f * 0.5f;
	desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX) - desc.fSizeX * 0.3;
	//desc.fX = (g_iWinSizeX * 0.5f);
	desc.fY = (g_iWinSizeY * 0.8f) + (desc.fSizeY);
	desc.iPassIndex = 4;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI"), &desc)))
		return E_FAIL;




	desc.fSizeX = 512.f * 0.5f;
	desc.fSizeY = 512.f * 0.5f;
	desc.fX = (g_iWinSizeX * 0.5f) - (desc.fSizeX + 300.f);
	desc.fY = (g_iWinSizeY * 0.5f) + (desc.fSizeY);
	desc.iPassIndex = 4;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerIcon"), &desc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Player_SKill_UI(const wstring& strLayerTag)
{
	UI::UI_DESC desc;



	desc.strModelName = TEXT("Amanda_X");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f);
	desc.fY = 580.f;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Amanda_C");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + 0.5f;
	desc.fY = 580.f;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Amanda_XC");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + desc.fSizeX + 1.0f;
	desc.fY = 580.f;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;



	desc.strModelName = TEXT("Matilda_X");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f);
	desc.fY = 580.f + desc.fSizeY;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Matilda_C");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + 0.5f;
	desc.fY = 580.f + desc.fSizeY;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;

	desc.strModelName = TEXT("Matilda_XC");
	desc.fSizeX = 125.f * 0.5f;
	desc.fSizeY = 100.f * 0.5f;
	desc.fX = (800.f) + desc.fSizeX + desc.fSizeX + 1.0f;
	desc.fY = 580.f + desc.fSizeY;
	desc.iPassIndex = 6;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
		return E_FAIL;



	//desc.strModelName = TEXT("Player_Skill_Dash");
	//desc.fSizeX = 125.f * 0.5f;
	//desc.fSizeY = 200.f * 0.5f;
	//desc.fX = (800.f) + desc.fSizeX + desc.fSizeX + desc.fSizeX + 2.0f;
	//desc.fY = 580.f + 25.f;
	//desc.iPassIndex = 6;
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_UI_PlayerSkill"), &desc)))
	//	return E_FAIL;



	return S_OK;

}

HRESULT CLevel_Stage_2::Ready_Layer_FadeIn_Out(const wstring& strLayerTag)
{
	Fade_In_Out::Fade_In_Out_DESC FadeDesc;
	FadeDesc.fSizeX = g_iWinSizeX;
	FadeDesc.fSizeY = g_iWinSizeY;
	FadeDesc.fX = g_iWinSizeX * 0.5f;
	FadeDesc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_Fade_In_Out"), &FadeDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_Camera(const wstring & strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_FreeCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_Effect(const wstring& strLayerTag)
{
	CParticle_Mesh::PARTICLE_DESC desc;
	desc.strModelName = TEXT("Boss_C_Mass_Falling");
	desc.vStartPos = _float4(0.f, 0.f, 0.f, 1.0f);
	desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN;

	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_Particle"), &desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_BackGround(const wstring & strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_0_Stage(const wstring& strLayerTag)
{

	CStage::CStage_DESC desc;
	//desc.strModelName = TEXT("StageGround_A_57_New_A");
	//desc.strModelName = TEXT("StageGroundAssemble_C_SM_A_66");
	desc.strModelName = TEXT("StageGroundAssemble_A_SM_C_66");
	desc.vPosition = _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_CStage"),&desc)))
		return E_FAIL;



	desc.strModelName = TEXT("StageGroundAssemble_A_GS_C_66");
	desc.vPosition = _float4(0.f, 0.f, sqrtf(2) * 17.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_CStage_SM"), &desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_2_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_2_Monster(const wstring & strLayerTag)
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
	
	for (int i = 0; i < 4; i++)
	{
		desc.strModelName = TEXT("NewMoldTest");
		desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
		desc.strDeconModelTag = TEXT("NewMold_Deco_Segment");
		desc.fHp = 30.f;
		if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_NewMold"), &desc)))
			return E_FAIL;
	}
	for (int i = 0; i < 3; i++)
	{
		desc.strModelName = TEXT("NewMoldMobile_C");
		desc.vPosition = _float4(fRandomX(Gen), 0.f, fRandomZ(Gen), 1.0f);
		desc.fHp = 30.f;
		if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_NewMold_C"), &desc)))
			return E_FAIL;
	}

	
	

	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_Environment(const wstring& strLayerTag)
{
	//CEnvironmentObj::CEnvironmentObj_DESC desc;
	//desc.strModelName = TEXT("Ventilator");
	//desc.vPosition = _float4(0.f, 0.f, 0.f, 1.0f);
	//desc.vScale = _float3(1.f, 1.f, 1.f);
	//desc.vRotation= _float3(0.f, 1.f, 0.f);
	//desc.vRotationAngle = 45.f;
	//
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_EnvironmentObj"), &desc)))
	//	return E_FAIL;

	CGameObject::GAMEOBJECT_DESC ItemDesc;
	ItemDesc.strModelName = TEXT("Interactor_Itembox_HP");
	ItemDesc.vPosition = _float4(-21.f, 0.f, 26.f, 1.0f);
	ItemDesc.vScale = _float3(1.f, 1.f, 1.f);
	ItemDesc.vRotation = _float3(0.f, 1.f, 0.f);
	ItemDesc.vRotationAngle = 45.f;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_ItemBox"), &ItemDesc)))
		return E_FAIL;


	ItemDesc.strModelName = TEXT("Interactor_Itembox_Gold");
	ItemDesc.vPosition = _float4(-17.f, 0.f, 30.f, 1.0f);
	ItemDesc.vScale = _float3(1.f, 1.f, 1.f);
	ItemDesc.vRotation = _float3(0.f, 1.f, 0.f);
	ItemDesc.vRotationAngle = 45.f;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_ItemBox"), &ItemDesc)))
		return E_FAIL;


	ItemDesc.strModelName = TEXT("Interactor_Itembox_Random");
	ItemDesc.vPosition = _float4(-9.f, 0.f, 37.f, 1.0f);
	ItemDesc.vScale = _float3(1.f, 1.f, 1.f);
	ItemDesc.vRotation = _float3(0.f, 1.f, 0.f);
	ItemDesc.vRotationAngle = 45.f;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_ItemBox"), &ItemDesc)))
		return E_FAIL;



	



	return S_OK;
}

HRESULT CLevel_Stage_2::Ready_Layer_1_Segment(const wstring& strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC SegmentDesc;
	SegmentDesc.strModelName = TEXT("Segment_GS_Deco");
	SegmentDesc.vPosition = _float4(0.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &SegmentDesc)))
		return E_FAIL;


	SegmentDesc.strModelName = TEXT("Segment_SM_Deco");
	SegmentDesc.vPosition = _float4(0.f, 0.f, 24.f, 1.0f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_CSegment_SM"), &SegmentDesc)))
		return E_FAIL;

	//desc.strModelName = TEXT("Segment_002");
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CSegment"), &desc)))
	//	return E_FAIL;
	//

	CGameObject::GAMEOBJECT_DESC SegmentLineDesc;
	SegmentLineDesc.vPosition = _float4(0.f, 0.f, 24.f, 1.0f);
	SegmentLineDesc.strModelName = TEXT("Segment_GS_Stroke");
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_STAGE_2, strLayerTag, TEXT("Prototype_GameObject_CSegment_Line"), &SegmentLineDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Stage_2 * CLevel_Stage_2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Stage_2*		pInstance = new CLevel_Stage_2(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Stage_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage_2::Free()
{
	__super::Free();

}
