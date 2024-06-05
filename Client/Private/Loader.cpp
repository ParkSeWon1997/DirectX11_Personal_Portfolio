#include "stdafx.h"
#include "..\Public\Loader.h"


#include "GameInstance.h"
#include "Matilda_Body_Player.h"
#include "Amanda_Body_Player.h"
#include "PlayerCamera.h"
#include "FreeCamera.h"
#include "ForkLift.h"
#include "EnvironmentObj.h"
#include "Terrain.h"
#include "Monster.h"
#include "CNewMold.h"
#include "CNewMold_C.h"
#include "CNewMold_D.h"
#include "CBoss_B.h"
#include "CBoss_D.h"
#include "CBoss_D_Picker.h"
#include"CMiniBoss.h"
#include "Weapon.h"
#include "Player.h"
#include"CBullet.h"
#include"CPlayerBullet.h"
#include"CPlayerBullet_Mine.h"
#include "CStage.h"
#include "CStage_SM.h"
#include "Sky.h"
#include "CDecorationParts.h"
#include"CHandBoss.h"
#include"CBoneCollider.h"
#include"CHandBullet.h"
#include"CMold_CBullet.h"
#include"CBoss_B_Bullet.h"
#include"CNoneTexBullet.h"
#include"Particle_Mesh.h"
#include"Segment.h"
#include"Segment_SM.h"
#include"SegmentLine.h"
#include"CDrone.h"

#include"UpgradeMachine.h"
#include"UpgradeMachineTop.h"
#include"Potal.h"

#include"UI.h"




LEVEL CLoader::m_eNextLevel = LEVEL{ LEVEL_END };


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);

	CLoader*		pLoader = (CLoader*)pArg;


	if (FAILED(pLoader->Loading()))
		return 1;

	CoUninitialize();

	return 0;
}

