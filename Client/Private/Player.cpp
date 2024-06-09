#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include"Loader.h"

#include "PartObject.h"
#include "Matilda_Body_Player.h"
#include "Amanda_Body_Player.h"
#include "Weapon.h"
#include"PlayerCamera.h"
#include"../Public/Timer.h"

#include"CBullet.h"
//#include"CSwordThow.h"
#include"CDrone.h"

#include"Particle_Mesh.h"
#include"Monster.h"
#include"CPlayerBullet.h"
#include"CPlayerBullet_Mine.h"
#include"CTotalSingleton.h"
#include"UI_PlayerIcon.h"

#include"UI.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
	
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject{ rhs }
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{

	CGameObject::GAMEOBJECT_DESC	GameObjectDesc{};

	GameObjectDesc.fSpeedPerSec = 7.f;
	GameObjectDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	//	m_pTransformCom->Set_Scale(10.f, 10.f, 10.f);

	if (FAILED(Add_PartObjects()))
		return E_FAIL;



	if (CTotalSingleton::GetInstance()->GetPlayerAbility().bIsFirstData==false)
	{
		m_Ability.fDashPower = 2.f;
		m_Ability.fDashCoolTime = 1.f;
		m_Ability.iDashCount = 3;
		m_Ability.fMoveSpeed = 1.0f;
		m_Ability.fHp = 100.f;
		m_Ability.fMaxHp = 100.f;
		m_Ability.fSkillCoolTime[Player_Abililty::COOL_TIME_SKILL_Z] = 0.5f;
		m_Ability.fSkillCoolTime[Player_Abililty::COOL_TIME_SKILL_X] = 1.0f;
		m_Ability.fSkillCoolTime[Player_Abililty::COOL_TIME_SKILL_C] = 2.0f;
		m_Ability.fSkillCoolTime[Player_Abililty::COOL_TIME_SKILL_X_C] = 3.0f;
		m_Ability.fSkillCoolTime[Player_Abililty::COOL_TIME_SKILL_Z_SPACE] = 7.0f;
		m_Ability.eSwordMasterType = SWORD_BALANCE;
		m_Ability.eGunSlingerType = GUN_BALANCE;

		m_Ability.bIsFirstData = true;
	}
	else
	{
		m_Ability = CTotalSingleton::GetInstance()->GetPlayerAbility();

	}


	/* 플레이어의 Transform이란 녀석은 파츠가 될 바디와 웨폰의 부모 행렬정보를 가지는 컴포넌트가 될거다. */

	return S_OK;
}

