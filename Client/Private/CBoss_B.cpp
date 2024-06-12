#include "stdafx.h"
#include "CBoss_B.h"
#include "GameInstance.h"
#include"Loader.h"


#include"CBoneCollider.h"



#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"

#include"Player.h"
#include"Particle_Mesh.h"
#include"CBullet.h"
#include"Fade_In_Out.h"
#include"UI.h"
CBoss_B::CBoss_B(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CBoss_B::CBoss_B(const CBoss_B& rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss_B::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CBoss_B::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_PartObjects()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CMonster_DESC*>(pArg)->vPosition));
		
	}
	m_fHp = 1000.f;
	m_fHpMax = 1000.f;
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vMovePos[0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vMovePos[1] =XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_vMovePos[2] = XMVectorSet(0.f, 0.f, 10.f, 1.f);
	m_vMovePos[3] = XMVectorSet(10.f, 0.f, 10.f, 1.f);
	m_pTransformCom->Rotation(XMConvertToRadians(180.f));
	m_pTransformCom->Set_RotationSpeed(10.f);

	CActionNode* pDoIdleNode = CActionNode::create([this](_float deltaTime) { return DoIdle(deltaTime); });
	CActionNode* pDoMoveNode = CActionNode::create([this](_float deltaTime) { return DoMove(deltaTime); });

	CActionNode* pDoPowerfulAttack1Node = CActionNode::create([this](_float deltaTime) { return DoPowerfulAttack1(deltaTime); });
	CActionNode* pDoPowerfulAttack2Node = CActionNode::create([this](_float deltaTime) { return DoPowerfulAttack2(deltaTime); });
	CActionNode* pDoPowerfulAttack3Node = CActionNode::create([this](_float deltaTime) { return DoPowerfulAttack3(deltaTime); });


	CActionNode* pJumpAttackNode = CActionNode::create([this](_float deltaTime) { return DoJumpAttack(deltaTime); });
	CActionNode* pRightHandAttackNode = CActionNode::create([this](_float deltaTime) { return DoRightHandAttack(deltaTime); });
	CActionNode* pLeftHandAttackNode = CActionNode::create([this](_float deltaTime) { return DoLeftHandAttack(deltaTime); });
	CActionNode* pPullUpAttackNode = CActionNode::create([this](_float deltaTime) { return DoPullUpAttack(deltaTime); });
	CActionNode* pIsAliveNode = CActionNode::create([this](_float deltaTime) { return DoIsAlive(deltaTime); });



	CSequence* pDoPowerfulAttackNode = CSequence::Create(pDoPowerfulAttack1Node, pDoPowerfulAttack2Node, pDoPowerfulAttack3Node);
	CSelector* pAttackSelector = CSelector::Create(pDoPowerfulAttackNode, pJumpAttackNode, pRightHandAttackNode, pLeftHandAttackNode, pPullUpAttackNode);

	m_pRootNode = CSequence::Create(pIsAliveNode,pDoIdleNode, pDoMoveNode, pAttackSelector);

	return S_OK;
}

void CBoss_B::Priority_Tick(_float fTimeDelta)
{
	




	__super::Priority_Tick(fTimeDelta);
}

void CBoss_B::Tick(_float fTimeDelta)
{
	UI* pUi = dynamic_cast<UI*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Boss_Ui"), 0));
	if (pUi)
	{
		pUi->Set_MaxHp(m_fHpMax);
		pUi->Set_Hp(m_fHp);
	}








	m_fHitTime += fTimeDelta;
	if (m_fHitTime >= 0.5f)
	{
		m_bIsHit= false;
		m_fHitTime = 0.0f;
	}




	for (int i = 0; i < HAND_END; i++)
	{
		m_HandMatrix[i] = XMLoadFloat4x4(m_pHandMatrix[i]);
		m_HandMatrix[i].r[0] = XMVector3Normalize(m_HandMatrix[i].r[0]);
		m_HandMatrix[i].r[1] = XMVector3Normalize(m_HandMatrix[i].r[1]);
		m_HandMatrix[i].r[2] = XMVector3Normalize(m_HandMatrix[i].r[2]);

		//m_HandMatrix[i] = m_pTransformCom->Get_WorldMatrix() * m_HandMatrix[i];
	}



//	cout<<m_fHp<<endl;
// 
	m_pRootNode->Evaluate(fTimeDelta);
	
	CreateEffect_By_Motion(fTimeDelta);

	if(m_bIsBulletSpawn)
		Make_Falling_Bullet(fTimeDelta);
	//cout << m_fHp << endl;


	
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject->Tick(fTimeDelta);
	}

	if (m_fHp <= 0.0f)
		m_eCurState = CBoss_B_STATES::STATES_SMASH;

	__super::Tick(fTimeDelta);
}