CVIBuffer_Instance::INSTANCE_DESC CLoader::Make_ParticleDesc(wstring wstrMeshFath)
{
	CVIBuffer_Instance::INSTANCE_DESC desc={};

	decltype(auto) load_data = Load_Data_Text <wstring, wstring,
		wstring, _uint,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, float,
		wstring, _uint ,
		wstring, float,
		wstring, float,
		wstring, float>(wstrMeshFath);

	desc.iNumInstance =		get<3>(load_data);
	desc.vOffsetPos.x =		get<5>(load_data);
	desc.vOffsetPos.y =		get<7>(load_data);
	desc.vOffsetPos.z =		get<9>(load_data);
	desc.vPivotPos.x =		get<11>(load_data);
	desc.vPivotPos.y =		get<13>(load_data);
	desc.vPivotPos.z =		get<15>(load_data);
	desc.vRange.x =			get<17>(load_data);
	desc.vRange.y =			get<19>(load_data);
	desc.vRange.z =			get<21>(load_data);
	desc.vSize.x =			get<23>(load_data);
	desc.vSize.y =			get<25>(load_data);
	desc.vSpeed.x =			get<27>(load_data);
	desc.vSpeed.y =			get<29>(load_data);
	desc.vLifeTime.x =		get<31>(load_data);
	desc.vLifeTime.y =		get<33>(load_data);
	desc.isLoop =			get<35>(load_data);
	desc.vTargetPos.x =		get<37>(load_data);
	desc.vTargetPos.y =		get<39>(load_data);
	desc.vTargetPos.z =		get<41>(load_data);
	desc.iType_Index =		get<43>(load_data);
	desc.fSize_X =			get<45>(load_data);
	desc.fSize_Y =			get<47>(load_data);
	desc.fSize_Z =			get<49>(load_data);
	



	return desc;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{


	EnterCriticalSection(&m_Critical_Section);

	HRESULT			hr{};

	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		hr = Loading_For_LogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;
	case LEVEL_STAGE_1:
		hr = Loading_For_Stage_1();
		break;
	case LEVEL_STAGE_2:
		hr = Loading_For_Stage_2();
		break;
	case LEVEL_STAGE_3:
		hr = Loading_For_Stage_3();
		break;
	case LEVEL_CHANGE_STAGE:
		hr = Loading_For_Change_Stage();
		break;
	case LEVEL_BOSS_1:
		hr= Loading_For_Boss_1();
		break;
	case LEVEL_BOSS_2:
		hr = Loading_For_Boss_2();
		break;

		break;
	}

	LeaveCriticalSection(&m_Critical_Section);

	if (FAILED(hr))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Loading_For_LogoLevel()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));	


	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));	


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Panel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Panel_Frame_State.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Player/Player.jpg"), 1))))
		return E_FAIL;
	
	/*Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noise001"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise001.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noise002"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise002.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noise003"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise003.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noise004"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise004.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noise005"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise005.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noise011"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise011.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texturenoise013"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise013.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texturenoise014"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise014.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_noiseRepeat"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noiseRepeat.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pattern_Cicle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Pattern_Cicle.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pattern_Line"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Pattern_Line.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Pattern_Line_Vertical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Pattern_Line_Vertical.png"), 1))))
		return E_FAIL;

	/*UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Panel_Frame_InGame_Deco_Left_Botom"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Ui/Panel_Frame_InGame_Deco_Left_Botom.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Panel_Frame_InGame_Deco_Left_Top"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Ui/Panel_Frame_InGame_Deco_Legt_Top.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Panel_Frame_InGame_Deco_Right_Botom"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Ui/Panel_Frame_InGame_Deco_Right_Botom.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Panel_Frame_InGame_Deco_Right_Top"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Ui/Panel_Frame_InGame_Deco_Right_Top.png"), 1))))
		return E_FAIL;



#pragma endregion
	

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_Component_Model_ForkLift */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/ForkLift/ForkLift.fbx"))))
		return E_FAIL;
	



	/* For.Prototype_Component_Model_Fiona */

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx"))))
		return E_FAIL;









	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_newMatilda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newMatilda/newMatilda.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_newAmanda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newAmanda/newAmanda.fbx"))))
		return E_FAIL;




	/*Boss_A*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_A/Boss_A.fbx"))))
		return E_FAIL;


	/*Boss_B*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_B/Boss_B.fbx"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_B_Cylinder"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_B/attack_boss_b_cylinder/attack_boss_b_cylinder.fbx"))))
		return E_FAIL;




	/*Boss_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_C_Right_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_C_Right_Hand/Boss_C.fbx"))))
		return E_FAIL;
	/*Boss_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_C_Left_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_C_Left_Hand/Boss_C.fbx"))))
		return E_FAIL;


	/*Boss_D*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_D/Boss_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_D_Picker_New"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_D/Boss_D_Picker_New.fbx"))))
		return E_FAIL;


	/* MiniBoss_A*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_A/Miniboss_A.fbx"))))
		return E_FAIL;

	/* MiniBoss_B*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_B/Miniboss_B.fbx"))))
		return E_FAIL;

	/*MiniBoss_B_side*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_B_Side"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_B_Side/Miniboss_B_Side.fbx"))))
		return E_FAIL;



	/* MiniBoss_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_C/Miniboss_C.fbx"))))
		return E_FAIL;


	/*NewMoldTest*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMoldTest"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldTest/NewMoldTest.fbx"))))
		return E_FAIL;


	/*NewMoldMobile_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMoldMobile_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldMobile_C/NewMoldMobile_C.fbx"))))
		return E_FAIL;
	
	/*NewMoldMobile_D*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMoldMobile_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldMobile_D/NewMoldMobile_D.fbx"))))
		return E_FAIL;

	




	/*Bossintro_A_v2*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bossintro_A_v2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Bossintro_A_v2/Bossintro_A_v2.fbx"))))
		return E_FAIL;



	/*Bossintro_B*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bossintro_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Bossintro_B/Bossintro_B.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Bomb*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Bomb"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Bomb/NewMold_Deco_Bomb.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Diamond*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Diamond"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Diamond/NewMold_Deco_Diamond.fbx"))))
		return E_FAIL;




	/*NewMold_Deco_Electrocity*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Electrocity"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Electrocity/NewMold_Deco_Electrocity.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Feather*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Feather"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Feather/NewMold_Deco_Feather.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Flower*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Flower"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Flower/NewMold_Deco_Flower.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Flower_Blue*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Flower_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Flower_Blue/NewMold_Deco_Flower_Blue.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Leaf*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Leaf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Leaf/NewMold_Deco_Leaf.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Mine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Mine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Mine/NewMold_Deco_Mine.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Platform*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Platform"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Platform/NewMold_Deco_Platform.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Rock*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Rock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Rock/NewMold_Deco_Rock.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Segment"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Segment/NewMold_Deco_Segment.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Spike*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Spike"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Spike/NewMold_Deco_Spike.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Time*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Time"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Time/NewMold_Deco_Time.fbx"))))
		return E_FAIL;



	










	/*Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_A_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_A_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_SM_A_66.fbx"))))
		return E_FAIL;
	
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_E_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_E_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_SM_A_66.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_D_GS_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_GS_A_57.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_D_SM_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_SM_A_57.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_C_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_C_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_SM_A_66.fbx"))))
		return E_FAIL;







	

	
	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.001.fbx"))))
		return E_FAIL;
	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.002.fbx"))))
		return E_FAIL;	
	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_Pivot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot.fbx"))))
		return E_FAIL;





	/*Segment.Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_Pivot_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot_Deco.fbx"))))
		return E_FAIL;


	/* Segment_GS_Deco*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_BG.fbx"))))
		return E_FAIL;





	/* Segment_SM_Deco*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_SM_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_Deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_SM_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_BG.fbx"))))
		return E_FAIL;



	/* Segment_Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_Stroke"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_Deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_Stroke_Default_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Stroke_Default_Line.fbx"))))
		return E_FAIL;






























	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("atomTri.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.002.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grenade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/Grenade.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Attack_MiniBoss_C_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/MiniBoss_C/Attack_MiniBoss_C_C.fbx"))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Attack_MiniBoss_C_B"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/MiniBoss_C/Attack_MiniBoss_C_B.fbx"))))
	//	return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Attack_MiniBoss_C_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Boss_bullet/Attack_MiniBoss_C_B.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Boss_bullet/BossAttack_C.fbx"))))
		return E_FAIL;


	/* Drone*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CubeDroneSword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneSword.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CubeDroneBullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneBullet.fbx"))))
		return E_FAIL;

	/*PlayerMine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PlayerMine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/Mine_0.fbx"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PlayerHommingMissile"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/HommingMissile.fbx"))))
		return E_FAIL;


#pragma region RankBonus
	/* Interactor_RankBonus_Boss*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_S.fbx"))))
		return E_FAIL;



	/*Interactor_RankBonus_Game*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_S.fbx"))))
		return E_FAIL;


	/*Interactor_RankBonus_MiniBoss*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_S.fbx"))))
		return E_FAIL;


	/*Interactor_RankBonus_Minigame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_S.fbx"))))
		return E_FAIL;
#pragma endregion



#pragma region Item
	/*Item_Box*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_EXP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_EXP.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_Gold"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Gold.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_HP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_HP.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_Random"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Random.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Shield.fbx"))))
		return E_FAIL;



	/*Items*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_EXP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_EXP_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_HP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_HP_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Life"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Life.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Shield_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Coin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Coin_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin_B.fbx"))))
		return E_FAIL;
#pragma endregion







	/*Objects_Shell*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Shell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Shell/Shell.fbx"))))
		return E_FAIL;

#pragma region EnvironmentObj
	/*UpgradeMachine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Foothold_Shop"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/Foothold_Shop.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("UpgradeMachine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("UpgradeMachine2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine2.fbx"))))
		return E_FAIL;



	/*EnvironmentObj*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ventilator"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/Ventilator.fbx"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("rubber_cone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/rubber_cone.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("DefaultBox"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/DefaultBox.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NoneTex"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/NoneTex.010.fbx"))))
		return E_FAIL;



	/*Objects_Changer_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Icon_Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Gun.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Icon_Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Sword.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Rot_Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Gun.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Rot_Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Sword.fbx"))))
		return E_FAIL;
#pragma endregion

#pragma endregion


















	lstrcpy(m_szLoadingText, TEXT("네비게이션(을) 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.dat")))))
		return E_FAIL;

#pragma region Shader
	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Stage"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Default_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Segment"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Segment.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Segment_SM*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Segment_SM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Segment_SM.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Segment_line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Segment_line"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_SegmentLine.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;







	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Vtx_HandBullet"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Vtx_HandBullet.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxParticle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Mesh.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstance_Mesh_Size"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Mesh_Size.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Mesh_Upgrade"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Default_VtxMesh_Upgrade.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

		/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Mesh_Player_Matilda"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPlayer_matilda.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
		/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Mesh_Player_Amanda"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPlayer_Amanda.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;


		/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Mesh_Stage_GM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Stage.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
		/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Mesh_Stage_SM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Stage_SM.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;



	
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstance_Rect"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Rect.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
	//	return E_FAIL;






#pragma endregion




#pragma region Particle
	lstrcpy(m_szLoadingText, TEXT("파티클을 로딩 중 입니다."));
	/*Particle*/


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_ExplosionFrag_001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_001_Up_And_Down"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_ExplosionFrag_002_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.002.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_002_Up_And_Down"))))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_ExplosionFrag_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_Spread"))))))
		return E_FAIL;





	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_Player_Amanda_Trace_Dash_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda_Trace_Dash.fbx", Make_ParticleDesc(TEXT("Effect_Player_Amanda_Trace_Dash_Random_Pop"))))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.001.fbx", Make_ParticleDesc(TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"))))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.002.fbx", Make_ParticleDesc(TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_ExplosionDust_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionDust_Explosion"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_Shockwave_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Effect_Shockwave_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_HitRing_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("Effect_HitRing_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HitRing_Boss_B"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("HitRing_Boss_B"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_HitRing_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("Boss_HitRing_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("ExplosionGroundEffect_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;
		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_hit002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit002.fbx", Make_ParticleDesc(TEXT("Effect_hit002_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_hit003_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit003.fbx", Make_ParticleDesc(TEXT("Effect_hit003_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Effect_hit004_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit004.fbx", Make_ParticleDesc(TEXT("Effect_hit004_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_Attack_Warning_Ray_SizeDown"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Ray.fbx", Make_ParticleDesc(TEXT("Boss_Attack_Warning_Ray_SizeDown"))))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_Warning_RayTest"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_RayTest.fbx", Make_ParticleDesc(TEXT("BossAttack_Warning_RayTest"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_Effect_Timestop_Sphere_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Sphere.fbx", Make_ParticleDesc(TEXT("Boss_Effect_Timestop_Sphere_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_Effect_Timestop_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/TimeStop.fbx", Make_ParticleDesc(TEXT("Boss_Effect_Timestop_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_Down_Effect_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Sphere.fbx", Make_ParticleDesc(TEXT("Boss_Down_Effect_Explosion"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Elect"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.fbx", Make_ParticleDesc(TEXT("Elect"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Elect.001"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.001.fbx", Make_ParticleDesc(TEXT("Elect.001"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Elect.002"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.002.fbx", Make_ParticleDesc(TEXT("Elect.002"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sphere_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Sphere_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Test"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Test"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_Attack_A"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack/BossAttack_A.002.fbx", Make_ParticleDesc(TEXT("Boss_Attack_A"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_A_Line"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack/BossAttack_A.002.fbx", Make_ParticleDesc(TEXT("BossAttack_A_Line"))))))
		return E_FAIL;



#pragma region Player_Balance_X
/*---------------------Player_Balance_X------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_X_Balalnce_newSlash_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_newSlash_Rotate"))))))
		return E_FAIL;
		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_X_Balalnce_newSlash.001_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.001.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_newSlash.001_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_X_Balalnce_SlashHaftone001_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashHaftone.001.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_SlashHaftone001_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_X_Balalnce_slashShockwave_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_slashShockwave_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_X_Balalnce_swordThrowing_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_swordThrowing_Rotate"))))))
		return E_FAIL;


/*-------------------------------------------------------------*/
#pragma endregion


#pragma region Player_Balance_C
	/*---------------------Player_Balance_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Balalnce_LowpolySphere16_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_LowpolySphere16_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Balalnce_stingerRing.001_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stinger.001.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerRing.001_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Balalnce_stingerHalftone_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerHalftone.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerHalftone_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Balalnce_stinger.001_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stinger.001_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Balalnce_stingerRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/Matilda_Trace_Attack.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Balance_XC
	/*---------------------Player_Balance_XC------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_LowpolySphere8_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere8_Explosion"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_LowpolySphere16_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere16_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_LowpolySphere16_Next_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere16_Next_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.001.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.002.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.003.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.004.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.005.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_slashLine_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_slashLine_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_slashLine_Next_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_slashLine_Next_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_swordGroundWind_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/swordGroundWind.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_swordGroundWind_Size_Up"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/
#pragma endregion



#pragma region Player_Technic_X
	/*---------------------Player_Technic_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_swordThrowing_Rotate_x"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_swordThrowing_Rotate_x"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_slashShockwave_Rotate_x"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_slashShockwave_Rotate_x"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_hitRing_SIZE_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_hitRing_SIZE_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_hit_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hit.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_hit_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_swordThrow_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrow.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_swordThrow_Pop"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/



#pragma endregion




#pragma region Player_Technic_C

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_C_slashShockwave_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashShockwave_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_C_slashCircleSword_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircleSword.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashCircleSword_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_C_slashCircle_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircle.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashCircle_Rotate"))))))
		return E_FAIL;




#pragma endregion

#pragma region Player_Technic_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_XC_slashCircle_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/XC/slashCircle.fbx", Make_ParticleDesc(TEXT("Player_Technic_XC_slashCircle_SizeUP"))))))
		return E_FAIL;




#pragma endregion


#pragma region Player_Power_X
	/*---------------------Player_Power_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_Matilda_Trace_Jump_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/Matilda_Trace_Jump.fbx", Make_ParticleDesc(TEXT("Player_Power_X_Matilda_Trace_Jump_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_slashLine_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Power_X_slashLine_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Power_X_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_LowpolyCylinder6_Size_Down_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolyCylinder6.fbx", Make_ParticleDesc(TEXT("Player_Power_X_LowpolyCylinder6_Size_Down_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_hitRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Power_X_hitRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"))))))
		return E_FAIL;



	/*-------------------------------------------------------------*/
#pragma endregion





#pragma region Player_Power_C
	/*---------------------Player_Power_C------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Power_HitRing001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit001.fbx", Make_ParticleDesc(TEXT("Player_C_Power_HitRing001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Power_HitRing003_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit003.fbx", Make_ParticleDesc(TEXT("Player_C_Power_HitRing003_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Power_LowPlySphere8_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_C_Power_LowPlySphere8_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Power_LowPlySphere16_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_C_Power_LowPlySphere16_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_C_Power_Stomp_SIzeDown_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/slashStomp.fbx", Make_ParticleDesc(TEXT("Player_C_Power_Stomp_SIzeDown_Y"))))))
		return E_FAIL;
	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Power_XC
/*---------------------Player_Power_XC------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_XC_Matilda_Trace_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/Matilda_Trace.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_Matilda_Trace_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_XC_newSlash.001_Size_UP_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/newSlash.001.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_newSlash.001_Size_UP_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_XC_stingerRing.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_stingerRing.001_Pop"))))))
		return E_FAIL;

/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Reverse_X
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"))))))
		return E_FAIL;



#pragma endregion


#pragma region Player_Reverse_C
/*---------------------Player_Reverse_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_Matilda_Trace_Jump_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/Matilda_Trace_Jump.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_Matilda_Trace_Jump_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_LowpolyCylinder6_Size_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolyCylinder6.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_LowpolyCylinder6_Size_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_ThrowingKnife_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/ThrowingKnife.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_ThrowingKnife_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_LowpolySphere16_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_LowpolySphere16_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_hitRing_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_hitRing_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_atomStar0022_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/atomStar0022.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_atomStar0022_SizeUP"))))))
		return E_FAIL;
/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Reverse_XC
/*---------------------Player_Reverse_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_stingerRing.001_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_stingerRing.001_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_stingerHalftone_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerHalftone.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_stingerHalftone_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_SlashCubic_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/SlashCubic.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_SlashCubic_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_LowpolySphere16_Size_Up"))))))
		return E_FAIL;

	
	/*-------------------------------------------------------------*/
#pragma endregion


#pragma region Player_Amanda_Default_Attack
	/*---------------------Player_Amanda_Default_Attack------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Beam.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Muzzle_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Muzzle.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Muzzle_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_hit001_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/hit001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_hit001_Size_Up_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Flame_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Flame.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Flame_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Shell001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Amanda_Balance_X
	/*---------------------Player_Amanda_Balance_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_Grenade_Rotate_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/Grenade.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_Grenade_Rotate_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.002.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"))))))
		return E_FAIL;
	
	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Amanda_Balance_C
	/*---------------------Player_Amanda_Balance_C------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/MuzzleBuck.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_Grappling_Pop_Texture"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/Grappling.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_Grappling_Pop_Texture"))))))
		return E_FAIL;




	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Amanda_Balance_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_hit001_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/hit001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_hit001_Size_Up_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_LowpolySphere16_2_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_LowpolySphere16_2_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_LowpolySphere16_1_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_LowpolySphere16_1_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Ult_Beam_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Beam.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Beam_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Ult_Ring_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Ring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Ring_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Ult_Spring_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Spring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Spring_Rotate_Z"))))))
		return E_FAIL;




	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Shell.002_Up_And_Down_Texture"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Shell.002.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Shell.002_Up_And_Down_Texture"))))))
		return E_FAIL;
#pragma endregion

#pragma region Player_Amanda_Technic_X
	//없음






#pragma endregion
#pragma region Player_Amanda_Technic_C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Technic_C_WireBarrier_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Technic/C/WireBarrier.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Technic_C_WireBarrier_Rotate_Z"))))))
		return E_FAIL;
#pragma endregion
#pragma region Player_Amanda_POWER_X
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_2"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_1"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_1"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_2"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_1"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_1"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_hit_Size_Down_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/hit.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_hit_Size_Down_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_ExplosionSpark001_Szie_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_ExplosionSpark001_Szie_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_atomStar0022_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/atomStar0022.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_atomStar0022_Size_Up_X"))))))
		return E_FAIL;
#pragma endregion
#pragma region Player_Amanda_POWER_C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/MuzzleBuck.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_LowpolySphere16_Size_Up"))))))
		return E_FAIL;

#pragma endregion
#pragma region Player_Amanda_POWER_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_MuzzleBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/MuzzleBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_MuzzleBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_Flame_Size_UP_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Flame.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Flame_Size_UP_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_BulletBuck_Size_Up"))))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_Ult_Beam_Vertical_Size_Down_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Beam_Vertical.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Ult_Beam_Vertical_Size_Down_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_Ult_Ring_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Ring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Ult_Ring_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down_2"))))))
		return E_FAIL;

#pragma endregion
#pragma region Player_Amanda_REVERSE_X




#pragma endregion

#pragma region Player_Swap
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_Style_Sword_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Sword.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_Style_Sword_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_Style_Gun_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Gun.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_Style_Gun_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_LowpolySphere16_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_LowpolySphere16_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_atomTri_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_atomTri.002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.002.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri.002_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_atomTri.001_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.001.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri.001_Spread"))))))
		return E_FAIL;

#pragma endregion




#pragma region Potal
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potal_Effect_hitRing_Rotate_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/PotalEffect/hitRing.fbx", Make_ParticleDesc(TEXT("Potal_Effect_hitRing_Rotate_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potal_Effect_LowpolyCylinder6_Rotate_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/PotalEffect/LowpolyCylinder6.fbx", Make_ParticleDesc(TEXT("Potal_Effect_LowpolyCylinder6_Rotate_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potal_Effect_atomTri_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/PotalEffect/atomTri.fbx", Make_ParticleDesc(TEXT("Potal_Effect_atomTri_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potal_Effect_atomTri.001_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/PotalEffect/atomTri.001.fbx", Make_ParticleDesc(TEXT("Potal_Effect_atomTri.001_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potal_Effect_atomTri.002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/PotalEffect/atomTri.002.fbx", Make_ParticleDesc(TEXT("Potal_Effect_atomTri.002_Spread"))))))
		return E_FAIL;
#pragma endregion

















	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("UpgradeMachine_Sizeup"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/TimeStop.fbx", Make_ParticleDesc(TEXT("UpgradeMachine_Sizeup"))))))
		return E_FAIL;

#pragma endregion





















	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("swordThrow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrow.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("swordThrowing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrowing.fbx"))))
		return E_FAIL;



	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_C_slashStomp"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/slashStomp.fbx"))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("atomTri"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("atomTri.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.001.fbx"))))
		return E_FAIL;








	lstrcpy(m_szLoadingText, TEXT("충돌체 원형을 로딩 중 입니다."));
	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FreeCamera*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FreeCamera"),
		CFreeCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_PlayerCamera*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerCamera"),
		CPlayerCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
	//	CMonster ::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Boss_D */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_D"),
		CBoss_D::Create(m_pDevice, m_pContext))))
		return E_FAIL;
		/* For.Prototype_GameObject_Boss_B */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_B"),
		CBoss_B::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hand_Boss"),
		CHandBoss::Create(m_pDevice, m_pContext))))
		return E_FAIL;




	/* For.Prototype_GameObject_CBoss_D_Picker */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CBoss_D_Picker"),
		CBoss_D_Picker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_GameObject_NewMold*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NewMold"),
		CNewMold::Create(m_pDevice, m_pContext))))
		return E_FAIL;

		/* Prototype_GameObject_NewMold*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NewMold_C"),
		CNewMold_C::Create(m_pDevice, m_pContext))))
		return E_FAIL;

		/* Prototype_GameObject_NewMold*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NewMold_D"),
		CNewMold_D::Create(m_pDevice, m_pContext))))
		return E_FAIL;

		/* Prototype_GameObject_MiniBoss*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MiniBoss_A"),
		CMiniBoss::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	/* Prototype_GameObject_MiniBoss*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MiniBoss_B"),
		CMiniBoss::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_GameObject_ForkLift */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift"),
		CForkLift::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.EnvironmentObj */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_EnvironmentObj"),
		CEnvironmentObj::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.UpgradeMachine */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UpgradeMachine"),
		CUpgradeMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.UpgradeMachine */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UpgradeMachineTop"),
		CUpgradeMachineTop::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Potal */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Potal"),
		CPotal::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Matilda_Body_Player"),
		Matilda_Body_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Amanda_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Amanda_Body_Player"),
		Amanda_Body_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"),
		CBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Drone */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Drone"),
		CDrone::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SwordThrow"),
	//	CSwordThow::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerBullet"),
		CPlayerBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerBullet_Mine"),
		CPlayerBullet_Mine::Create(m_pDevice, m_pContext))))
		return E_FAIL;





	/* For.Prototype_GameObject_DecorationParts */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Decoration"),
		CDecorationParts::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_BoneCollider */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BoneCollider"),
		CBoneCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_GameObject_Stage */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CStage"),
		CStage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Stage */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CStage_SM"),
		CStage_SM::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_particle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		CParticle_Mesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HandBullet"),
		CHandBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_B_Bullet"),
		CBoss_B_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NoneTex_Bullet"),
		CNoneTexBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CMold_Bullet"),
		CMold_CBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CSegment"),
		CSegment::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CSegment_SM"),
		CSegment_SM::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CSegment_Line"),
		CSegmentLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),
		UI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	











	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage_1()
{

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_Panel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Panel_Frame_State.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Player/Player.jpg"), 1))))
		return E_FAIL;

	/*Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_noise001"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise001.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_noise002"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise002.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_noise005"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise005.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_noise011"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise011.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texturenoise013"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise013.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Texture_noiseRepeat"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noiseRepeat.png"), 1))))
		return E_FAIL;






	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_Component_Model_ForkLift */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Model_ForkLift"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/ForkLift/ForkLift.fbx"))))
		return E_FAIL;




	/* For.Prototype_Component_Model_Fiona */

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx"))))
		return E_FAIL;









	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Model_newMatilda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newMatilda/newMatilda.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Model_newAmanda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newAmanda/newAmanda.fbx"))))
		return E_FAIL;




	/*Boss_A*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_A/Boss_A.fbx"))))
		return E_FAIL;


	/*Boss_B*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_B/Boss_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_B_Cylinder"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_B/attack_boss_b_cylinder/attack_boss_b_cylinder.fbx"))))
		return E_FAIL;




	/*Boss_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_C_Right_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_C_Right_Hand/Boss_C.fbx"))))
		return E_FAIL;
	/*Boss_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_C_Left_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_C_Left_Hand/Boss_C.fbx"))))
		return E_FAIL;


	/*Boss_D*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_D/Boss_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_D_Picker_New"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_D/Boss_D_Picker_New.fbx"))))
		return E_FAIL;


	/* MiniBoss_A*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Miniboss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_A/Miniboss_A.fbx"))))
		return E_FAIL;

	/* MiniBoss_B*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Miniboss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_B/Miniboss_B.fbx"))))
		return E_FAIL;

	/*MiniBoss_B_side*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Miniboss_B_Side"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_B_Side/Miniboss_B_Side.fbx"))))
		return E_FAIL;



	/* MiniBoss_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Miniboss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_C/Miniboss_C.fbx"))))
		return E_FAIL;


	/*NewMoldTest*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMoldTest"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldTest/NewMoldTest.fbx"))))
		return E_FAIL;


	/*NewMoldMobile_C*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMoldMobile_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldMobile_C/NewMoldMobile_C.fbx"))))
		return E_FAIL;

	/*NewMoldMobile_D*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMoldMobile_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldMobile_D/NewMoldMobile_D.fbx"))))
		return E_FAIL;






	/*Bossintro_A_v2*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Bossintro_A_v2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Bossintro_A_v2/Bossintro_A_v2.fbx"))))
		return E_FAIL;



	/*Bossintro_B*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Bossintro_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Bossintro_B/Bossintro_B.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Bomb*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Bomb"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Bomb/NewMold_Deco_Bomb.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Diamond*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Diamond"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Diamond/NewMold_Deco_Diamond.fbx"))))
		return E_FAIL;




	/*NewMold_Deco_Electrocity*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Electrocity"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Electrocity/NewMold_Deco_Electrocity.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Feather*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Feather"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Feather/NewMold_Deco_Feather.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Flower*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Flower"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Flower/NewMold_Deco_Flower.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Flower_Blue*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Flower_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Flower_Blue/NewMold_Deco_Flower_Blue.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Leaf*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Leaf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Leaf/NewMold_Deco_Leaf.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Mine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Mine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Mine/NewMold_Deco_Mine.fbx"))))
		return E_FAIL;
	/*NewMold_Deco_Platform*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Platform"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Platform/NewMold_Deco_Platform.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Rock*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Rock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Rock/NewMold_Deco_Rock.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Segment"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Segment/NewMold_Deco_Segment.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Spike*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Spike"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Spike/NewMold_Deco_Spike.fbx"))))
		return E_FAIL;

	/*NewMold_Deco_Time*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NewMold_Deco_Time"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Time/NewMold_Deco_Time.fbx"))))
		return E_FAIL;















	/*Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_A_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_A_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_SM_A_66.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_E_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_E_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_SM_A_66.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_D_GS_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_GS_A_57.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_D_SM_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_SM_A_57.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_C_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("StageGroundAssemble_C_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_SM_A_66.fbx"))))
		return E_FAIL;





	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.001.fbx"))))
		return E_FAIL;
	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.002.fbx"))))
		return E_FAIL;
	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_Pivot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot.fbx"))))
		return E_FAIL;





	/*Segment.Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_Pivot_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot_Deco.fbx"))))
		return E_FAIL;


	/* Segment_GS_Deco*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_GS_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_GS_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_BG.fbx"))))
		return E_FAIL;


	/* Segment_SM_Deco*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_SM_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_Deco.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_SM_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_BG.fbx"))))
		return E_FAIL;
	/* Segment_Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_GS_Stroke"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_Deco.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Segment_GS_Stroke_Default_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Stroke_Default_Line.fbx"))))
		return E_FAIL;







	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("atomTri.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.002.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Grenade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/Grenade.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Attack_MiniBoss_C_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/MiniBoss_C/Attack_MiniBoss_C_C.fbx"))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Attack_MiniBoss_C_B"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/MiniBoss_C/Attack_MiniBoss_C_B.fbx"))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Attack_MiniBoss_C_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Boss_bullet/Attack_MiniBoss_C_B.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("BossAttack_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Boss_bullet/BossAttack_C.fbx"))))
		return E_FAIL;


	/* Drone*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("CubeDroneSword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneSword.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("CubeDroneBullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneBullet.fbx"))))
		return E_FAIL;

	/*PlayerMine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("PlayerMine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/Mine_0.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("PlayerHommingMissile"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/HommingMissile.fbx"))))
		return E_FAIL;




	/* Interactor_RankBonus_Boss*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Boss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Boss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Boss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Boss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Boss_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_S.fbx"))))
		return E_FAIL;



	/*Interactor_RankBonus_Game*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Game_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Game_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Game_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Game_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Game_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_S.fbx"))))
		return E_FAIL;


	/*Interactor_RankBonus_MiniBoss*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_MiniBoss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_MiniBoss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_MiniBoss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_MiniBoss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_MiniBoss_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_S.fbx"))))
		return E_FAIL;


	/*Interactor_RankBonus_Minigame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Minigame_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_A.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Minigame_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Minigame_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_C.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Minigame_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_D.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_RankBonus_Minigame_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_S.fbx"))))
		return E_FAIL;



	/*Item_Box*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_Itembox_EXP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_EXP.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_Itembox_Gold"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Gold.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_Itembox_HP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_HP.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_Itembox_Random"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Random.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Interactor_Itembox_Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Shield.fbx"))))
		return E_FAIL;



	/*Items*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_EXP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_EXP_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_HP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_HP_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_Life"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Life.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_Shield_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_Coin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Item_Coin_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin_B.fbx"))))
		return E_FAIL;




	/*Objects_Changer_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Changer_Icon_Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Gun.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Changer_Icon_Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Sword.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Changer_Rot_Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Gun.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Changer_Rot_Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Sword.fbx"))))
		return E_FAIL;




	/*Objects_Shell*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Shell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Shell/Shell.fbx"))))
		return E_FAIL;


	/*UpgradeMachine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Foothold_Shop"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/Foothold_Shop.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("UpgradeMachine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("UpgradeMachine2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine2.fbx"))))
		return E_FAIL;



	/*EnvironmentObj*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Ventilator"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/Ventilator.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("rubber_cone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/rubber_cone.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("DefaultBox"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/DefaultBox.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("NoneTex"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/NoneTex.010.fbx"))))
		return E_FAIL;





















	lstrcpy(m_szLoadingText, TEXT("네비게이션(을) 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.dat")))))
		return E_FAIL;


	
#pragma region Shader
	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_Stage"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Default_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Segment"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Segment.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Segment_SM*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Segment_SM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Segment_SM.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Segment_line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Segment_line"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_SegmentLine.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;







	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_Vtx_HandBullet"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Vtx_HandBullet.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_VtxParticle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Mesh.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_VtxInstance_Mesh_Size"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Mesh_Size.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_Mesh_Upgrade"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Default_VtxMesh_Upgrade.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_Mesh_Player_Matilda"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPlayer_matilda.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_Mesh_Player_Amanda"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPlayer_Amanda.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_Mesh_Stage_GM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Stage.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Shader_Mesh_Stage_SM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Stage_SM.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;


#pragma region Particle
	lstrcpy(m_szLoadingText, TEXT("파티클을 로딩 중 입니다."));
	/*Particle*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_ExplosionFrag_001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_001_Up_And_Down"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_ExplosionFrag_002_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.002.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_002_Up_And_Down"))))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_ExplosionFrag_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_Spread"))))))
		return E_FAIL;





	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_Player_Amanda_Trace_Dash_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda_Trace_Dash.fbx", Make_ParticleDesc(TEXT("Effect_Player_Amanda_Trace_Dash_Random_Pop"))))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.001.fbx", Make_ParticleDesc(TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.002.fbx", Make_ParticleDesc(TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_ExplosionDust_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionDust_Explosion"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_Shockwave_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Effect_Shockwave_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_HitRing_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("Effect_HitRing_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("HitRing_Boss_B"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("HitRing_Boss_B"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_HitRing_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("Boss_HitRing_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("ExplosionGroundEffect_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_hit002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit002.fbx", Make_ParticleDesc(TEXT("Effect_hit002_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_hit003_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit003.fbx", Make_ParticleDesc(TEXT("Effect_hit003_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Effect_hit004_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit004.fbx", Make_ParticleDesc(TEXT("Effect_hit004_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_Attack_Warning_Ray_SizeDown"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Ray.fbx", Make_ParticleDesc(TEXT("Boss_Attack_Warning_Ray_SizeDown"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("BossAttack_Warning_RayTest"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_RayTest.fbx", Make_ParticleDesc(TEXT("BossAttack_Warning_RayTest"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_Effect_Timestop_Sphere_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Sphere.fbx", Make_ParticleDesc(TEXT("Boss_Effect_Timestop_Sphere_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_Effect_Timestop_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/TimeStop.fbx", Make_ParticleDesc(TEXT("Boss_Effect_Timestop_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_Down_Effect_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Sphere.fbx", Make_ParticleDesc(TEXT("Boss_Down_Effect_Explosion"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Elect"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.fbx", Make_ParticleDesc(TEXT("Elect"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Elect.001"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.001.fbx", Make_ParticleDesc(TEXT("Elect.001"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Elect.002"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.002.fbx", Make_ParticleDesc(TEXT("Elect.002"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Sphere_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Sphere_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Test"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Test"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Boss_Attack_A"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack/BossAttack_A.002.fbx", Make_ParticleDesc(TEXT("Boss_Attack_A"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("BossAttack_A_Line"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack/BossAttack_A.002.fbx", Make_ParticleDesc(TEXT("BossAttack_A_Line"))))))
		return E_FAIL;



#pragma region Player_Balance_X
	/*---------------------Player_Balance_X------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_X_Balalnce_newSlash_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_newSlash_Rotate"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_X_Balalnce_newSlash.001_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.001.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_newSlash.001_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_X_Balalnce_SlashHaftone001_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashHaftone.001.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_SlashHaftone001_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_X_Balalnce_slashShockwave_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_slashShockwave_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_X_Balalnce_swordThrowing_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_swordThrowing_Rotate"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion


#pragma region Player_Balance_C
	/*---------------------Player_Balance_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Balalnce_LowpolySphere16_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_LowpolySphere16_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Balalnce_stingerRing.001_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stinger.001.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerRing.001_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Balalnce_stingerHalftone_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerHalftone.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerHalftone_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Balalnce_stinger.001_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stinger.001_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Balalnce_stingerRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/Matilda_Trace_Attack.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Balance_XC
	/*---------------------Player_Balance_XC------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_LowpolySphere8_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere8_Explosion"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_LowpolySphere16_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere16_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_LowpolySphere16_Next_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere16_Next_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.001.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.002.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.003.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.004.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.005.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_slashLine_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_slashLine_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_slashLine_Next_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_slashLine_Next_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Balalnce_XC_swordGroundWind_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/swordGroundWind.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_swordGroundWind_Size_Up"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/
#pragma endregion



#pragma region Player_Technic_X
	/*---------------------Player_Technic_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_X_swordThrowing_Rotate_x"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_swordThrowing_Rotate_x"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_X_slashShockwave_Rotate_x"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_slashShockwave_Rotate_x"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_X_hitRing_SIZE_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_hitRing_SIZE_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_X_hit_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hit.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_hit_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_X_swordThrow_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrow.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_swordThrow_Pop"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/



#pragma endregion




#pragma region Player_Technic_C

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_C_slashShockwave_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashShockwave_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_C_slashCircleSword_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircleSword.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashCircleSword_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_C_slashCircle_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircle.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashCircle_Rotate"))))))
		return E_FAIL;




#pragma endregion

#pragma region Player_Technic_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Technic_XC_slashCircle_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/XC/slashCircle.fbx", Make_ParticleDesc(TEXT("Player_Technic_XC_slashCircle_SizeUP"))))))
		return E_FAIL;




#pragma endregion


#pragma region Player_Power_X
	/*---------------------Player_Power_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_X_Matilda_Trace_Jump_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/Matilda_Trace_Jump.fbx", Make_ParticleDesc(TEXT("Player_Power_X_Matilda_Trace_Jump_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_X_slashLine_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Power_X_slashLine_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_X_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Power_X_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_X_LowpolyCylinder6_Size_Down_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolyCylinder6.fbx", Make_ParticleDesc(TEXT("Player_Power_X_LowpolyCylinder6_Size_Down_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_X_hitRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Power_X_hitRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"))))))
		return E_FAIL;



	/*-------------------------------------------------------------*/