void CPlayer::Priority_Tick(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
		pPartObject->Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{
	CMonster* pClosestMonster = nullptr;
	float fClosestDistance = FLT_MAX;
	_vector vPlayerPos = this->Get_PositionVector();

	_uint iLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_2_Monster"));
	CMonster* pMonster = nullptr;
	for (int i = 0; i < iLayerSize; i++)
	{
		pMonster = static_cast<CMonster*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Monster"), i));
		if (pMonster != nullptr)
		{
			_vector vMonsterPos = pMonster->Get_PositionVector();
			float fDistance = XMVectorGetX(XMVector3Length(vMonsterPos - vPlayerPos));
			if (fDistance < fClosestDistance)
			{
				fClosestDistance = fDistance;
				pClosestMonster = pMonster;
			}
		}

	}
	if (pClosestMonster != nullptr)
	{
		_vector vMonsterPos = pClosestMonster->Get_PositionVector();

		m_bIsMonsterHave= true;
		m_vCloseTargetPos = vMonsterPos;
	}






	UI* pUi = dynamic_cast<UI*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Player_Ui"), 0));
	if (pUi)
	{
		pUi->Set_MaxHp(m_Ability.fMaxHp);
		pUi->Set_Hp(m_Ability.fHp);
	}

	cout << m_Ability.fHp << endl;







	CTotalSingleton::GetInstance()->SetPlayerAbility(m_Ability);
	m_eSwordMasterType = m_Ability.eSwordMasterType;
	m_eGunSlingerType = m_Ability.eGunSlingerType;

	_vector vPrePosition = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	//cout << vPrePosition.m128_f32[0] << " " << vPrePosition.m128_f32[1] << " " << vPrePosition.m128_f32[2] << endl;

	_uint iPlayerIconLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_Player_Ui"));

	UI_PlayerIcon* pPlayerIcon = nullptr;
	pPlayerIcon = dynamic_cast<UI_PlayerIcon*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Player_Ui"), 1));

		_float fMaskOnPlayerPos = 0.0;
		fMaskOnPlayerPos =m_pGameInstance->PickMaskMap(this->Get_Position());

		CHRACTER_TYPE ePreCharacterType = m_eCharacterType;
		if (fMaskOnPlayerPos == 0.0f)
		{
			pPlayerIcon->Set_TexIndex(0);

			m_eCharacterType = CHRACTER_TYPE::CHRACTER_SWORD_MASTER;
			

		}
		else if (fMaskOnPlayerPos == 1.0f)
		{
			pPlayerIcon->Set_TexIndex(1);
			m_eCharacterType = CHRACTER_TYPE::CHRACTER_GUN_SLINGER;
			
		}
	
		if (ePreCharacterType != m_eCharacterType)
		{
			MakeChangeparticle(fTimeDelta);
		}
		
		

	
	Move(fTimeDelta);	

	if (m_eCurState == STATE_ATTACK_DASH_SLIDING_SHOT)
	{
		m_pTransformCom->Go_Straight(fTimeDelta * 3.0f,m_pNavigationCom);
	}

	if (m_eCurState == STATE_DASH)
	{
		m_pTransformCom->Go_Straight(fTimeDelta * m_Ability.fDashPower, m_pNavigationCom);
	}


	//Test

	if (KEY_TAP(DIK_Z))
	{
		
		SetStatePressZ(fTimeDelta);	
	}


	if (KEY_TAP(DIK_X) && KEY_TAP(DIK_C))
	{
		SetStatePress_X_C(fTimeDelta);
	}

	else if (KEY_TAP(DIK_X))
	{
		
		SetStatePressX(fTimeDelta);
	}
	else if (KEY_TAP(DIK_C))
	{
		SetStatePressC(fTimeDelta);
	}
	 
	JumpingState(fTimeDelta);

	

	//이전 위치랑 같다면 무조건 스탑 모션이 나오고 있음 //이거는 나중에 더 처리 해야함
	_bool IsSamePrePos = XMVector3Equal(vPrePosition, m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION));
	if (IsSamePrePos &&
		m_eCurState != STATE_DEFAULT_IDLE &&
		m_eCurState != STATE_HIT &&
		m_eCurState != STATE_COMBO_ATTACK_LEFT &&
		m_eCurState != STATE_COMBO_ATTACK_RIGHT &&
		m_eCurState != STATE_ATTACK_DEFAULT_END &&
		m_eCurState != STATE_BACKSTEP &&
		m_eCurState	!= STATE_THROW_WEAPON &&
		m_eCurState	!=STATE_BEFORE_ATTACK_MOTION &&
		m_eCurState	!= STATE_AFTER_ATTACK_IDLE &&
		m_eCurState !=STATE_DASH &&
		m_eCurState !=STATE_MATILD_D_SHORT&& 
		m_eCurState	!= STATE_ATTACK_STINGER &&
		m_eCurState	!= STATE_ATTACK_SPIN &&
		m_eCurState	!= STATE_ATTACK_SPIN_END &&
		m_eCurState	!= STATE_ATTACK_STOMP &&
		m_eCurState	!= STATE_ATTACK_THORW &&
		m_eCurState	!=STATE_JUMP_END &&
		m_eCurState	!=STATE_ATTACK_ULTIMATE_BALANCE &&
		m_eCurState	!=STATE_ATTACK_ULTIMATE_TECHNIC &&
		m_eCurState	!=STATE_ATTACK_ULTIMATE_POWER &&
		m_eCurState	!=STATE_ATTACK_ULTIMATE_REVERSE &&
		m_eCurState != STATE_ATTACK_SHOT &&
		m_eCurState != STATE_ATTACK_SHOT_CRITICAL &&
		m_eCurState	!= STATE_ATTACK_SHOT_DASH &&
		m_eCurState	!= STATE_ATTACK_DASH_BUCK&&
		m_eCurState	!= STATE_ATTACK_DASH_BUCK_NEXT &&
		m_eCurState != STATE_ATTACK_DASH_SLIDING_SHOT &&
		m_eCurState != STATE_ATTACK_SLIDING &&
		m_eCurState != STATE_ATTACK_SHOT_CROUCH &&
		m_eCurState != STATE_ATTACK_SHOT_UPPER &&
		m_eCurState != STATE_ATTACK_GHOST_CROUCH)
	{

		
		m_eCurState = STATE_MOVE_AFTER_STOP;
	}

	
	


	for (auto& pPartObject : m_PartObjects)
	{
		if (typeid(*pPartObject) == typeid(Matilda_Body_Player))
		{
			dynamic_cast<Matilda_Body_Player*>(pPartObject)->SetAnimState(m_eCurState);
			dynamic_cast<Matilda_Body_Player*>(pPartObject)->SetAttackType(m_eSwordMasterType);
			dynamic_cast<Matilda_Body_Player*>(pPartObject)->SetCharacterType(m_eCharacterType);

		}
		if (typeid(*pPartObject) == typeid(Amanda_Body_Player))
		{
			//continue;
			
				dynamic_cast<Amanda_Body_Player*>(pPartObject)->SetAnimState(m_eCurState);
				dynamic_cast<Amanda_Body_Player*>(pPartObject)->SetAttackType(m_eSwordMasterType);
				dynamic_cast<Amanda_Body_Player*>(pPartObject)->SetCharacterType(m_eCharacterType);
		}
		////test
		//if (typeid(*pPartObject) == typeid(Matilda_Body_Player))
		//{
		//	continue;
		//}
		pPartObject->Tick(fTimeDelta);
	}


	if (m_bIsMove_After_Skill)
	{
		DoMove_After_Skill();
	}

	if (m_bIsMonsterDetact)
	{
		vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};

		
		m_fQuestTime += fTimeDelta;
		static _uint iNextIndex= 0;
		_uint iLayerSize= m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_2_Monster"));
		CMonster* pMonster = nullptr;
		if (m_fQuestTime > 0.5f)
		{
			pMonster = static_cast<CMonster*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Monster"), iNextIndex));
			
		}
		if (pMonster != nullptr)
		{
			_vector vMonsterPos = pMonster->Get_PositionVector();
			_float4 fMonsetPos;
			XMStoreFloat4(&fMonsetPos, vMonsterPos);
			_vector vPlayerPos = XMVectorSet(fMonsetPos.x, 0.0f, fMonsetPos.z, 1.0f);

		
			// 지정된 위치에 없는 경우에만 플레이어 위치 변경
			if (fMonsetPos.x >= -24.0f && fMonsetPos.x <= 24.0f && fMonsetPos.z >= 0.0f && fMonsetPos.z <= 48.0f)
			{
				m_pTransformCom->Go_Teleport(vMonsterPos, m_pNavigationCom);
			}

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z,TEXT("Player_Power_XC_Matilda_Trace_Size_Up_Z"),_float4(0.1f,0.2f,1.0f,0.3f)});
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X,TEXT("Player_Power_XC_newSlash.001_Size_UP_X"),_float4(0.1f,0.2f,1.0f,0.3f)});
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Power_XC_stingerRing.001_Pop"),_float4(0.1f,0.2f,1.0f,0.3f)});

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
			m_bIsAttacking = true;
			m_bIsCollision = true;
			m_fQuestTime = 0.0f;
			iNextIndex++;
		}
		if (iNextIndex == iLayerSize)
		{
			m_bIsAttacking = false;
			m_bIsCollision = false;
			iNextIndex = 0;
			m_bIsMonsterDetact = false;
			m_fQuestTime = 0.0f;
		}
	




	}
	
}