void CBoss_B::Late_Tick(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject->Late_Tick(fTimeDelta);
	}

	__super::Late_Tick(fTimeDelta);
}

HRESULT CBoss_B::Render()
{
	__super::Render();
	return S_OK;
}

NodeStates CBoss_B::DoIdle(_float fTimeDelta)
{
	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		assert("No Player address");

	


	if (m_eCurState == CBoss_B_STATES::STATES_IDLE)
	{
		m_pTransformCom->TurnToTarget(fTimeDelta, pPlayer->Get_PositionVector());
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_IDLE, true));


		fChangeTime += fTimeDelta;
		if (fChangeTime >= 5.0f)
		{
			m_eCurState = GetRandomState();
			fChangeTime = 0.0f;
			return NodeStates::SUCCESS;
		}

		return NodeStates::FAILURE;
	}
	else
  	{
		return NodeStates::SUCCESS;
	}

	

}

NodeStates CBoss_B::DoMove(_float fTimeDelta)
{
	if (m_eCurState == CBoss_B_STATES::STATES_MOVE)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_MOVE, true));
		_float3	vPlayerPos = m_pPlayer->Get_Position();
		_float3	bossPos = this->Get_Position();
		float distance = (float)sqrt(pow(bossPos.x - vPlayerPos.x, 2) + pow(bossPos.y - vPlayerPos.y, 2) + pow(bossPos.z - vPlayerPos.z, 2));
		m_pTransformCom->LookAt(m_pPlayer->Get_PositionVector());
		m_pTransformCom->Go_Straight(fTimeDelta * 0.5f, m_pNavigationCom);
		double fCurPos = m_pModelCom->Get_CurrentPosition();

		

		
			if (fCurPos >= 0.32 && fCurPos <= 0.34)
			{
				m_pPlayer->Set_CameraShake(true, 0.1f);
				m_pGameInstance->Play_Sound_Z(TEXT("SFX_Ult_Ready [1].wav"), SOUND_EFFECT, 0.6f);
			}
			if (fCurPos >= 0.88 && fCurPos <= 0.90)
			{
				m_pPlayer->Set_CameraShake(true, 0.1f);
				m_pGameInstance->Play_Sound_Z(TEXT("SFX_Ult_Ready [1].wav"), SOUND_EFFECT, 0.6f);
			}



	

		if (distance <= 5.0f)
		{
 			m_eCurState = GetRandomState();


		}

	}
	else
	{
		return NodeStates::SUCCESS;
	}

}



