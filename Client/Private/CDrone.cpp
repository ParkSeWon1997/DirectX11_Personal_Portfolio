#include "stdafx.h"
#include "..\Public\CDrone.h"

#include "GameInstance.h"
#include"Loader.h"
#include"Particle_Mesh.h"
#include"CPlayerBullet.h"
CDrone::CDrone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CDrone::CDrone(const CDrone& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CDrone::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDrone::Initialize(void* pArg)
{
	CDrone::CDrone_DESC* pDesc = (CDrone::CDrone_DESC*)pArg;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->vPosition));
		m_pTransformCom->Scaling(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&pDesc->vRotation), pDesc->vRotationAngle);
		m_pTransformCom->Set_Look(pDesc->vDirLook);
		m_pPlayerWorldMatrix = pDesc->pPlayerWorldMatrix;
		_matrix mPlayerWorldMatrix = XMLoadFloat4x4(pDesc->pPlayerWorldMatrix);

		m_fOrbitRadius=XMVectorGetX(XMVector3Length(XMLoadFloat4(&pDesc->vPosition)- mPlayerWorldMatrix.r[3]));
		m_fOrbitSpeed = 2.0f;
		m_fCurrentAngle = pDesc->fCurAngle;
		m_eChracterType = pDesc->eType;

	}
	//m_pTransformCom->Scaling(1.1f, 1.1f, 1.1f);
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));

	m_fLifeTime = 10.0f;

	return S_OK;
}

void CDrone::Priority_Tick(_float fTimeDelta)
{
}

void CDrone::Tick(_float fTimeDelta)
{

	m_fMakeParticleTime += fTimeDelta;
	m_fElapsedTime += fTimeDelta;
	if (m_fElapsedTime >= m_fLifeTime)
	{
			m_bIsDead = true;
	}

	//m_pTransformCom->Rotation(XMConvertToRadians(30.f));
	Orbit(fTimeDelta);
	if(m_fMakeParticleTime>=3.0f)
		Make_Particle(fTimeDelta);
}

void CDrone::Late_Tick(_float fTimeDelta)
{

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CDrone::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	
	
	
	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();
	
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		
	
		m_pShaderCom->Begin(0);
	
		m_pModelCom->Render(i);
	}
	if (m_bIsDead)
	{
		m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Player_Drone"), this);
	}

	return S_OK;
}

void CDrone::Orbit(_float fTimeDelta)
{
	if (m_pPlayerWorldMatrix == nullptr)
		return;

	_matrix mPlayerWorldMatrix = XMLoadFloat4x4(m_pPlayerWorldMatrix);
	// 현재 각도 갱신
	m_fCurrentAngle += m_fOrbitSpeed * fTimeDelta;
	if (m_fCurrentAngle >= XM_2PI)
		m_fCurrentAngle -= XM_2PI;

	// 플레이어의 위치 가져오기
	_float4x4 PlayerWorldMatrix;
	XMStoreFloat4x4(&PlayerWorldMatrix, mPlayerWorldMatrix);

	_float3 vPlayerPosition = _float3(PlayerWorldMatrix._41, PlayerWorldMatrix._42, PlayerWorldMatrix._43);

	// 드론의 새로운 위치 계산
	_float fX = vPlayerPosition.x + m_fOrbitRadius * cosf(m_fCurrentAngle);
	_float fZ = vPlayerPosition.z + m_fOrbitRadius * sinf(m_fCurrentAngle);
	_float fY = vPlayerPosition.y; // 필요 시 y좌표도 업데이트

	// 드론 위치 갱신
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fX, fY, fZ, 1.0f));



	
}

void CDrone::Make_Particle(_float fTimeDelta)
{

	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	CPlayerBullet::CPlayetBULLET_DESC CSwordThowDesc{};
	switch (m_eChracterType)
	{
	case Client::CPlayer::CHRACTER_SWORD_MASTER:
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Technic_XC_slashCircle_SizeUP"),_float4(0.0f,0.0f,1.0f,0.3f) });

		CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
		CSwordThowDesc.fRadius = 1.5f;
		CSwordThowDesc.strModelName = TEXT("swordThrowing");
		CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);
		CSwordThowDesc.BulletState = &CBullet::Pop;
		CSwordThowDesc.fLifeTime = 0.5f;
		m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);
		m_fMakeParticleTime= 0.0f;
		break;
	case Client::CPlayer::CHRACTER_GUN_SLINGER:
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"),_float4(1.0f,0.8f,0.0f,0.6f) });
		vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z,TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"),_float4(0.9f,0.0f,0.1f,0.7f) });
		CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
		CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f), -m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.5f);
		CSwordThowDesc.fRadius = 3.5f;
		CSwordThowDesc.strModelName = TEXT("swordThrowing");
		CSwordThowDesc.vPosition = _float4(this->Get_Position().x, this->Get_Position().y, this->Get_Position().z, 1.0f);
		CSwordThowDesc.BulletState = &CBullet::Pop;
		CSwordThowDesc.fLifeTime = 0.7f;
		m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &CSwordThowDesc);
		m_fMakeParticleTime = 0.0f;

		break;
	case Client::CPlayer::CHRACTER_END:
		break;
	default:
		break;
	}




}

HRESULT CDrone::Add_Components()
{

	/* For.Com_Collider */
	CBounding_OBB::BOUNDING_OBB_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_OBB;
	ColliderDesc.vExtents = _float3(1.0f, 1.0f, 1.0f);
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

HRESULT CDrone::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CDrone* CDrone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDrone* pInstance = new CDrone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CDrone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDrone::Clone(void* pArg)
{
	CDrone* pInstance = new CDrone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CDrone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDrone::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
