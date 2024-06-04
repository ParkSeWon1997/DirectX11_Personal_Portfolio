#include "stdafx.h"
#include "CHandBoss.h"
#include "GameInstance.h"
#include"Loader.h"

#include"CBoneCollider.h"


#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"

#include"Player.h"
#include"Particle_Mesh.h"
#include"CHandBullet.h"
#include"CBullet.h"

CHandBoss::CHandBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CHandBoss::CHandBoss(const CHandBoss& rhs)
	: CMonster(rhs)
{
}

HRESULT CHandBoss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHandBoss::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if(FAILED(__super::Add_Components()))
		return E_FAIL;

	if(FAILED(Add_PartObjects()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_eCurPart=static_cast<CHandBoss_DESC*>(pArg)->ePart;



		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CMonster_DESC*>(pArg)->vPosition));
	}
	m_pTransformCom->Rotation(XMConvertToRadians(180.f));
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);



	fRandTime = RandomNum<_float>(5.0f, 15.0f);
	m_fHp = 1000.f;

	m_pTransformCom->Set_RotationSpeed(90.f);
	if(m_eCurPart== PARTS_HAND_R)
		m_eCurState = STATES_IDLE;
	else
	{

		//m_eCurState = STATES_ATTACK_B_0;
		m_eCurState = STATES_IDLE;
		
	}

	//m_eCurState=(CHandBoss_STATES)RandomNum<_float>((_uint)CHandBoss_STATES::STATES_IDLE, (_float)CHandBoss_STATES::STATES_ATTACK_B_0);
	CActionNode* pDoIdleNode = CActionNode::create([this](_float deltaTime) { return DoIdle(deltaTime); });
	CActionNode* pDoAttack_B_0Node = CActionNode::create([this](_float deltaTime) { return DoFollow(deltaTime); });
	CActionNode* pDoAttack_B_1Node = CActionNode::create([this](_float deltaTime) { return DoTakeDown(deltaTime); });
	CActionNode* pDoAttack_B_2Node = CActionNode::create([this](_float deltaTime) { return DoHandsUp(deltaTime); });
	CActionNode* pDoAttack_A_0Node = CActionNode::create([this](_float deltaTime) { return DoAttack_A_0(deltaTime); });
	CActionNode* pDoAttack_A_1Node = CActionNode::create([this](_float deltaTime) { return DoAttack_A_1(deltaTime); });
	CActionNode* pDoAttack_A_2Node = CActionNode::create([this](_float deltaTime) { return DoAttack_A_2(deltaTime); });
	CActionNode* pDoAttack_C_0Node = CActionNode::create([this](_float deltaTime) { return DoAttack_C_0(deltaTime); });
	CActionNode* pDoAttack_C_1Node = CActionNode::create([this](_float deltaTime) { return DoAttack_C_1(deltaTime); });
	CActionNode* pDoAttack_C_2Node = CActionNode::create([this](_float deltaTime) { return DoAttack_C_2(deltaTime); });
	CActionNode* pDoAttack_D_0Node = CActionNode::create([this](_float deltaTime) { return DoAttack_D_0(deltaTime); });
	CActionNode* pDoAttack_D_1Node = CActionNode::create([this](_float deltaTime) { return DoAttack_D_1(deltaTime); });
	CActionNode* pDoAttack_D_2Node = CActionNode::create([this](_float deltaTime) { return DoAttack_D_2(deltaTime); });
	CActionNode* pDoAttack_E_0Node = CActionNode::create([this](_float deltaTime) { return DoAttack_E_0(deltaTime); });
	CActionNode* pDoAttack_E_1Node = CActionNode::create([this](_float deltaTime) { return DoAttack_E_1(deltaTime); });
	CActionNode* pDoAttack_Groggy_StartNode = CActionNode::create([this](_float deltaTime) { return DoAttack_Groggy_Start(deltaTime); });
	CActionNode* pDoAttack_Groggy_EndNode = CActionNode::create([this](_float deltaTime) { return DoAttack_Groggy_End(deltaTime); });



	CSequence* pFirstComboSeq = CSequence::Create(pDoAttack_A_0Node, pDoAttack_A_1Node, pDoAttack_A_2Node);
	CSequence* pSecondComboSeq = CSequence::Create(pDoAttack_B_0Node, pDoAttack_B_1Node, pDoAttack_B_2Node);
	CSequence* pThirdComboSeq = CSequence::Create(pDoAttack_C_0Node, pDoAttack_C_1Node, pDoAttack_C_2Node);
	CSequence* pFourththComboSeq = CSequence::Create(pDoAttack_D_0Node, pDoAttack_D_1Node, pDoAttack_D_2Node);
	CSequence* pFifthComboSeq = CSequence::Create(pDoAttack_E_0Node, pDoAttack_E_1Node);
	CSequence* pSixthComboSeq = CSequence::Create(pDoAttack_Groggy_StartNode, pDoAttack_Groggy_EndNode);
	CSelector* pAttackSelector = CSelector::Create(pFirstComboSeq, pSecondComboSeq, pThirdComboSeq, pFourththComboSeq, pFifthComboSeq, pSixthComboSeq);







	m_pRootNode = CSequence::Create(pDoIdleNode, pAttackSelector);

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(10.f,0.0f,20.0f,1.0f));
	return S_OK;
}

