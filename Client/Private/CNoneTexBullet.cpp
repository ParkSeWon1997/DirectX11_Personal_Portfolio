#include "stdafx.h"
#include "CNoneTexBullet.h"
#include "GameInstance.h"
#include"Loader.h"

#include"Player.h"

#include"Particle_Mesh.h"
CNoneTexBullet::CNoneTexBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CNoneTexBullet::CNoneTexBullet(const CBullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CNoneTexBullet::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CNoneTexBullet::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (pArg != nullptr)
	{

		
	}


	return S_OK;
}

void CNoneTexBullet::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CNoneTexBullet::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CNoneTexBullet::Late_Tick(_float fTimeDelta)
{
	//if (this->Get_Position().y <= 0.0f)
	//{
	//	m_bIsDead = true;
	//}


	if (m_pColliderCom == nullptr)
		return;
			
	__super::Late_Tick(fTimeDelta);
}

HRESULT CNoneTexBullet::Render()
{
	if (m_bIsDead)
	{
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), this);
	
	}


	__super::Render();
	return S_OK;
}


HRESULT CNoneTexBullet::Add_Components()
{
	CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_SPHERE;
	ColliderDesc.fRadius = 0.8f;
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;



	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CNoneTexBullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CNoneTexBullet* CNoneTexBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNoneTexBullet* pInstance = new CNoneTexBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CNoneTexBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNoneTexBullet::Clone(void* pArg)
{
	CNoneTexBullet* pInstance = new CNoneTexBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CNoneTexBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CNoneTexBullet::Free()
{
	__super::Free();
}
