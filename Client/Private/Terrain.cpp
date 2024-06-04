#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"
#include"Loader.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-45.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(12.0f, 0.0f, 12.0f, 1.0f));

	//if (pArg != nullptr)
	//{
	//
	//	m_pTransformCom->Set_Scale(((CTerrain_DESC*)pArg)->fScale.x, ((CTerrain_DESC*)pArg)->fScale.y, ((CTerrain_DESC*)pArg)->fScale.z);
	//	FXMVECTOR vAxix = XMLoadFloat3(&((CTerrain_DESC*)pArg)->fRotateAxis);
	//	m_pTransformCom->Rotation(vAxix, XMConvertToRadians(((CTerrain_DESC*)pArg)->fRotateAngle));
	//
	//}


	return S_OK;
}

void CTerrain::Priority_Tick(_float fTimeDelta)
{
}

void CTerrain::Tick(_float fTimeDelta)
{
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldFloat4x4());
}

void CTerrain::Late_Tick(_float fTimeDelta)
{
	
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	//if (FAILED(Bind_ShaderResources()))
	//	return E_FAIL;
	//
	//m_pShaderCom->Begin(0);
	//m_pVIBufferCom->Bind_Buffers();
	//m_pVIBufferCom->Render();
#ifdef _DEBUG

	m_pNavigationCom->Render();

#endif
	return S_OK;
}

_vector CTerrain::Picking_Terrain()
{
	if (m_pVIBufferCom == nullptr || m_pTransformCom == nullptr || m_pShaderCom == nullptr)
		return _vector{ 0 };
	else
	{
		_vector test = {};
		test = m_pVIBufferCom->Picking_OnTerrain(g_hWnd,
			m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW),
			m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ),
			XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix())
		);



		return test;
		//m_pTransformCom.
	}

}

HRESULT CTerrain::Add_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	/* For.Com_Navigation */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
		return E_FAIL;



	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTerrain*		pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