void CPlayer::Late_Tick(_float fTimeDelta)
{

	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject->Late_Tick(fTimeDelta);

	}

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
	
	CCollider* pPartObjCollider = dynamic_cast<CCollider*>(m_PartObjects[PART_BODY_MATILDA]->Get_Component(TEXT("Com_Collider")));


	_uint iLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_Bullet"));

	for (_uint i = 0; i < iLayerSize; ++i)
	{
		CBullet* pBullet = dynamic_cast<CBullet*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), i));
		if (pBullet != nullptr)
		{
			if (!pBullet->Get_IsCollision() && pBullet->Intersect(TEXT("Com_Collider"), pPartObjCollider))
			{
				_float fDamage = pBullet->Get_Damage();
				m_Ability.fHp -= fDamage;
			
				pBullet->Set_IsCollision(true);
			}
		}
	}
	

	
}

HRESULT CPlayer::Render()
{

#ifdef _DEBUG
	m_pNavigationCom->Render();
#endif
	//m_pGameInstance->Render_Font(TEXT("Font_Default"), TEXT("담배 피러 가자"), _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

	return S_OK;
}

HRESULT CPlayer::Add_Components()
{


	CNavigation::NAVIGATION_DESC	NavigationDesc{};

	NavigationDesc.iCurrentCellIndex = 0;

	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Add_PartObjects()
{
	_vector vTargetPos = XMVectorSet(0.0f, 10.0f, -10.0f, 0.0f);
	_float fAngle = 40.f;
	if (CLoader::m_eNextLevel == LEVEL_STAGE_1)
	{
		vTargetPos= XMVectorSet(0.0f, 15.0f, -10.0f, 0.0f); 
		fAngle=60.f;
	}
	if (CLoader::m_eNextLevel == LEVEL_STAGE_3)
	{
		vTargetPos = XMVectorSet(0.0f, 20.0f, -3.0f, 0.0f);
		fAngle = 70.f;
	}

	
	/*카메라 복제*/
	//Prototype_GameObject_FreeCamera

	CPlayerCamera::Player_CAMERA_DESC		CameraDesc{};
	CameraDesc.fSensor = 0.1f;
	CameraDesc.vEye = _float4(0.0f, 10.f, -15.f, 1.f);
	CameraDesc.vAt = _float4(0.0f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 3000.f;
	CameraDesc.fSpeedPerSec = 20.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.f);
	CameraDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	CameraDesc.fAngle = fAngle;
	CameraDesc.vTargetPos = vTargetPos;
	CGameObject* pCamera = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_PlayerCamera"),&CameraDesc);
	if (nullptr == pCamera)
		return E_FAIL;

	m_PartObjects.push_back(pCamera);








	/* 바디객체를 복제해온다. */
	Matilda_Body_Player::Matilda_BODY_DESC			MatildaBodyDesc{};
	//CPartObject::PARTOBJ_DESC		BodyDesc{};
	MatildaBodyDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	MatildaBodyDesc.fSpeedPerSec = 0.f;
	MatildaBodyDesc.fRotationPerSec = 0.f;
	MatildaBodyDesc.m_pPlayer = this;
	MatildaBodyDesc.strModelName = TEXT("Prototype_Component_Model_newMatilda");
	//m_pAnimFsm->SetAnimations;
	
	
	//dynamic_cast<Matilda_Body_Player*>
	Matilda_Body_Player* pMatildaBody = dynamic_cast<Matilda_Body_Player*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Matilda_Body_Player"), &MatildaBodyDesc));
	if (nullptr == pMatildaBody)
		return E_FAIL;
	m_PartObjects.emplace_back(pMatildaBody);
	
	//
	Amanda_Body_Player::Amanda_BODY_DESC			AmandaBodyDesc{};
	AmandaBodyDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	AmandaBodyDesc.fSpeedPerSec = 0.f;
	AmandaBodyDesc.fRotationPerSec = 0.f;
	AmandaBodyDesc.m_pPlayer = this;
	AmandaBodyDesc.strModelName = TEXT("Prototype_Component_Model_newAmanda");
	Amanda_Body_Player* pAmandaBody = dynamic_cast<Amanda_Body_Player*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Amanda_Body_Player"), &AmandaBodyDesc));
	//CGameObject* pAmandaBody = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Amanda_Body_Player"), &AmandaBodyDesc);
	if (nullptr == pAmandaBody)
		return E_FAIL;
	m_PartObjects.emplace_back(pAmandaBody);
	



	/* 무기객체를 복제해온다. */
	CWeapon::WEAPON_DESC			WeaponDesc{};
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	WeaponDesc.strModelName = TEXT("Prototype_Component_Model_ForkLift");
	WeaponDesc.vPosition = _float4(0.f, 0.f, 0.f,1.0f);
	WeaponDesc.vScale = _float3(0.1f, 0.1f, 0.1f);
	WeaponDesc.vRotation = _float3(0.0f, 1.f, 0.0f);
	WeaponDesc.vRotationAngle = 90.f;
	WeaponDesc.pPlayer = this;
	CModel* pModelCom = dynamic_cast<CModel*>(pMatildaBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;


	//특정 뼈에 붙히는 작업
	//
	WeaponDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("Bone Weapon003");
	if (nullptr == WeaponDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	
	CGameObject*		pWeapon = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == pWeapon)
		return E_FAIL;
	m_PartObjects.emplace_back(pWeapon);
	dynamic_cast<Matilda_Body_Player*>(m_PartObjects[PART_BODY_MATILDA])->SetWeapon(dynamic_cast<CWeapon*>(m_PartObjects[PART_WEAPON]));
	dynamic_cast<Amanda_Body_Player*>(m_PartObjects[PART_BODY_AMANDA])->SetWeapon(dynamic_cast<CWeapon*>(m_PartObjects[PART_WEAPON]));

	return S_OK;
}

_bool CPlayer::Intersect(PART ePartObjID, const wstring& strComponetTag, CCollider* pTargetCollider)
{
	CCollider* pPartObjCollider = dynamic_cast<CCollider*>(m_PartObjects[ePartObjID]->Get_Component(strComponetTag));
	if (nullptr == pPartObjCollider)
		return false;
	
	if (pTargetCollider->Intersect(pPartObjCollider))
	{
		
		return true;
	}
	else
	{
		return false;
	}
	 
}

void CPlayer::Set_CameraShake(_bool bIsCutScene, _float fShakeTime)
{


	dynamic_cast<CPlayerCamera*>(m_PartObjects[PART_CAMERA])->Set_Shake(bIsCutScene, fShakeTime);



}

void CPlayer::Set_CameraTargetPos(_vector vTargetPos)
{
	dynamic_cast<CPlayerCamera*>(m_PartObjects[PART_CAMERA])->Set_TargetPos(vTargetPos);

}

void CPlayer::Move(_float fTimeDelta)
{
	_vector vUp = m_pTransformCom->Get_State(CTransform::STATE::STATE_UP);
	
	
	if (m_eCurState != STATE_COMBO_ATTACK_LEFT &&
		m_eCurState!= STATE_COMBO_ATTACK_RIGHT &&
		m_eCurState!= STATE_ATTACK_SHOT &&
		m_eCurState!= STATE_ATTACK_SHOT_CRITICAL)
	{
		if (CTotalSingleton::GetInstance()->GetUiOpen() == false)
		{

			if (m_eCurState != STATE_DASH)
			{

				if (KEY_HOLD(DIK_UP) && KEY_HOLD(DIK_LEFT))
				{
					CParticle_Mesh::PARTICLE_DESC desc;
					desc.vStartPos = _float4(0.f, 0.f, 0.f, 1.0f);

					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(-45.f));
					m_eCurState = STATE_RUN;
				}
				else if (KEY_HOLD(DIK_UP) && KEY_HOLD(DIK_RIGHT))
				{
					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(45.f));
					m_eCurState = STATE_RUN;
				}
				else if (KEY_HOLD(DIK_DOWN) && KEY_HOLD(DIK_LEFT))
				{
					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(-135.f));
					m_eCurState = STATE_RUN;
				}
				else if (KEY_HOLD(DIK_DOWN) && KEY_HOLD(DIK_RIGHT))
				{
					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(135.f));
					m_eCurState = STATE_RUN;
				}


				else if (KEY_HOLD(DIK_UP))
				{

					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(0.f));
					m_eCurState = STATE_RUN;
				}

				else if (KEY_HOLD(DIK_DOWN))
				{
					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));
					m_eCurState = STATE_RUN;
				}


				else if (KEY_HOLD(DIK_LEFT))
				{
					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(-90.f));
					m_eCurState = STATE_RUN;
				}

				else if (KEY_HOLD(DIK_RIGHT))
				{
					m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
					m_pTransformCom->Rotation(vUp, XMConvertToRadians(90.f));
					m_eCurState = STATE_RUN;
				}
			}
			if (KEY_TAP(DIK_SPACE))
			{

				m_eCurState = STATE_DASH;



			}


		}

	}

}

