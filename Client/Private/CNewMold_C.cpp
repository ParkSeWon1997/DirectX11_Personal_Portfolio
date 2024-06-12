#include "stdafx.h"
#include "CNewMold_C.h"
#include "GameInstance.h"
#include"Loader.h"

#include"CDecorationParts.h"





#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"
#include"CBullet.h"
#include"Player.h"
#include"Particle_Mesh.h"
#include"CMold_CBullet.h"

CNewMold_C::CNewMold_C(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CNewMold_C::CNewMold_C(const CNewMold_C& rhs)
	: CMonster(rhs)
{
}

HRESULT CNewMold_C::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CNewMold_C::Initialize(void* pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_PartObjects()))
		return E_FAIL;


	if (pArg != nullptr)
	{
		_matrix	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(-45.0f));
		_vector temp = XMVector3TransformCoord(XMLoadFloat4(&static_cast<CMonster_DESC*>(pArg)->vPosition), PreTransformMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, temp);
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CMonster_DESC*>(pArg)->vPosition));
	}
	m_pTransformCom->Rotation(XMConvertToRadians(RandomNum<float>(0.0f, 360.f)));

	if (FAILED(Add_PartObjects()))
		return E_FAIL;



	CActionNode* pDoIdleNode = CActionNode::create([this](_float deltaTime) { return DoIdle(deltaTime); });
	CActionNode* pDoMove = CActionNode::create([this](_float deltaTime) { return DoMove(deltaTime); });
	CActionNode* pDoHit = CActionNode::create([this](_float deltaTime) { return DoHit(deltaTime); });
	CActionNode* pDoAttack = CActionNode::create([this](_float deltaTime) { return DoAttack(deltaTime); });
	CActionNode* pDoIsAlive = CActionNode::create([this](_float deltaTime) { return DoIsAlive(); });



	CSelector* pDefalutRoutineSelector = CSelector::Create(pDoAttack);



	m_pRootNode = CSequence::Create(pDoIsAlive, pDoHit, pDoIdleNode, pDoMove, pDefalutRoutineSelector);


	return S_OK;
}

void CNewMold_C::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CNewMold_C::Tick(_float fTimeDelta)
{

	m_HandMatrix= XMLoadFloat4x4(m_pHandMatrix);
	m_HandMatrix.r[0]= XMVector3Normalize(m_HandMatrix.r[0]);
	m_HandMatrix.r[1] = XMVector3Normalize(m_HandMatrix.r[1]);
	m_HandMatrix.r[2] = XMVector3Normalize(m_HandMatrix.r[2]);

	m_pRootNode->Evaluate(fTimeDelta);
	m_fHittime += fTimeDelta;

	if (m_fHp <= 0.0f)
		m_eCurState = CNewMold_C_STATES::STATES_SMASH;



	_bool Isfloor = false;
	if (m_bIsHit)
	{
		//m_pTransformCom->Go_Jump(fTimeDelta, 3.0f, 1.0f, Isfloor);
		m_pTransformCom->LookAt(m_pPlayer->Get_PositionVector());
		m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
		m_eCurState = CNewMold_C_STATES::STATES_HURT;

	}
	if (m_fHittime >= 0.5f)
	{
		m_bIsHit = false;
		m_fHittime = 0.0f;
	}


	__super::Tick(fTimeDelta);
}

void CNewMold_C::Late_Tick(_float fTimeDelta)
{



	__super::Late_Tick(fTimeDelta);
}

HRESULT CNewMold_C::Render()
{
	__super::Render();
	return S_OK;
}

NodeStates CNewMold_C::DoIdle(_float fTimeDelta)
{

	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 fPos = this->Get_Position();

	if (m_eCurState == CNewMold_C_STATES::STATES_IDLE)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_C_STATES::STATES_IDLE, true));



		m_fChangeTime += fTimeDelta;
		if (m_fChangeTime >= 5.0f)
		{
			m_eCurState = GetRandomState();
			m_fChangeTime = 0.0f;
			return NodeStates::SUCCESS;
		}


		return NodeStates::FAILURE;
	}
	else
		return NodeStates::SUCCESS;

}

NodeStates CNewMold_C::DoAttack(_float fTimeDelta)
{
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 fPos = this->Get_Position();

	if (m_eCurState == CNewMold_C_STATES::STATES_ATTACK)
	{
		if (m_pTransformCom->TurnToTarget(fTimeDelta*0.5f, m_pPlayer->Get_PositionVector()))
		{
		
			_bool Isfloor = false;
			m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_C_STATES::STATES_ATTACK, false));
			double fCurPos = m_pModelCom->Get_CurrentPosition();
			if (fCurPos >= 0.02 && fCurPos <= 0.04)
			{
				vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X,TEXT("Mobile_C_Beam_Before_Size_UP_X"),_float4(1.0f,0.0f,0.0f,0.5f)},
				};
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fPos.x, fPos.y + 1.0f, fPos.z, 1.0f), -vLook);

				
				m_fPlayerPos = m_pPlayer->Get_Position();
				//m_pTransformCom->Go_Jump(fTimeDelta, 5.0f, 1.0f, Isfloor);
			}




			if (m_pModelCom->Get_AnimFinished())
			{
				Make_particle_Bullet(m_fPlayerPos);

				m_eCurState = CNewMold_C_STATES::STATES_IDLE;
				return NodeStates::SUCCESS;
			}
			else
				return NodeStates::RUNNING;
		}
		else
			return NodeStates::RUNNING;
	}
	else
	{

		return NodeStates::FAILURE;
	}
}

