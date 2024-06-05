#include "stdafx.h"
#include "CPlayerBullet_Mine.h"

#include "GameInstance.h"
#include"Loader.h"
#include"Particle_Mesh.h"
#include"CPlayerBullet.h"
CPlayerBullet_Mine::CPlayerBullet_Mine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CPlayerBullet_Mine::CPlayerBullet_Mine(const CPlayerBullet_Mine& rhs)
	:CBullet(rhs)
{
}

HRESULT CPlayerBullet_Mine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerBullet_Mine::Initialize(void* pArg)
{
	m_eColliderType=static_cast<CPlayetBULLET_MINE_DESC*>(pArg)->eColliderType;
	m_fRadius = static_cast<CPlayetBULLET_MINE_DESC*>(pArg)->fRadius;
	m_vCenter = static_cast<CPlayetBULLET_MINE_DESC*>(pArg)->vCenter;
	m_vExtents= static_cast<CPlayetBULLET_MINE_DESC*>(pArg)->vExtents;
	m_vRotation = static_cast<CPlayetBULLET_MINE_DESC*>(pArg)->vRotation;

	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_bParticleCreate = ((CPlayetBULLET_MINE_DESC*)pArg)->bParticleCreate;
		m_eParticleType = ((CPlayetBULLET_MINE_DESC*)pArg)->eParticleType;
		m_pTransformCom->Set_Look(((CPlayetBULLET_MINE_DESC*)pArg)->vDir);
		m_fSpeed= ((CPlayetBULLET_MINE_DESC*)pArg)->fSpeedPerSec;
		m_fMaxLifeTime = ((CPlayetBULLET_MINE_DESC*)pArg)->fLifeTime;
		m_vTargetPos = ((CPlayetBULLET_MINE_DESC*)pArg)->vTargetPos;
		m_pMatPlayerWorld = ((CPlayetBULLET_MINE_DESC*)pArg)->pMatPlayerWorld;
		m_eCharacterType = ((CPlayetBULLET_MINE_DESC*)pArg)->eCharacterType;
		m_fInitSpeed= ((CPlayetBULLET_MINE_DESC*)pArg)->fInitSpeed;

	}
	return S_OK;


	
}

void CPlayerBullet_Mine::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CPlayerBullet_Mine::Tick(_float fTimeDelta)
{
	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > m_fMaxLifeTime)
		m_bIsDead = true;
	if (m_bParticleCreate)
	{
		MakeParticle();
	}




	__super::Tick(fTimeDelta);
}

void CPlayerBullet_Mine::Late_Tick(_float fTimeDelta)
{


	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

	
}

HRESULT CPlayerBullet_Mine::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	
	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
	
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pShaderCom->Bind_RawValue("g_fTime", &m_fLifeTime, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_MaxTime", &m_fMaxLifeTime, sizeof(_float));



		m_pShaderCom->Begin(1);
	
		m_pModelCom->Render(i);
	}
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	CPlayerBullet::CPlayetBULLET_DESC CSwordThowDesc{};
	if (m_bIsDead)
	{
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD_NONE_DISOLVE,TEXT("Player_Amanda_Balance_X_LowpolySphere16_Spread"),_float4(0.1f,0.0f,0.0f,0.3f),false ,false,*this });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD_NONE_DISOLVE,TEXT("Player_Amanda_Balance_X_LowpolySphere8_Spread"),_float4(0.1f,0.0f,0.0f,0.3f),false ,false,*this });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect_Pop"),_float4(1.0f,0.9f,0.9f,0.7f),false ,true,*this });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Amanda_Balance_X_ExplosionGroundEffect.001_Pop"),_float4(1.0f,0.9f,0.9f,0.7f),false ,true,*this });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_X_ExplosionFrag_Up_And_Down"),_float4(1.0f,1.0f,0.0f,0.7f),false ,true,*this });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_X_ExplosionFrag.002_Up_And_Down"),_float4(0.9f,0.1f,0.0f,0.7f),false ,true,*this });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_X_ExplosionFrag.001_Up_And_Down"),_float4(1.0f,0.5f,0.0f,0.7f),false ,true,*this });
		CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);


		CSwordThowDesc.fRadius = 5.0f;
		CSwordThowDesc.strModelName = TEXT("swordThrowing");
		CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);
		CSwordThowDesc.BulletState = &CBullet::Pop;
		CSwordThowDesc.fLifeTime = 1.f;
		m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);


		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), this);

	}


	return S_OK;
}

HRESULT CPlayerBullet_Mine::Add_Components()
{
	/* For.Com_Collider */
	//CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDescSphre{};
	//ColliderDescSphre.eType = m_eColliderType;
	//ColliderDescSphre.fRadius = m_fRadius;
	//ColliderDescSphre.vCenter = m_vCenter;



	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), MakeColliderDesc(m_eColliderType))))
	//	return E_FAIL;



	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerBullet_Mine::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	return S_OK;
}

void CPlayerBullet_Mine::Go(_float fTimeDelta)
{

	m_pTransformCom->Go_Straight(fTimeDelta);



}

