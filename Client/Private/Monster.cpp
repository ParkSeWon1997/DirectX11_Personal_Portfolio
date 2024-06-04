#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"
#include"Loader.h"

#include"Player.h"
#include"CBullet.h"
//#include"CSwordThow.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECT_DESC	GameObjectDesc{};

	GameObjectDesc.fSpeedPerSec = 7.f;
	GameObjectDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	GameObjectDesc.strModelName=static_cast<CMonster_DESC*>(pArg)->strModelName;
	m_strDeconModelTag= static_cast<CMonster_DESC*>(pArg)->strDeconModelTag;
	//GameObjectDesc.
	if (FAILED(__super::Initialize(&GameObjectDesc)))
		return E_FAIL;
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{	
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&static_cast<CMonster_DESC*>(pArg)->vPosition));
	}
	//m_pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));

	return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{

}

void CMonster::Tick(_float fTimeDelta)
{




	m_pModelCom->Play_Animation(fTimeDelta);

	for (size_t i = 0; i < COLLIDER_END; i++)
	{
		if (m_pColliderCom[i] == nullptr)
			continue;
		if (m_vpSocketMatrix != nullptr && m_pParentMatrix != nullptr)
		{
			_matrix 	pSocketMatrix = XMLoadFloat4x4(m_vpSocketMatrix);
			
			pSocketMatrix.r[0] = XMVector3Normalize(pSocketMatrix.r[0]);
			pSocketMatrix.r[1] = XMVector3Normalize(pSocketMatrix.r[1]);
			pSocketMatrix.r[2] = XMVector3Normalize(pSocketMatrix.r[2]);
			
			m_pColliderCom[i]->Tick(m_pTransformCom->Get_WorldMatrix() * pSocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
		}
		else
			m_pColliderCom[i]->Tick(m_pTransformCom->Get_WorldMatrix());

	}

}

void CMonster::Late_Tick(_float fTimeDelta)
{
	_uint iLayerSize=m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"));
	


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		return;
	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		

		if (m_pColliderCom[i]==nullptr)
			continue;

		if (m_pColliderCom[COLLIDER_OBB] != nullptr)
		{
			if (pPlayer->Intersect(CPlayer::PART_WEAPON, TEXT("Com_Collider"), m_pColliderCom[COLLIDER_OBB]))
			{
				if(pPlayer->IsAttacking())
					m_bIsHit = true;
			}
			for (_uint i = 0; i < iLayerSize; ++i)
			{
				CBullet* pBullet = dynamic_cast<CBullet*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), i));
				if (pBullet != nullptr)
				{
					if (pBullet->Intersect(TEXT("Com_Collider"), m_pColliderCom[COLLIDER_OBB]))
					{
						//pBullet->Set_Dead();
						m_bIsHit = true;
					}
				}
			}
		}
		if (m_pColliderCom[COLLIDER_AABB] != nullptr)
		{
			if (pPlayer->Intersect(CPlayer::PART_WEAPON, TEXT("Com_Collider"), m_pColliderCom[COLLIDER_AABB]))
			{
				m_bIsHit = true;
			}


		}

		if (m_pColliderCom[COLLIDER_SPHERE] != nullptr)
		{
			if (pPlayer->Intersect(CPlayer::PART_BODY_MATILDA, TEXT("Com_Collider"), m_pColliderCom[COLLIDER_SPHERE]))
				m_bIsDetact = true;
			else
				m_bIsDetact = false;
		}
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

HRESULT CMonster::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		

		
		//if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_NORMALS)))
		//	return E_FAIL;		

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	if (m_bIsDead)
	{
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Monster"), this);
	}
	

}

HRESULT CMonster::Add_Components()
{
	
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC	NavigationDesc{};
	NavigationDesc.iCurrentCellIndex = 0;

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;






	return S_OK;
}


HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}


void CMonster::Free()
{
	__super::Free();

	for(_uint i = 0; i < COLLIDER_END; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
}
