#include "stdafx.h"
#include "..\Public\UpgradeMachine.h"

#include "GameInstance.h"
#include"Loader.h"

#include "Player.h"
#include"UpgradeMachineTop.h"
#include"Particle_Mesh.h"

CUpgradeMachine::CUpgradeMachine(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{
}

CUpgradeMachine::CUpgradeMachine(const CUpgradeMachine & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CUpgradeMachine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUpgradeMachine::Initialize(void * pArg)
{
	CUpgradeMachine::CUpgradeMachine_DESC*	GameObjectDesc=static_cast<CUpgradeMachine::CUpgradeMachine_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&GameObjectDesc->vPosition));
		m_pTransformCom->Scaling(GameObjectDesc->vScale.x, GameObjectDesc->vScale.y, GameObjectDesc->vScale.z);
		m_pTransformCom->QuatRotation(XMLoadFloat3(&GameObjectDesc->vRotation), GameObjectDesc->vRotationAngle);

		m_eMachineType = GameObjectDesc->eMachineType;
	}

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f,1.0f));

	m_vOriginSize= m_pTransformCom->Get_Scaled();
	m_vTargetSize.x= m_vOriginSize.x + 0.5f;
	m_vTargetSize.y = m_vOriginSize.y+ 1.f;
	m_vTargetSize.z= m_vOriginSize.z + 0.5f;
	m_bIsGrow = true;

	if(FAILED(Make_MachineTop()))
		return E_FAIL;





	return S_OK;
}

void CUpgradeMachine::Priority_Tick(_float fTimeDelta)
{
}

void CUpgradeMachine::Tick(_float fTimeDelta)
{		if (m_bIsFoot)
		{
			_float3 vSize = m_pTransformCom->Get_Scaled();
			if (m_bIsGrow)
			{
				// 크기 증가
				vSize.x += fTimeDelta* 5.f;
				vSize.z += fTimeDelta * 5.f;

				// 목표 크기에 도달했는지 확인
				if (vSize.x >= m_vTargetSize.x && vSize.z >= m_vTargetSize.z)
				{
					vSize.x = m_vTargetSize.x;
					vSize.z = m_vTargetSize.z;
					m_bIsGrow = false; // 크기를 줄이도록 설정
				}
			}
			else 
			{
				// 크기 감소
				vSize.x -= fTimeDelta * 5.f;
				vSize.z -= fTimeDelta * 5.f;

				// 원래 크기에 도달했는지 확인
				if (vSize.x <= m_vOriginSize.x && vSize.z <= m_vOriginSize.z)
				{
					vSize.x = m_vOriginSize.x;
					vSize.z = m_vOriginSize.z;
					//m_bIsGrow = true; // 크기를 다시 키우도록 설정
				}
			}

			m_pTransformCom->Set_Scale(vSize.x, 1.f, vSize.z);
			if(m_bIsFullCharge)
			{
				switch (m_eMachineType)
				{
				case Client::CUpgradeMachine::MACHINE_ADD_HP:
					break;
				case Client::CUpgradeMachine::MACHINE_ADD_SHIELD:
					break;
				case Client::CUpgradeMachine::MACHINE_ADD_MOVE_SPEED:
					break;
				case Client::CUpgradeMachine::MACHINE_ADD_ATTACK_SPEED:
					break;
				case Client::CUpgradeMachine::MACHINE_ADD_ATTACK_DAMAGE:
					break;
				case Client::CUpgradeMachine::MACHINE_ADD_DASH_COUNT:
					break;
				case Client::CUpgradeMachine::MACHINE_ADD_EXP:
					break;
				case Client::CUpgradeMachine::MACHINE_TYPE_END:
					break;
				default:
					break;
				}
				m_pMachineTop->Set_Activated(true);

				




			}

		}
	


}
	