void CPlayer::SetStatePressZ(_float fTimeDelta)
{
	switch (m_eCharacterType)
	{
	case Client::CPlayer::CHRACTER_SWORD_MASTER:
		if (m_Ability.iComboCount == 0)
		{
			if (m_bNextAttack)
			{
				if (m_bIsMonsterHave)
				{
					_vector vCloseTargetPos2D = XMVectorSet(XMVectorGetX(m_vCloseTargetPos), 0.0f, XMVectorGetZ(m_vCloseTargetPos), 1.0f);
					m_pTransformCom->LookAt(vCloseTargetPos2D);
				}
				//m_pTransformCom->Set_Look(m_vCloseTargetDir);
				
				m_eCurState = STATE_COMBO_ATTACK_LEFT;
				m_Ability.iComboCount++;
			}
		}	

		else if (m_Ability.iComboCount == 1)
		{
			if (m_bNextAttack)
			{
				if (m_bIsMonsterHave)
				{
					_vector vCloseTargetPos2D = XMVectorSet(XMVectorGetX(m_vCloseTargetPos), 0.0f, XMVectorGetZ(m_vCloseTargetPos), 1.0f);
					m_pTransformCom->LookAt(vCloseTargetPos2D);
				}
				m_eCurState = STATE_COMBO_ATTACK_RIGHT;
				m_Ability.iComboCount = 0;
			}
		}
	


		break;
	case Client::CPlayer::CHRACTER_GUN_SLINGER:

		if (m_Ability.iComboCount == 0)
		{
			
			if (m_bIsFire)
			{
			
				m_bIsFire = false;
			}
			if (m_bNextAttack)
			{
				if (m_bIsMonsterHave)
				{
					_vector vCloseTargetPos2D = XMVectorSet(XMVectorGetX(m_vCloseTargetPos), 0.0f, XMVectorGetZ(m_vCloseTargetPos), 1.0f);
					m_pTransformCom->LookAt(vCloseTargetPos2D);
				}
					m_eCurState = STATE_ATTACK_SHOT;
					m_Ability.iComboCount++;
				
			}
		}
		else if (m_Ability.iComboCount == 1)
		{
		
				if (m_bNextAttack)
				{
					
					if (m_bIsMonsterHave)
					{
						_vector vCloseTargetPos2D = XMVectorSet(XMVectorGetX(m_vCloseTargetPos), 0.0f, XMVectorGetZ(m_vCloseTargetPos), 1.0f);
						m_pTransformCom->LookAt(vCloseTargetPos2D);
					}
					m_eCurState = STATE_ATTACK_SHOT_CRITICAL;
					m_Ability.iComboCount = 0;
				}
		}

		
		break;
	case Client::CPlayer::CHRACTER_END:
		break;
	default:
		break;
	}


}