NodeStates CBoss_B::DoPowerfulAttack1(_float fTimeDelta)
{
	if (m_eCurState == CBoss_B_STATES::STATES_GO_POWERFULATTACK)
	{

		_vector vPlayerPos = m_pPlayer->Get_PositionVector();

		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_ATTACK_B, false));
		double fCurPos = m_pModelCom->Get_CurrentPosition();

		if (fCurPos >= 0.90 && fCurPos <= 0.91)
		{

			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Slam [1].wav"), SOUND_EFFECT, 0.6f);
		}


		if (fCurPos >= 0.93 && fCurPos <= 0.95)
		{

			
			CBullet::CBullet_DESC BulletDesc{};
			BulletDesc.strModelName = TEXT("Boss_B_Cylinder");
			BulletDesc.fSpeedPerSec = 20.f;
			BulletDesc.vPosition = _float4(m_pPlayer->Get_Position().x, 10.0f, m_pPlayer->Get_Position().z, 1.0f);
			BulletDesc.vTargetPos = vPlayerPos;
			BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
			BulletDesc.BulletState = &CBullet::Falling;  //만약 자식 클래스에서 재정의를 했다면 그 함수를 호출함
			BulletDesc.fDamage= 5.0f;
 			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_Boss_B_Bullet"), &BulletDesc);
		}







		if (m_pModelCom->Get_AnimFinished())
		{
			//fChangeTime = 0.0f;
			m_eCurState = CBoss_B_STATES::STATES_GO_POWERFULATTACK2;
			return NodeStates::SUCCESS;
		}
		else
		{
			return NodeStates::RUNNING;
		}
	}
	else
	{
		return NodeStates::RUNNING;
	}

}

NodeStates CBoss_B::DoPowerfulAttack2(_float fTimeDelta)
{
	_vector vPlayerPos = m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player"))->Get_PositionVector();
	_bool bIsJump = false;
	if (m_eCurState == CBoss_B_STATES::STATES_GO_POWERFULATTACK2)
	{

		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_ATTACK_D, false));	
		double fCurPos= m_pModelCom->Get_CurrentPosition();
		if (fCurPos >= 0.95 && fCurPos <= 1.0)
		{
			m_bIsJumpAttack= true;
			m_pTransformCom->LookAt(vPlayerPos);
		}

		
		if (m_bIsJumpAttack)
		{
			
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
			m_pTransformCom->Go_Jump(fTimeDelta, 10.f, 4.0f, bIsJump);
		}

		if (bIsJump)
		{
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Award [1].wav"), SOUND_EFFECT, 0.7f);
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Smashed_Shield [1].wav"), SOUND_EFFECT, 0.6f);
			m_bIsJumpAttack = false;
		}
		
	
		
		//m_pTransformCom->Go_Jump(fTimeDelta, 10.f, 5.0f, m_bIsJumpAttack);
		
		if (m_pModelCom->Get_AnimFinished())
		{
			//this->Set_Position()
			//fChangeTime = 0.0f;
			//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vOriginPos);
			m_eCurState = CBoss_B_STATES::STATES_GO_POWERFULATTACK3;
			return NodeStates::SUCCESS;
		}
		else
		{
			return NodeStates::RUNNING;
		}
	}
	else
	{

		return NodeStates::RUNNING;
	}
}

NodeStates CBoss_B::DoPowerfulAttack3(_float fTimeDelta)
{
	if (m_eCurState == CBoss_B_STATES::STATES_GO_POWERFULATTACK3)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_ATTACK_C, false));
		_vector vPlayerPos = m_pPlayer->Get_PositionVector();
		double fCurPos = m_pModelCom->Get_CurrentPosition();
		if (fCurPos >= 1.55 && fCurPos <= 1.56)
		{
			_matrix mLeftBoneMatrix = m_HandMatrix[LEFT_HAND_9] * m_pTransformCom->Get_WorldMatrix();
			_matrix mRightBoneMatrix = m_HandMatrix[RIGHT_HAND_9] * m_pTransformCom->Get_WorldMatrix();

			_vector vLeftBonePos = mLeftBoneMatrix.r[3];
			_vector vRightBonePos = mRightBoneMatrix.r[3];

			_float4 fLeftBonePos = {};
			_float4 fRightBonePos = {};

			XMStoreFloat4(&fLeftBonePos, vLeftBonePos);
			XMStoreFloat4(&fRightBonePos, vRightBonePos);

			_float4 fMidBonePos = {
		   (fLeftBonePos.x + fRightBonePos.x) / 2.0f,
		   (fLeftBonePos.y + fRightBonePos.y) / 2.0f,
		   (fLeftBonePos.z + fRightBonePos.z) / 2.0f,
		   (fLeftBonePos.w + fRightBonePos.w) / 2.0f
			};





			CBullet::CBullet_DESC BulletDesc{};
			BulletDesc.strModelName = TEXT("BossAttack_C");
			BulletDesc.fSpeedPerSec = 20.f;
			BulletDesc.vPosition = _float4(fMidBonePos.x, fMidBonePos.y,fMidBonePos.z, 1.0f);
			BulletDesc.vTargetPos = vPlayerPos;
			BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
			BulletDesc.BulletState = &CBullet::Spread;  //만약 자식 클래스에서 재정의를 했다면 그 함수를 호출함
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);


		}





		if (m_pModelCom->Get_AnimFinished())
		{
			fChangeTime = 0.0f;
			m_eCurState = CBoss_B_STATES::STATES_MOVE;
			return NodeStates::SUCCESS;
		}
		else
		{
			return NodeStates::FAILURE;
		}
	}
}

