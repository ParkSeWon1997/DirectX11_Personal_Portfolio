#include "stdafx.h"
#include "CMiniBoss.h"
#include "GameInstance.h"
#include"Loader.h"

#include"CDecorationParts.h"





#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"

#include"Player.h"


CMiniBoss::CMiniBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CMiniBoss::CMiniBoss(const CMiniBoss& rhs)
	: CMonster(rhs)
{
}

HRESULT CMiniBoss::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMiniBoss::Initialize(void* pArg)
{


	if (FAILED(__super::Initialize(pArg)))
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

	CInverter* pNot = CInverter::Create(pDoMove);




	CSelector* pDefalutRoutineSelector = CSelector::Create(pDoIsAlive,pNot, pDoHit, pDoAttack, pDoIdleNode);





	//vector<CNode*> pRootNodes;
	//pRootNodes.push_back(pDoIdleNode);
	//
	//
	m_pRootNode = CSequence::Create(pDefalutRoutineSelector);


	return S_OK;
}

void CMiniBoss::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CMiniBoss::Tick(_float fTimeDelta)
{


	m_pRootNode->Evaluate(fTimeDelta);


	__super::Tick(fTimeDelta);
}

void CMiniBoss::Late_Tick(_float fTimeDelta)
{





	__super::Late_Tick(fTimeDelta);
}

HRESULT CMiniBoss::Render()
{
	__super::Render();
	return S_OK;
}



HRESULT CMiniBoss::Add_Components()
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

HRESULT CMiniBoss::Add_PartObjects()
{




	return S_OK;
}

CMiniBoss* CMiniBoss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiniBoss* pInstance = new CMiniBoss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMiniBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CMiniBoss::Clone(void* pArg)
{
	CMiniBoss* pInstance = new CMiniBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMiniBoss");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CMiniBoss::Free()
{
	__super::Free();



	Safe_Release(m_pRootNode);
}






NodeStates CMiniBoss::DoIdle(_float fTimeDelta)
{
	if (m_eCurState == CMiniBoss_STATES::STATES_IDLE)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CMiniBoss_STATES::STATES_IDLE, true));
		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;



}

NodeStates CMiniBoss::DoAttack(_float fTimeDelta)
{

	if (m_eCurState == CMiniBoss_STATES::STATES_ATTACK)
	{
		//_bool Isfloor = false;
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CMiniBoss_STATES::STATES_ATTACK, false));






		if (m_pModelCom->Get_AnimFinished())
		{
			//m_pTransformCom->Go_Jump(fTimeDelta, 5.0f, Isfloor);
			m_eCurState = CMiniBoss_STATES::STATES_IDLE;
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

NodeStates CMiniBoss::DoMove(_float fTimeDelta)
{

	if (DoDetact(fTimeDelta))
		return NodeStates::SUCCESS;





	return NodeStates::FAILURE;
}

NodeStates CMiniBoss::DoHit(_float fTimeDelta)
{
	_bool Isfloor = false;
	if (m_bIsHit)
	{
		m_pTransformCom->Go_Jump(fTimeDelta, 5.0f, 1.0f, Isfloor);
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CMiniBoss_STATES::STATES_HURT, true));
		m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
		if (Isfloor)
		{

			m_eCurState = CMiniBoss_STATES::STATES_IDLE;
			m_bIsHit = false;

		}




		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;

}





_bool CMiniBoss::DoDetact(_float fTimeDelta)
{

	m_fTime += fTimeDelta;
	random_device randomDevice;
	mt19937_64 Gen(randomDevice());

	if (m_bIsDetact == false)
	{
		uniform_real_distribution<float> fRandomRotateTime(3.f, 15.f);
		_float fRotateTime = fRandomRotateTime(Gen);
		if (m_fTime >= fRotateTime)
		{
			uniform_real_distribution<float> RandomDist(0.f, 360.f);

			_float fRandomAngle = RandomDist(Gen);

			//mt19937_64; << 굳이 써야할까?  정말 더 랜덤한 각도를 주려면 해봐도 될 듯
			//mt19937; << 이거 쓰면 충분할 듯

			//m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CMiniBoss_STATES::STATES_MOVE, true));
			m_pTransformCom->QuatRotation(m_pTransformCom->Get_State(CTransform::STATE_UP), fRandomAngle);
			m_fTime = 0;
			//m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

		}
		uniform_real_distribution<float> fRandomGoTime(1.f, 6.f);
		_float fGoTime = fRandomGoTime(Gen);
		if (m_fTime >= 1.0f)
		{
			m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CMiniBoss_STATES::STATES_MOVE, true));
			m_pTransformCom->Go_Straight(fTimeDelta * 0.5f, m_pNavigationCom);
			return false;
		}

	}
	else if (m_bIsDetact == true)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
		if (nullptr == pPlayer)
			assert("No Player address");

		_float  fDistance = 0.f;

		_vector	vPlayerPos = pPlayer->Get_PositionVector();
		_vector	vNewMoldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		fDistance = XMVectorGetX(XMVector3Length(vPlayerPos - vNewMoldPos));

		if (fDistance <= 4.0f)
			m_eCurState = CMiniBoss_STATES::STATES_ATTACK;
		else
		{
			m_pTransformCom->LookAt(pPlayer->Get_PositionVector());
			m_pTransformCom->Go_Straight(fTimeDelta * 0.5f, m_pNavigationCom);
		}

		//m_eCurState = CMiniBoss_STATES::STATES_ATTACK;
		return true;
	}










}

NodeStates CMiniBoss::DoIsAlive()
{
	if (m_bIsDead)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CMiniBoss_STATES::STATES_SMASH, false));
		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;

}