#pragma endregion





#pragma region Player_Power_C
	/*---------------------Player_Power_C------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Power_HitRing001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit001.fbx", Make_ParticleDesc(TEXT("Player_C_Power_HitRing001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Power_HitRing003_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit003.fbx", Make_ParticleDesc(TEXT("Player_C_Power_HitRing003_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Power_LowPlySphere8_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_C_Power_LowPlySphere8_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Power_LowPlySphere16_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_C_Power_LowPlySphere16_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_C_Power_Stomp_SIzeDown_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/slashStomp.fbx", Make_ParticleDesc(TEXT("Player_C_Power_Stomp_SIzeDown_Y"))))))
		return E_FAIL;
	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Power_XC
/*---------------------Player_Power_XC------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_XC_Matilda_Trace_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/Matilda_Trace.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_Matilda_Trace_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_XC_newSlash.001_Size_UP_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/newSlash.001.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_newSlash.001_Size_UP_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_XC_stingerRing.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_stingerRing.001_Pop"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Reverse_X
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"))))))
		return E_FAIL;



#pragma endregion


#pragma region Player_Reverse_C
	/*---------------------Player_Reverse_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_C_Matilda_Trace_Jump_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/Matilda_Trace_Jump.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_Matilda_Trace_Jump_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_C_LowpolyCylinder6_Size_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolyCylinder6.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_LowpolyCylinder6_Size_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_C_ThrowingKnife_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/ThrowingKnife.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_ThrowingKnife_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_C_LowpolySphere16_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_LowpolySphere16_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_C_hitRing_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_hitRing_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_C_atomStar0022_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/atomStar0022.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_atomStar0022_SizeUP"))))))
		return E_FAIL;
	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Reverse_XC
/*---------------------Player_Reverse_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_XC_stingerRing.001_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_stingerRing.001_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_XC_stingerHalftone_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerHalftone.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_stingerHalftone_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_XC_SlashCubic_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/SlashCubic.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_SlashCubic_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Reverse_XC_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_LowpolySphere16_Size_Up"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion


#pragma region Player_Amanda_Default_Attack
	/*---------------------Player_Amanda_Default_Attack------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Beam.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Default_Attack_Muzzle_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Muzzle.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Muzzle_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Default_Attack_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Default_Attack_hit001_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/hit001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_hit001_Size_Up_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Default_Attack_Flame_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Flame.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Flame_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Shell001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Amanda_Balance_X
	/*---------------------Player_Amanda_Balance_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_Grenade_Rotate_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/Grenade.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_Grenade_Rotate_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.002.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Amanda_Balance_C
	/*---------------------Player_Amanda_Balance_C------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_C_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/MuzzleBuck.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_C_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_C_Grappling_Pop_Texture"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/Grappling.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_Grappling_Pop_Texture"))))))
		return E_FAIL;




	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Amanda_Balance_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_hit001_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/hit001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_hit001_Size_Up_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_LowpolySphere16_2_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_LowpolySphere16_2_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_LowpolySphere16_1_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_LowpolySphere16_1_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_Ult_Beam_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Beam.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Beam_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_Ult_Ring_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Ring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Ring_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_Ult_Spring_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Spring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Spring_Rotate_Z"))))))
		return E_FAIL;




	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Balance_XC_Shell.002_Up_And_Down_Texture"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Shell.002.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Shell.002_Up_And_Down_Texture"))))))
		return E_FAIL;
#pragma endregion

#pragma region Player_Amanda_Technic_X
	//없음






#pragma endregion
#pragma region Player_Amanda_Technic_C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Technic_C_WireBarrier_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Technic/C/WireBarrier.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Technic_C_WireBarrier_Rotate_Z"))))))
		return E_FAIL;
#pragma endregion
#pragma region Player_Amanda_POWER_X
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_2"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_1"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_1"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_2"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_1"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_1"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_X_hit_Size_Down_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/hit.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_hit_Size_Down_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_X_ExplosionSpark001_Szie_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_ExplosionSpark001_Szie_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_X_atomStar0022_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/atomStar0022.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_atomStar0022_Size_Up_X"))))))
		return E_FAIL;
#pragma endregion
#pragma region Player_Amanda_POWER_C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_C_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/MuzzleBuck.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_C_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_LowpolySphere16_Size_Up"))))))
		return E_FAIL;

#pragma endregion
#pragma region Player_Amanda_POWER_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_MuzzleBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/MuzzleBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_MuzzleBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_Flame_Size_UP_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Flame.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Flame_Size_UP_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_BulletBuck_Size_Up"))))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_Ult_Beam_Vertical_Size_Down_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Beam_Vertical.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Ult_Beam_Vertical_Size_Down_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_Ult_Ring_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Ring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Ult_Ring_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down_2"))))))
		return E_FAIL;

#pragma endregion
#pragma region Player_Amanda_REVERSE_X




#pragma endregion

#pragma region Player_Swap
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Swap_Effect_Style_Sword_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Sword.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_Style_Sword_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Swap_Effect_Style_Gun_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Gun.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_Style_Gun_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Swap_Effect_LowpolySphere16_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_LowpolySphere16_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Swap_Effect_atomTri_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Swap_Effect_atomTri.002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.002.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri.002_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Swap_Effect_atomTri.001_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.001.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri.001_Spread"))))))
		return E_FAIL;

#pragma endregion


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("UpgradeMachine_Sizeup"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/TimeStop.fbx", Make_ParticleDesc(TEXT("UpgradeMachine_Sizeup"))))))
		return E_FAIL;



#pragma endregion



#pragma endregion





















	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("swordThrow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrow.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("swordThrowing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrowing.fbx"))))
		return E_FAIL;



	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Player_Power_C_slashStomp"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/slashStomp.fbx"))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("atomTri"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("atomTri.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.001.fbx"))))
		return E_FAIL;










	lstrcpy(m_szLoadingText, TEXT("충돌체 원형을 로딩 중 입니다."));
	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STAGE_1, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	

	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));





	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));










	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Stage_2()
{

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Stage_3()
{

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Change_Stage()
{

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_Panel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Panel_Frame_State.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Player/Player.jpg"), 1))))
		return E_FAIL;



	/*Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_noise001"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise001.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_noise002"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise002.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_noise005"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise005.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_noise011"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise011.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texturenoise013"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noise013.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Texture_noiseRepeat"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/noiseRepeat.png"), 1))))
		return E_FAIL;









	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Model_newMatilda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newMatilda/newMatilda.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Model_newAmanda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newAmanda/newAmanda.fbx"))))
		return E_FAIL;







	/*Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_A_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_A_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_SM_A_66.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_E_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_E_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_SM_A_66.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_D_GS_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_GS_A_57.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_D_SM_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_SM_A_57.fbx"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_C_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_GS_A_66.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("StageGroundAssemble_C_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_SM_A_66.fbx"))))
		return E_FAIL;





	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.001.fbx"))))
		return E_FAIL;
	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.002.fbx"))))
		return E_FAIL;
	/*Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_Pivot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot.fbx"))))
		return E_FAIL;





	/*Segment.Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_Pivot_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot_Deco.fbx"))))
		return E_FAIL;


	/* Segment_GS_Deco*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_GS_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Deco.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_GS_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_BG.fbx"))))
		return E_FAIL;


	/* Segment_SM_Deco*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_SM_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_Deco.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_SM_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_BG.fbx"))))
		return E_FAIL;
	/* Segment_Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_GS_Stroke"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_Deco.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Segment_GS_Stroke_Default_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Stroke_Default_Line.fbx"))))
		return E_FAIL;






	/* Weapon Object*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("atomTri.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.002.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Grenade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/Grenade.fbx"))))
		return E_FAIL;
	/* Drone*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("CubeDroneSword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneSword.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("CubeDroneBullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneBullet.fbx"))))
		return E_FAIL;
	

	/*PlayerMine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("PlayerMine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/Mine_0.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("PlayerHommingMissile"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/HommingMissile.fbx"))))
		return E_FAIL;




	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("swordThrow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrow.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("swordThrowing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrowing.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("slashStomp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/slashStomp.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("atomTri"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("atomTri.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.001.fbx"))))
		return E_FAIL;





	/*Items*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_EXP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_EXP_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_HP"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_HP_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_Life"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Life.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_Shield_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield_B.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_Coin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Item_Coin_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin_B.fbx"))))
		return E_FAIL;


	/*Objects_Changer_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Changer_Icon_Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Gun.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Changer_Icon_Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Sword.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Changer_Rot_Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Gun.fbx"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Changer_Rot_Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Sword.fbx"))))
		return E_FAIL;


	/*UpgradeMachine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Foothold_Shop"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/Foothold_Shop.fbx"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("UpgradeMachine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine.fbx"))))
		return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("UpgradeMachine2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine2.fbx"))))
		return E_FAIL;



	/* For.Prototype_Component_Model_ForkLift */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Model_ForkLift"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/ForkLift/ForkLift.fbx"))))
		return E_FAIL;






	lstrcpy(m_szLoadingText, TEXT("네비게이션(을) 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.dat")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_Stage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_Stage"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Default_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Segment*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Segment"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Segment.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Segment_SM*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Segment_SM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Segment_SM.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Segment_line*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Segment_line"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_SegmentLine.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;







	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_Vtx_HandBullet"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Vtx_HandBullet.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_VtxParticle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Mesh.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_VtxInstance_Mesh_Size"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Mesh_Size.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_Mesh_Upgrade"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Default_VtxMesh_Upgrade.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_Mesh_Player_Matilda"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPlayer_matilda.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_Mesh_Player_Amanda"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxPlayer_Amanda.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_Mesh_Stage_GM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Stage.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Shader_Mesh_Stage_SM"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_Stage_SM.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("파티클을 로딩 중 입니다."));
	/*Particle*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_ExplosionFrag_001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_001_Up_And_Down"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_ExplosionFrag_002_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.002.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_002_Up_And_Down"))))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_ExplosionFrag_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionFrag_Spread"))))))
		return E_FAIL;





	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_Player_Amanda_Trace_Dash_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda_Trace_Dash.fbx", Make_ParticleDesc(TEXT("Effect_Player_Amanda_Trace_Dash_Random_Pop"))))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.001.fbx", Make_ParticleDesc(TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.002.fbx", Make_ParticleDesc(TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_ExplosionDust_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Effect_ExplosionDust_Explosion"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_Shockwave_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Effect_Shockwave_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_HitRing_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("Effect_HitRing_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("HitRing_Boss_B"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("HitRing_Boss_B"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Boss_HitRing_SizeUp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", Make_ParticleDesc(TEXT("Boss_HitRing_SizeUp"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("ExplosionGroundEffect_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_hit002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit002.fbx", Make_ParticleDesc(TEXT("Effect_hit002_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_hit003_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit003.fbx", Make_ParticleDesc(TEXT("Effect_hit003_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Effect_hit004_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/hit004.fbx", Make_ParticleDesc(TEXT("Effect_hit004_Spread"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Boss_Attack_Warning_Ray_SizeDown"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Ray.fbx", Make_ParticleDesc(TEXT("Boss_Attack_Warning_Ray_SizeDown"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("BossAttack_Warning_RayTest"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_RayTest.fbx", Make_ParticleDesc(TEXT("BossAttack_Warning_RayTest"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Boss_Effect_Timestop_Sphere_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Sphere.fbx", Make_ParticleDesc(TEXT("Boss_Effect_Timestop_Sphere_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Boss_Effect_Timestop_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/TimeStop.fbx", Make_ParticleDesc(TEXT("Boss_Effect_Timestop_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Boss_Down_Effect_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Sphere.fbx", Make_ParticleDesc(TEXT("Boss_Down_Effect_Explosion"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Elect"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.fbx", Make_ParticleDesc(TEXT("Elect"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Elect.001"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.001.fbx", Make_ParticleDesc(TEXT("Elect.001"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Elect.002"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Elect.002.fbx", Make_ParticleDesc(TEXT("Elect.002"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Sphere_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Sphere_Pop"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Test"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Test"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Boss_Attack_A"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack/BossAttack_A.002.fbx", Make_ParticleDesc(TEXT("Boss_Attack_A"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("BossAttack_A_Line"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack/BossAttack_A.002.fbx", Make_ParticleDesc(TEXT("BossAttack_A_Line"))))))
		return E_FAIL;



#pragma region Player_Balance_X
	/*---------------------Player_Balance_X------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_X_Balalnce_newSlash_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_newSlash_Rotate"))))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_X_Balalnce_newSlash.001_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.001.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_newSlash.001_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_X_Balalnce_SlashHaftone001_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashHaftone.001.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_SlashHaftone001_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_X_Balalnce_slashShockwave_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_slashShockwave_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_X_Balalnce_swordThrowing_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_X_Balalnce_swordThrowing_Rotate"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion


#pragma region Player_Balance_C
	/*---------------------Player_Balance_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Balalnce_LowpolySphere16_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_LowpolySphere16_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Balalnce_stingerRing.001_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stinger.001.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerRing.001_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Balalnce_stingerHalftone_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerHalftone.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerHalftone_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Balalnce_stinger.001_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stinger.001_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Balalnce_stingerRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_stingerRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/Matilda_Trace_Attack.fbx", Make_ParticleDesc(TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Balance_XC
	/*---------------------Player_Balance_XC------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_LowpolySphere8_Explosion"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere8_Explosion"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_LowpolySphere16_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere16_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_LowpolySphere16_Next_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_LowpolySphere16_Next_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.001.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.002.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.003.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.004.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.005.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_Matilda_Trace_Ult_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_slashLine_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_slashLine_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_slashLine_Next_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_slashLine_Next_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Balalnce_XC_swordGroundWind_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/swordGroundWind.fbx", Make_ParticleDesc(TEXT("Player_Balalnce_XC_swordGroundWind_Size_Up"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/
#pragma endregion



#pragma region Player_Technic_X
	/*---------------------Player_Technic_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_X_swordThrowing_Rotate_x"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_swordThrowing_Rotate_x"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_X_slashShockwave_Rotate_x"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_slashShockwave_Rotate_x"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_X_hitRing_SIZE_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_hitRing_SIZE_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_X_hit_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hit.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_hit_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_X_swordThrow_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrow.fbx", Make_ParticleDesc(TEXT("Player_Technic_X_swordThrow_Pop"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/



#pragma endregion




#pragma region Player_Technic_C

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_C_slashShockwave_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashShockwave_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_C_slashCircleSword_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircleSword.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashCircleSword_Rotate"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_C_slashCircle_Rotate"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircle.fbx", Make_ParticleDesc(TEXT("Player_Technic_C_slashCircle_Rotate"))))))
		return E_FAIL;




#pragma endregion

#pragma region Player_Technic_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Technic_XC_slashCircle_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/XC/slashCircle.fbx", Make_ParticleDesc(TEXT("Player_Technic_XC_slashCircle_SizeUP"))))))
		return E_FAIL;




#pragma endregion


#pragma region Player_Power_X
	/*---------------------Player_Power_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_X_Matilda_Trace_Jump_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/Matilda_Trace_Jump.fbx", Make_ParticleDesc(TEXT("Player_Power_X_Matilda_Trace_Jump_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_X_slashLine_Size_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/slashLine.fbx", Make_ParticleDesc(TEXT("Player_Power_X_slashLine_Size_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_X_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Power_X_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_X_LowpolyCylinder6_Size_Down_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolyCylinder6.fbx", Make_ParticleDesc(TEXT("Player_Power_X_LowpolyCylinder6_Size_Down_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_X_hitRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Power_X_hitRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"))))))
		return E_FAIL;



	/*-------------------------------------------------------------*/