void CPlayer::SetStatePressX(_float fTimeDelta)
{
	CPlayerBullet::CPlayetBULLET_DESC CSwordThowDesc{};
	CPlayerBullet_Mine::CPlayetBULLET_MINE_DESC CMineDesc{};
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = this->GetLookDir();
	_vector vPlayerUp=m_pTransformCom->Get_State(CTransform::STATE::STATE_UP);
	_float4 vPlayerPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);
	const int numBullets = 6; // 생성할 총알의 개수
	const float fanAngle = 60.0f; // 부채꼴의 전체 각도
	const float halfFanAngle = fanAngle / 2.0f; // 부채꼴의 반각
	const float angleStep = fanAngle / (numBullets - 1); // 각 총알 간의 각도 간격

	if (m_eCurState != STATE_RUN)
	{
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			switch (m_eSwordMasterType)
			{
			case Client::CPlayer::SWORD_BALANCE:
				m_bIsJump = true;
				m_eCurState = STATE_BACKSTEP;
				break;
			case Client::CPlayer::SWORD_TECHNNIC:
				
				CSwordThowDesc.fRadius = 0.5f;
				CSwordThowDesc.strModelName = TEXT("swordThrowing");
				CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);
				CSwordThowDesc.vDir = this->GetLookDir();
				CSwordThowDesc.BulletState = &CBullet::ComeBack;
				CSwordThowDesc.fSpeedPerSec = 55.f;
				CSwordThowDesc.bParticleCreate = true;
				CSwordThowDesc.fLifeTime = 1.f;
				CSwordThowDesc.fDamage = 10.f;
				CSwordThowDesc.eParticleType = CPlayerBullet::CPLAYER_BULLET_PARTICLE_TYPE::TECHNIC_X;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);
				//총알 생성	
				m_bIsJump = true;
				m_eCurState = STATE_BACKSTEP;
				break;
			case Client::CPlayer::SWORD_POWER:
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Power_X_Matilda_Trace_Jump_Spread"),_float4(0.2f,0.2f,1.0f,0.5f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Power_X_LowpolySphere16_Size_Up"),_float4(0.1f,0.1f,0.8f,0.3f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Y,TEXT("Player_Power_X_LowpolyCylinder6_Size_Down_Y"),_float4(0.9f,0.9f,0.9f,0.5f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Power_X_hitRing_Size_Up"),_float4(0.3f,0.3f,0.9f,0.7f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"),_float4(0.8f,0.8f,0.8f,0.5f) });
				for (int i = 0; i < 10; ++i)
				{
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z_NONE_DISOLVE,TEXT("Player_Power_X_slashLine_Size_Down_Z"),_float4(0.8f,0.8f,0.8f,0.5f),true });
				}

				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -vPlayerLook);
				CSwordThowDesc.fRadius = 5.0f;
				CSwordThowDesc.strModelName = TEXT("swordThrowing");
				CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);
				CSwordThowDesc.vDir = this->GetLookDir();
				CSwordThowDesc.BulletState = &CBullet::Pop;
				CSwordThowDesc.fLifeTime = 1.f;
				CSwordThowDesc.fDamage = 10.f;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);

				m_eCurState = STATE_MATILD_D_SHORT;
				m_bIsJump = false;
				break;
			case Client::CPlayer::SWORD_REVERSE:

				CSwordThowDesc.fRadius = 3.5f;
				CSwordThowDesc.strModelName = TEXT("swordThrowing");
				CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y+1.f, this->Get_Position().z, 1.0f);
				CSwordThowDesc.vDir = this->GetLookDir();
				CSwordThowDesc.BulletState = &CBullet::Draven_Bullet;
				CSwordThowDesc.fSpeedPerSec = 5.f;
				CSwordThowDesc.fLifeTime = 10.f;
				CSwordThowDesc.bParticleCreate = true;
				CSwordThowDesc.pMatPlayerWorld = m_pTransformCom->Get_WorldFloat4x4();
				CSwordThowDesc.eParticleType = CPlayerBullet::CPLAYER_BULLET_PARTICLE_TYPE::REVERSE_X;
				CSwordThowDesc.fDamage = 10.f;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);


				m_bIsJump = true;
				m_eCurState = STATE_BACKSTEP;
				break;
			case Client::CPlayer::SWORD_END:
				break;
			default:
				break;
			}
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			switch (m_eGunSlingerType)
			{
			case Client::CPlayer::GUN_BALANCE:
				
				for (int i = 0; i < 6; i++)
				{
					float angle = -halfFanAngle + i * angleStep; // 각 총알의 회전 각도
					_matrix rotationMatrix = XMMatrixRotationY(XMConvertToRadians(angle)); // Y축을 기준으로 회전 행렬 생성
					_vector rotatedDir = XMVector3TransformNormal(vPlayerLook, rotationMatrix);
					
					CSwordThowDesc.fRadius = 3.5f;
					CSwordThowDesc.strModelName = TEXT("swordThrowing");
					_float4 vPos = _float4(this->Get_Position().x, this->Get_Position().y + 1.f, this->Get_Position().z, 1.0f);
					vPos.x+= XMVectorGetX(rotatedDir);
					vPos.y+= 1.0f;
					vPos.z+= XMVectorGetZ(rotatedDir);
					CSwordThowDesc.vPosition= vPos;
					//CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y + 1.f, this->Get_Position().z, 1.0f);
					CSwordThowDesc.vDir = rotatedDir;
					CSwordThowDesc.BulletState = &CBullet::Spread;
					CSwordThowDesc.fSpeedPerSec = 5.f;
					CSwordThowDesc.fLifeTime = 10.f;
					CSwordThowDesc.bParticleCreate = true;
					CSwordThowDesc.pMatPlayerWorld = m_pTransformCom->Get_WorldFloat4x4();
					CSwordThowDesc.eParticleType = CPlayerBullet::CPLAYER_BULLET_PARTICLE_TYPE::BALANCE_X;
					CSwordThowDesc.eCharacterType = m_eCharacterType;
					CSwordThowDesc.fInitSpeed = 5.f+i+1;
					CSwordThowDesc.fDamage = 20.f;
					m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);

				}


				m_bIsJump = true;
				m_eCurState = STATE_BACKSTEP;
				break;
			case Client::CPlayer::GUN_TECHNNIC:
				for (int i = 0; i < 6; i++)
				{
					float angle = -halfFanAngle + i * angleStep; // 각 총알의 회전 각도
					_matrix rotationMatrix = XMMatrixRotationY(XMConvertToRadians(angle)); // Y축을 기준으로 회전 행렬 생성
					_vector rotatedDir = XMVector3TransformNormal(vPlayerLook, rotationMatrix);

					CMineDesc.fRadius = 3.5f;
					CMineDesc.strModelName = TEXT("PlayerMine");
					_float4 vPos = _float4(this->Get_Position().x+i*3.f, this->Get_Position().y + 1.f, this->Get_Position().z, 1.0f);
					vPos.x += XMVectorGetX(rotatedDir);
					vPos.y += 1.0f;
					vPos.z += XMVectorGetZ(rotatedDir);
					CMineDesc.vPosition = vPos;
					//CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y + 1.f, this->Get_Position().z, 1.0f);
					CMineDesc.vDir = rotatedDir;
					CMineDesc.BulletState = &CBullet::Spread;
					CMineDesc.fSpeedPerSec = 5.f;
					CMineDesc.fLifeTime = 10.f;
					CMineDesc.pMatPlayerWorld = m_pTransformCom->Get_WorldFloat4x4();
					CMineDesc.eCharacterType = m_eCharacterType;
					CMineDesc.fDamage= 20.f;
					m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet_Mine"), &CMineDesc);

				}


				m_bIsJump = true;
				m_eCurState = STATE_BACKSTEP;
				break;
			case Client::CPlayer::GUN_POWER:


				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_2"),_float4(0.2f,0.2f,0.2f,0.7f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD_NONE_DISOLVE,TEXT("Player_Amanda_Power_X_LowpolySphere16_Spread_1"),_float4(0.9f,0.9f,0.9f,0.2f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Power_X_ExplosionFrag.001_Up_And_Down"),_float4(0.1f,0.1f,0.0f,0.4f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Y,TEXT("Player_Amanda_Power_X_hit_Size_Down_Y"),_float4(0.9f,0.1f,0.0f,0.4f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z,TEXT("Player_Amanda_Power_X_ExplosionSpark001_Szie_Down_Z"),_float4(0.6f,0.1f,0.0f,0.4f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X,TEXT("Player_Amanda_Power_X_atomStar0022_Size_Up_X"),_float4(1.0f,0.1f,0.0f,0.4f) });
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -vPlayerLook);





				for (int i = 0; i < 3; i++)
				{
					CMonster* pMonster = nullptr;
					pMonster = static_cast<CMonster*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Monster"), i));



					_float fAngle = XM_2PI * (_float)i / 3.0f;
					_vector vOffset = XMVectorSet(cosf(fAngle) * 5.0f, 1.f, sinf(fAngle) * 5.0f, 0.f);

					_vector vDronePos = XMLoadFloat4(&vPlayerPosition) + vOffset;
					_float4 vPosition = {};
					XMStoreFloat4(&vPosition, vDronePos);

					_vector vLook = XMVector3Normalize(XMLoadFloat4(&vPlayerPosition) - vDronePos);


					CMineDesc.fRadius = 3.5f;
					CMineDesc.strModelName = TEXT("PlayerHommingMissile");
					CMineDesc.vPosition = vPosition;
					//CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y + 1.f, this->Get_Position().z, 1.0f);
					CMineDesc.vDir = -vLook;
					CMineDesc.BulletState = &CBullet::Go_TargetPos;
					CMineDesc.fSpeedPerSec = 10.f;
					CMineDesc.fLifeTime = 10.f;
					CMineDesc.bParticleCreate = true;
					CMineDesc.eCharacterType = m_eCharacterType;
					CMineDesc.vTargetPos =  pMonster==nullptr?XMVectorSet(0.f,0.f,0.f,1.f):pMonster->Get_PositionVector();
					CMineDesc.eParticleType= CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE::POWER_X;
					CMineDesc.fDamage = 20.f;
					m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet_Mine"), &CMineDesc);


				}


				m_bIsJump = true;
				m_eCurState = STATE_BACKSTEP;
				break;
			case Client::CPlayer::GUN_REVERSE:
				m_bIsJump = true;
				m_eCurState = STATE_BACKSTEP;
				break;
			case Client::CPlayer::GUN_END:
				break;
			default:
				break;
			}
			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			break;
		}

		


		

	}



	
}

