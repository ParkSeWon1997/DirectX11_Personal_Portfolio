
#include "..\Public\Particle_Mesh.h"

#include "GameInstance.h"

CParticle_Mesh::CParticle_Mesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CParticle_Mesh::CParticle_Mesh(const CParticle_Mesh & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticle_Mesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Mesh::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((PARTICLE_DESC*)pArg)->vStartPos));
		if (((PARTICLE_DESC*)pArg)->bIsRandome)
		{
			//m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		}
	}

	return S_OK;
}

void CParticle_Mesh::Priority_Tick(_float fTimeDelta)
{
	
}

void CParticle_Mesh::Tick(_float fTimeDelta)
{
	switch (m_eParticleType)
	{
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_NONE:
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD:
		m_pModelCom->Spread(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_DROP:
		m_pModelCom->Drop(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN:
		m_pModelCom->Up_And_Down(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION:
		m_pModelCom->Explosion(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP:
		m_pModelCom->Pop(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_RANDOM_POP:
		m_pModelCom->RandomPop(fTimeDelta);
		break;

	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP:
		m_pModelCom->SizeUp(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN:
		m_pModelCom->SizeDown(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_X:
		m_pModelCom->SizeDown_X(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Y:
		m_pModelCom->SizeDown_Y(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z:
		m_pModelCom->SizeDown_Z(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X:
		m_pModelCom->SizeUp_X(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Y:
		m_pModelCom->SizeUp_Y(fTimeDelta);
		break;

	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z:
		m_pModelCom->SizeUp_Z(fTimeDelta);
		break;

	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_MAX:
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_X:
		m_pModelCom->Rotation_X(fTimeDelta);
		break;


	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_Y:
		m_pModelCom->Rotation_Y(fTimeDelta);
		break;

	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_Z:
		m_pModelCom->Rotation_Z(fTimeDelta);
		break;




	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_END:
		break;
	default:
		break;
	}



	//m_pModelCom->Spread(fTimeDelta);
	//m_pModelCom->Drop(fTimeDelta);
	//m_pModelCom->Explosion(fTimeDelta);
	//m_pModelCom->SizeUp(fTimeDelta);
	//m_pModelCom->Pop(fTimeDelta);
	//m_pModelCom->Up_And_Down(fTimeDelta);
	//m_pModelCom->RandomPop(fTimeDelta);
	//m_pModelCom->RandRotate_SizeUp(fTimeDelta);
	//m_pModelCom->SizeDown(fTimeDelta);
	
}

void CParticle_Mesh::Late_Tick(_float fTimeDelta)
{
	CVIBuffer_Instance::INSTANCE_DESC 	ParticleDesc={};	

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CParticle_Mesh::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();


	m_pShaderCom->Begin(2);

	m_pModelCom->Bind_Buffer();

	m_pModelCom->Render_Particle();


	//for (size_t i = 0; i < iNumMeshes; i++)
	//{
	//	if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
	//		return E_FAIL;
	//
	//	m_pShaderCom->Begin(0);
	//	m_pModelCom->Render_Instance(i);
	//
	//
	//}


	return S_OK;
}

void CParticle_Mesh::Set_ParticleType(const PARTICLE_TYPE eParticleType)
{
	m_pModelCom->Set_ParticleTpye((_uint)eParticleType);
	m_eParticleType = eParticleType;
}

void CParticle_Mesh::Set_ParticleProperty(const CVIBuffer_Instance::INSTANCE_DESC& ParticleDesc)
{
	m_pModelCom->Set_ParticleProperty(ParticleDesc);
}

const CVIBuffer_Instance::INSTANCE_DESC CParticle_Mesh::Get_InstanceBufferDesc()
{
	return m_pModelCom->Get_ParticleProperty();
}



HRESULT CParticle_Mesh::Add_Components()
{
	m_strModelName;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_strModelName,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	//Prototype_Component_Shader_VtxMesh
	//Prototype_Component_Shader_VtxParticle
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxParticle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Panel"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	
	
	return S_OK;
}

HRESULT CParticle_Mesh::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;

	

	return S_OK;
}

CParticle_Mesh * CParticle_Mesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CParticle_Mesh*		pInstance = new CParticle_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CParticle_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParticle_Mesh::Clone(void * pArg)
{
	CParticle_Mesh*		pInstance = new CParticle_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CParticle_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