void CHandBoss::Priority_Tick(_float fTimeDelta)
{


	__super::Priority_Tick(fTimeDelta);
}

void CHandBoss::Tick(_float fTimeDelta)
{


	m_HandPalmMatrix = XMLoadFloat4x4(m_HandPalm);
	m_HandPalmMatrix.r[0] = XMVector3Normalize(m_HandPalmMatrix.r[0]);
	m_HandPalmMatrix.r[1] = XMVector3Normalize(m_HandPalmMatrix.r[1]);
	m_HandPalmMatrix.r[2] = XMVector3Normalize(m_HandPalmMatrix.r[2]);


	if (KEY_AWAY(DIK_0))
	{
		m_fHp -= 50.f;
	}


	m_pRootNode->Evaluate(fTimeDelta);
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject->Tick(fTimeDelta);
	}

	

	//뼈 자체의 월드행렬


	__super::Tick(fTimeDelta);

	m_pColliderCom[COLLIDER_OBB]->Tick(m_HandPalmMatrix * m_pTransformCom->Get_WorldMatrix());
}

void CHandBoss::Late_Tick(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject->Late_Tick(fTimeDelta);
	}

	__super::Late_Tick(fTimeDelta);
}

HRESULT CHandBoss::Render()
{
	__super::Render();
	return S_OK;
}

HRESULT CHandBoss::Add_Components()
{
	



	CBounding_OBB::BOUNDING_OBB_DESC		ColliderOBBDesc{};
	
	ColliderOBBDesc.eType = CCollider::TYPE_OBB;
	ColliderOBBDesc.vExtents = _float3(2.5f, 2.5f, 2.5f);
	ColliderOBBDesc.vCenter = _float3(0.f, ColliderOBBDesc.vExtents.y*-2.0f, 0.f);
	ColliderOBBDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_OBB]), &ColliderOBBDesc)))
		return E_FAIL;

	/* For.Com_Collider_AABB */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderAABBDesc{};
	
	ColliderAABBDesc.eType = CCollider::TYPE_AABB;
	ColliderAABBDesc.vExtents = _float3(1.f, 0.7f, 0.3f);
	ColliderAABBDesc.vCenter = _float3(0.f, ColliderAABBDesc.vExtents.y, 0.f);
	
	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_AABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_AABB]), &ColliderAABBDesc)))
		return E_FAIL;




	
	return S_OK;
}

NodeStates CHandBoss::DoIdle(_float fTimeDelta)
{
	_vector vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vInterpolatedPos = XMVectorLerp(vMonsterPos, m_vOriginPos, fTimeDelta);

	//cout << fChangeTime << endl;
	if (m_eCurState == CHandBoss_STATES::STATES_IDLE)
	{
		fChangeTime+= fTimeDelta;
	
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vInterpolatedPos);
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_IDLE, true));
		if (fChangeTime >= fRandTime)
		{
			fRandTime= RandomNum<_float>(5.0f, 10.0f);
			m_eCurState = GetRandomAttack();

			return NodeStates::SUCCESS;
		}
	}
	else
		return NodeStates::SUCCESS;
	//if (fChangeTime >= fRandTime)
	//{
	//	m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_IDLE, true));
	//	fRandTime = RandomNum<_float>(1.0f, 3.0f);
	//	m_eCurState = GetRandomAttack();
	//	return NodeStates::SUCCESS;
	//}
	
	

}

