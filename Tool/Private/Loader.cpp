
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Terrain.h"
#include "Monster.h"
#include"ForkLift.h"
#include "FreeCamera.h"
#include"VIBuffer_Instance.h"
#include"Particle_Mesh.h"

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
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Panel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/MoblieColor.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Player/Player.jpg"), 1))))
		return E_FAIL;





	
	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	_matrix		PreTransformMatrix;
	PreTransformMatrix =XMMatrixIdentity();

	/* For.Prototype_Component_Model_ForkLift */

	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/ForkLift/ForkLift.fbx", PreTransformMatrix))))
		return E_FAIL;



	/* For.Prototype_Component_Model_Fiona */
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix))))
		return E_FAIL;

	//PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	PreTransformMatrix = XMMatrixIdentity();
	vector<const _char*> vDeleteMatildamesh;
	vDeleteMatildamesh.push_back("Matilda_Face");
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_newMatilda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newMatilda/newMatilda.fbx", PreTransformMatrix, &vDeleteMatildamesh))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	vector<const _char*> vDeleteAmandamesh;
	vDeleteAmandamesh.push_back("Amanda_Face");
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_newAmanda"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/newAmanda/newAmanda.fbx", PreTransformMatrix, &vDeleteAmandamesh))))
		return E_FAIL;




	/*NewMoldTest*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMoldTest"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldTest/NewMoldTest.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMoldMobile_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldMobile_C/NewMoldMobile_C.fbx", PreTransformMatrix))))
		return E_FAIL;

	//PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMoldMobile_B"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldMobile_B/NewMoldMobile_Bout.fbx", PreTransformMatrix))))
	//	return E_FAIL;


	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_D/Miniboss_D.fbx", PreTransformMatrix))))
		return E_FAIL;

	
	

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMoldMobile_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMoldMobile_D/NewMoldMobile_D.fbx", PreTransformMatrix))))
		return E_FAIL;




	/*Boss_A*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_A/Boss_A.fbx", PreTransformMatrix))))
		return E_FAIL;


	

	/*Boss_B*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_B/Boss_B.fbx", PreTransformMatrix))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_B_Cylinder"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_B/attack_boss_b_cylinder/attack_boss_b_cylinder.fbx", PreTransformMatrix))))
		return E_FAIL;





	/*Boss_C*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_C_Right_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_C_Right_Hand/Boss_C.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*Boss_C*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_C_Left_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_C_Left_Hand/Boss_C.fbx", PreTransformMatrix))))
		return E_FAIL;


	/*Boss_D*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_D/Boss_D.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boss_D_Picker_New"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Boss_D/Boss_D_Picker_New.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* MiniBoss_A*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_A/Miniboss_A.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* MiniBoss_B*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_B/Miniboss_B.fbx", PreTransformMatrix))))
		return E_FAIL;

	/*MiniBoss_B_side*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_B_Side"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_B_Side/Miniboss_B_Side.fbx", PreTransformMatrix))))
		return E_FAIL;




	/* MiniBoss_C*/
		PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Miniboss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Miniboss_C/Miniboss_C.fbx", PreTransformMatrix))))
		return E_FAIL;





	/*Bossintro_A_v2*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bossintro_A_v2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Bossintro_A_v2/Bossintro_A_v2.fbx", PreTransformMatrix))))
		return E_FAIL;



	/*Bossintro_B*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bossintro_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Bossintro_B/Bossintro_B.fbx", PreTransformMatrix))))
		return E_FAIL;

	/*NewMold_Deco_Bomb*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Bomb"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Bomb/NewMold_Deco_Bomb.fbx", PreTransformMatrix))))
		return E_FAIL;

	/*NewMold_Deco_Diamond*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Diamond"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Diamond/NewMold_Deco_Diamond.fbx", PreTransformMatrix))))
		return E_FAIL;




	/*NewMold_Deco_Electrocity*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Electrocity"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Electrocity/NewMold_Deco_Electrocity.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Feather*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Feather"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Feather/NewMold_Deco_Feather.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Flower*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Flower"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Flower/NewMold_Deco_Flower.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Flower_Blue*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Flower_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Flower_Blue/NewMold_Deco_Flower_Blue.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Leaf*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Leaf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Leaf/NewMold_Deco_Leaf.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Mine*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Mine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Mine/NewMold_Deco_Mine.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Platform*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Platform"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Platform/NewMold_Deco_Platform.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Rock*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Rock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Rock/NewMold_Deco_Rock.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Segment*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Segment"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Segment/NewMold_Deco_Segment.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Spike*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Spike"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Spike/NewMold_Deco_Spike.fbx", PreTransformMatrix))))
		return E_FAIL;
	/*NewMold_Deco_Time*/
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NewMold_Deco_Time"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/NewMold_Deco_Time/NewMold_Deco_Time.fbx", PreTransformMatrix))))
		return E_FAIL;
	
	
	
	




	/*Stage*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_E_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_GS_A_66.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_E_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_SM_A_66.fbx", PreTransformMatrix))))
		return E_FAIL;



	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_A_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_GS_A_66.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_A_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_A_SM_A_66.fbx", PreTransformMatrix))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_D_GS_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_GS_A_57.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_D_SM_A_57"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_D_SM_A_57.fbx", PreTransformMatrix))))
		return E_FAIL;

		PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_E_SM_A_66_TEST"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_E_SM_A_66_TEST.fbx", PreTransformMatrix))))
		return E_FAIL;



	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_C_SM_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_SM_A_66.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StageGroundAssemble_C_GS_A_66"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Stage/StageGroundAssemble_C_GS_A_66.fbx", PreTransformMatrix))))
		return E_FAIL;

	


	/* Segment*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.001.fbx", PreTransformMatrix))))
		return E_FAIL;
	/* Segment*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.002.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_Pivot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot.fbx", PreTransformMatrix))))
		return E_FAIL;	


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_Pivot_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot.deco.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_Pivot_LineTest"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/SegmentPivot_Deco.fbx", PreTransformMatrix))))
		return E_FAIL;


	






	/* Segment.Line*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment.deco.fbx", PreTransformMatrix))))
		return E_FAIL;
	
	/* Segment_GS_Deco*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Deco.fbx", PreTransformMatrix))))
		return E_FAIL;
	/* Segment_SM_Deco*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_SM_Deco"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_Deco.fbx", PreTransformMatrix))))
		return E_FAIL;
	/* Segment_GS_Stroke*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_Stroke"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Stroke.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_BG.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_SM_BG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_SM_BG.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* Segment_GS_Stroke*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Segment_GS_Stroke_Default_Line"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Segment/Segment_GS_Stroke_Default_Line.fbx", PreTransformMatrix))))
		return E_FAIL;












	/*Particle*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("swordThrow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrow.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("swordThrowing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/swordThrowing.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grenade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/Grenade.fbx", PreTransformMatrix))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("slashShockwave"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/slashShockwave.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("slashStomp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/slashStomp.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("atomTri"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("atomTri.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.001.fbx", PreTransformMatrix))))
		return E_FAIL;PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("atomTri.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Throw/atomTri.002.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Boss_bullet/BossAttack_C.fbx", PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Attack_MiniBoss_C_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Boss_bullet/Attack_MiniBoss_C_B.fbx", PreTransformMatrix))))
		return E_FAIL;







	/*Particle*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Attack_MiniBoss_C_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/MiniBoss_C/Attack_MiniBoss_C_C.fbx", PreTransformMatrix))))
		return E_FAIL;



	/*Effect*/
	//TYPE_PARTICLE
	//TYPE_NONANIM
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ElectColumnHit"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/ElectColumnHit.fbx", PreTransformMatrix))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionSpark001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionSpark001.fbx", PreTransformMatrix))))
		return E_FAIL; 


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GrapplingDust"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/GrapplingDust.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/LowpolySphere16.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	





	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionFrag.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.001.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionFrag.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.002.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionFrag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionFrag.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_Warning_Circle.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.001.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_Warning_Circle.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Circle.002.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("hitRing.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/hitRing.001.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionGroundEffect"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ExplosionGroundEffect.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/ExplosionGroundEffect.001.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TimeStop"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/TimeStop.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 
	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_Warning_Ray"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_Ray.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_Warning_RayTest"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack_Warning_RayTest.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sphere"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Sphere.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("hit002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/hit002.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("hit003"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/hit003.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("hit004"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/hit004.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;



//-----Player_Balance_X---------------------------------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_X_slashHaftone.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashHaftone.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_X_slashShockwave"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/slashShockwave.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_X_swordThrowing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/swordThrowing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_X_newSlash.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_X_newSlash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/X/newSlash.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
//---------------------------------------------


//-----Player_Balance_C-----------

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_C_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_C_stinger.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stinger.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_C_stingerHalftone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerHalftone.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_C_stingerRing.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_C_stingerRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/stingerRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_C_Matilda_Trace_Attack"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/C/Matilda_Trace_Attack.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


//---------------------------------------------

//-----Player_Balance_XC-----------
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_ExplosionFrag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/ExplosionFrag.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_LowpolySphere8"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere8.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.002.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.003.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.004.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.005.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_Matilda_Trace_Ult"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/Matilda_Trace_Ult.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_slashLine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/slashLine.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Balalnce_XC_swordGroundWind"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Balalnce/XC/swordGroundWind.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;




//---------------------------------------------



//-----Player_Technic_X-----------
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_hit"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hit.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_hitRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/hitRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_slashShockwave"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/slashShockwave.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_swordThrowing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrowing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_X_swordThrow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/X/swordThrow.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;





//---------------------------------------------






//-----Player_Technic_C-----------
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_C_slashCircle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircle.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_C_slashCircleSword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashCircleSword.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_C_slashShockwave"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/slashShockwave.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_C_Torus.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/C/Torus.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;




//---------------------------------------------

//-----Player_Technic_XC-----------
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Technic_XC_slashCircle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Technic/XC/slashCircle.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

//---------------------------------------------

//-----Player_Power_C
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_C_hit001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_C_hit003"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/hit003.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_C_LowpolySphere8"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere8.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_C_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_C_slashStomp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/C/slashStomp.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
//---------------------------------

//-----Player_Power_X--------------

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_ExplosionFrag.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/ExplosionFrag.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_hitRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/hitRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_LowpolyCylinder6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolyCylinder6.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_slashLine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/slashLine.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_swordGroundWind"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/swordGroundWind.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_X_Matilda_Trace_Jump"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/X/Matilda_Trace_Jump.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

//---------------------------------


//-----Player_Power_XC--------------

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_XC_Matilda_Trace"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/Matilda_Trace.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_XC_newSlash.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/newSlash.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Power_XC_stingerRing.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Power/XC/stingerRing.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


//---------------------------------
//-----Player_Reverse_C--------------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_X_slashShockwave"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/slashShockwave.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_X_swordThrowing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/X/swordThrowing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;



//---------------------------------

//-----Player_Reverse_C--------------


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_atomStar0022"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/atomStar0022.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_hitRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/hitRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_ThrowingKnife"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/ThrowingKnife.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_LowpolyCylinder6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/LowpolyCylinder6.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_C_Matilda_Trace_Jump"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/C/Matilda_Trace_Jump.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;



//---------------------------------
//-----Player_Reverse_XC--------------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_SlashCubic"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/SlashCubic.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_stingerHalftone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerHalftone.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Reverse_XC_stingerRing.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Reverse/XC/stingerRing.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


//---------------------------------
//-----Player_Amanda_Default_Attack--------------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Beam"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Beam.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Flame"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Flame.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_hit001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/hit001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Muzzle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Muzzle.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Default_Attack_Shell001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/DefaultAttack/Shell001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
//---------------------------------

//-----Player_Amanda_Balance_X-----------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionFrag.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
		PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionFrag.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.002.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionFrag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionFrag.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/ExplosionGroundEffect.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_LowpolySphere8"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere8.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_X_Grenade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/X/Grenade.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;





//---------------------------------

//-----Player_Amanda_Balance_C-----------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_Grappling"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/Grappling.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_MuzzleBuck.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/MuzzleBuck.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_ShotRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/ShotRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_C_BulletBuck"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/C/BulletBuck.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

//---------------------------------
//-----Player_Amanda_Balance_XC-----------

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Ult_Spring"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Spring.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Ult_Ring"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Ring.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Ult_Beam"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Ult_Beam.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_hit001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/hit001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_ExplosionFrag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/ExplosionFrag.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_ExplosionFrag.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/ExplosionFrag.002.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Balance_XC_Shell.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Balalnce/XC/Shell.002.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

//---------------------------------
//-----Player_Amanda_Technic_X-----------
	//없음


//---------------------------------

//-----Player_Amanda_Technic_C-----------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Technic_C_WireBarrier"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Technic/C/WireBarrier.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

//---------------------------------
 //-----Player_Amanda_Power_X-----------

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_atomStar0022"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/atomStar0022.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_ExplosionSpark001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/ExplosionSpark001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_hit"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/hit.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_X_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/X/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

//---------------------------------
 //-----Player_Amanda_Power_C-----------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_BulletBuck"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/BulletBuck.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_MuzzleBuck.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/MuzzleBuck.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_C_ShotRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/C/ShotRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


//---------------------------------
//-----Player_Amanda_Power_XC-----------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_BulletBuck"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/BulletBuck.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_Flame"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Flame.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_MuzzleBuck"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/MuzzleBuck.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ShotRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ShotRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ExplosionFrag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionFrag.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_ExplosionGroundEffect"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/ExplosionGroundEffect.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_Ult_Ring"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Ring.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Amanda_Power_XC_Ult_Beam_Vertical"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda/Power/XC/Ult_Beam_Vertical.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;


//---------------------------------

//-----Player_Swap_Effect-----------
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_atomTri.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.001.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_atomTri.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.002.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_atomTri"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/atomTri.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_LowpolySphere16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/LowpolySphere16.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_Style_Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Gun.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Player_Swap_Effect_Style_Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Swap/Style_Sword.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;
//---------------------------------






//----------Potal----------------

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potal_Effect_LowpolyCylinder6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/PotalEffect/LowpolyCylinder6.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potal_Effect_hitRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/PotalEffect/hitRing.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;






//---------------------------------


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("hit005"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/hit005.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 


	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Elect"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Elect.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Elect.001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Elect.001.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 

	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Elect.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Elect.002.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 	
	
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossAttack_A.002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/BossAttack/BossAttack_A.002.fbx", PreTransformMatrix,nullptr))))
		return E_FAIL; 


	







	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Amanda_Trace_Dash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Particle/Effect/Player/Amanda_Trace_Dash.fbx", PreTransformMatrix, nullptr))))
		return E_FAIL;




	
	/* Drone*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CubeDroneSword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneSword.fbx", PreTransformMatrix))))
		return E_FAIL;


	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CubeDroneBullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Drone/CubeDroneBullet.fbx", PreTransformMatrix))))
		return E_FAIL;
	
	/*Mine*/
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Mine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/Mine_0.fbx", PreTransformMatrix))))
		return E_FAIL;
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HommingMissile"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Mine/HommingMissile.fbx", PreTransformMatrix))))
		return E_FAIL;








	/* Interactor_RankBonus_Boss*/
	PreTransformMatrix=XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_A.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_B.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_D.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_C.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Boss_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Boss_S.fbx", PreTransformMatrix))))
		return E_FAIL;



	/*Interactor_RankBonus_Game*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_A.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_B.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_C.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_D.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Game_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Game_S.fbx", PreTransformMatrix))))
		return E_FAIL;


	/*Interactor_RankBonus_MiniBoss*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_A.fbx", PreTransformMatrix))))
		return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_B.fbx", PreTransformMatrix))))
		return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_C.fbx", PreTransformMatrix))))
		return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_D.fbx", PreTransformMatrix))))
		return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_MiniBoss_S"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_MiniBoss_S.fbx", PreTransformMatrix))))
		return E_FAIL;


		/*Interactor_RankBonus_Minigame*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_A"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_A.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_B.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_C"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_C.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_D"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_D.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_RankBonus_Minigame_S"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/RankBonus/Interactor_RankBonus_Minigame_S.fbx", PreTransformMatrix))))
			return E_FAIL;



		/*Item_Box*/
		PreTransformMatrix = XMMatrixIdentity();
		PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_EXP"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_EXP.fbx", PreTransformMatrix))))
			return E_FAIL;
	

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_Gold"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Gold.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_HP"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_HP.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_Random"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Random.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Interactor_Itembox_Shield"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Interactor_Itembox_Shield.fbx", PreTransformMatrix))))
			return E_FAIL;



		/*Items*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_EXP"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP.fbx", PreTransformMatrix))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_EXP_B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_EXP_B.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_HP"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_HP_B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_HP_B.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Life"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Life.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Shield"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Shield_B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Shield_B.fbx", PreTransformMatrix))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Coin"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin.fbx", PreTransformMatrix))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Item_Coin_B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Item/Item_Coin_B.fbx", PreTransformMatrix))))
			return E_FAIL;


		/*Objects_Changer_Icon*/
		PreTransformMatrix = XMMatrixIdentity();
		PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Icon_Gun"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Gun.fbx", PreTransformMatrix))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Icon_Sword"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Icon_Sword.fbx", PreTransformMatrix))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Rot_Gun"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Gun.fbx", PreTransformMatrix))))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Changer_Rot_Sword"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Changer/Changer_Rot_Sword.fbx", PreTransformMatrix))))
			return E_FAIL;


	

		/*Objects_Shell*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Shell"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/Shell/Shell.fbx", PreTransformMatrix))))
			return E_FAIL;


		/*UpgradeMachine*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Foothold_Shop"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/Foothold_Shop.fbx", PreTransformMatrix))))
			return E_FAIL;
		
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("UpgradeMachine"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine.fbx", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("UpgradeMachine2"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/UpgradeMachine/UpgradeMachine2.fbx", PreTransformMatrix))))
			return E_FAIL;


		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("DefaultBox"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/DefaultBox.fbx", PreTransformMatrix))))
			return E_FAIL;


		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Box"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/Box.fbx", PreTransformMatrix))))
			return E_FAIL;	

		PreTransformMatrix = XMMatrixTranslation(0.f, 60.f, 0.f) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ventilator"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/Ventilator.fbx", PreTransformMatrix))))
			return E_FAIL;




		PreTransformMatrix = XMMatrixIdentity();
		PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("rubber_cone"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/PhysxObject/rubber_cone.fbx", PreTransformMatrix))))
			return E_FAIL;

		PreTransformMatrix = XMMatrixIdentity();
		PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NoneTex"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Object/NoneTex.010.fbx", PreTransformMatrix))))
			return E_FAIL;

	






	






	
	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxParticle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../ShaderFiles/Shader_VtxInstance_Mesh.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
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

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AnimObject"),
		CMonster ::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ForkLift */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NoAnimObject"),
		CForkLift::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
		/* For.Prototype_GameObject_Particle_Mesh */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Mesh"),
		CParticle_Mesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;




	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

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
