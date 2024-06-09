#include "stdafx.h"
#include "..\Public\CutScene.h"

#include "GameInstance.h"
#include"Loader.h"

#include"Player.h"
#include"CBullet.h"
//#include"CSwordThow.h"

#include"Particle_Mesh.h"
CutScene::CutScene(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CutScene::CutScene(const CutScene & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CutScene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CutScene::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{	
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(10.f,0.0f,10.f,1.0f));
	}
	//m_pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();


	return S_OK;
}

void CutScene::Priority_Tick(_float fTimeDelta)
{

}

void CutScene::Tick(_float fTimeDelta)
{



	m_pModelCom->Play_Animation(fTimeDelta);

	m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	int temp = 0;
}

void CutScene::Late_Tick(_float fTimeDelta)
{


	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CutScene::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


}

HRESULT CutScene::Add_Components()
{
	
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Sequence_M_130525"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC	NavigationDesc{};
	NavigationDesc.iCurrentCellIndex = 0;

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;






	return S_OK;
}


HRESULT CutScene::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}


CutScene* CutScene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CutScene* pInstance = new CutScene(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CutScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CutScene::Clone(void* pArg)
{
	CutScene* pInstance = new CutScene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CutScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CutScene::Free()
{
	__super::Free();

	for(_uint i = 0; i < COLLIDER_END; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
}
