#include "stdafx.h"
#include "..\Public\EnvironmentObj.h"

#include "GameInstance.h"
#include"Loader.h"

#include "Player.h"


CEnvironmentObj::CEnvironmentObj(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CEnvironmentObj::CEnvironmentObj(const CEnvironmentObj & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CEnvironmentObj::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnvironmentObj::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECT_DESC*	GameObjectDesc=static_cast<CGameObject::GAMEOBJECT_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&GameObjectDesc->vPosition));
		m_pTransformCom->Scaling(GameObjectDesc->vScale.x, GameObjectDesc->vScale.y, GameObjectDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&GameObjectDesc->vRotation), GameObjectDesc->vRotationAngle);
	}

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f,1.0f));

	return S_OK;
}

void CEnvironmentObj::Priority_Tick(_float fTimeDelta)
{
}

void CEnvironmentObj::Tick(_float fTimeDelta)
{
	

	
}

void CEnvironmentObj::Late_Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		return;
	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		if (m_pColliderCom[i] == nullptr)
			continue;

		m_pColliderCom[i]->Tick(m_pTransformCom->Get_WorldMatrix());
		//pPlayer->Intersect(CPlayer::PART_WEAPON, TEXT("Com_Collider"), m_pColliderCom[i]);
	}






	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	for (size_t i = 0; i < COLLIDER_END; i++)
	{
		if (m_pColliderCom[i] != nullptr)
			m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
	}

#endif
}

HRESULT CEnvironmentObj::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		//g_Texture
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	


	return S_OK;
}

HRESULT CEnvironmentObj::Add_Components()
{

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_Stage"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Shader */
//	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
//		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
//		return E_FAIL;	

	/* For.Com_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		ColliderOBBDesc{};

	ColliderOBBDesc.eType = CCollider::TYPE_OBB;
	ColliderOBBDesc.vExtents = _float3(0.3f, 0.7f, 0.3f);
	ColliderOBBDesc.vCenter = _float3(0.f, ColliderOBBDesc.vExtents.y, 0.f);
	ColliderOBBDesc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_OBB]), &ColliderOBBDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CEnvironmentObj::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CEnvironmentObj * CEnvironmentObj::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnvironmentObj*		pInstance = new CEnvironmentObj(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEnvironmentObj");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnvironmentObj::Clone(void * pArg)
{
	CEnvironmentObj*		pInstance = new CEnvironmentObj(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEnvironmentObj");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnvironmentObj::Free()
{
	__super::Free();
	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}



	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}