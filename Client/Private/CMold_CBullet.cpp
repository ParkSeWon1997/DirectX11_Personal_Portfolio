#include "stdafx.h"
#include "CMold_CBullet.h"

#include "GameInstance.h"
#include"Loader.h"

CMold_CBullet::CMold_CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CMold_CBullet::CMold_CBullet(const CMold_CBullet& rhs)
	:CBullet(rhs)
{
}

HRESULT CMold_CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMold_CBullet::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{


	}
	return S_OK;
}

void CMold_CBullet::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CMold_CBullet::Tick(_float fTimeDelta)
{
	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > 0.5f)
		m_bIsDead = true;


	__super::Tick(fTimeDelta);
}

void CMold_CBullet::Late_Tick(_float fTimeDelta)
{
	if (m_pColliderCom == nullptr)
		return;


	__super::Late_Tick(fTimeDelta);
}

HRESULT CMold_CBullet::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	if (m_bIsDead)
	{

		if (FAILED(m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), this)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CMold_CBullet::Add_Components()
{
	/* For.Com_Collider */
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
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMold_CBullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

void CMold_CBullet::Go(_float fTimeDelta)
{

	m_pTransformCom->Go_Straight(fTimeDelta);



}

CMold_CBullet* CMold_CBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMold_CBullet* pInstance = new CMold_CBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMold_CBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMold_CBullet::Clone(void* pArg)
{
	CMold_CBullet* pInstance = new CMold_CBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMold_CBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMold_CBullet::Free()
{
	__super::Free();
}
