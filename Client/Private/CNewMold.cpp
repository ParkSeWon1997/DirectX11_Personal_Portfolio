#include "stdafx.h"
#include "CNewMold.h"
#include "GameInstance.h"
#include"Loader.h"

#include"CDecorationParts.h"





#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"

#include"Player.h"

#include"Particle_Mesh.h"
#include"CBullet.h"
CNewMold::CNewMold(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CNewMold::CNewMold(const CNewMold& rhs)
	: CMonster(rhs)

{
}

HRESULT CNewMold::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CNewMold::Initialize(void* pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (pArg != nullptr)
	{
		_matrix	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(-45.0f));
	_vector temp=	XMVector3TransformCoord(XMLoadFloat4(&static_cast<CMonster_DESC*>(pArg)->vPosition), PreTransformMatrix);
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

	

	CSelector* pDefalutRoutineSelector = CSelector::Create(pDoAttack);



	m_pRootNode=CSequence::Create(pDoIsAlive,pDoHit,pDoIdleNode, pDoMove,pDefalutRoutineSelector);

	
	return S_OK;
}

void CNewMold::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CNewMold::Tick(_float fTimeDelta)
{
	

	//m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	m_pRootNode->Evaluate(fTimeDelta);

	

	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject->Tick(fTimeDelta);
	}


	
	if(m_fHp<=0.0f)
		m_eCurState = CNewMold_STATES::STATES_SMASH;
	


	_bool Isfloor = false;
	if (m_bIsHit)
	{
		m_pTransformCom->Go_Jump(fTimeDelta, 3.0f, 1.0f, Isfloor);
		m_pTransformCom->LookAt(m_pPlayer->Get_PositionVector());
		m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);

	}
	if (Isfloor)
	{
		m_bIsHit = false;
	}

	__super::Tick(fTimeDelta);
}

void CNewMold::Late_Tick(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		pPartObject->Late_Tick(fTimeDelta);

	}




	__super::Late_Tick(fTimeDelta);
}

HRESULT CNewMold::Render()
{
	__super::Render();
	return S_OK;
}



