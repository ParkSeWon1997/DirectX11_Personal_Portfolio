#include "stdafx.h"
#include "CNewMold_D.h"
#include "GameInstance.h"
#include"Loader.h"

#include"CDecorationParts.h"





#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"

#include"Player.h"


CNewMold_D::CNewMold_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CNewMold_D::CNewMold_D(const CNewMold_D& rhs)
	: CMonster(rhs)
{
}

HRESULT CNewMold_D::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CNewMold_D::Initialize(void* pArg)
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
	CActionNode* pDoHit= CActionNode::create([this](_float deltaTime) { return DoHit(deltaTime); });
	CActionNode* pDoAttack= CActionNode::create([this](_float deltaTime) { return DoAttack(deltaTime); });
	CActionNode* pDoIsAlive = CActionNode::create([this](_float deltaTime) { return DoIsAlive(); });
	
	CInverter* pNot = CInverter::Create(pDoMove);
	
	


	CSelector* pDefalutRoutineSelector = CSelector::Create(pDoIsAlive,pNot, pDoHit, pDoAttack, pDoIdleNode);





	//vector<CNode*> pRootNodes;
	//pRootNodes.push_back(pDoIdleNode);
	//
	//
	m_pRootNode=CSequence::Create(pDefalutRoutineSelector);


	return S_OK;
}

void CNewMold_D::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CNewMold_D::Tick(_float fTimeDelta)
{
	

	//m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	m_pRootNode->Evaluate(fTimeDelta);
	int tmep = 0;
	tmep = m_pModelCom->Get_AnimationCount();
	
	__super::Tick(fTimeDelta);
}

void CNewMold_D::Late_Tick(_float fTimeDelta)
{





	__super::Late_Tick(fTimeDelta);
}

HRESULT CNewMold_D::Render()
{
	__super::Render();
	return S_OK;
}



HRESULT CNewMold_D::Add_Components()
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

HRESULT CNewMold_D::Add_PartObjects()
{



	return S_OK;
}

CNewMold_D* CNewMold_D::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNewMold_D* pInstance = new CNewMold_D(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CNewMold_D");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CNewMold_D::Clone(void* pArg)
{
	CNewMold_D* pInstance = new CNewMold_D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CNewMold_D");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CNewMold_D::Free()
{
	__super::Free();



	Safe_Release(m_pRootNode);
}






NodeStates CNewMold_D::DoIdle(_float fTimeDelta)
{
	if (m_eCurState == CNewMold_D_STATES::STATES_IDLE)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_D_STATES::STATES_IDLE, true));
		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;


	
}

NodeStates CNewMold_D::DoAttack(_float fTimeDelta)
{

	if (m_eCurState == CNewMold_D_STATES::STATES_ATTACK)
	{
		//_bool Isfloor = false;
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_D_STATES::STATES_ATTACK, false));
		





		if (m_pModelCom->Get_AnimFinished())
		{
			//m_pTransformCom->Go_Jump(fTimeDelta, 5.0f, Isfloor);
			m_eCurState = CNewMold_D_STATES::STATES_IDLE;
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

NodeStates CNewMold_D::DoMove(_float fTimeDelta)
{

	if(DoDetact(fTimeDelta))
		return NodeStates::SUCCESS;





	return NodeStates::FAILURE;
}

NodeStates CNewMold_D::DoHit(_float fTimeDelta)
{
	_bool Isfloor = false;
	if (m_bIsHit)
	{
		m_pTransformCom->Go_Jump(fTimeDelta, 5.0f,1.0f, Isfloor);
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_D_STATES::STATES_HURT, true));
		m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
		if (Isfloor)
		{
			
				m_eCurState = CNewMold_D_STATES::STATES_IDLE;
				m_bIsHit = false;
			
		}
		



		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;

}




_bool CNewMold_D::DoDetact(_float fTimeDelta)
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

			//mt19937_64; << ���� ����ұ�?  ���� �� ������ ������ �ַ��� �غ��� �� ��
			//mt19937; << �̰� ���� ����� ��
			
			//m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_D_STATES::STATES_MOVE, true));
			m_pTransformCom->QuatRotation(m_pTransformCom->Get_State(CTransform::STATE_UP), fRandomAngle);
			m_fTime = 0;
			//m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
			
		}
		uniform_real_distribution<float> fRandomGoTime(1.f, 6.f);
		_float fGoTime = fRandomGoTime(Gen);
		if (m_fTime >= 1.0f)
		{
			m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_D_STATES::STATES_MOVE, true));
			m_pTransformCom->Go_Straight(fTimeDelta*0.5f, m_pNavigationCom);
			return false;
		}
		
	}
	else if (m_bIsDetact == true)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
		if (nullptr == pPlayer)
			assert("No Player address");

		_float  fDistance = 0.f;
		
		_vector	vPlayerPos =pPlayer->Get_PositionVector();
		_vector	vNewMoldPos=m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		fDistance= XMVectorGetX(XMVector3Length(vPlayerPos - vNewMoldPos));

	if (fDistance <= 4.0f)
		m_eCurState = CNewMold_D_STATES::STATES_ATTACK;
	else
	{
		m_pTransformCom->LookAt(pPlayer->Get_PositionVector());
		m_pTransformCom->Go_Straight(fTimeDelta * 0.5f, m_pNavigationCom);
	}

		//m_eCurState = CNewMold_D_STATES::STATES_ATTACK;
		return true;
	}

	


	





}

NodeStates CNewMold_D::DoIsAlive()
{


	if (m_bIsDead)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_D_STATES::STATES_SMASH, false));

		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;

}


