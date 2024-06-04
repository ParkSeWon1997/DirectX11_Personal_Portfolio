#include "stdafx.h"
#include "..\Public\SegmentLine.h"

#include "GameInstance.h"
#include"Loader.h"


CSegmentLine::CSegmentLine(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSegmentLine::CSegmentLine(const CSegmentLine & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSegmentLine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSegmentLine::Initialize(void * pArg)
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
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.0f, 0.001f, sqrtf(2) * 17.f, 1.0f));
	}

	
	return S_OK;
}

void CSegmentLine::Priority_Tick(_float fTimeDelta)
{
}

void CSegmentLine::Tick(_float fTimeDelta)
{
	

	
}

void CSegmentLine::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CSegmentLine::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	
	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();
	
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		
	
		m_pShaderCom->Begin(1);
	
		m_pModelCom->Render(i);
	}
	
	
	return S_OK;
}

HRESULT CSegmentLine::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Segment_line"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	

	return S_OK;
}

HRESULT CSegmentLine::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CSegmentLine * CSegmentLine::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSegmentLine*		pInstance = new CSegmentLine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSegmentLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSegmentLine::Clone(void * pArg)
{
	CSegmentLine*		pInstance = new CSegmentLine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSegmentLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSegmentLine::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