void CPlayer::SetStatePressC(_float fTimeDelta)
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	CPlayerBullet::CPlayetBULLET_DESC CSwordThowDesc{};
	_vector vPlayerLook = this->GetLookDir();
	if (m_eCurState != STATE_RUN)
	{
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			switch (m_eSwordMasterType)
			{
			case Client::CPlayer::SWORD_BALANCE:
				m_eCurState = STATE_ATTACK_STINGER;
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"),_float4(0.8f,0.8f,0.8f,0.5f) });
				
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -vPlayerLook);
				break;
			case Client::CPlayer::SWORD_TECHNNIC:
				m_eCurState = STATE_ATTACK_SPIN;
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_C_Balalnce_Matilda_Trace_Attack_Pop"),_float4(0.8f,0.8f,0.8f,0.5f) });

				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -vPlayerLook);
				break;
			case Client::CPlayer::SWORD_POWER:
				if (m_bIsMonsterHave)
				{
					_vector vCloseTargetPos2D = XMVectorSet(XMVectorGetX(m_vCloseTargetPos), 0.0f, XMVectorGetZ(m_vCloseTargetPos), 1.0f);
					m_pTransformCom->LookAt(vCloseTargetPos2D);
				}
				m_eCurState = STATE_ATTACK_STOMP;
				m_bIsJump = true;
				break;
			case Client::CPlayer::SWORD_REVERSE:

				m_eCurState = STATE_ATTACK_THORW;
				break;
			case Client::CPlayer::SWORD_END:
				break;
			default:
				break;
			}
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			switch (m_eGunSlingerType)
			{
			case Client::CPlayer::GUN_BALANCE:
				m_eCurState = STATE_ATTACK_SHOT_DASH;
				break;
			case Client::CPlayer::GUN_TECHNNIC:
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD_NONE_DISOLVE,TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"),_float4(0.1f,0.0f,0.0f,0.3f)});
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD_NONE_DISOLVE,TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"),_float4(0.1f,0.0f,0.0f,0.3f)});
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"),_float4(1.0f,0.9f,0.9f,0.7f)});
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"),_float4(1.0f,0.9f,0.9f,0.7f)});
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"),_float4(1.0f,1.0f,0.0f,0.7f)});
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"),_float4(0.9f,0.1f,0.0f,0.7f)});
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"),_float4(1.0f,0.5f,0.0f,0.7f)});
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z,TEXT("Player_Amanda_Technic_C_WireBarrier_Rotate_Z"),_float4(0.1f,0.0f,0.0f,0.3f),false ,true,*this });
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -vPlayerLook);


				CSwordThowDesc.fRadius = 3.0f;
				CSwordThowDesc.strModelName = TEXT("swordThrowing");
				CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);
				CSwordThowDesc.BulletState = &CBullet::Pop;
				CSwordThowDesc.fLifeTime = 1.f;
				CSwordThowDesc.fDamage = 10.f;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);
				m_eCurState = STATE_ATTACK_DASH_BUCK;
				m_bIsJump = true;
				break;
			case Client::CPlayer::GUN_POWER:
				m_eCurState = STATE_ATTACK_DASH_SLIDING_SHOT;
				break;
			case Client::CPlayer::GUN_REVERSE:
				m_eCurState = STATE_DASH;
				break;
			case Client::CPlayer::GUN_END:
				break;
			default:
				break;
			}
			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			break;
		}
		


		

	}


}

