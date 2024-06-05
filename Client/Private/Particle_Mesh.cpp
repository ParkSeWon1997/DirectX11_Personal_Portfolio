#include"stdafx.h"
#include "..\Public\Particle_Mesh.h"

#include "GameInstance.h"
#include"Loader.h"

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
		m_eParticleType = ((PARTICLE_DESC*)pArg)->eParticleType;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((PARTICLE_DESC*)pArg)->vStartPos));
		m_pTransformCom->Set_Look(((PARTICLE_DESC*)pArg)->vLookDir);
		m_vColor = ((PARTICLE_DESC*)pArg)->fColor;
		m_bIsBloom = ((PARTICLE_DESC*)pArg)->bBloom;
		m_pGameCallObject = ((PARTICLE_DESC*)pArg)->pGameCallObject;
		m_fDisolveSpeed = ((PARTICLE_DESC*)pArg)->fDisolveSpeed;
		m_vPos= ((PARTICLE_DESC*)pArg)->vOffsetPos;
		if (((PARTICLE_DESC*)pArg)->bIsRandome)
		{
			_float3 vRandomPos;
			_float3 vRandomDir;
			RandomPosAndDirection(&vRandomPos, &vRandomDir, ((PARTICLE_DESC*)pArg)->vStartPos);

			// 랜덤한 위치 설정
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vRandomPos.x, vRandomPos.y, vRandomPos.z, 1.0f));

			// 랜덤한 방향 설정
			m_pTransformCom->Set_Look(XMVectorSet(vRandomDir.x, vRandomDir.y, vRandomDir.z, 0.0f));
		}
	}

	return S_OK;
}

void CParticle_Mesh::Priority_Tick(_float fTimeDelta)
{
	
}