void CPlayerBullet_Mine::Go_TargetPos(_float fTimeDelta)
{
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 fCurPos;
	XMStoreFloat3(&fCurPos, vPos);

	_float3 fTargetPos;
	XMStoreFloat3(&fTargetPos, m_vTargetPos);
	_float diffX = fCurPos.x - fTargetPos.x;
	_float diffZ = fCurPos.z - fTargetPos.z;

	// 두 좌표 사이의 거리 계산 (y 좌표 무시)
	_float distanceSquared = diffX * diffX + diffZ * diffZ;

	// 근사치 비교를 위한 허용 거리 (squared)
	const _float epsilon = 3.2f * 3.2f; // 여기서 0.1f는 허용 거리를 의미

	// 타겟 위치에 근접했는지 확인
	if (distanceSquared <= epsilon)
	{
		m_bIsDead = true;
		return;
	}
	else
	{
		if (XMVector3Equal(m_vTargetPos, XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
		{
			m_pTransformCom->Go_Straight(fTimeDelta);
		}
		else
		{
			m_pTransformCom->Set_RotationSpeed(20.f);
			m_pTransformCom->TurnToTarget(fTimeDelta, m_vTargetPos);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}
	}



}

void CPlayerBullet_Mine::ComeBack(_float fTimeDelta)
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	
	m_fLookAcc += fTimeDelta * m_fSpeed;
	cout << m_fLookAcc << endl;

	_float fLook= sin(XMConvertToRadians(m_fLookAcc));
	
	//다시 돌아오는 시점
	if (m_fLookAcc > 25.f)
	{
		m_fLookAcc = 0.f;
		switch (m_eParticleType)
		{
		case Client::CPlayerBullet_Mine::BALANCE_X:
			break;
		case Client::CPlayerBullet_Mine::BALANCE_C:
			break;
		case Client::CPlayerBullet_Mine::BALANCE_XC:
			break;
		case Client::CPlayerBullet_Mine::TECHNIC_X:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP_NONE_DISOLVE,TEXT("Player_Technic_X_swordThrow_Pop"),_float4(0.0f,0.0f,1.0f,0.3f) });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
			break;
		case Client::CPlayerBullet_Mine::TECHNIC_C:
			break;
		case Client::CPlayerBullet_Mine::TECHNIC_XC:
			break;
		case Client::CPlayerBullet_Mine::POWER_X:
			break;
		case Client::CPlayerBullet_Mine::POWER_C:
			break;
		case Client::CPlayerBullet_Mine::POWER_XC:
			break;
		case Client::CPlayerBullet_Mine::REVERSE_X:
			break;
		case Client::CPlayerBullet_Mine::REVERSE_C:
			break;
		case Client::CPlayerBullet_Mine::REVERSE_XC:
			break;
		case Client::CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE_END:
			break;
		default:
			break;
		}


	

		m_pTransformCom->Set_State(CTransform::STATE_LOOK, -vLook);

	}

	vPos += XMVector3Normalize(vLook) * fLook;// *fTimeDelta * 10.f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

}

void CPlayerBullet_Mine::Draven_Bullet(_float fTimeDelta)
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};


	m_fElapsedTime += fTimeDelta*0.5f;
	_matrix mPlayerworld =XMLoadFloat4x4(m_pMatPlayerWorld);
	_vector vPlayerLook= mPlayerworld.r[CTransform::STATE_LOOK];
	_vector vPlayerPos = mPlayerworld.r[CTransform::STATE_POSITION];

	_float3 f3PlayerPos;
	XMStoreFloat3(&f3PlayerPos, vPlayerPos);

	_float3 f3MyPos = this->Get_Position();

	_bool bIsTouchPlayer  = (abs(f3MyPos.x - f3PlayerPos.x) < 4.0f && abs(f3MyPos.z - f3PlayerPos.z) < 4.0f);
	
	_bool bIsGround = (f3MyPos.y <= 0.0f);

	_bool Isfloor = false;

	if (bIsGround)
	{
  		if (bIsTouchPlayer)
		{
			m_pTransformCom->Set_Look(vPlayerLook);
			m_pTransformCom->Go_Straight(fTimeDelta);
			m_fElapsedTime = 0.0f;
			m_pTransformCom->Go_Jump(fTimeDelta, 100.5f, 100.0f, Isfloor);
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE,TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE,TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
		
		}
		else
		{
			m_bIsDead = true;
		}

	}
	else 
	{
		m_pTransformCom->Up_And_Down(fTimeDelta, 5.0f, 15.0f, m_fElapsedTime);
	}



	

}