void CUpgradeMachine::Late_Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Player")));
	if (nullptr == pPlayer)
		return;
	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		if (m_pColliderCom[i] == nullptr)
			continue;

		m_pColliderCom[i]->Tick(m_pTransformCom->Get_WorldMatrix());
		if (pPlayer->Intersect(CPlayer::PART_BODY_MATILDA, TEXT("Com_Collider"), m_pColliderCom[i]))
		{

			m_fGrowingTime += fTimeDelta;
			m_bIsFoot=	true;
			if (m_fGrowingTime >= 1.f)
			{
				m_iOnOff[0] = 1;
			
			}
			if(m_fGrowingTime>=2.f)
			{
				m_iOnOff[1] = 1;
			}
			if (m_fGrowingTime >= 3.f)
			{
				m_iOnOff[2] = 1;
			}
			if (m_fGrowingTime >= 4.f)
			{
				m_iOnOff[3] = 1;
	
			}
			if (m_fGrowingTime >= 4.5f)
			{
				for (_uint i = 0; i < 4; ++i)
				{
					m_iOnOff[i] = 0;
				}
				m_bIsFullCharge = true;
				m_fGrowingTime = 0.f;
				vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN,TEXT("Boss_Attack_Warning_Ray_SizeDown"),_float4(1.0f,0.9f,0.9f,0.8f)},
					{CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("UpgradeMachine_Sizeup"),_float4(1.0f,0.9f,0.9f,0.8f)},

				};
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(this->Get_Position().x, 0.0f, this->Get_Position().z, 1.0f));
			
			}
			
		}
		else
		{
			m_bIsGrow = true;
			m_bIsFoot = false;
			m_fGrowingTime = 0.f;
			m_bIsFullCharge = false;
			for (_uint i = 0; i < 4; ++i)
			{
				m_iOnOff[i] = 0;
			}

		}
	}






	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	for (size_t i = 0; i < COLLIDER_END; i++)
	{
		if (m_pColliderCom[i] != nullptr)
			m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
	}

#endif
}

HRESULT CUpgradeMachine::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int  i = 0; i < iNumMeshes; i++)
	{
		//g_Texture
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		

		if (FAILED(m_pShaderCom->Bind_RawValue("g_MeshNum", &i,sizeof(int))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_intArray("g_IntParam", m_iOnOff,4)))
			return E_FAIL;
		


		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	


	return S_OK;
}

HRESULT CUpgradeMachine::Make_MachineTop()
{

	CGameObject::GAMEOBJECT_DESC desc{};

	desc.strModelName = TEXT("UpgradeMachine");
	desc.vPosition = _float4(this->Get_Position().x, 5.0f, this->Get_Position().z,1.0f);
	desc.vScale = _float3(1.5f, 1.5f, 1.5f);
	desc.vRotation = _float3(0.f, 1.f, 0.f);
	desc.vRotationAngle = 0.f;
	if (FAILED(m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Environment"), TEXT("Prototype_UpgradeMachineTop"), &desc)))
		return E_FAIL;
	
	m_pMachineTop= dynamic_cast<CUpgradeMachineTop*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Environment")));
	if (nullptr == m_pMachineTop)
		return E_FAIL;


	return S_OK;
}

HRESULT CUpgradeMachine::Add_Components()
{

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_Mesh_Upgrade"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Shader */
//	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_VtxMesh"),
//		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
//		return E_FAIL;	

	/* For.Com_Collider_OBB */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_SPHERE;
	ColliderDesc.fRadius = 1.f;
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider_OBB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLLIDER_SPHERE]), &ColliderDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUpgradeMachine::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CUpgradeMachine * CUpgradeMachine::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUpgradeMachine*		pInstance = new CUpgradeMachine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUpgradeMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUpgradeMachine::Clone(void * pArg)
{
	CUpgradeMachine*		pInstance = new CUpgradeMachine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUpgradeMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUpgradeMachine::Free()
{
	__super::Free();
	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}



	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