void CParticle_Mesh::Tick(_float fTimeDelta)
{
	m_fDisolveTime += fTimeDelta * m_fDisolveSpeed;
	if (m_fDisolveTime < 0.f)
		m_fDisolveTime = 0.0f;



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
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_HALF_ALHPA:
		m_pModelCom->Pop(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_TEXTURE:
		m_pModelCom->Pop(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X:
		m_pModelCom->Rotation_X(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y:
		m_pModelCom->Rotation_Y(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z:
		m_pModelCom->Rotation_Z(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD_NONE_DISOLVE:
		m_pModelCom->Spread(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z_NONE_DISOLVE:
		m_pModelCom->SizeDown_Z(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_NONE_DISOLVE:
		m_pModelCom->SizeUp(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP_NONE_DISOLVE:
		m_pModelCom->Pop(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE:
		m_pModelCom->Rotation_X(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y_NONE_DISOLVE:
		m_pModelCom->Rotation_Y(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z_NONE_DISOLVE:
		m_pModelCom->Rotation_Z(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP_TEXTURE:
		m_pModelCom->Pop(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN_TEXTURE:
		m_pModelCom->Up_And_Down(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_TEXTURE:
		m_pModelCom->Rotation_X(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z_TEXTURE:
		m_pModelCom->SizeUp_Z(fTimeDelta);
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_PARTICLE_TYPE_EXPLOSION_NONE_DISOLVE:
		m_pModelCom->Explosion(fTimeDelta);
		break;


	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_END:
		break;
	default:
		break;
	}


	//if (m_bIsUpdate)
	//{
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION,m_vPos);
	//}

}

void CParticle_Mesh::Late_Tick(_float fTimeDelta)
{
	if (m_pGameCallObject != nullptr)
	{
		_vector vMovePos=	XMVectorAdd(m_pGameCallObject->Get_PositionVector(), m_vPos);
		vMovePos = XMVectorSetW(vMovePos, 1.0f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vMovePos);
		if (m_pGameCallObject->IsDead())
		{
			//m_bIsDead = true;
		}
			
	}

	if (m_bIsBloom)
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONLIGHT, this);
	}
	else
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_EFFECT, this);
}

HRESULT CParticle_Mesh::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	switch (m_eParticleType)
	{
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_NONE:
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(1);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_DROP:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(2);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(3);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;
			m_pShaderCom->Begin(4);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(5);
			m_pModelCom->Render_Instance(i);

		}
	
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_RANDOM_POP:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(6);
			m_pModelCom->Render_Instance(i);

		}
	
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP:

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(7);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN:

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(8);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_X:

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(9);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Y:

			for (size_t i = 0; i < iNumMeshes; i++)
			{
				if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
					return E_FAIL;
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
					return E_FAIL;

				m_pShaderCom->Begin(10);
				m_pModelCom->Render_Instance(i);

			}
			break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z:

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(11);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(9);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Y:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(10);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(11);
			m_pModelCom->Render_Instance(i);

		}
		break;

	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_HALF_ALHPA:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(12);
			m_pModelCom->Render_Instance(i);

		}

		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_COLOR_CHANGE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(13);
			m_pModelCom->Render_Instance(i);

		}
		break;


	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_TEXTURE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(14);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(15);
			m_pModelCom->Render_Instance(i);
		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(15);
			m_pModelCom->Render_Instance(i);
		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(15);
			m_pModelCom->Render_Instance(i);
		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z_NONE_DISOLVE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(16);
			m_pModelCom->Render_Instance(i);

		}
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_NONE_DISOLVE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(17);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP_NONE_DISOLVE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(17);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD_NONE_DISOLVE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(18);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(17);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y_NONE_DISOLVE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(17);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z_NONE_DISOLVE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(17);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN_TEXTURE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(19);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_TEXTURE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(19);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP_TEXTURE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(19);
			m_pModelCom->Render_Instance(i);

		}
		break;
	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z_TEXTURE:
		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material_Instance(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
				return E_FAIL;

			m_pShaderCom->Begin(19);
			m_pModelCom->Render_Instance(i);

		}
		break;
	


	case Client::CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_END:
		break;
	default:
		break;
	}



	m_pModelCom->Bind_Buffer();

	m_pModelCom->Render_Particle();

	if (m_pModelCom->Get_ParticleDesc().IsDead)
	{
		if (FAILED(m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Effect"), this)))
			return E_FAIL;
		
	}
	if (m_bIsDead)
	{
		if (FAILED(m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Effect"), this)))
			return E_FAIL;
	}

	return S_OK;
}

void CParticle_Mesh::Set_ParticleProperty(const CVIBuffer_Instance::INSTANCE_DESC& ParticleDesc)
{
	m_pModelCom->Set_ParticleProperty(ParticleDesc);
}

const CVIBuffer_Instance::INSTANCE_DESC CParticle_Mesh::Get_InstanceBufferDesc()
{
	return m_pModelCom->Get_ParticleProperty();
}



void CParticle_Mesh::RandomPosAndDirection(_float3* pOutPosition, _float3* pOutDirection,const _float4& vStartPos)
{
	// 일정 범위 내에서 랜덤한 위치 생성
	_float fRange = 3.0f; // 예시로 범위를 5로 설정
	_float fRandomX = (static_cast<_float>(rand()) / RAND_MAX) * fRange * 2.0f - fRange; // -fRange부터 fRange까지의 랜덤한 값
	_float fRandomZ = (static_cast<_float>(rand()) / RAND_MAX) * fRange * 2.0f - fRange;

	// 랜덤한 위치 설정
	*pOutPosition = _float3(vStartPos.x + fRandomX, vStartPos.y, vStartPos.z + fRandomZ);

	// 랜덤한 방향 생성
	_float fRandomDirX = (static_cast<_float>(rand()) / RAND_MAX) * 2.0f - 1.0f; // -1부터 1까지의 랜덤한 값
	_float fRandomDirY = (static_cast<_float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
	_float fRandomDirZ = (static_cast<_float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
	XMVECTOR vRandomDir = XMVector3Normalize(XMLoadFloat3(&_float3(fRandomDirX, fRandomDirY, fRandomDirZ)));

	// 랜덤한 방향 설정
	XMStoreFloat3(pOutDirection, vRandomDir);
}

HRESULT CParticle_Mesh::Add_Components()
{
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxParticle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Texture_noise001"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	
	
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
	if (FAILED(m_pShaderCom->Bind_RawValue("g_DisolveAmount", &m_fDisolveTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;
	

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

	Safe_Release(m_pGameCallObject);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
