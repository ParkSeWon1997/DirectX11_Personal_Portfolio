#include "stdafx.h"
#include "CHandBullet.h"
#include "GameInstance.h"
#include"Loader.h"

#include"Player.h"

#include"Particle_Mesh.h"
CHandBullet::CHandBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CHandBullet::CHandBullet(const CBullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CHandBullet::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CHandBullet::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (pArg != nullptr)
	{
		
		m_pTransformCom->Rotation(static_cast<CBullet_DESC*>(pArg)->fRotatedAngle);
		
	}
	//if (FAILED(Add_Components()))
	//	return E_FAIL;
	//if (pArg != nullptr)
	//{
	//
	//	static_cast<CBullet_DESC*>(pArg)->m_pWorldMatrix.r[3];
	//
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CBullet_DESC*>(pArg)->vPosition));
	//	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, static_cast<CBullet_DESC*>(pArg)->m_pWorldMatrix.r[3]);
	//	m_pTransformCom->Set_State(CTransform::STATE_LOOK, static_cast<CBullet_DESC*>(pArg)->vDir);
	//
	//}


	return S_OK;
}

void CHandBullet::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CHandBullet::Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (pPlayer == nullptr)
		return;

	_vector vPlayerPos = pPlayer->Get_PositionVector();

	__super::Tick(fTimeDelta);

}

void CHandBullet::Late_Tick(_float fTimeDelta)
{
	if (this->Get_Position().y <= 0.0f)
	{
		m_bIsDead = true;
	}


	if (m_pColliderCom == nullptr)
		return;
			
	__super::Late_Tick(fTimeDelta);
}

HRESULT CHandBullet::Render()
{
	if (m_bIsDead)
	{
		_vector vPosBeforeDie = this->Get_PositionVector();
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), this);
		vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION,TEXT("Effect_ExplosionDust_Explosion")},
		};

		CParticle_Mesh::Make_Particle(vecDesc, vPosBeforeDie);
	}


	__super::Render();
	return S_OK;
}


HRESULT CHandBullet::Add_Components()
{
	CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_SPHERE;
	ColliderDesc.fRadius = 0.5f;
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;



	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_Vtx_HandBullet"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHandBullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CHandBullet* CHandBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHandBullet* pInstance = new CHandBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CHandBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHandBullet::Clone(void* pArg)
{
	CHandBullet* pInstance = new CHandBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CHandBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CHandBullet::Free()
{
	__super::Free();
}
