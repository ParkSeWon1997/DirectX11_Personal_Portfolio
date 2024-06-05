#include "stdafx.h"
#include "..\Public\UpgradeMachineTop.h"

#include "GameInstance.h"
#include"Loader.h"

#include "Player.h"


CUpgradeMachineTop::CUpgradeMachineTop(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CUpgradeMachineTop::CUpgradeMachineTop(const CUpgradeMachineTop & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CUpgradeMachineTop::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUpgradeMachineTop::Initialize(void * pArg)
{
	CGameObject::GAMEOBJECT_DESC*	GameObjectDesc = (CGameObject::GAMEOBJECT_DESC*)pArg;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&GameObjectDesc->vPosition));
		m_pTransformCom->Scaling(GameObjectDesc->vScale.x, GameObjectDesc->vScale.y, GameObjectDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&GameObjectDesc->vRotation), GameObjectDesc->vRotationAngle);

	}

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f,1.0f));

	return S_OK;
}

void CUpgradeMachineTop::Priority_Tick(_float fTimeDelta)
{
}

void CUpgradeMachineTop::Tick(_float fTimeDelta)
{		

	_float fNowPos_Y = this->Get_Position().y;
		m_fUpAcc += fTimeDelta * m_fSpeed;
		_float fUp_y = sin(XMConvertToRadians(m_fUpAcc));
		if (m_fUpAcc >= 360.0f)
			m_fUpAcc = 0.f;
		this->Set_Position(_float3(this->Get_Position().x, m_fTargetDownPos_Y + fUp_y, this->Get_Position().z));

}
	


void CUpgradeMachineTop::Late_Tick(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_SHADOWOBJ, this);

}

HRESULT CUpgradeMachineTop::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int  i = 0; i < iNumMeshes; i++)
	{
		//g_Texture
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}
	


	return S_OK;
}

HRESULT CUpgradeMachineTop::Render_LightDepth()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	/* ±¤¿ø ±âÁØÀÇ ºä º¯È¯Çà·Ä. */
	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 10.f, -10.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), (_float)g_iWinSizeX / g_iWinSizeY, 0.1f, 1000.f));

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;

	int	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;


		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CUpgradeMachineTop::Add_Components()
{

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_Mesh_Upgrade"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;



	return S_OK;
}

HRESULT CUpgradeMachineTop::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CUpgradeMachineTop * CUpgradeMachineTop::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUpgradeMachineTop*		pInstance = new CUpgradeMachineTop(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUpgradeMachineTop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUpgradeMachineTop::Clone(void * pArg)
{
	CUpgradeMachineTop*		pInstance = new CUpgradeMachineTop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUpgradeMachineTop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUpgradeMachineTop::Free()
{
	__super::Free();



	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