void CPlayer::SetStatePress_X_C(_float fTimeDelta)
{
	CDrone::CDrone_DESC DroneDesc{};



	if (m_eCurState != STATE_RUN)
	{
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			switch (m_eSwordMasterType)
			{
			case Client::CPlayer::SWORD_BALANCE:
				m_eCurState = STATE_ATTACK_ULTIMATE_BALANCE;
				break;
			case Client::CPlayer::SWORD_TECHNNIC:
				MakeDrone(3, 5.0f, TEXT("CubeDroneSword"));
				m_eCurState = STATE_ATTACK_ULTIMATE_TECHNIC;
				break;
			case Client::CPlayer::SWORD_POWER:
				m_bIsMonsterDetact = true;
				m_eCurState = STATE_ATTACK_ULTIMATE_POWER;
				break;
			case Client::CPlayer::SWORD_REVERSE:
				m_eCurState = STATE_ATTACK_ULTIMATE_REVERSE;
				break;
			case Client::CPlayer::SWORD_END:
				break;
			default:
				break;
			}
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			switch (m_eGunSlingerType)
			{
			case Client::CPlayer::GUN_BALANCE:
				if (m_bIsMonsterHave)
				{
					_vector vCloseTargetPos2D = XMVectorSet(XMVectorGetX(m_vCloseTargetPos), 0.0f, XMVectorGetZ(m_vCloseTargetPos), 1.0f);
					m_pTransformCom->LookAt(vCloseTargetPos2D);
				}
				m_eCurState = STATE_ATTACK_ULTIMATE_BALANCE;
				break;
			case Client::CPlayer::GUN_TECHNNIC:
				MakeDrone(3, 5.0f, TEXT("CubeDroneBullet"));

				m_eCurState = STATE_ATTACK_ULTIMATE_TECHNIC;
				break;
			case Client::CPlayer::GUN_POWER:
				m_eCurState = STATE_ATTACK_ULTIMATE_POWER;
				break;
			case Client::CPlayer::GUN_REVERSE:
				m_eCurState = STATE_ATTACK_ULTIMATE_REVERSE;
				break;
			case Client::CPlayer::GUN_END:
				break;
			default:
				break;
			}
			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			break;
		}
		


		

	}

}

void CPlayer::JumpingState(_float fTimeDelta)
{
	if (m_bIsJump)
	{
		_bool Isfloor = false;
		_float fJumpHeight = 1.f;
		_float fbackDashPower = m_eSwordMasterType == CPlayer::SWORD_MASTER_TYPE::SWORD_REVERSE ? 1.0f : 2.0f;

		switch (m_eCurState)
		{
		case Client::CPlayer::STATE_BACKSTEP:
			m_pTransformCom->Go_Backward(fTimeDelta * fbackDashPower, m_pNavigationCom);
			break;
		case Client::CPlayer::STATE_ATTACK_STOMP:
			m_pTransformCom->Go_Straight(fTimeDelta * fbackDashPower, m_pNavigationCom);
			dynamic_cast<CWeapon*>(m_PartObjects[PART_WEAPON])->SetBoxColliderSize(_float3(15.0f, 10.0f, 15.0f));
			break;
		case Client::CPlayer::STATE_ATTACK_DASH_BUCK:
			m_pTransformCom->Go_Straight(fTimeDelta * fbackDashPower, m_pNavigationCom);
			fJumpHeight = 2.f;
			break;
		case Client::CPlayer::STATE_END:
			break;
		default:
			break;
		}
		m_pTransformCom->Go_Jump(fTimeDelta, 5.f, fJumpHeight, Isfloor);

		if (Isfloor)
		{
			dynamic_cast<CWeapon*>(m_PartObjects[PART_WEAPON])->SetBoxColliderSize(_float3(1.5f, 5.f, 2.5f));
			m_bIsJump = false;
		}
	}


}

