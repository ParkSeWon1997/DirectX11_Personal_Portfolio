#include "stdafx.h"
#include "..\Public\Rank.h"

#include "GameInstance.h"
#include"Loader.h"
#include"CTotalSingleton.h"

#include "Player.h"
#include"UpgradeMachineTop.h"
#include"Particle_Mesh.h"
#include"Level_Loading.h"

CRank::CRank(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CRank::CRank(const CRank & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CRank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRank::Initialize(void * pArg)
{
	
	CGameObject::GAMEOBJECT_DESC* GameObjectDesc = static_cast<CGameObject::GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&GameObjectDesc->vPosition));
		m_pTransformCom->Scaling(GameObjectDesc->vScale.x, GameObjectDesc->vScale.y, GameObjectDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&GameObjectDesc->vRotation), GameObjectDesc->vRotationAngle);
		m_fTargetDownPos_Y= GameObjectDesc->vPosition.y;

	}




	
	
	m_fHp = 100.0f;

	return S_OK;
}

void CRank::Priority_Tick(_float fTimeDelta)
{
}

void CRank::Tick(_float fTimeDelta)
{		
	


}
	


void CRank::Late_Tick(_float fTimeDelta)
{
	

	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		if (m_pColliderCom == nullptr)
			continue;

		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		//pPlayer->Intersect(CPlayer::PART_WEAPON, TEXT("Com_Collider"), m_pColliderCom[i]);
	}
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		return ;
	if (pPlayer->Intersect(CPlayer::PART_WEAPON, TEXT("Com_Collider"), m_pColliderCom))
	{


	}

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
	

#ifdef _DEBUG
	
		if (m_pColliderCom != nullptr)
			m_pGameInstance->Add_DebugComponent(m_pColliderCom);
	

#endif
}

HRESULT CRank::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	int	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int  i = 0; i < iNumMeshes; i++)
	{
		//g_Texture
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		
	
		m_pShaderCom->Begin(0);
	
		m_pModelCom->Render(i);
	}
	


	return S_OK;
}






HRESULT CRank::Add_Components()
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
	ColliderOBBDesc.vExtents = _float3(0.5f, 1.5f, 0.5f);
	ColliderOBBDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderOBBDesc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderOBBDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CRank::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CRank * CRank::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRank*		pInstance = new CRank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CRank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRank::Clone(void * pArg)
{
	CRank*		pInstance = new CRank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CRank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRank::Free()
{
	__super::Free();
	
		Safe_Release(m_pColliderCom);
	



	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