NodeStates CBoss_B::DoJumpAttack(_float fTimeDelta)
{

	_vector vPlayerPos = m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player"))->Get_PositionVector();
	_bool bIsJump = false;


	if (m_eCurState == CBoss_B_STATES::STATES_ATTACK_D )
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_ATTACK_D, false));
		double fCurPos = m_pModelCom->Get_CurrentPosition();
		if (fCurPos >= 0.95 && fCurPos <= 1.0)
		{
			m_bIsJumpAttack = true;
			m_pTransformCom->LookAt(vPlayerPos);
		}


		if (m_bIsJumpAttack)
		{

			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
			m_pTransformCom->Go_Jump(fTimeDelta, 10.f, 4.0f, bIsJump);
		}

		if (bIsJump)
		{
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Award [1].wav"), SOUND_EFFECT, 0.7f);
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Smashed_Shield [1].wav"), SOUND_EFFECT, 0.6f);

			m_bIsJumpAttack = false;
		}

		if (m_pModelCom->Get_AnimFinished())
		{
			//this->Set_Position()
			//fChangeTime = 0.0f;
			//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vOriginPos);
			m_eCurState = CBoss_B_STATES::STATES_IDLE;
			return NodeStates::SUCCESS;
		}
		else
		{
			return NodeStates::RUNNING;
		}
	}
	else
	{

		return NodeStates::FAILURE;
	}
	
}

NodeStates CBoss_B::DoRightHandAttack(_float fTimeDelta)
{
	if (m_eCurState == CBoss_B_STATES::STATES_ATTACK_A)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_ATTACK_A, false));

		double fCurPos = m_pModelCom->Get_CurrentPosition();
		if (fCurPos >= 0.85 && fCurPos <= 0.86)
		{
			m_pPlayer->Set_CameraShake(true, 0.07f);
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Slam [1].wav"), SOUND_EFFECT, 0.6f);
		}

		


		if (m_pModelCom->Get_AnimFinished())
		{

			m_bIsBulletSpawn = true;
			//fChangeTime = 0.0f;
			m_eCurState = CBoss_B_STATES::STATES_IDLE;
			return NodeStates::SUCCESS;
		}
		else
		{
			return NodeStates::RUNNING;
		}
	}
	else
	{
		return NodeStates::FAILURE;
	}

	
}

NodeStates CBoss_B::DoLeftHandAttack(_float fTimeDelta)
{
	_vector vPlayerPos = m_pPlayer->Get_PositionVector();
	if (m_eCurState == CBoss_B_STATES::STATES_ATTACK_B)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_ATTACK_B, false));
		double fCurPos = m_pModelCom->Get_CurrentPosition();

		if (fCurPos >= 0.90 && fCurPos <= 0.91)
		{

			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Slam [1].wav"), SOUND_EFFECT, 0.6f);
		}


		if (fCurPos >= 0.93 && fCurPos <= 0.95)
		{


			CBullet::CBullet_DESC BulletDesc{};
			BulletDesc.strModelName = TEXT("Boss_B_Cylinder");
			BulletDesc.fSpeedPerSec = 20.f;
			BulletDesc.vPosition = _float4(m_pPlayer->Get_Position().x, 10.0f, m_pPlayer->Get_Position().z, 1.0f);
			BulletDesc.vTargetPos = vPlayerPos;
			BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
			BulletDesc.BulletState = &CBullet::Falling;  //만약 자식 클래스에서 재정의를 했다면 그 함수를 호출함
			BulletDesc.fDamage = 1.0f;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_Boss_B_Bullet"), &BulletDesc);
		}







		if (m_pModelCom->Get_AnimFinished())
		{
			//fChangeTime = 0.0f;
			m_eCurState = CBoss_B_STATES::STATES_IDLE;
			return NodeStates::SUCCESS;
		}
		else
		{
			return NodeStates::RUNNING;
		}
	}
	else
	{
		return NodeStates::FAILURE;
	}

	
}

