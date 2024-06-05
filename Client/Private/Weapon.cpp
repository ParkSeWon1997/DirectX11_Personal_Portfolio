#include "stdafx.h"
#include "..\Public\Weapon.h"

#include "GameInstance.h"
#include"Loader.h"

#include"Particle_Mesh.h"
#include"Player.h"

#include"CPlayerBullet.h"

CWeapon::CWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CPartObject{ pDevice, pContext }
{
}

CWeapon::CWeapon(const CWeapon & rhs)
	: CPartObject{ rhs }
{
}

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void * pArg)
{
	WEAPON_DESC*		pDesc = (WEAPON_DESC*)pArg;
		
	m_pSocketMatrix = pDesc->pCombinedTransformationMatrix;	
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMLoadFloat4(&pDesc->vPosition));
		m_pTransformCom->Scaling(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&pDesc->vRotation), pDesc->vRotationAngle);
		m_pPlayer = pDesc->pPlayer;
		
	}
	//m_pTransformCom->Scaling(1.1f, 1.1f, 1.1f);
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));
	

	return S_OK;
}

void CWeapon::Priority_Tick(_float fTimeDelta)
{
}

void CWeapon::Tick(_float fTimeDelta)
{
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);





	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);








	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));


	m_pColliderCom->Tick(XMLoadFloat4x4(&m_WorldMatrix));

	
	

}

void CWeapon::Late_Tick(_float fTimeDelta)
{

	if (m_bParticleCreate)
	{
		MakeParticle();
	}



	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CWeapon::Render()
{
		//if (FAILED(Bind_ShaderResources()))
		//	return E_FAIL;
		//
		//
		//
		//_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();
		//
		//for (size_t i = 0; i < iNumMeshes; i++)
		//{
		//	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
		//		return E_FAIL;		
		//
		//	m_pShaderCom->Begin(0);
		//
		//	m_pModelCom->Render(i);
		//}


	return S_OK;
}

void CWeapon::MakeParticle()
{

		CPlayerBullet::CPlayetBULLET_DESC CSwordThowDesc{};


		_matrix matworld = XMLoadFloat4x4(&m_WorldMatrix);

		_vector vBonePos = matworld.r[3];
		_float4 fBonePos = { };
		XMStoreFloat4(&fBonePos, vBonePos);
		vector<CParticle_Mesh::PARTICLE_DESC> vecDesc={};
		_vector vPlayerLook = dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir();


		_vector vNewPos= vBonePos+vPlayerLook*10.0f;

		switch (m_eParticleType)
		{
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::BALANCE_X:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_X_Balalnce_SlashHaftone001_Rotate"),_float4(0.1f,0.2f,1.0f,0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_X_Balalnce_slashShockwave_Rotate"),_float4(0.1f,0.0f,0.9f,0.2f),false ,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_X_Balalnce_swordThrowing_Rotate"),_float4(0.0f,0.2f,1.0f,0.2f)  });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_X_Balalnce_newSlash.001_Rotate"),_float4(0.2f,0.2f,1.0f,0.5f),false ,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_X_Balalnce_newSlash_Rotate"),_float4(0.3f,0.2f,0.8f,0.2f) ,false ,true });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(XMVectorGetX(vNewPos), XMVectorGetY(vNewPos), XMVectorGetZ(vNewPos), 1.0f), -vPlayerLook);
			CSwordThowDesc.fRadius = 5.0f;
			CSwordThowDesc.strModelName = TEXT("swordThrowing");
			CSwordThowDesc.vPosition= _float4(XMVectorGetX(vNewPos), XMVectorGetY(vNewPos), XMVectorGetZ(vNewPos), 1.0f);
			CSwordThowDesc.vDir = vPlayerLook;
			CSwordThowDesc.fLifeTime=1.0f;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);
			m_bParticleCreate = false;

			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::BALANCE_C:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::BALANCE_XC:
			CSwordThowDesc.fRadius = 5.0f;
			CSwordThowDesc.strModelName = TEXT("swordThrowing");
			CSwordThowDesc.vPosition = _float4(XMVectorGetX(vNewPos), XMVectorGetY(vNewPos), XMVectorGetZ(vNewPos), 1.0f);
			CSwordThowDesc.vDir = vPlayerLook;
			CSwordThowDesc.fLifeTime = 0.5f;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);
			m_bParticleCreate = false;
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::TECHNIC_X:

			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::TECHNIC_C:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::TECHNIC_XC:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::POWER_X:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::POWER_C:
			vecDesc.push_back({CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_C_Power_HitRing001_Pop"),_float4(0.8f,0.8f,0.8f,0.5f)});
			vecDesc.push_back({CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_C_Power_HitRing003_SizeUP"),_float4(0.7f,0.7f,0.7f,0.2f)});
			vecDesc.push_back({CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_C_Power_LowPlySphere8_SizeUP"),_float4(0.7f,0.6f,0.7f,0.3f)});
			vecDesc.push_back({CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_C_Power_LowPlySphere16_SizeUP"),_float4(0.8f,0.8f,1.0f,0.3f)});
			vecDesc.push_back({CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down"),_float4(0.1f,0.1f,0.2f,0.5f)});
			vecDesc.push_back({CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Y,TEXT("Player_C_Power_Stomp_SIzeDown_Y"),_float4(0.1f,0.1f,1.0f,0.7f)});
		
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fBonePos.x, 0.0f, fBonePos.z, 1.0f), -vPlayerLook);
			m_bParticleCreate = false;
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::POWER_XC:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::REVERSE_X:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::REVERSE_C:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::REVERSE_XC:
			break;
		case Client::CWeapon::WEAPON_PARTICLE_TYPE::WEAPON_PARTICLE_TYPE_END:
			break;
		default:
			break;
		}


}

HRESULT CWeapon::Add_Components()
{

	/* For.Com_Collider */
	CBounding_OBB::BOUNDING_OBB_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_OBB;
	ColliderDesc.vExtents = _float3(1.5f, 5.f, 2.5f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	
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

HRESULT CWeapon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CWeapon * CWeapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWeapon*		pInstance = new CWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CWeapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWeapon::Clone(void * pArg)
{
	CWeapon*		pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CWeapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
