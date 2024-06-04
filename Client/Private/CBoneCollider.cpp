#include "stdafx.h"
#include"CBoneCollider.h"

#include "GameInstance.h"
#include"Loader.h"


#include "Player.h"

CBoneCollider::CBoneCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBoneCollider::CBoneCollider(const CBoneCollider& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CBoneCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoneCollider::Initialize(void* pArg)
{
	CBoneCollider_DESC* pDesc = (CBoneCollider_DESC*)pArg;

	
	m_pSocketMatrix = pDesc->pCombinedTransformationMatrix;
	m_iNumSocketIndex = pDesc->pCombinedTransformationMatrix.size();
	m_pBoundingDesc= &pDesc->pBoundingDesc;
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

void CBoneCollider::Priority_Tick(_float fTimeDelta)
{
}

void CBoneCollider::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumSocketIndex; ++i)
	{

		_matrix		pSocketMatrix = XMLoadFloat4x4(m_pSocketMatrix[i]);

		pSocketMatrix.r[0] = XMVector3Normalize(pSocketMatrix.r[0]);
		pSocketMatrix.r[1] = XMVector3Normalize(pSocketMatrix.r[1]);
		pSocketMatrix.r[2] = XMVector3Normalize(pSocketMatrix.r[2]);


		XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * pSocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	
		m_pColliderCom[i]->Tick(XMLoadFloat4x4(&m_WorldMatrix));

	}




	//for (auto& pSoketMatrix : m_pSocketMatrix)
	//{
	//	_matrix		SocketMatrix = XMLoadFloat4x4(pSoketMatrix);
	//
	//	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	//	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	//	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);
	//
	//	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	//}


	//_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
	//
	//SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	//SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	//SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);
	//
	//XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	//
	//

	//m_pColliderCom->Tick(XMLoadFloat4x4(&m_WorldMatrix));

}

void CBoneCollider::Late_Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		return;
	for (_uint i = 0; i < m_iNumSocketIndex; ++i)
	{
		if (m_pColliderCom[i] == nullptr)
			continue;

		pPlayer->Intersect(CPlayer::PART_WEAPON, TEXT("Com_Collider"), m_pColliderCom[i]);
	}




	for (_uint i = 0; i < m_iNumSocketIndex; ++i)
	{

		_matrix		pSocketMatrix = XMLoadFloat4x4(m_pSocketMatrix[i]);

		pSocketMatrix.r[0] = XMVector3Normalize(pSocketMatrix.r[0]);
		pSocketMatrix.r[1] = XMVector3Normalize(pSocketMatrix.r[1]);
		pSocketMatrix.r[2] = XMVector3Normalize(pSocketMatrix.r[2]);


		XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * pSocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

		m_pColliderCom[i]->Tick(XMLoadFloat4x4(&m_WorldMatrix));

	}






	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);



#ifdef _DEBUG
	for (_uint i = 0; i < m_iNumSocketIndex; ++i)
		m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);

#endif


}

HRESULT CBoneCollider::Render()
{


	return S_OK;
}

HRESULT CBoneCollider::Add_Components()
{


	for (_uint i = 0; i < m_iNumSocketIndex; ++i)
	{
		CCollider* pCollider = nullptr;
		std::wstringstream ss;
		ss << L"Com_Collider_OBB_" << i;
		std::wstring componentTag = ss.str();

		if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
			componentTag.c_str(), reinterpret_cast<CComponent**>(&pCollider), m_pBoundingDesc)))
			return E_FAIL;

		m_pColliderCom.push_back(pCollider);
	}




	/* For.Com_Model */
	/*Prototype_Component_Model_ForkLift*/

//	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
//		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
//		return E_FAIL;
//
//	/* For.Com_Shader */
//	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
//		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
//		return E_FAIL;

	return S_OK;
}

HRESULT CBoneCollider::Bind_ShaderResources()
{
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
	//	return E_FAIL;
	
	
	return S_OK;
}

CBoneCollider* CBoneCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoneCollider* pInstance = new CBoneCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoneCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoneCollider::Clone(void* pArg)
{
	CBoneCollider* pInstance = new CBoneCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoneCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoneCollider::Free()
{
	__super::Free();
	for (auto& pCollider : m_pColliderCom)
		Safe_Release(pCollider);




}