NodeStates CBoss_B::DoPullUpAttack(_float fTimeDelta)
{
	if (m_eCurState == CBoss_B_STATES::STATES_ATTACK_C)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_ATTACK_C, false));
		_vector vPlayerPos = m_pPlayer->Get_PositionVector();
		double fCurPos = m_pModelCom->Get_CurrentPosition();
		if (fCurPos >= 0.90 && fCurPos <= 0.91)
		{
			m_pPlayer->Set_CameraShake(true, 0.07f);
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Slam [1].wav"), SOUND_EFFECT, 0.6f);
		}





		if (fCurPos >= 1.55 && fCurPos <= 1.56)
		{
			_matrix mLeftBoneMatrix = m_HandMatrix[LEFT_HAND_9] * m_pTransformCom->Get_WorldMatrix();
			_matrix mRightBoneMatrix = m_HandMatrix[RIGHT_HAND_9] * m_pTransformCom->Get_WorldMatrix();

			_vector vLeftBonePos = mLeftBoneMatrix.r[3];
			_vector vRightBonePos = mRightBoneMatrix.r[3];

			_float4 fLeftBonePos = {};
			_float4 fRightBonePos = {};

			XMStoreFloat4(&fLeftBonePos, vLeftBonePos);
			XMStoreFloat4(&fRightBonePos, vRightBonePos);

			_float4 fMidBonePos = {
		   (fLeftBonePos.x + fRightBonePos.x) / 2.0f,
		   (fLeftBonePos.y + fRightBonePos.y) / 2.0f,
		   (fLeftBonePos.z + fRightBonePos.z) / 2.0f,
		   (fLeftBonePos.w + fRightBonePos.w) / 2.0f
			};





			CBullet::CBullet_DESC BulletDesc{};
			BulletDesc.strModelName = TEXT("BossAttack_C");
			BulletDesc.fSpeedPerSec = 20.f;
			BulletDesc.vPosition = _float4(fMidBonePos.x, fMidBonePos.y, fMidBonePos.z, 1.0f);
			BulletDesc.vTargetPos = vPlayerPos;
			BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
			BulletDesc.BulletState = &CBullet::Spread;  //만약 자식 클래스에서 재정의를 했다면 그 함수를 호출함
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);

			
		}

		if (m_pModelCom->Get_AnimFinished())
		{
	
			m_eCurState = CBoss_B_STATES::STATES_IDLE;
			return NodeStates::SUCCESS;
		}
		else
		{
			return NodeStates::RUNNING;
		}
	}
	else
	{
		return NodeStates::FAILURE;
	}
}