NodeStates CHandBoss::DoFollow(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_B_0)
	{
		//m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));

		_vector vPlayerPos = m_pPlayer->Get_PositionVector();
		_vector vBonePos = m_HandPalmMatrix.r[3];
		_vector vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_vector vInterpolatedPos = XMVectorLerp(vMonsterPos, vPlayerPos, fTimeDelta);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vInterpolatedPos);


		//_vector vLook = XMVector3Normalize(vPlayerPos - vMonsterPos);
		//_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		//_vector vUp = XMVector3Cross(vLook, vRight);


		_float fDistance = XMVectorGetX(XMVector3Length(vMonsterPos - vPlayerPos));
		// 일정 거리 이내에 있을 경우
		float fThresholdDistance = 2.0f; // 위치가 같다고 인정할 거리
		if (fDistance <= fThresholdDistance)
		{
			//m_pTransformCom->LookAt(vPlayerPos);
			switch (m_eCurPart)
			{
			case Client::CHandBoss::PARTS_HAND_L:
				_vector vLefthand = vMonsterPos + vBonePos;
				_float4 fLeftHand = {};
				XMStoreFloat4(&fLeftHand, vLefthand);

				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fLeftHand.x, 1.0f, fLeftHand.z,1.0f));
				//m_pTransformCom->Rotation(vUp, XMConvertToRadians(-45.f));

				break;
			case Client::CHandBoss::PARTS_HAND_R:
				_vector vRightHand = vMonsterPos + vBonePos;
				_float4 fRightHand = {};
				XMStoreFloat4(&fRightHand, vRightHand);

				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fRightHand.x, 1.0f, fRightHand.z, 1.0f));
				break;
			case Client::CHandBoss::PARTS_END:
				break;
			default:
				break;
			}

			m_eCurState = CHandBoss_STATES::STATES_ATTACK_B_1;
			return NodeStates::SUCCESS;

		}
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_B_0, false));
		//if (m_pModelCom->Get_AnimFinished())
		//{
		//	m_eCurState = CHandBoss_STATES::STATES_ATTACK_B_1;
		//	return NodeStates::SUCCESS;
		//}
		//else
		//	return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;


	
}

NodeStates CHandBoss::DoTakeDown(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_B_1)
	{
		
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_B_1, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			_vector vPlayerPos = m_pPlayer->Get_PositionVector();
			_matrix mBoneWorldMatrix = m_HandPalmMatrix * m_pTransformCom->Get_WorldMatrix();

			_vector vBulletStartPos = mBoneWorldMatrix.r[3];
			_vector vForward = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			_vector vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);




			for (int i = 0; i < 8; i++)
			{
				_float fAngle = XM_2PI *((_float)i / 8.f);
				_matrix rotationMatrix = XMMatrixRotationAxis(vUp, fAngle); // Y축을 기준으로 회전
				_vector vDirection = XMVector3TransformNormal(vForward, rotationMatrix); // 회전된 방향 벡터
				_float4 fPos = {};
				XMStoreFloat4(&fPos, vBulletStartPos);

				//m_pTransformCom.
				//CHandBullet::Test()
				CBullet::CBullet_DESC BulletDesc{};
				BulletDesc.strModelName = TEXT("BossAttack_C");
				BulletDesc.fSpeedPerSec = 40.f;
				BulletDesc.vPosition=_float4(fPos.x, 5.0f, fPos.z, 1.0f);
				BulletDesc.vTargetPos = vPlayerPos;
				BulletDesc.vDir = vDirection;
				BulletDesc.fRotatedAngle = RandomNum<_float>(0.f, 360.f);
				BulletDesc.BulletState = &CBullet::Spread;  //만약 자식 클래스에서 재정의를 했다면 그 함수를 호출
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);
			}


			_vector vBonePos = mBoneWorldMatrix.r[3];
			_float4 fBonePos = {};
			XMStoreFloat4(&fBonePos, vBonePos);
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION,TEXT("Boss_Down_Effect_Explosion"),_float4(0.2f,0.2f,0.2f,0.5f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.8f,0.8f,0.8f,0.5f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down"),_float4(0.1f,0.1f,0.1f,0.3f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop"),_float4(0.5f,0.8f,0.8f,0.5f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop"),_float4(0.3f,0.8f,0.8f,0.5f)},

			};
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fBonePos.x, 0.0f, fBonePos.z, 1.0f));

			m_eCurState = CHandBoss_STATES::STATES_ATTACK_B_2;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;
}

