#include "stdafx.h"
#include"CDecorationParts.h"

#include "GameInstance.h"
#include"Loader.h"



CDecorationParts::CDecorationParts(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CDecorationParts::CDecorationParts(const CDecorationParts& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CDecorationParts::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDecorationParts::Initialize(void* pArg)
{
	DECORATION_DESC* pDesc = static_cast<DECORATION_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pCombinedTransformationMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->vPosition));
		m_pTransformCom->Scaling(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&pDesc->vRotation), pDesc->vRotationAngle);

	}
	//m_pTransformCom->Scaling(1.1f, 1.1f, 1.1f);
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));


	return S_OK;
}

void CDecorationParts::Priority_Tick(_float fTimeDelta)
{
}

void CDecorationParts::Tick(_float fTimeDelta)
{
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

	//m_pColliderCom->Tick(XMLoadFloat4x4(&m_WorldMatrix));


}

void CDecorationParts::Late_Tick(_float fTimeDelta)
{



	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CDecorationParts::Render()
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
#ifdef _DEBUG
		//m_pColliderCom->Render();
#endif


	return S_OK;
}

HRESULT CDecorationParts::Add_Components()
{
	/* For.Com_Collider */
	//CBounding_OBB::BOUNDING_OBB_DESC		ColliderDesc{};
	//
	//ColliderDesc.eType = CCollider::TYPE_OBB;
	//ColliderDesc.vExtents = _float3(1.5f, 2.f, 2.5f);
	//ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
	//ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	//
	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	//	return E_FAIL;


	/* For.Com_Model */
	/*Prototype_Component_Model_ForkLift*/

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	

	return S_OK;
}

HRESULT CDecorationParts::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CDecorationParts* CDecorationParts::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDecorationParts* pInstance = new CDecorationParts(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CDecorationParts");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDecorationParts::Clone(void* pArg)
{
	CDecorationParts* pInstance = new CDecorationParts(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CDecorationParts");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDecorationParts::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