NodeStates CBoss_B::DoIsAlive(_float fTimeDelta)
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};



	if (m_eCurState == CBoss_B_STATES::STATES_SMASH)
	{
		m_fAnimSpeed= 0.5f;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
		pPlayer->Set_CameraShake(true, 0.2f);
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CBoss_B_STATES::STATES_SMASH, false));
		double fCurPos = m_pModelCom->Get_CurrentPosition();
		if (fCurPos >= 0.55 && fCurPos <= 0.555)
		{
			m_pGameInstance->Play_Sound_Z(TEXT("SFX_Smashed_Boss001 [1].wav"), SOUND_EFFECT, 0.8f);

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Object_Boss_Dead_Dead_2_Spread"),_float4(0.2f,0.2f,0.2f,0.7f),false,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Object_Boss_Dead_Dead_Spread"),_float4(0.8f,0.8f,0.8f,0.3f),false,true });

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f));
		}


		if (m_pModelCom->Get_AnimFinished())
		{
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Monster_Dead_spark.C_Spread"),_float4(0.8f, 0.9f, 1.0f, 0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Monster_Dead_spark.B_Spread"),_float4(0.2f, 0.4f, 0.8f, 0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Monster_Dead_spark.A_Spread"),_float4(0.2f, 0.4f, 0.8f, 0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Monster_Dead_ElectColumn_Pop"),_float4(0.678f,0.847f,0.902f,0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Monster_Dead_ElectColumn.001_Pop"),_float4(0.6f, 0.4f, 0.8f, 0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Monster_Dead_ElectColumnHit_Size_Up"),_float4(0.6f, 0.4f, 0.8f, 0.3f) });
			

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			m_bIsDead = true;
			return NodeStates::FAILURE;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;

}

void CBoss_B::Make_Falling_Bullet(_float fTimeDelta)
{

	m_fBulletSpawnTime += fTimeDelta;

	if (m_vBulletSpawnDesc.empty())
	{
		for (int i = 0; i < 15; i++)
		{
			_float spawnTime = RandomNum<_float>(0.0f, 5.0f); // 예시로 0~5초 사이의 랜덤 시간 설정
			m_vBulletSpawnDesc.push_back({ spawnTime, false });

		}



	}

	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"),_float4(0.8f,0.8f,0.8f,0.5f)},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"),_float4(0.8f,0.8f,0.8f,0.5f)},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.2f,0.2f,0.2f,0.5f)},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop")},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop")},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Effect_hit002_Spread")},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Effect_hit003_Spread")},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Effect_hit004_Spread")},
	   {CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN,TEXT("Boss_Attack_Warning_Ray_SizeDown"),_float4(0.8f,0.0f,0.0f,0.7f)},
	};


	bool allBulletsSpawned = true;


	for (auto& spawnInfo : m_vBulletSpawnDesc)
	{
		if (!spawnInfo.bisSpawned && m_fBulletSpawnTime >= spawnInfo.fspawnTime)
		{
			_float fRandX = RandomNum<_float>(-23.f, 23.f);
			_float fRandZ = RandomNum<_float>(1.f, 47.f);
			CBullet::CBullet_DESC BulletDesc{};
			BulletDesc.strModelName = TEXT("BossAttack_C");
			BulletDesc.fSpeedPerSec = 20.f;
			BulletDesc.vPosition = _float4(fRandX, 50.f, fRandZ, 1.0f);
			BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
			BulletDesc.BulletState = &CBullet::Falling;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fRandX, 0.0f, fRandZ, 1.0f));

			spawnInfo.bisSpawned = true; // Bullet이 생성된 것을 표시
		}


		if (!spawnInfo.bisSpawned)
		{
			allBulletsSpawned = false;
		}
	}


	if (allBulletsSpawned)
	{
		m_vBulletSpawnDesc.clear();
		m_fBulletSpawnTime = 0.0f; // 총알 생성 시간을 초기화
		// 추가적으로 총알 생성이 완료되었다는 이벤트를 발생시키는 로직을 추가할 수 있다.
		m_bIsBulletSpawn = false;
	}





}

CBoss_B::CBoss_B_STATES CBoss_B::GetRandomState()
{
	CBoss_B_STATES pRandomStates[] =
	{
		CBoss_B_STATES::STATES_ATTACK_A,
		CBoss_B_STATES::STATES_ATTACK_B,
		CBoss_B_STATES::STATES_ATTACK_C,
		CBoss_B_STATES::STATES_ATTACK_D,
		CBoss_B_STATES::STATES_MOVE,
		CBoss_B_STATES::STATES_GO_POWERFULATTACK

	};

	random_device randomDevice;
	mt19937_64 Gen(randomDevice());
	uniform_int_distribution<> dis(0, sizeof(pRandomStates) / sizeof(pRandomStates[0]) - 1);
	int iRandomIndex = dis(Gen);



	return pRandomStates[(CBoss_B_STATES)iRandomIndex];
	
}

