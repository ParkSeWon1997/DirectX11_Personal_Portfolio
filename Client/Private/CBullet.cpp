#include "stdafx.h"
#include "..\Public\CBullet.h"

#include "GameInstance.h"
#include"Loader.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CBullet::CBullet(const CBullet& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
		m_pBulletAction = static_cast<CBullet_DESC*>(pArg)->BulletState;
		m_vTargetPos = static_cast<CBullet_DESC*>(pArg)->vTargetPos;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CBullet_DESC*>(pArg)->vPosition));
		m_pTransformCom->Set_Look(static_cast<CBullet_DESC*>(pArg)->vDir);
		m_fDamage = static_cast<CBullet_DESC*>(pArg)->fDamage;

	}


	return S_OK;
}

void CBullet::Priority_Tick(_float fTimeDelta)
{
	


}

void CBullet::Tick(_float fTimeDelta)
{

	if (m_pBulletAction != nullptr)
	{
		(this->*m_pBulletAction)(fTimeDelta);
	}

	if(m_pModelCom!=nullptr)
		m_pModelCom->Play_Animation(fTimeDelta);
}

void CBullet::Late_Tick(_float fTimeDelta)
{
	


	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);



#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif


}

HRESULT CBullet::Render()
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
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), this);
	}


	return S_OK;
}



void CBullet::Falling(_float fTimeDelta)
{

	m_pTransformCom->Go_Down(fTimeDelta);

	//여기서 충돌 체크, 그리고 땅에 닿았으면 삭제

}

void CBullet::Spread(_float fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;


	m_pTransformCom->Up_And_Down(fTimeDelta, 5.0f, 20.0f, m_fElapsedTime);

	// 지면에 닿으면 초기화
	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	if (vPos.y <= 0.0f)
	{
		vPos.y = 0.0f;
		m_fElapsedTime = 0.0f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
	}
}

void CBullet::Go(_float fTimeDelta)
{
	if (this->Get_Position().y <= 0.1f)
	{
		m_bIsDead = true;
		return;
	}

	_float4 vPos = {};
	XMStoreFloat4(&vPos, m_vTargetPos);

	if (this->Get_Position().y <= vPos.y)
	{
		m_bIsDead = true;
		return;
	}

	m_pTransformCom->LookAt(m_vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta);


}

void CBullet::Go_TargetPos(_float fTimeDelta)
{
}

void CBullet::Draven_Bullet(_float fTimeDelta)
{
}

void CBullet::Pop(_float fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;
	if(m_fElapsedTime>=2.0f)
		m_bIsDead = true;



}

void CBullet::ComeBack(_float fTimeDelta)
{


}







HRESULT CBullet::Add_Components()
{
	//TEXT("Prototype_Component_Model_ForkLift")
	//m_strModelName
	/* For.Com_Model */


	/* For.Com_Collider */
	//CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDesc{};
	//
	//ColliderDesc.eType = CCollider::TYPE_SPHERE;
	//ColliderDesc.fRadius = 0.5f;
	//ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	//
	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	//	return E_FAIL;
	//
	//
	//
	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;
	//
	///* For.Com_Shader */
	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;
	//
	return S_OK;
}

HRESULT CBullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

_bool CBullet::Intersect(const wstring& strComponetTag, CCollider* pTargetCollider)
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

CBullet* CBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet* pInstance = new CBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet::Clone(void* pArg)
{
	CBullet* pInstance = new CBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