void CPlayer::DoMove_After_Skill()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));
	_vector vAfterMovePos = {0.0f,0.0f,0.0f,1.0f};
	CModel* pModelCom = dynamic_cast<CModel*>(m_PartObjects[PART_BODY_AMANDA]->Get_Component(TEXT("Com_Model")));
	CBullet::CBullet_DESC CSwordThowDesc{};
	
	//_vector vAfterMovePos = {};

 	switch (m_eCurState)
	{

	case Client::CPlayer::STATE_COMBO_ATTACK_LEFT:
		m_fMovePower = 1.5f;
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_COMBO_ATTACK_RIGHT:
		m_fMovePower = 1.5f;
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;

	case Client::CPlayer::STATE_DASH:
		//vPosition = vPosition + vLook * m_fMovePower;
		//m_bIsMove_After_Skill = false;
		break;

	case Client::CPlayer::STATE_THROW_WEAPON:
		
		m_fMovePower = 3.0f;
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;

	case Client::CPlayer::STATE_MATILD_D_SHORT:
		vPosition = vPosition - vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_STINGER:
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_SPIN:
		m_fMovePower = 2.0f;
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_STOMP:
		m_fMovePower = 2.0f;
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_THORW:
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;

	case Client::CPlayer::STATE_ATTACK_ULTIMATE_BALANCE:
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_ULTIMATE_TECHNIC:
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_ULTIMATE_POWER:
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_ULTIMATE_REVERSE:
		vPosition = vPosition + vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_SHOT:
		m_fMovePower = 0.5f;
		vPosition = vPosition - vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_SHOT_CRITICAL:
		vPosition = vPosition - vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_SHOT_DASH:
		m_fMovePower = 2.5f;
		vPosition = vPosition - vLook * m_fMovePower;
		m_bIsMove_After_Skill = false;
		break;
	case Client::CPlayer::STATE_ATTACK_DASH_BUCK:
		break;
	case Client::CPlayer::STATE_ATTACK_DASH_BUCK_NEXT:
		break;
	case Client::CPlayer::STATE_ATTACK_DASH_SLIDING_SHOT:
		break;
	case Client::CPlayer::STATE_ATTACK_SLIDING:
		m_fMovePower = 2.5f;
		vPosition = vPosition + vLook * m_fMovePower;
		break;
	case Client::CPlayer::STATE_END:
		break;
	default:
		break;
	}



	m_pTransformCom->Go_Teleport(vPosition, m_pNavigationCom);

}

void CPlayer::MakeDrone(_int DroneCount, _float fDistance, wstring DroneName)
{
	_float4 vPlayerPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);

	for (_int i = 0; i < DroneCount; ++i)
	{
		_float fAngle = XM_2PI*(_float)i / (_float)DroneCount;
		_vector vOffset = XMVectorSet(cosf(fAngle)* fDistance, 0.f, sinf(fAngle)* fDistance, 0.f);

		_vector vDronePos = XMLoadFloat4(&vPlayerPosition) + vOffset;
		_float4 vPosition = {};
		XMStoreFloat4(&vPosition, vDronePos);

		_vector vLook = XMVector3Normalize(XMLoadFloat4(&vPlayerPosition)- vDronePos);

		CDrone::CDrone_DESC DroneDesc{};
		DroneDesc.strModelName = DroneName;
		DroneDesc.vPosition = vPosition;
		DroneDesc.vDirLook = vLook;
		DroneDesc.pPlayerWorldMatrix= m_pTransformCom->Get_WorldFloat4x4();
		DroneDesc.fCurAngle = fAngle;
		DroneDesc.eType = m_eCharacterType;
		m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Player_Drone"), TEXT("Prototype_GameObject_Drone"), &DroneDesc);
	}

}

void CPlayer::MakeChangeparticle(_float fTimeDelta)
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = this->GetLookDir();
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_Style_Sword_Spread"),_float4(0.0f,0.0f,1.0f,0.3f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Swap_Effect_LowpolySphere16_Pop"),_float4(1.0f,1.0f,1.0f,0.1f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_atomTri_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_atomTri.002_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_atomTri.001_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,false, });
			
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y+1.0f, this->Get_Position().z, 1.0f));
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_Style_Gun_Spread"),_float4(1.0f,0.753f,0.796f,0.3f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Swap_Effect_LowpolySphere16_Pop"),_float4(1.0f,0.0f,1.0f,0.1f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_atomTri_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_atomTri.002_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,false, });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Swap_Effect_atomTri.001_Spread"),_float4(1.0f,1.0f,1.0f,0.3f),false ,false, });
			
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y+ 1.0f, this->Get_Position().z, 1.0f));
			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			break;
		}



}




CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();


	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
	int cnt = 0;


	for (auto& pPartObject : m_PartObjects)
	{
		Safe_Release(pPartObject);
	}
	m_PartObjects.clear();

}

template<typename T, typename U>
_bool CPlayer::CoolTimeCheck(T val, U fTimedelta)
{
	_float fTime = 0.0f;
	fTime += fTimedelta;
	if (fTime >= val)
	{
		return true;
	}
	else
		return false;

	//return 

}