void CBoss_B::CreateEffect_By_Motion(_float fTimeDelta)
{
	double fCurPos = m_pModelCom->Get_CurrentPosition();


	if (m_pModelCom->Get_AnimationIndex() == CBoss_B_STATES::STATES_ATTACK_A)
	{
	

		if (fCurPos >= 0.95 && fCurPos <= 0.97)
		{
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION,TEXT("Boss_Down_Effect_Explosion"),_float4(0.2f,0.2f,0.2f,0.5f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.2f,0.2f,0.2f,0.5f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down"),_float4(0.2f,0.2f,0.2f,0.5f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop"),_float4(0.8f,0.8f,0.8f,0.5f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop"),_float4(0.8f,0.8f,0.8f,0.5f)},

			};

			_matrix mBoneMatrix = m_HandMatrix[RIGHT_HAND_10] * m_pTransformCom->Get_WorldMatrix();
			_vector vBonePos = mBoneMatrix.r[3];
			_float4 fBonePos = {};
			XMStoreFloat4(&fBonePos, vBonePos);

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fBonePos.x, fBonePos.y, fBonePos.z, 1.0f));

		}



	}


	if (m_pModelCom->Get_AnimationIndex() == CBoss_B_STATES::STATES_ATTACK_B)
	{
		if (fCurPos >= 0.95 && fCurPos <= 0.98)
		{
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION,TEXT("Boss_Down_Effect_Explosion")},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down")},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down")},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop")},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop")},

			};

			_matrix mBoneMatrix = m_HandMatrix[LEFT_HAND_10] * m_pTransformCom->Get_WorldMatrix();
			_vector vBonePos = mBoneMatrix.r[3];
			_float4 fBonePos = {};
			XMStoreFloat4(&fBonePos, vBonePos);

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fBonePos.x, fBonePos.y, fBonePos.z, 1.0f));

		}



	}

	if (m_pModelCom->Get_AnimationIndex() == CBoss_B_STATES::STATES_ATTACK_C)
	{
		if (fCurPos >= 0.91 && fCurPos <= 0.93)
		{
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("HitRing_Boss_B")},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Boss_HitRing_SizeUp")},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down")},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down")}
			};

			_matrix mLeftBoneMatrix = m_HandMatrix[LEFT_HAND_9] * m_pTransformCom->Get_WorldMatrix();
			_matrix mRightBoneMatrix = m_HandMatrix[RIGHT_HAND_9] * m_pTransformCom->Get_WorldMatrix();

			_vector vLeftBonePos = mLeftBoneMatrix.r[3];
			_vector vRightBonePos = mRightBoneMatrix.r[3];

			_float4 fLeftBonePos = {};
			_float4 fRightBonePos = {};

			XMStoreFloat4(&fLeftBonePos, vLeftBonePos);
			XMStoreFloat4(&fRightBonePos, vRightBonePos);

			_float4 fMidBonePos = {
		   (fLeftBonePos.x + fRightBonePos.x) / 2.0f,
		   (fLeftBonePos.y + fRightBonePos.y) / 2.0f,
		   (fLeftBonePos.z + fRightBonePos.z) / 2.0f,
		   (fLeftBonePos.w + fRightBonePos.w) / 2.0f
			};

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fMidBonePos.x, fMidBonePos.y, fMidBonePos.z, 1.0f));


		}





	}



	if (m_pModelCom->Get_AnimationIndex() == CBoss_B_STATES::STATES_ATTACK_D)
	{
		if (fCurPos >= 0.98 && fCurPos <= 1.0)
		{
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP, TEXT("HitRing_Boss_B"),_float4(0.3f,0.3f,0.3f,0.5f)},
				{ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Boss_HitRing_SizeUp"),_float4(0.8f,0.8f,0.8f,0.4f) },

				{ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.8f,0.8f,0.8f,0.4f) },
				{ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down"),_float4(0.0f,0.0f,0.0f,1.0f) },
				{ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop"),_float4(0.8f,0.8f,0.8f,0.4f) },
				{ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop") ,_float4(0.8f,0.8f,0.8f,0.4f)},

			};

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, 0.0f, this->Get_Position().z, 1.0f));
		}
		if (fCurPos >= 1.75 && fCurPos <= 1.77)
		{

			_uint iFadeLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_Fade_In_Out"));

			for (_uint i = 0; i < iFadeLayerSize; ++i)
			{
				Fade_In_Out* pFadeInOut = static_cast<Fade_In_Out*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Fade_In_Out"), i));
				pFadeInOut->Start_FadeIn();
			}

			//세그먼트 라인 위치 바꾸기
			_uint iLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_1_Segment"));
			_float fRandAngle = RandomNum<_float>(0.f, 360.f);
			for (_uint i = 0; i < iLayerSize; ++i)
			{
				CGameObject* pSegment = m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_1_Segment"), i);
				pSegment->Set_Rotation(XMConvertToRadians(fRandAngle));
				
			}






			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
			pPlayer->Set_CameraShake(true,0.3f);


			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("HitRing_Boss_B"),_float4(0.3f,0.3f,0.3f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Boss_HitRing_SizeUp"),_float4(0.3f,0.3f,0.3f,0.5f)},

				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.3f,0.3f,0.3f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down"),_float4(0.3f,0.3f,0.3f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop"),_float4(0.3f,0.3f,0.3f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop"),_float4(0.3f,0.3f,0.3f,0.5f)},

			};

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, 0.0f, this->Get_Position().z, 1.0f));
		}

	}


	


}











