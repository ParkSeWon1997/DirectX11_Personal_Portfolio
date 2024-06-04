
#include "..\Public\Terrain.h"

#include "GameInstance.h"

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
	if (pArg != nullptr)
	{

		m_pTransformCom->Set_Scale(((CTerrain_DESC*)pArg)->fScale.x, ((CTerrain_DESC*)pArg)->fScale.y, ((CTerrain_DESC*)pArg)->fScale.z);
		FXMVECTOR vAxix = XMLoadFloat3(&((CTerrain_DESC*)pArg)->fRotateAxis);
		m_pTransformCom->Rotation(vAxix, XMConvertToRadians(((CTerrain_DESC*)pArg)->fRotateAngle));

	}


	return S_OK;
}

void CTerrain::Priority_Tick(_float fTimeDelta)
{
}

void CTerrain::Tick(_float fTimeDelta)
{
}

void CTerrain::Late_Tick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();
#ifdef _DEBUG



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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_DIFFUSE]))))
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

	if (FAILED(m_pTextureCom[TEX_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_float("g_PixelID", &m_fObjOwnderID)))
	//	return E_FAIL;

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




	Safe_Release(m_pShaderCom);
	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);
	Safe_Release(m_pVIBufferCom);
}
