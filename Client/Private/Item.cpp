#include "stdafx.h"
#include "..\Public\Item.h"

#include "GameInstance.h"
#include"Loader.h"
#include"CTotalSingleton.h"

#include "Player.h"
#include"UpgradeMachineTop.h"
#include"Particle_Mesh.h"
#include"Level_Loading.h"

#include"CBullet.h"


CItem::CItem(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CItem::CItem(const CItem & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void * pArg)
{
	
	CItem::CItem_DESC* GameObjectDesc = static_cast<CItem::CItem_DESC*>(pArg);
	GameObjectDesc->fSpeedPerSec = 5.0f;



	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	if (pArg != nullptr)
	{

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&GameObjectDesc->vPosition));
		m_pTransformCom->Scaling(GameObjectDesc->vScale.x, GameObjectDesc->vScale.y, GameObjectDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&GameObjectDesc->vRotation), GameObjectDesc->vRotationAngle);
		m_vTargetPos= GameObjectDesc->vTargetPos;
		m_vDirection = GameObjectDesc->vDirection;
		
	}
	

	return S_OK;
}

void CItem::Priority_Tick(_float fTimeDelta)
{
}

void CItem::Tick(_float fTimeDelta)
{		

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));

	m_fLifeTime += fTimeDelta;

	_vector vItemPos = this->Get_PositionVector();
	_vector vPlayerPos = pPlayer->Get_PositionVector();
	_vector vDistance = XMVectorSet(XMVectorGetX(vPlayerPos) - XMVectorGetX(vItemPos), 0.0f, XMVectorGetZ(vPlayerPos) - XMVectorGetZ(vItemPos), 0.0f);
	_float fDistance = XMVectorGetX(XMVector3Length(vDistance));


	
	if (m_fLifeTime >= 3.0f)
	{
		m_pTransformCom->LookAt(pPlayer->Get_PositionVector());

		// 플레이어와의 거리에 따른 속도 보간
		_float fMaxDistance = 20.0f; // 최대 거리 (이 거리 이상에서는 최대 속도)
		_float fMinSpeed = 1.0f; // 최소 속도
		_float fMaxSpeed = 30.f; // 최대 속도

		_float fInterpolatedSpeed = fMaxSpeed;
		if (fDistance < fMaxDistance)
			fInterpolatedSpeed = fMinSpeed + (fMaxSpeed - fMinSpeed) * (fDistance / fMaxDistance);

		m_pTransformCom->Set_Speed(fInterpolatedSpeed);
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	else
	{
		if (!bClose)
		{
			m_pTransformCom->Go_ToTarget(fTimeDelta, 1.0f, m_vTargetPos, bClose);
			m_pTransformCom->Set_Speed(7.f);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}
	}

	
}
	


void CItem::Late_Tick(_float fTimeDelta)
{
	

	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		if (m_pColliderCom == nullptr)
			continue;

		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		//pPlayer->Intersect(CPlayer::PART_WEAPON, TEXT("Com_Collider"), m_pColliderCom[i]);
	}
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		return ;
	if (pPlayer->Intersect(CPlayer::PART_BODY_MATILDA, TEXT("Com_Collider"), m_pColliderCom))
	{




		m_bIsDead = true;
	
	}



	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
	

#ifdef _DEBUG
	
		if (m_pColliderCom != nullptr)
			m_pGameInstance->Add_DebugComponent(m_pColliderCom);
	

#endif
}

HRESULT CItem::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	int	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int  i = 0; i < iNumMeshes; i++)
	{
		//g_Texture
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		

		m_pShaderCom->Begin(2);
	
		m_pModelCom->Render(i);
	}


	if (m_bIsDead)
	{
		if (FAILED(m_pGameInstance->Delete_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Environment"), this)))
			return E_FAIL;


		m_pGameInstance->Play_Sound_Z(TEXT("SFX_8bit002 [1].wav"), SOUND_EFFECT, 0.3f);

		if (m_strModelName == TEXT("Item_HP"))
		{
			CTotalSingleton::GetInstance()->SetPlayerAbillty_AddHp(1.0f);
		}
		if (m_strModelName == TEXT("Item_HP_B"))
		{
			CTotalSingleton::GetInstance()->SetPlayerAbillty_AddHp(3.0f);
		}
		if (m_strModelName == TEXT("Item_Coin"))
		{
			CTotalSingleton::GetInstance()->AddCoin(100);
		}

		if (m_strModelName == TEXT("Item_Coin_B"))
		{
			CTotalSingleton::GetInstance()->AddCoin(300);
		}



	}


	return S_OK;
}






HRESULT CItem::Add_Components()
{

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;




	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	

	/* For.Com_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		ColliderOBBDesc{};

	ColliderOBBDesc.eType = CCollider::TYPE_OBB;
	ColliderOBBDesc.vExtents = _float3(0.5f, 1.5f, 0.5f);
	ColliderOBBDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderOBBDesc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderOBBDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CItem::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CItem * CItem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CItem*		pInstance = new CItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem*		pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();
	
		Safe_Release(m_pColliderCom);
	



	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