NodeStates CNewMold_C::DoMove(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_eCurState == CNewMold_C_STATES::STATES_MOVE)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_C_STATES::STATES_MOVE, true));
		m_pTransformCom->Go_Straight(fTimeDelta * 0.5f, m_pNavigationCom);
		if (m_fTime >= 3.0f)
		{
			_float fRandomAngle = RandomNum<float>(0.0f, 360.f);
			m_pTransformCom->QuatRotation(m_pTransformCom->Get_State(CTransform::STATE_UP), fRandomAngle);
			m_eCurState = CNewMold_C_STATES::STATES_IDLE;
			m_fTime = 0.0f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;


	}
	else
		return NodeStates::SUCCESS;

}

NodeStates CNewMold_C::DoHit(_float fTimeDelta)
{
	_bool Isfloor = false;
	if (m_eCurState== CNewMold_C_STATES::STATES_HURT)
	{
		m_eCurState = CNewMold_C_STATES::STATES_HURT;

		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_C_STATES::STATES_HURT, false));


		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CNewMold_C_STATES::STATES_IDLE;
			m_bIsHit = false;
		}
		else
		{
			return NodeStates::RUNNING;
		}

	}
	else
		return NodeStates::SUCCESS;

}

NodeStates CNewMold_C::DoIsAlive()
{
	if (m_eCurState == CNewMold_C_STATES::STATES_SMASH)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_C_STATES::STATES_SMASH, false));
		if (m_pModelCom->Get_AnimFinished())
		{
			m_bIsDead = true;
			return NodeStates::FAILURE;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		return NodeStates::SUCCESS;

}

CNewMold_C::CNewMold_C_STATES CNewMold_C::GetRandomState()
{
	CNewMold_C_STATES pRandomStates[] =
	{
		CNewMold_C_STATES::STATES_MOVE,
		CNewMold_C_STATES::STATES_ATTACK,

	};

	random_device randomDevice;
	mt19937_64 Gen(randomDevice());
	uniform_int_distribution<> dis(0, sizeof(pRandomStates) / sizeof(pRandomStates[0]) - 1);
	int iRandomIndex = dis(Gen);



	return pRandomStates[(CNewMold_C_STATES)iRandomIndex];
}

void CNewMold_C::Make_particle_Bullet(_float3 PlayerPos)
{
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 fPos = this->Get_Position();
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Mobile_C_Beam_After_Pop"),_float4(1.0f,0.2f,0.1f,0.8f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Mobile_C_LowpolySphere16_Size_Up"),_float4(1.0f,0.647f,0.0f,0.8f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Mobile_C_ShotRing_Size_Up"),_float4(1.0f,1.0f,0.1f,0.8f)},


	};
	CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fPos.x, fPos.y+1.0f, fPos.z, 1.0f),-vLook);

	CBullet::CBullet_DESC BulletDesc{};
	BulletDesc.strModelName = TEXT("Attack_MiniBoss_C_B");
	BulletDesc.fSpeedPerSec = 10.f;
	BulletDesc.vPosition = _float4(fPos.x, fPos.y+1.0f, fPos.z, 1.0f);
	BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
	BulletDesc.BulletState = &CBullet::Go;
	BulletDesc.fDamage = 10.0f;

	m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_CMold_Bullet"), &BulletDesc);



}



HRESULT CNewMold_C::Add_Components()
{
	/* For.Com_Collider_AABB */
	//CBounding_AABB::BOUNDING_AABB_DESC		ColliderAABBDesc{};
	//
	//ColliderAABBDesc.eType = CCollider::TYPE_AABB;
	//ColliderAABBDesc.vExtents = _float3(1.f, 0.7f, 0.3f);
	//ColliderAABBDesc.vCenter = _float3(0.f, ColliderAABBDesc.vExtents.y, 0.f);
	//
	//
	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
	//	TEXT("Com_Collider_AABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_AABB]), &ColliderAABBDesc)))
	//	return E_FAIL;

	/* For.Com_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		ColliderOBBDesc{};

	ColliderOBBDesc.eType = CCollider::TYPE_OBB;
	ColliderOBBDesc.vExtents = _float3(1.3f, 0.7f, 1.3f);
	ColliderOBBDesc.vCenter = _float3(0.f, ColliderOBBDesc.vExtents.y, 0.f);
	ColliderOBBDesc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_OBB]), &ColliderOBBDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_SPHERE;
	ColliderDesc.fRadius = 5.5f;
	ColliderDesc.vCenter = _float3(0.f, 0.0f, 0.f);

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_SPHERE"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_SPHERE]), &ColliderDesc)))
		return E_FAIL;












	if (FAILED(__super::Add_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CNewMold_C::Add_PartObjects()
{


	m_pHandMatrix= m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.005");

	return S_OK;
}

CNewMold_C* CNewMold_C::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNewMold_C* pInstance = new CNewMold_C(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CNewMold_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CNewMold_C::Clone(void* pArg)
{
	CNewMold_C* pInstance = new CNewMold_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CNewMold_C");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CNewMold_C::Free()
{
	__super::Free();


	Safe_Release(m_pRootNode);
}







