#include "stdafx.h"
#include "..\Public\ItemBox.h"

#include "GameInstance.h"
#include"Loader.h"
#include"CTotalSingleton.h"

#include "Player.h"
#include"UpgradeMachineTop.h"
#include"Particle_Mesh.h"
#include"Level_Loading.h"

#include"CBullet.h"
#include"Item.h"

CItemBox::CItemBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CItemBox::CItemBox(const CItemBox & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CItemBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemBox::Initialize(void * pArg)
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


	}




	
	
	m_fHp = 30.0f;

	return S_OK;
}

void CItemBox::Priority_Tick(_float fTimeDelta)
{
}

void CItemBox::Tick(_float fTimeDelta)
{		
	m_fHitTime+=fTimeDelta;
	if (m_fHitTime > 0.8f)
	{
		m_bIsHit=false;
		m_fHitTime=0.f;
	}

	if (m_fHp <= 0.f)
	{
		m_bIsDead=true;
	}


}
	


void CItemBox::Late_Tick(_float fTimeDelta)
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
		if (pPlayer->IsAttacking() && !pPlayer->GetIsCollision())
		{
			_float fDamage = pPlayer->GetDamage();
			pPlayer->SetIsCollision(true);
			if (m_fHp > 0.f)
			{
				m_fHp -= fDamage;
			}
			m_bIsHit = true;
		
		}
	}



	_uint iLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"));

	for (_uint i = 0; i < iLayerSize; ++i)
	{
		CBullet* pBullet = dynamic_cast<CBullet*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), i));
		if (pBullet != nullptr)
		{
			if (!pBullet->Get_IsCollision() && pBullet->Intersect(TEXT("Com_Collider"), m_pColliderCom))
			{
				_float fDamage = pBullet->Get_Damage();
				pBullet->Set_IsCollision(true);
				if (m_fHp > 0.f)
				{

					m_fHp -= fDamage;
				}
				m_bIsHit = true;
			}
		}
	}




	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
	

#ifdef _DEBUG
	
		if (m_pColliderCom != nullptr)
			m_pGameInstance->Add_DebugComponent(m_pColliderCom);
	

#endif
}

HRESULT CItemBox::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	int	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int  i = 0; i < iNumMeshes; i++)
	{
		//g_Texture
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		


		float HitStatus = m_bIsHit ? 1.0f : 0.0f;

		m_pShaderCom->Bind_RawValue("g_HitStatus", &HitStatus, sizeof(float));
	
		m_pShaderCom->Begin(2);
	
		m_pModelCom->Render(i);
	}


	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	if (m_bIsDead)
	{
		if (FAILED(m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Environment"), this)))
			return E_FAIL;

		m_pGameInstance->Play_Sound_Z(TEXT("SFX_Hit002 [1].wav"), SOUND_EFFECT, 0.5f);
		CItem::CItem_DESC ItemDesc = {};
		if (m_strModelName == TEXT("Interactor_Itembox_HP"))
		{
			for (int i = 0; i < 10; i++)
			{
				_float fRandX = RandomNum<_float>(-5.f, 5.f)+this->Get_Position().x;
				_float fRandZ = RandomNum<_float>(-5.f, 5.f)+this->Get_Position().z;
		
				ItemDesc.strModelName = TEXT("Item_HP");
				ItemDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y+1.f, this->Get_Position().z, 1.0f);
				ItemDesc.vScale = _float3(1.f, 1.f, 1.f);
				ItemDesc.vRotation = _float3(0.f, 1.f, 0.f);
				ItemDesc.vRotationAngle = 45.f;
				ItemDesc.vTargetPos=XMVectorSet(fRandX, 1.f, fRandZ, 1.0f);
				if (FAILED(m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Environment"), TEXT("Prototype_Item"), &ItemDesc)))
					return E_FAIL;
			}
		}

		if (m_strModelName == TEXT("Interactor_Itembox_Gold"))
		{
			for (int i = 0; i < 10; i++)
			{
				_float fRandX = RandomNum<_float>(-5.f, 5.f) + this->Get_Position().x;
				_float fRandZ = RandomNum<_float>(-5.f, 5.f) + this->Get_Position().z;

				ItemDesc.strModelName = TEXT("Item_Coin_B");
				ItemDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y + 1.f, this->Get_Position().z, 1.0f);
				ItemDesc.vScale = _float3(1.f, 1.f, 1.f);
				ItemDesc.vRotation = _float3(0.f, 1.f, 0.f);
				ItemDesc.vRotationAngle = 45.f;
				ItemDesc.vTargetPos = XMVectorSet(fRandX, 1.f, fRandZ, 1.0f);
				if (FAILED(m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Environment"), TEXT("Prototype_Item"), &ItemDesc)))
					return E_FAIL;
			}
		}
		if (m_strModelName == TEXT("Interactor_Itembox_Random"))
		{
			_float iRand = RandomNum<_float>(0.0f, 1.0f);
			wstring strModelName = L"";
			if (iRand == 0.0f)
			{
				strModelName = L"Item_HP";
			}
			else
			{
				strModelName = L"Item_Coin_B";
			}



			for (int i = 0; i < 10; i++)
			{
				_float fRandX = RandomNum<_float>(-5.f, 5.f) + this->Get_Position().x;
				_float fRandZ = RandomNum<_float>(-5.f, 5.f) + this->Get_Position().z;

				ItemDesc.strModelName = strModelName;
				ItemDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y + 1.f, this->Get_Position().z, 1.0f);
				ItemDesc.vScale = _float3(1.f, 1.f, 1.f);
				ItemDesc.vRotation = _float3(0.f, 1.f, 0.f);
				ItemDesc.vRotationAngle = 45.f;
				ItemDesc.vTargetPos = XMVectorSet(fRandX, 1.f, fRandZ, 1.0f);
				if (FAILED(m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Environment"), TEXT("Prototype_Item"), &ItemDesc)))
					return E_FAIL;
			}
		}

		

		





		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Object_Dead_Dead_Spread"),_float4(1.0f,1.0f,1.0f,0.5f),false,true });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Object_Dead_Dead_2_Spread"),_float4(1.0f,1.0f,1.0f,0.5f),false,true });



		CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f));


	}


	return S_OK;
}






HRESULT CItemBox::Add_Components()
{

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;




	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	

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

HRESULT CItemBox::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CItemBox * CItemBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CItemBox*		pInstance = new CItemBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CItemBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItemBox::Clone(void * pArg)
{
	CItemBox*		pInstance = new CItemBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CItemBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemBox::Free()
{
	__super::Free();
	
		Safe_Release(m_pColliderCom);
	



	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