HRESULT CNewMold::Add_Components()
{
	/* For.Com_Collider_AABB */
	//CBounding_AABB::BOUNDING_AABB_DESC		ColliderAABBDesc{};
	//
	//ColliderAABBDesc.eType = CCollider::TYPE_AABB;
	//ColliderAABBDesc.vExtents = _float3(1.f, 0.7f, 0.3f);
	//ColliderAABBDesc.vCenter = _float3(0.f, ColliderAABBDesc.vExtents.y, 0.f);
	//
	//
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
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

HRESULT CNewMold::Add_PartObjects()
{
//Bone.HeadDeco.b
//Bone.HeadDeco.fr
//Bone.HeadDeco.l
//Bone.HeadDeco.r
//m_szName = 0x000002a4afdbf2b0 "Bone.HeadDeco.b"

	CDecorationParts::DECORATION_DESC		DecorationDesc{};
	DecorationDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	DecorationDesc.pCombinedTransformationMatrix = m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.Top");
	DecorationDesc.strModelName = m_strDeconModelTag;
	DecorationDesc.vScale = _float3(1.f, 1.f, 1.f);
	//DecorationDesc.vPosition = _float4(0.f, 0.f, 0.f,1.0f);
	DecorationDesc.vRotation = _float3(1.f, 0.f, 0.f);
	//DecorationDesc.vRotationAngle = 90.0f;
	if(nullptr == DecorationDesc.pCombinedTransformationMatrix)
		return E_FAIL;

	CGameObject* pDecoration = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Decoration"), &DecorationDesc);
	if (nullptr == pDecoration)
		return E_FAIL;
	m_PartObjects.emplace_back(pDecoration);



	return S_OK;
}

CNewMold* CNewMold::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNewMold* pInstance = new CNewMold(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CNewMold");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CNewMold::Clone(void* pArg)
{
	CNewMold* pInstance = new CNewMold(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CNewMold");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CNewMold::Free()
{
	__super::Free();


	for (auto& pPartObject : m_PartObjects)
		Safe_Release(pPartObject);

	m_PartObjects.clear();

	Safe_Release(m_pRootNode);
}






NodeStates CNewMold::DoIdle(_float fTimeDelta)
{
	if (m_eCurState == CNewMold_STATES::STATES_IDLE)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_STATES::STATES_IDLE, true));



		m_fChangeTime += fTimeDelta;
		if (m_fChangeTime >= 3.0f)
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

NodeStates CNewMold::DoAttack(_float fTimeDelta)
{

	if (m_eCurState == CNewMold_STATES::STATES_ATTACK)
	{
		_bool Isfloor = false;
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_STATES::STATES_ATTACK, false));
		double fCurPos = m_pModelCom->Get_CurrentPosition();
		if(fCurPos>=0.02&&fCurPos<=0.04)
		{
			vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Effect_BossAttack_Warning_Circle_001_SizeUp"),_float4(0.8f,0.1f,0.1f,0.8f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Effect_BossAttack_Warning_Circle_002_SizeUp"),_float4(1.0f,0.1f,0.1f,0.8f)}
			};

			m_fPlayerPos =m_pPlayer->Get_Position();

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(m_fPlayerPos.x, 0.0f, m_fPlayerPos.z, 1.0f));

		m_pTransformCom->Go_Jump(fTimeDelta, 5.0f, 1.0f, Isfloor);
		}

	
		

		if (m_pModelCom->Get_AnimFinished())
		{
			Make_particle_Bullet(m_fPlayerPos);

			m_eCurState = CNewMold_STATES::STATES_IDLE;
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

CNewMold::CNewMold_STATES CNewMold::GetRandomState()
{

	CNewMold_STATES pRandomStates[] =
	{
		CNewMold_STATES::STATES_MOVE,
		CNewMold_STATES::STATES_ATTACK,

	};

	random_device randomDevice;
	mt19937_64 Gen(randomDevice());
	uniform_int_distribution<> dis(0, sizeof(pRandomStates) / sizeof(pRandomStates[0]) - 1);
	int iRandomIndex = dis(Gen);



	return pRandomStates[(CNewMold_STATES)iRandomIndex];
	
}

void CNewMold::Make_particle_Bullet(_float3 PlayerPos)
{
	
		vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_TEXTURE,TEXT("Boss_Attack_A")},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.7f,0.7f,0.7f,0.8f)},
				{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down"),_float4(0.3f,0.3f,0.3f,0.8f)},
				//{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop")},
				//{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop")},

		};
		CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(PlayerPos.x, 0.0f, PlayerPos.z, 1.0f));

		CBullet::CBullet_DESC BulletDesc{};
		BulletDesc.strModelName = TEXT("NoneTex");
		BulletDesc.fSpeedPerSec = 40.f;
		BulletDesc.vPosition = _float4(PlayerPos.x, 0.0f, PlayerPos.z, 1.0f);
		BulletDesc.vDir = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
		BulletDesc.BulletState = &CBullet::Pop;
		BulletDesc.fDamage = 0.5f;
		m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Bullet"), TEXT("Prototype_GameObject_NoneTex_Bullet"), &BulletDesc);


}

NodeStates CNewMold::DoMove(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_eCurState == CNewMold_STATES::STATES_MOVE)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_STATES::STATES_MOVE, true));
		m_pTransformCom->Go_Straight(fTimeDelta*0.5f, m_pNavigationCom);
		if (m_fTime >= 3.0f)
		{
			_float fRandomAngle= RandomNum<float>(0.0f, 360.f);
			m_pTransformCom->QuatRotation(m_pTransformCom->Get_State(CTransform::STATE_UP), fRandomAngle);
			m_eCurState = CNewMold_STATES::STATES_IDLE;
			m_fTime = 0.0f;
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
		

	}
	else
		return NodeStates::SUCCESS;



}

NodeStates CNewMold::DoHit(_float fTimeDelta)
{
	_bool Isfloor = false;
	if (m_bIsHit)
	{
		m_eCurState = CNewMold_STATES::STATES_HURT;
	
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_STATES::STATES_HURT, false));
	
	
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CNewMold_STATES::STATES_IDLE;
			//m_bIsHit = false;
		}
		else
		{
			return NodeStates::RUNNING;
		}

	}
	else
		return NodeStates::SUCCESS;

}


NodeStates CNewMold::DoIsAlive()
{
	if (m_eCurState== CNewMold_STATES::STATES_SMASH)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(CNewMold_STATES::STATES_SMASH, false));
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


