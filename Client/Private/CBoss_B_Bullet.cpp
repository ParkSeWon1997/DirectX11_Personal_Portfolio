#include "stdafx.h"
#include "CBoss_B_Bullet.h"
#include "GameInstance.h"
#include"Loader.h"

#include"Player.h"

#include"Particle_Mesh.h"
CBoss_B_Bullet::CBoss_B_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CBoss_B_Bullet::CBoss_B_Bullet(const CBullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CBoss_B_Bullet::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CBoss_B_Bullet::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{


	}

	m_fHp = 50.f;
	
	m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(0, true));
	return S_OK;
}

void CBoss_B_Bullet::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CBoss_B_Bullet::Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (pPlayer == nullptr)
		return;

	_vector vPlayerPos = pPlayer->Get_PositionVector();

	for (int i = 0; i < 3; i++)
	{
		m_BoneMatrix[i]= XMLoadFloat4x4(m_pBoneMatrix[i]);
		m_BoneMatrix[i].r[0] = XMVector3Normalize(m_BoneMatrix[i].r[0]);
		m_BoneMatrix[i].r[1] = XMVector3Normalize(m_BoneMatrix[i].r[1]);
		m_BoneMatrix[i].r[2] = XMVector3Normalize(m_BoneMatrix[i].r[2]);
	}
	if (m_fHp <= 0.f)
	{
		m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(1, false));
		
	}
		
	if (m_pModelCom->Get_AnimationIndex() == 1)
	{
		if (m_pModelCom->Get_AnimFinished())
		{
			m_bIsDead = true;
		}
	}

	__super::Tick(fTimeDelta);

}

void CBoss_B_Bullet::Late_Tick(_float fTimeDelta)
{

	if (m_pColliderCom == nullptr)
		return;
			
	_uint iLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"));



	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		return;








	__super::Late_Tick(fTimeDelta);
}

HRESULT CBoss_B_Bullet::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		//if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		//	return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}






	if (m_bIsDead)
	{
		_vector vPosBeforeDie = this->Get_PositionVector();
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Bullet"), this);




		_matrix mBoneMatrix0 = m_BoneMatrix[0] * m_pTransformCom->Get_WorldMatrix();
		_matrix mBoneMatrix1 = m_BoneMatrix[1] * m_pTransformCom->Get_WorldMatrix();
		_matrix mBoneMatrix2 = m_BoneMatrix[2] * m_pTransformCom->Get_WorldMatrix();
		_vector vBonePos0 = mBoneMatrix0.r[3];
		_vector vBonePos1 = mBoneMatrix1.r[3];
		_vector vBonePos2 = mBoneMatrix2.r[3];

		_float4 f4BonePos0, f4BonePos1, f4BonePos2 = {};
		XMStoreFloat4(&f4BonePos0, vBonePos0);
		XMStoreFloat4(&f4BonePos1, vBonePos1);
		XMStoreFloat4(&f4BonePos2, vBonePos2);



		vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_Up_And_Down")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_002_Up_And_Down")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Effect_ExplosionFrag_001_Up_And_Down")},

		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect_Pop")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("ExplosionGroundEffect.001_Pop")},
		{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION,TEXT("Effect_ExplosionDust_Explosion")},
		};

		_bool bIsAllCreate = true;


		auto pCreateParticle = [&](const _float4& f4BonePos, float fThresHold) {
			if (f4BonePos.y <= fThresHold)
			{
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(f4BonePos.x, f4BonePos.y, f4BonePos.z, 1.0f));
			}
			else
			{
				bIsAllCreate = false;
			}


			};
		pCreateParticle(f4BonePos0, 0.3f);
		pCreateParticle(f4BonePos1, 0.3f);
		pCreateParticle(f4BonePos2, 0.3f);

		if (bIsAllCreate)
		{
			m_bIsDead = true;
		}

	}




	return S_OK;

}


void CBoss_B_Bullet::Falling(_float fTimeDelta)
{
	if (this->Get_Position().y <= 0.0f)
	{
		m_Isfloor = true;
	}

	if(!m_Isfloor)
		m_pTransformCom->Go_Down(fTimeDelta);




}

HRESULT CBoss_B_Bullet::Add_Components()
{
	CBounding_OBB::BOUNDING_OBB_DESC		OBB_Desc{};
	OBB_Desc.eType = CCollider::TYPE_OBB;
	OBB_Desc.vExtents = _float3(2.3f, 3.7f, 2.3f);
	OBB_Desc.vCenter = _float3(0.f, OBB_Desc.vExtents.y, 0.f);
	OBB_Desc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);


	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBB_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;



	m_pBoneMatrix[0]= m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.001");
	m_pBoneMatrix[1] = m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.002");
	m_pBoneMatrix[2] = m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone.003");







	return S_OK;
}

HRESULT CBoss_B_Bullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CBoss_B_Bullet* CBoss_B_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_B_Bullet* pInstance = new CBoss_B_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_B_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_B_Bullet::Clone(void* pArg)
{
	CBoss_B_Bullet* pInstance = new CBoss_B_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_B_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBoss_B_Bullet::Free()
{
	__super::Free();
}
