#include "stdafx.h"
#include "CGravityBullet.h"

#include "GameInstance.h"
#include"Loader.h"
#include"Particle_Mesh.h"

CGravityBullet::CGravityBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CGravityBullet::CGravityBullet(const CGravityBullet& rhs)
	:CBullet(rhs)
{
}

HRESULT CGravityBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGravityBullet::Initialize(void* pArg)
{
	m_eColliderType=static_cast<CGravityBULLET_DESC*>(pArg)->eColliderType;
	m_fRadius = static_cast<CGravityBULLET_DESC*>(pArg)->fRadius;
	m_vCenter = static_cast<CGravityBULLET_DESC*>(pArg)->vCenter;
	m_vExtents= static_cast<CGravityBULLET_DESC*>(pArg)->vExtents;
	m_vRotation = static_cast<CGravityBULLET_DESC*>(pArg)->vRotation;

	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_bParticleCreate = ((CGravityBULLET_DESC*)pArg)->bParticleCreate;
		m_eParticleType = ((CGravityBULLET_DESC*)pArg)->eParticleType;
		m_pTransformCom->Set_Look(((CGravityBULLET_DESC*)pArg)->vDir);
		m_fSpeed= ((CGravityBULLET_DESC*)pArg)->fSpeedPerSec;
		m_fMaxLifeTime = ((CGravityBULLET_DESC*)pArg)->fLifeTime;
		m_vTargetPos = ((CGravityBULLET_DESC*)pArg)->vTargetPos;
		m_pMatPlayerWorld = ((CGravityBULLET_DESC*)pArg)->pMatPlayerWorld;
		m_eCharacterType = ((CGravityBULLET_DESC*)pArg)->eCharacterType;
		m_fInitSpeed= ((CGravityBULLET_DESC*)pArg)->fInitSpeed;
		m_fDamage = ((CGravityBULLET_DESC*)pArg)->fDamage;
	}
	
	
	return S_OK;
}

void CGravityBullet::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CGravityBullet::Tick(_float fTimeDelta)
{
	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > m_fMaxLifeTime)
		m_bIsDead = true;




	__super::Tick(fTimeDelta);
}

void CGravityBullet::Late_Tick(_float fTimeDelta)
{
	if (m_pColliderCom == nullptr)
		return;




	__super::Late_Tick(fTimeDelta);
}

HRESULT CGravityBullet::Render()
{
	
	if (m_bIsDead)
	{
		
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_GravityBullet"), this);

	}


	return S_OK;
}

HRESULT CGravityBullet::Add_Components()
{
	/* For.Com_Collider */
	//CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDescSphre{};
	//ColliderDescSphre.eType = m_eColliderType;
	//ColliderDescSphre.fRadius = m_fRadius;
	//ColliderDescSphre.vCenter = m_vCenter;



	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), MakeColliderDesc(m_eColliderType))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGravityBullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}




void CGravityBullet::Pop(_float fTimeDelta)
{
	
}



CCollider::COLLIDER_DESC* CGravityBullet::MakeColliderDesc(CCollider::TYPE eTpye)
{
	



	if (eTpye == CCollider::TYPE::TYPE_AABB)
	{
		ColliderDescAABB.eType = eTpye;
		ColliderDescAABB.vCenter = m_vCenter;
		ColliderDescAABB.vExtents = m_vExtents;
		
		return &ColliderDescAABB;
	}
	if(eTpye==CCollider::TYPE::TYPE_OBB)
	{
		ColliderDescOBB.eType = eTpye;
		ColliderDescOBB.vExtents = m_vExtents;
		ColliderDescOBB.vCenter = m_vCenter;
		ColliderDescOBB.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
		
		return &ColliderDescOBB;
	}
	if(eTpye==CCollider::TYPE::TYPE_SPHERE)
	{
		ColliderDescSphre.eType = eTpye;
		ColliderDescSphre.fRadius = m_fRadius;
		ColliderDescSphre.vCenter = m_vCenter;
		
		return &ColliderDescSphre;
	}


	

	
}

CGravityBullet* CGravityBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGravityBullet* pInstance = new CGravityBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CGravityBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGravityBullet::Clone(void* pArg)
{
	CGravityBullet* pInstance = new CGravityBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CGravityBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGravityBullet::Free()
{
	__super::Free();
}