NodeStates CHandBoss::DoHandsUp(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_B_2)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_B_2, false));
		if (m_pModelCom->Get_AnimFinished())
		{

			SetRandomData();
			m_eCurState = CHandBoss_STATES::STATES_IDLE;
			fChangeTime = 0.f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
	{
		return NodeStates::FAILURE;

	}

}


NodeStates CHandBoss::DoAttack_A_0(_float fTimeDelta)
{

	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_A_0)
	{
		_matrix mBoneWorldMatrix = m_HandPalmMatrix * m_pTransformCom->Get_WorldMatrix();


		_vector vBonePos = mBoneWorldMatrix.r[3];
		_float4 fBonePos = {};
		XMStoreFloat4(&fBonePos, vBonePos);

		if (m_pModelCom->Get_AnimFinished())
		{
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION,TEXT("Boss_Down_Effect_Explosion"),_float4(0.2f,0.2f,0.2f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down")},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down")},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop")},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop")},
	
			};
			//vecDesc.push_back(desc);

			//vecDesc.

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fBonePos.x,0.0f, fBonePos.z,1.0f));


			m_eCurState = CHandBoss_STATES::STATES_ATTACK_A_1;
			return NodeStates::SUCCESS;
		}
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_A_0, false));

			return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;


}

NodeStates CHandBoss::DoAttack_A_1(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_A_1)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_A_1, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CHandBoss_STATES::STATES_ATTACK_A_2;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;




	
}

NodeStates CHandBoss::DoAttack_A_2(_float fTimeDelta)
{
 	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_A_2)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_A_2, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			SetRandomData();
			m_eCurState = CHandBoss_STATES::STATES_IDLE;
			fChangeTime= 0.f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::FAILURE;


	
}



NodeStates CHandBoss::DoAttack_C_0(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_C_0)
	{

		_vector vTargeForRightHandPos = XMVectorSet(-20.f,0.0f,50.f,1.0f);
		_vector vTargeForLftHandPos = XMVectorSet(20.f, 0.0f, 50.f, 1.0f);
		_vector vPlayerPos = m_pPlayer->Get_PositionVector();
		_vector vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_matrix mBoneWorldMatrix = m_HandPalmMatrix * m_pTransformCom->Get_WorldMatrix();


		_vector vBonePos = mBoneWorldMatrix.r[3];



		_vector vInterpolatedPos = {};
		//_vector vInterpolatedPos = XMVectorLerp(vMonsterPos, vTargePos, fTimeDelta);
		_float fDistance = {}; 
		switch (m_eCurPart)
		{
		case Client::CHandBoss::PARTS_HAND_L:
			vInterpolatedPos = XMVectorLerp(vMonsterPos, vTargeForLftHandPos, fTimeDelta);
			fDistance = XMVectorGetX(XMVector3Length(vMonsterPos - vTargeForLftHandPos));
			

			break;
		case Client::CHandBoss::PARTS_HAND_R:
			vInterpolatedPos = XMVectorLerp(vMonsterPos, vTargeForRightHandPos, fTimeDelta);
			fDistance=XMVectorGetX(XMVector3Length(vMonsterPos - vTargeForRightHandPos));
			break;
		case Client::CHandBoss::PARTS_END:
			break;
		default:
			break;
		}
		
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vInterpolatedPos);
 		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_C_0, false));
		
		// 일정 거리 이내에 있을 경우
		float fThresholdDistance = 2.0f; // 위치가 같다고 인정할 거리
		if (fDistance <= fThresholdDistance)
		{
			
			
		
			
			//m_pTransformCom->LookAt(vPlayerPos);

				if (m_pModelCom->Get_AnimFinished())
				{

					m_eCurState = CHandBoss_STATES::STATES_ATTACK_C_1;
					return NodeStates::SUCCESS;

				}
				else
					return NodeStates::RUNNING;
			
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;



}