HRESULT CBoss_B::Add_PartObjects()
{
	CBoneCollider::CBoneCollider_DESC BoneColliderDesc{};
	BoneColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();

	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.009"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.R.009"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.010"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.R.010"));
	BoneColliderDesc.pBoundingDesc.eType = CCollider::TYPE_OBB;
	BoneColliderDesc.pBoundingDesc.vExtents = _float3(0.7f, 0.7f, 0.7f);
	BoneColliderDesc.pBoundingDesc.vCenter = _float3(0.f, BoneColliderDesc.pBoundingDesc.vExtents.y, 0.f);
	//BoneColliderDesc.pBoundingDesc.vRotation = _float3(0.f, 0.f, 0.f);
	CGameObject* pBoneCollider = nullptr;
	pBoneCollider = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_BoneCollider"), &BoneColliderDesc);
	if (pBoneCollider == nullptr)
		return E_FAIL;

	m_PartObjects.emplace_back(pBoneCollider);




	m_pHandMatrix[LEFT_HAND_9] = m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.009");
	m_pHandMatrix[RIGHT_HAND_9] = m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.R.009");
	m_pHandMatrix[LEFT_HAND_10]= m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.010");
	m_pHandMatrix[RIGHT_HAND_10] = m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.R.010");


	return S_OK;
}

HRESULT CBoss_B::Add_Components()
{

	/* For.Com_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		ColliderOBBDesc{};
	
	ColliderOBBDesc.eType = CCollider::TYPE_OBB;
	ColliderOBBDesc.vExtents = _float3(3.3f, 2.7f, 3.3f);
	ColliderOBBDesc.vCenter = _float3(0.f, ColliderOBBDesc.vExtents.y, 0.f);
	ColliderOBBDesc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);
	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_OBB]), &ColliderOBBDesc)))
		return E_FAIL;



	if(FAILED(__super::Add_Components()))
		return E_FAIL;
	
	



	return S_OK;
}

CBoss_B* CBoss_B::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_B*		pInstance = new CBoss_B(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
		{
			MSG_BOX("Failed To Created : CBoss_B");
			Safe_Release(pInstance);
		}
	
	return pInstance;
}
	

CGameObject* CBoss_B::Clone(void* pArg)
{
	CBoss_B* pInstance = new CBoss_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_B");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CBoss_B::Free()
{
	__super::Free();

	for (auto& pPartObject : m_PartObjects)
		Safe_Release(pPartObject);

	m_PartObjects.clear();

	Safe_Release(m_pRootNode);
}