#pragma endregion





#pragma region Player_Power_C
	/*---------------------Player_Power_C------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Power_HitRing001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit001.fbx", Make_ParticleDesc(TEXT("Player_C_Power_HitRing001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Power_HitRing003_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit003.fbx", Make_ParticleDesc(TEXT("Player_C_Power_HitRing003_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Power_LowPlySphere8_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_C_Power_LowPlySphere8_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Power_LowPlySphere16_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_C_Power_LowPlySphere16_SizeUP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_C_Power_Stomp_SIzeDown_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/slashStomp.fbx", Make_ParticleDesc(TEXT("Player_C_Power_Stomp_SIzeDown_Y"))))))
		return E_FAIL;
	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Power_XC
/*---------------------Player_Power_XC------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_XC_Matilda_Trace_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/Matilda_Trace.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_Matilda_Trace_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_XC_newSlash.001_Size_UP_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/newSlash.001.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_newSlash.001_Size_UP_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Power_XC_stingerRing.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_Power_XC_stingerRing.001_Pop"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Reverse_X
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/swordThrowing.fbx", Make_ParticleDesc(TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/slashShockwave.fbx", Make_ParticleDesc(TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"))))))
		return E_FAIL;



#pragma endregion


#pragma region Player_Reverse_C
	/*---------------------Player_Reverse_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_C_Matilda_Trace_Jump_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/Matilda_Trace_Jump.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_Matilda_Trace_Jump_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_C_LowpolyCylinder6_Size_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolyCylinder6.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_LowpolyCylinder6_Size_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_C_ThrowingKnife_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/ThrowingKnife.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_ThrowingKnife_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_C_LowpolySphere16_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_LowpolySphere16_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_C_hitRing_Size_UP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/hitRing.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_hitRing_Size_UP"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_C_atomStar0022_SizeUP"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/atomStar0022.fbx", Make_ParticleDesc(TEXT("Player_Reverse_C_atomStar0022_SizeUP"))))))
		return E_FAIL;
	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Reverse_XC
/*---------------------Player_Reverse_C------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_XC_stingerRing.001_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerRing.001.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_stingerRing.001_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_XC_stingerHalftone_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerHalftone.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_stingerHalftone_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_XC_SlashCubic_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/SlashCubic.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_SlashCubic_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Reverse_XC_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Reverse_XC_LowpolySphere16_Size_Up"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion


#pragma region Player_Amanda_Default_Attack
	/*---------------------Player_Amanda_Default_Attack------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Beam.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Default_Attack_Muzzle_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Muzzle.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Muzzle_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Default_Attack_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Default_Attack_hit001_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/hit001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_hit001_Size_Up_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Default_Attack_Flame_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Flame.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Flame_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Shell001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"))))))
		return E_FAIL;


	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Amanda_Balance_X
	/*---------------------Player_Amanda_Balance_X------------------------*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_Grenade_Rotate_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/Grenade.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_Grenade_Rotate_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere8.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.002.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"))))))
		return E_FAIL;

	/*-------------------------------------------------------------*/