NodeStates CHandBoss::DoAttack_C_1(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_C_1)
	{
		_vector vPlayerPos = m_pPlayer->Get_PositionVector();
		m_pTransformCom->LookAt(vPlayerPos);
		_matrix mBoneWorldMatrix= m_HandPalmMatrix * m_pTransformCom->Get_WorldMatrix();



		_vector vBonePos = m_HandPalmMatrix.r[3];
		_vector vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		vBonePos= XMVector3Normalize(vBonePos);
		vMonsterPos = XMVector3Normalize(vMonsterPos);

		//_float fDot = XMVectorGetX(XMVector3Dot(vBonePos, vMonsterPos));
		//m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(fDot));
		//m_pTransformCom->LookAt(vPlayerPos);

		_vector vMonsterLook = XMVector3Normalize(vMonsterPos - vPlayerPos);
		
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_C_1, false));
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.04 && dNowFramePos <= 0.80)
		{
			if (m_iBulletCount == 0)
			{
				//m_pTransformCom.
				CBullet::CBullet_DESC  BulletDesc{};
				BulletDesc.strModelName = TEXT("BossAttack_C");
				BulletDesc.fSpeedPerSec = 40.f;
				XMStoreFloat4(&BulletDesc.vPosition, mBoneWorldMatrix.r[3]);
				BulletDesc.vTargetPos = vPlayerPos;
				BulletDesc.vDir =  m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
				BulletDesc.BulletState = &CBullet::Go;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);
				m_iBulletCount++;
		
			}
		}
		if (dNowFramePos >= 0.25 && dNowFramePos <= 0.29)
		{
			if (m_iBulletCount == 1)
			{
				CBullet::CBullet_DESC  BulletDesc{};
				BulletDesc.strModelName = TEXT("BossAttack_C");
				BulletDesc.fSpeedPerSec = 40.f;
				XMStoreFloat4(&BulletDesc.vPosition, mBoneWorldMatrix.r[3]);
				BulletDesc.vTargetPos = vPlayerPos;
				BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
				BulletDesc.BulletState = &CBullet::Go;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);
				m_iBulletCount++;
			}
			
		}
		if(dNowFramePos>=0.43 && dNowFramePos<=0.55)
		{
			if (m_iBulletCount == 2)
			{
				CBullet::CBullet_DESC  BulletDesc{};
				BulletDesc.strModelName = TEXT("BossAttack_C");
				BulletDesc.fSpeedPerSec = 40.f;
				XMStoreFloat4(&BulletDesc.vPosition, mBoneWorldMatrix.r[3]);
				BulletDesc.vTargetPos = vPlayerPos;
				BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
				BulletDesc.BulletState = &CBullet::Go;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);
				m_iBulletCount++;
			}
		
		}
		


		if (m_pModelCom->Get_AnimFinished())
		{
			m_iBulletCount = 0;
			//CHandBullet::CHandBullet_DESC BulletDesc{};
			//BulletDesc.strModelName = TEXT("BossAttack_C");
			//BulletDesc.fSpeedPerSec = 20.f;
			//XMStoreFloat4(&BulletDesc.vPosition, mBoneWorldMatrix.r[3]);
			////BulletDesc.vPosition =
			//BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
			//BulletDesc.BulletState = &CHandBullet::Go;
			//m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);
			//int temp = 0;

			m_eCurState = CHandBoss_STATES::STATES_ATTACK_C_2;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;




}

NodeStates CHandBoss::DoAttack_C_2(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_C_2)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_C_2, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			SetRandomData();
			m_eCurState = CHandBoss_STATES::STATES_IDLE;
			fChangeTime = 0.f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::FAILURE;


}

NodeStates CHandBoss::DoAttack_D_0(_float fTimeDelta)
{

	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_D_0)
	{

		if (m_pModelCom->Get_AnimFinished())
		{

			



			m_eCurState = CHandBoss_STATES::STATES_ATTACK_D_1;
			return NodeStates::SUCCESS;
		}
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_D_0, false));

		return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;

}

