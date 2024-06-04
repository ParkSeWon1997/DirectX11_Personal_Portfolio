#include "stdafx.h"
#include "..\Public\Segment_SM.h"

#include "GameInstance.h"
#include"Loader.h"


CSegment_SM::CSegment_SM(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSegment_SM::CSegment_SM(const CSegment_SM & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSegment_SM::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSegment_SM::Initialize(void * pArg)
{

	CGameObject::GAMEOBJECT_DESC* GameObjectDesc = static_cast<CGameObject::GAMEOBJECT_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&GameObjectDesc->vPosition));

		m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-45.f));
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.0f, 0.0f, sqrtf(2) * 17.f, 1.0f));
	}

	

	return S_OK;
}

void CSegment_SM::Priority_Tick(_float fTimeDelta)
{
}

void CSegment_SM::Tick(_float fTimeDelta)
{
	

	
}

void CSegment_SM::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_MASK, this);
	//m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSegment_SM::Render()
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
	
	
	return S_OK;
}

HRESULT CSegment_SM::Add_Components()
{

	//m_strModelName
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Segment_SM"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	
		/* For.Com_Shader */
	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;	

	return S_OK;
}

HRESULT CSegment_SM::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CSegment_SM * CSegment_SM::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSegment_SM*		pInstance = new CSegment_SM(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSegment_SM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSegment_SM::Clone(void * pArg)
{
	CSegment_SM*		pInstance = new CSegment_SM(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSegment_SM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSegment_SM::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