#pragma endregion
#pragma region Player_Amanda_Balance_C
	/*---------------------Player_Amanda_Balance_C------------------------*/

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_C_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/MuzzleBuck.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_C_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_C_Grappling_Pop_Texture"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/Grappling.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_C_Grappling_Pop_Texture"))))))
		return E_FAIL;




	/*-------------------------------------------------------------*/
#pragma endregion

#pragma region Player_Amanda_Balance_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_hit001_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/hit001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_hit001_Size_Up_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_LowpolySphere16_2_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_LowpolySphere16_2_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_LowpolySphere16_1_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_LowpolySphere16_1_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_Ult_Beam_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Beam.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Beam_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_Ult_Ring_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Ring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Ring_Rotate_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_Ult_Spring_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Spring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Ult_Spring_Rotate_Z"))))))
		return E_FAIL;




	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Balance_XC_Shell.002_Up_And_Down_Texture"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Shell.002.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Balance_XC_Shell.002_Up_And_Down_Texture"))))))
		return E_FAIL;
#pragma endregion

#pragma region Player_Amanda_Technic_X
	//없음






#pragma endregion
#pragma region Player_Amanda_Technic_C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Technic_C_WireBarrier_Rotate_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Technic/C/WireBarrier.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Technic_C_WireBarrier_Rotate_Z"))))))
		return E_FAIL;
