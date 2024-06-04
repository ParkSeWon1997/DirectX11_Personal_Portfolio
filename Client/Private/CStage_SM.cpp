#include "stdafx.h"
#include "..\Public\CStage_SM.h"

#include "GameInstance.h"
#include"Loader.h"
CStage_SM::CStage_SM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CStage_SM::CStage_SM(const CStage_SM& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CStage_SM::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStage_SM::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_float4 fPos = static_cast<CGameObject::GAMEOBJECT_DESC*>(pArg)->vPosition;
		m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-45.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, fPos.y, fPos.z, 1.0f));

	}
	return S_OK;
}

void CStage_SM::Priority_Tick(_float fTimeDelta)
{
}

void CStage_SM::Tick(_float fTimeDelta)
{



}

void CStage_SM::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CStage_SM::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		//g_DiffuseTexture
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_MapMask"), m_pShaderCom, "g_MapMaskTexture");
		//if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

HRESULT CStage_SM::Add_Components()
{
	
	/* For.Com_Model */
	//TEXT("StageGround_A_57_New_A")
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	
	/* For.Com_Shader */
		//Prototype_Component_Shader_Mesh_Stage_SM
	//Prototype_Component_Shader_Mesh_Stage_GM
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_Mesh_Stage_GM"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	
	///* For.Com_Shader */
	//if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStage_SM::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CStage_SM* CStage_SM::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStage_SM* pInstance = new CStage_SM(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStage_SM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStage_SM::Clone(void* pArg)
{
	CStage_SM* pInstance = new CStage_SM(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStage_SM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage_SM::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