NodeStates CHandBoss::DoAttack_D_1(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_D_1)
	{

		if (m_pModelCom->Get_AnimFinished())
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));

			_vector vPlayerPos = pPlayer->Get_PositionVector();


			
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"),_float4(0.8f,0.8f,0.8f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"),_float4(0.8f,0.8f,0.8f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.3f,0.4f,0.5f,0.7f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop")},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop")},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Effect_hit002_Spread"),_float4(0.3f,0.2f,0.0f,0.7f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Effect_hit003_Spread"),_float4(0.2f,0.1f,0.1f,0.6f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Effect_hit004_Spread"),_float4(0.1f,0.0f,0.2f,0.5f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN,TEXT("Boss_Attack_Warning_Ray_SizeDown"),_float4(0.9f,0.3f,0.2f,0.7f)},
				
			};
			//vecDesc.push_back(desc);
			
			//vecDesc.
			
			CParticle_Mesh::Make_Particle(vecDesc, vPlayerPos);


			//test
			CBullet::CBullet_DESC  BulletDesc{};
			BulletDesc.strModelName = TEXT("BossAttack_C");
			BulletDesc.fSpeedPerSec = 40.f;
			BulletDesc.vPosition = _float4(pPlayer->Get_Position().x, 50.f, pPlayer->Get_Position().z, 1.0f);
			BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
			BulletDesc.BulletState = &CBullet::Falling;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);

			if (m_fHp < 1000.f)
			{
				for (int i = 0; i < 15; i++)
				{
					_float fRandX = RandomNum<_float>(-23.f, 23.f);
					_float fRandZ = RandomNum<_float>(1.f, 47.f);
					CBullet::CBullet_DESC  BulletDesc{};
					BulletDesc.strModelName = TEXT("BossAttack_C");
					BulletDesc.fSpeedPerSec = 40.f;
					BulletDesc.vPosition = _float4(fRandX, 50.f, fRandZ, 1.0f);
					BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
					BulletDesc.BulletState = &CBullet::Falling;
					BulletDesc.fRotatedAngle = RandomNum<_float>(0.f, 360.f);
					m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_HandBullet"), &BulletDesc);

					CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fRandX,0.0f, fRandZ,1.0f));

				}

			}




			m_eCurState = CHandBoss_STATES::STATES_ATTACK_D_2;
			return NodeStates::SUCCESS;
		}
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_D_1, false));

		return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;


}

NodeStates CHandBoss::DoAttack_D_2(_float fTimeDelta)
{

	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_D_2)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_D_2, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			SetRandomData();
			m_eCurState = CHandBoss_STATES::STATES_IDLE;
			fChangeTime = 0.f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::FAILURE;
}

NodeStates CHandBoss::DoAttack_E_0(_float fTimeDelta)
{


	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_E_0)
	{

		if (m_pModelCom->Get_AnimFinished())
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));

			_vector vPlayerPos = pPlayer->Get_PositionVector();


			CParticle_Mesh::PARTICLE_DESC desc;
			XMStoreFloat4(&desc.vStartPos, vPlayerPos);

			desc.strModelName = TEXT("Boss_Effect_Timestop_Sphere_Pop");
			desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_HALF_ALHPA;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Particle"), &desc);

			desc.strModelName = TEXT("Boss_Effect_Timestop_Pop");
			desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_HALF_ALHPA;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Particle"), &desc);



			m_eCurState = CHandBoss_STATES::STATES_ATTACK_E_1;
			return NodeStates::SUCCESS;
		}
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_E_0, false));

		return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;



	
}

NodeStates CHandBoss::DoAttack_E_1(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_E_1)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_E_1, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			CParticle_Mesh::PARTICLE_DESC desc;
			desc.vStartPos = _float4(0.f, 0.f, 0.f, 1.0f);
			
			desc.strModelName = TEXT("Boss_Effect_Timestop_Sphere_Pop");
			desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_HALF_ALHPA;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Particle"), &desc);
			
			desc.strModelName = TEXT("Boss_Effect_Timestop_Pop");
			desc.eParticleType = CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_HALF_ALHPA;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Particle"), &desc);
			SetRandomData();
			m_eCurState = CHandBoss_STATES::STATES_IDLE;
			fChangeTime = 0.f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::FAILURE;
	
}

NodeStates CHandBoss::DoAttack_Groggy_Start(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_GROGGY_START)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_GROGGY_START, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CHandBoss_STATES::STATES_ATTACK_GROGGY_END;
			return NodeStates::SUCCESS;
		}
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_GROGGY_START, false));

		return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;

}

