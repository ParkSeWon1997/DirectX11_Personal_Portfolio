#include "stdafx.h"
#include "..\Public\CSwordThow.h"

#include "GameInstance.h"
#include"Loader.h"


CSwordThow::CSwordThow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSwordThow::CSwordThow(const CSwordThow& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSwordThow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSwordThow::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
	
		static_cast<CSwordThow_DESC*>(pArg)->m_pWorldMatrix.r[3];
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CSwordThow_DESC*>(pArg)->vPosition));
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, static_cast<CSwordThow_DESC*>(pArg)->m_pWorldMatrix.r[3]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, static_cast<CSwordThow_DESC*>(pArg)->vDir);
		m_pTransformCom->Set_RotationSpeed(100.0f);

	}


	return S_OK;
}

void CSwordThow::Priority_Tick(_float fTimeDelta)
{
	


}

void CSwordThow::Tick(_float fTimeDelta)
{
	m_fLifeTime += fTimeDelta;
	if(m_fLifeTime>10.0f)
		m_bIsDead = true;

	m_pTransformCom->Go_Straight(fTimeDelta);

	
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CSwordThow::Late_Tick(_float fTimeDelta)
{
	

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
	
}

HRESULT CSwordThow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		//	return E_FAIL;


		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	if (m_bIsDead)
	{
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), this);
	}


	return S_OK;
}

HRESULT CSwordThow::Add_Components()
{
	//TEXT("Prototype_Component_Model_ForkLift")
	//m_strModelName
	/* For.Com_Model */


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

HRESULT CSwordThow::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

_bool CSwordThow::Intersect(const wstring& strComponetTag, CCollider* pTargetCollider)
{

	CCollider* pPartObjCollider = dynamic_cast<CCollider*>(this->Get_Component(strComponetTag));
	if (nullptr == pPartObjCollider)
		return false;

	if (pTargetCollider->Intersect(pPartObjCollider))
	{
		return true;
	}
	else
	{
		return false;
	}
}

CSwordThow* CSwordThow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSwordThow* pInstance = new CSwordThow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSwordThow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSwordThow::Clone(void* pArg)
{
	CSwordThow* pInstance = new CSwordThow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSwordThow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwordThow::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