void CPlayerBullet_Mine::Spread(_float fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;


	m_pTransformCom->Up_And_Down(fTimeDelta, m_fInitSpeed, 20.0f, m_fElapsedTime);

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

void CPlayerBullet_Mine::Pop(_float fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;
	if (m_fElapsedTime >= m_fMaxLifeTime)
		m_bIsDead = true;


}

void CPlayerBullet_Mine::MakeParticle()
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	static  _float fParticleCount = 0.0f;

	switch (m_eCharacterType)
	{
	case Client::CPlayer::CHRACTER_SWORD_MASTER:
		switch (m_eParticleType)
		{
		case Client::CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE::TECHNIC_X:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X,TEXT("Player_Technic_X_swordThrowing_Rotate_x"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X,TEXT("Player_Technic_X_slashShockwave_Rotate_x"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Technic_X_hitRing_SIZE_UP"),_float4(0.8f,0.8f,1.0f,0.3f) });
			//vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP_NONE_DISOLVE,TEXT("Player_Technic_X_swordThrow_Pop"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,this});
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
			m_bParticleCreate = false;
			break;
		case Client::CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE::REVERSE_X:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE,TEXT("Player_Reverse_X_swordThrowing_Rotate_X_Loop"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE,TEXT("Player_Reverse_X_slashShockwave_Rotate_X_Loop"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
			m_bParticleCreate = false;
			break;
		case Client::CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE::REVERSE_C:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Reverse_C_ThrowingKnife_Size_UP"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			//vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP_NONE_DISOLVE,TEXT("Player_Technic_X_swordThrow_Pop"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,this});
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
			m_bParticleCreate = false;
			break;
		case Client::CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE::REVERSE_XC:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Reverse_XC_SlashCubic_Pop"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
			m_bParticleCreate = false;
			break;
		case Client::CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE::CPLAYER_BULLET_PARTICLE_TYPE_END:
			break;
		default:
			break;
		}
		break;
	case Client::CPlayer::CHRACTER_GUN_SLINGER:
		switch (m_eParticleType)
		{
		case Client::CPlayerBullet_Mine::BALANCE_X:
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_TEXTURE,TEXT("Player_Amanda_Balance_X_Grenade_Rotate_X"),_float4(0.1f,0.2f,1.0f,0.3f),false ,true,*this });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
			m_bParticleCreate = false;
			break;
		case Client::CPlayerBullet_Mine::BALANCE_C:
			break;
		case Client::CPlayerBullet_Mine::BALANCE_XC:
			break;
		case Client::CPlayerBullet_Mine::TECHNIC_X:
			break;
		case Client::CPlayerBullet_Mine::TECHNIC_C:
			break;
		case Client::CPlayerBullet_Mine::TECHNIC_XC:
			break;
		case Client::CPlayerBullet_Mine::POWER_X:
			
			if (m_fLifeTime - m_fLastParticleCreateTime >= 0.2f)
			{
				m_fLastParticleCreateTime = m_fLifeTime;
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_2"),_float4(0.1f,0.2f,0.1f,0.3f),false ,true,*this,/*XMVectorSet(0.0f,-0.2f,0.5f,1.0f)*/ });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Amanda_Power_X_Tail_LowpolySphere16_Spread_1"),_float4(0.9f,0.9f,0.9f,0.5f),false ,true,*this,/*XMVectorSet(0.0f,-0.2f,1.0f,1.0f)*/ });
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
				

			
			}
			
			break;
		case Client::CPlayerBullet_Mine::POWER_C:
			break;
		case Client::CPlayerBullet_Mine::POWER_XC:
			break;
		case Client::CPlayerBullet_Mine::REVERSE_X:
			break;
		case Client::CPlayerBullet_Mine::REVERSE_C:
			break;
		case Client::CPlayerBullet_Mine::REVERSE_XC:
			break;
		case Client::CPlayerBullet_Mine::CPLAYER_BULLET_PARTICLE_TYPE_END:
			break;
		default:
			break;
		}

		break;
	case Client::CPlayer::CHRACTER_END:
		break;
	default:
		break;
	}

	



}

CCollider::COLLIDER_DESC* CPlayerBullet_Mine::MakeColliderDesc(CCollider::TYPE eTpye)
{
	



	if (eTpye == CCollider::TYPE::TYPE_AABB)
	{
		ColliderDescAABB.eType = eTpye;
		ColliderDescAABB.vCenter = m_vCenter;
		ColliderDescAABB.vExtents = m_vExtents;
		
		return &ColliderDescAABB;
	}
	if(eTpye==CCollider::TYPE::TYPE_OBB)
	{
		ColliderDescOBB.eType = eTpye;
		ColliderDescOBB.vExtents = m_vExtents;
		ColliderDescOBB.vCenter = m_vCenter;
		ColliderDescOBB.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
		
		return &ColliderDescOBB;
	}
	if(eTpye==CCollider::TYPE::TYPE_SPHERE)
	{
		ColliderDescSphre.eType = eTpye;
		ColliderDescSphre.fRadius = m_fRadius;
		ColliderDescSphre.vCenter = m_vCenter;
		
		return &ColliderDescSphre;
	}


	

	
}

CPlayerBullet_Mine* CPlayerBullet_Mine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerBullet_Mine* pInstance = new CPlayerBullet_Mine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayerBullet_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerBullet_Mine::Clone(void* pArg)
{
	CPlayerBullet_Mine* pInstance = new CPlayerBullet_Mine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayerBullet_Mine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerBullet_Mine::Free()
{
	__super::Free();
}