#pragma endregion
#pragma region Player_Amanda_POWER_X
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_2"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_1"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_1"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_2"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_1"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_1"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_X_hit_Size_Down_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/hit.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_hit_Size_Down_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_X_ExplosionSpark001_Szie_Down_Z"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/ExplosionSpark001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_ExplosionSpark001_Szie_Down_Z"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_X_atomStar0022_Size_Up_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/atomStar0022.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_X_atomStar0022_Size_Up_X"))))))
		return E_FAIL;
#pragma endregion
#pragma region Player_Amanda_POWER_C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_C_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/MuzzleBuck.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_C_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_C_LowpolySphere16_Size_Up"))))))
		return E_FAIL;

#pragma endregion
#pragma region Player_Amanda_POWER_XC
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_ShotRing_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ShotRing.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ShotRing_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_MuzzleBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/MuzzleBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_MuzzleBuck_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_LowpolySphere16_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_LowpolySphere16_Size_Up"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_Flame_Size_UP_Y"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Flame.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Flame_Size_UP_Y"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_BulletBuck_Size_Up"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/BulletBuck.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_BulletBuck_Size_Up"))))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_Ult_Beam_Vertical_Size_Down_X"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Beam_Vertical.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Ult_Beam_Vertical_Size_Down_X"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_Ult_Ring_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Ring.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_Ult_Ring_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect.001_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.001.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect.001_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down_2"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionFrag.fbx", Make_ParticleDesc(TEXT("Player_Amanda_Power_XC_ExplosionFrag_Up_And_Down_2"))))))
		return E_FAIL;

#pragma endregion
#pragma region Player_Amanda_REVERSE_X




#pragma endregion

#pragma region Player_Swap
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Swap_Effect_Style_Sword_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Sword.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_Style_Sword_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Swap_Effect_Style_Gun_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Gun.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_Style_Gun_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Swap_Effect_LowpolySphere16_Pop"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/LowpolySphere16.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_LowpolySphere16_Pop"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Swap_Effect_atomTri_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Swap_Effect_atomTri.002_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.002.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri.002_Spread"))))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Player_Swap_Effect_atomTri.001_Spread"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.001.fbx", Make_ParticleDesc(TEXT("Player_Swap_Effect_atomTri.001_Spread"))))))
		return E_FAIL;

#pragma endregion


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("UpgradeMachine_Sizeup"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Object/Particle/Effect/TimeStop.fbx", Make_ParticleDesc(TEXT("UpgradeMachine_Sizeup"))))))
		return E_FAIL;



















	




	lstrcpy(m_szLoadingText, TEXT("충돌체 원형을 로딩 중 입니다."));
	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_CHANGE_STAGE, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;





	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Boss_1()
{

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Boss_2()
{

	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
