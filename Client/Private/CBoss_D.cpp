#include "stdafx.h"
#include "CBoss_D.h"
#include "GameInstance.h"
#include"Loader.h"

CBoss_D::CBoss_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CBoss_D::CBoss_D(const CBoss_D& rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss_D::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CBoss_D::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CMonster_DESC*>(pArg)->vPosition));
	}

	m_pTransformCom->Rotation(XMConvertToRadians(180.f));

	return S_OK;
}

void CBoss_D::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CBoss_D::Tick(_float fTimeDelta)
{
	//m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	int tmep = 10;
	tmep = m_pModelCom->Get_AnimationCount();
	//m_pModelCom->Set_AnimationIndex();

	__super::Tick(fTimeDelta);
}

void CBoss_D::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CBoss_D::Render()
{
	__super::Render();
	return S_OK;
}

HRESULT CBoss_D::Add_Components()
{
	/* For.Com_Collider_AABB */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderAABBDesc{};
	
	ColliderAABBDesc.eType = CCollider::TYPE_AABB;
	ColliderAABBDesc.vExtents = _float3(5.f, 0.7f, 0.3f);
	ColliderAABBDesc.vCenter = _float3(0.f, ColliderAABBDesc.vExtents.y, 0.f);
	
	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_AABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_AABB]), &ColliderAABBDesc)))
		return E_FAIL;
	
	/* For.Com_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		ColliderOBBDesc{};
	
	ColliderOBBDesc.eType = CCollider::TYPE_OBB;
	ColliderOBBDesc.vExtents = _float3(0.3f, 0.7f, 0.3f);
	ColliderOBBDesc.vCenter = _float3(0.f, ColliderOBBDesc.vExtents.y, 0.f);
	ColliderOBBDesc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);
	
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_OBB]), &ColliderOBBDesc)))
		return E_FAIL;



	if(FAILED(__super::Add_Components()))
		return E_FAIL;
	
	



	return S_OK;
}

CBoss_D* CBoss_D::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_D*		pInstance = new CBoss_D(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
		{
			MSG_BOX("Failed To Created : CBoss_D");
			Safe_Release(pInstance);
		}
	
	return pInstance;
}
	

CGameObject* CBoss_D::Clone(void* pArg)
{
	CBoss_D* pInstance = new CBoss_D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_D");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CBoss_D::Free()
{
	__super::Free();
}