NodeStates CHandBoss::DoAttack_Groggy_End(_float fTimeDelta)
{
	if (m_eCurState == CHandBoss_STATES::STATES_ATTACK_GROGGY_END)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CHandBoss_STATES::STATES_ATTACK_GROGGY_END, false));

		if (m_pModelCom->Get_AnimFinished())
		{
			SetRandomData();
			m_eCurState = CHandBoss_STATES::STATES_IDLE;
			fChangeTime = 0.f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;

	}
	else
		return NodeStates::FAILURE;

	
}







CHandBoss::CHandBoss_STATES CHandBoss::GetRandomAttack()
{
	CHandBoss_STATES randomStates[] =
	{
		CHandBoss_STATES::STATES_ATTACK_A_0,
		CHandBoss_STATES::STATES_ATTACK_B_0,
		CHandBoss_STATES::STATES_ATTACK_C_0,
		CHandBoss_STATES::STATES_ATTACK_D_0,
		//CHandBoss_STATES::STATES_ATTACK_E_0
		
	};

	random_device randomDevice;
	mt19937_64 Gen(randomDevice());
	uniform_int_distribution<> dis(0, sizeof(randomStates) / sizeof(randomStates[0]) - 1);
	int iRandomIndex = dis(Gen);


	return randomStates[(CHandBoss_STATES)iRandomIndex];
}



void CHandBoss::SetRandomData()
{
	_float fRandX = RandomNum<_float>(-23.f, 23.f);
	_float fRandZ = RandomNum<_float>(1.f, 47.f);

	m_vOriginPos.m128_f32[0] = fRandX;
	m_vOriginPos.m128_f32[2] = fRandZ;



	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	_vector vPlayerPos = pPlayer->Get_PositionVector();



}

HRESULT CHandBoss::Add_PartObjects()
{
	//ColliderDesc.eType = CCollider::TYPE_OBB;
	//ColliderDesc.vExtents = _float3(0.5f, 1.f, 0.5f);
	//ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
	//ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);


	CBoneCollider::CBoneCollider_DESC BoneColliderDesc{};
	BoneColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.001"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.002"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.003"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.004"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.005"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.006"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.007"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.008"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.009"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.010"));
	BoneColliderDesc.pCombinedTransformationMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.011"));
	BoneColliderDesc.pBoundingDesc.eType= CCollider::TYPE_OBB;
	BoneColliderDesc.pBoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	BoneColliderDesc.pBoundingDesc.vCenter = _float3(0.f, BoneColliderDesc.pBoundingDesc.vExtents.y, 0.f);
	if (m_strModelName == TEXT("Boss_C_Left_Hand"))
	{
		BoneColliderDesc.pBoundingDesc.vRotation = _float3(0.f, 0.f, 0.0f);;
		//BoneColliderDesc.pBoundingDesc.vRotation = _float3(XMConvertToRadians(45.f), XMConvertToRadians(45.f),0.f );
	}
	if (m_strModelName == TEXT("Boss_C_Right_Hand"))
	{
		BoneColliderDesc.pBoundingDesc.vRotation = _float3(0.f, 0.f, 0.0f);

	}
	//BoneColliderDesc.pBoundingDesc.vRotation = _float3(0.f, 0.f, 0.f);
	CGameObject* pBoneCollider = nullptr;
	pBoneCollider=m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_BoneCollider"), &BoneColliderDesc);
	if(pBoneCollider==nullptr)
		return E_FAIL;

	m_PartObjects.emplace_back(pBoneCollider);


	m_HandPalm= m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.013");




	//m_szName = 0x000001df84b02ca0 "Bone.L.014"
	//"Bone.L.006"
	//m_vpSocketMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.005"));
	//m_vpSocketMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.008"));
	//m_vpSocketMatrix.push_back(m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.011"));
	//m_pSocketMatrix = m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.L.006");



	return S_OK;
}

CHandBoss* CHandBoss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHandBoss* pInstance = new CHandBoss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CHandBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHandBoss::Clone(void* pArg)
{
	CHandBoss* pInstance = new CHandBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CHandBoss");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CHandBoss::Free()
{
	__super::Free();

	for(auto& pPartObject : m_PartObjects)
		Safe_Release(pPartObject);

	m_PartObjects.clear();

	Safe_Release(m_pRootNode);
}
