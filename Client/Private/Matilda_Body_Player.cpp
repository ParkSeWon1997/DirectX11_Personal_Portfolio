#include "stdafx.h"
#include "Matilda_Body_Player.h"

#include "GameInstance.h"
#include"Loader.h"


#include "CFsm_Anim.h"
#include "Player.h"
#include"Weapon.h"



#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"

#include"CMyNode.h"


#include"CBullet.h"
#include"CPlayerBullet.h"
#include"Particle_Mesh.h"

Matilda_Body_Player::Matilda_Body_Player(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CPartObject{ pDevice, pContext }
{
}

Matilda_Body_Player::Matilda_Body_Player(const Matilda_Body_Player & rhs)
	: CPartObject{ rhs }
{
}

HRESULT Matilda_Body_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Matilda_Body_Player::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;	

	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(rand() % 20, true));	
	
	Matilda_BODY_DESC* pDesc = (Matilda_BODY_DESC*)pArg;
	m_pPlayer= pDesc->m_pPlayer;


	
	CActionNode* pIdle = CActionNode::create(bind(&Matilda_Body_Player::DoIdle, this));
	CActionNode* pHit= CActionNode::create(bind(&Matilda_Body_Player::DoHit, this));
	CActionNode* pRun= CActionNode::create(bind(&Matilda_Body_Player::DoRun, this));
	CActionNode* pDash = CActionNode::create(bind(&Matilda_Body_Player::DoDash, this));
	CActionNode* pMoveStop= CActionNode::create(bind(&Matilda_Body_Player::DoMoveAfterStop, this));
	CActionNode* pAttackLfet= CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Left, this));
	CActionNode* pAttackRight= CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Right, this));
	CActionNode* pAttackEnd= CActionNode::create(bind(&Matilda_Body_Player::DoAttack_End, this));
	CActionNode* pAttackEnd2= CActionNode::create(bind(&Matilda_Body_Player::DoAttack_End2, this));
	CActionNode* pBackDash = CActionNode::create(bind(&Matilda_Body_Player::DoBackDash, this));
	CActionNode* pThrowWeapon = CActionNode::create(bind(&Matilda_Body_Player::DoThrowWeapon, this));
	CActionNode* pAttackBefore = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Before, this));
	CActionNode* pAfterAttackIdle = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_After_Idle, this));
	CActionNode* pMatildaHidden_D_Short = CActionNode::create(bind(&Matilda_Body_Player::DoMatildaHidden_D_Short, this));
	CActionNode* pAttackStinger= CActionNode::create(bind(&Matilda_Body_Player::DoAttackStinger, this));
	CActionNode* pAttackSpin = CActionNode::create(bind(&Matilda_Body_Player::DoAttakcSpin, this));
	CActionNode* pAttackSpinEnd = CActionNode::create(bind(&Matilda_Body_Player::DoAttackSpinEnd, this));
	CActionNode* pAttackStomp = CActionNode::create(bind(&Matilda_Body_Player::DoAttackStomp, this));
	CActionNode* pAttackSThrow = CActionNode::create(bind(&Matilda_Body_Player::DoAttackThrow, this));
	CActionNode* pAttackJumpEnd = CActionNode::create(bind(&Matilda_Body_Player::DoAttackJumpEnd, this));
	CActionNode* pAttackUltimateBalance = CActionNode::create(bind(&Matilda_Body_Player::DoAttackUltimateBalance, this));
	CActionNode* pAttackUltimateTechnic = CActionNode::create(bind(&Matilda_Body_Player::DoAttackUltimateTechnic, this));
	CActionNode* pAttackUltimatePower = CActionNode::create(bind(&Matilda_Body_Player::DoAttackUltimatePower, this));
	CActionNode* pAttackUltimateEnd = CActionNode::create(bind(&Matilda_Body_Player::DoAttackUltimateReverse, this));
	
	CActionNode* pDoAttack_Shot = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Shot, this));
	CActionNode* pDoAttack_Shot_Critical = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Shot_Critical, this));
	CActionNode* pDoAttack_Shot_Dash = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Shot_Dash, this));
	CActionNode* pDoAttack_Dash_Buck = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Dash_Buck, this));
	CActionNode* pDoAttack_Dash_Buck_Next = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Dash_Buck_Next, this));
	CActionNode* pDoAttack_Dash_Slide_Shot = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Dash_Slide_Shot, this));
	CActionNode* pDoAttack_Shot_Crouch = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Shot_Crouch, this));
	CActionNode* pDoAttack_Shot_Upper = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Shot_Upper, this));
	CActionNode* pDoAttack_Shot_Ghost_Crouch = CActionNode::create(bind(&Matilda_Body_Player::DoAttack_Shot_Ghost_Crouch, this));
	
	CSelector* pRunOrStop = CSelector::Create(pRun,pDash,pMoveStop);
	
	
	CSelector* pComboAttackSelector = CSelector::Create(pAttackLfet, pAttackRight, pDoAttack_Shot, pDoAttack_Shot_Critical);
	
	CSequence* pBackDashSeq = CSequence::Create(pBackDash, pThrowWeapon, pAttackEnd);
	
	CSequence* pAttackSpinSeq= CSequence::Create(pAttackSpin, pAttackSpinEnd);
	
	
	CSequence* pListAttackThrowSeq= CSequence::Create(pAttackSThrow, pAttackJumpEnd);
	
	CSelector* pUltimateSelector = CSelector::Create(pAttackUltimateBalance, pDoAttack_Shot_Crouch, pAttackUltimateTechnic, pAttackUltimatePower, pDoAttack_Shot_Upper,pAttackUltimateEnd, pDoAttack_Shot_Ghost_Crouch);
	
	CSelector* pAttackSelector = CSelector::Create(pComboAttackSelector, pBackDashSeq, pDoAttack_Shot_Dash, pDoAttack_Dash_Buck, pDoAttack_Dash_Buck_Next, pDoAttack_Dash_Slide_Shot,pMatildaHidden_D_Short, pAttackStinger, pAttackSpinSeq, pAttackStomp ,pListAttackThrowSeq, pUltimateSelector);
	
	CSelector* pOtherAnimSelect = CSelector::Create(pRunOrStop, pAttackSelector);
	
	CSequence* pEndAttackSeq = CSequence::Create(pAttackBefore, pAfterAttackIdle);
	
	CSelector* pIdleOrOther = CSelector::Create(pIdle, pHit, pAttackEnd2, pEndAttackSeq, pOtherAnimSelect);
	
	
	m_pRootNode = CSequence::Create(pIdleOrOther);


	return S_OK;
}

void Matilda_Body_Player::Priority_Tick(_float fTimeDelta)
{
}

void Matilda_Body_Player::Tick(_float fTimeDelta)
{
	m_matWeaponMatrix = XMLoadFloat4x4(m_pWeaponMatrix);
	m_matWeaponMatrix.r[0] = XMVector3Normalize(m_matWeaponMatrix.r[0]);
	m_matWeaponMatrix.r[1] = XMVector3Normalize(m_matWeaponMatrix.r[1]);
	m_matWeaponMatrix.r[2] = XMVector3Normalize(m_matWeaponMatrix.r[2]);
	

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix()  * XMLoadFloat4x4(m_pParentMatrix));


 	CModel::ANIMATION_DESC		AnimDesc{ 0, true };
	m_pRootNode->Evaluate();
	
	AnimDesc.iAnimIndex = m_iAnimIndex;
	AnimDesc.isLoop = m_bLoop;


	m_pModelCom->Set_AnimationIndex(AnimDesc);

	m_pModelCom->Play_Animation(fTimeDelta* m_AnimSpeed);

	
	m_pColliderCom->Tick(XMLoadFloat4x4(&m_WorldMatrix));
	CreateEffect_By_Motion(fTimeDelta);
}

void Matilda_Body_Player::Late_Tick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_SHADOWOBJ, this);
#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif


}

HRESULT Matilda_Body_Player::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	
	
	
	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();
	
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);
	
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;		
		m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_MapMask"), m_pShaderCom, "g_MapMaskTexture");
		m_pShaderCom->Begin(0);
			m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT Matilda_Body_Player::Render_LightDepth()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	/* 광원 기준의 뷰 변환행렬. */
	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 10.f, -10.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), (_float)g_iWinSizeX / g_iWinSizeY, 0.1f, 1000.f));

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void Matilda_Body_Player::CreateEffect_By_Motion(_float fTimeDelta)
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	double fCurPos = m_pModelCom->Get_CurrentPosition();
	if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_STOMP)
	{
		//for.Particle
		if (fCurPos >= 0.48 && fCurPos < 0.50)
		{
			
			m_pWeapon->SetParticleCreate(true, CWeapon::WEAPON_PARTICLE_TYPE::POWER_C);
			
		}
	}


	if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_UPPER)
	{
		//for.Particle
		if (fCurPos >= 0.28 && fCurPos < 0.32)
		{
			m_pWeapon->SetParticleCreate(true, CWeapon::WEAPON_PARTICLE_TYPE::BALANCE_X);

		}


	}
	if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_ATTACK_STINGER)
	{
		//for.Particle
		if (fCurPos >= 0.12 && fCurPos < 0.16)
		{
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_NONE_DISOLVE,TEXT("Player_C_Balalnce_stingerRing_Size_Up"),_float4(0.8f,0.8f,1.0f,0.5f),false,true});
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_NONE_DISOLVE,TEXT("Player_C_Balalnce_stinger.001_Size_Down_Z"),_float4(0.8f,0.8f,1.0f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z,TEXT("Player_C_Balalnce_stingerHalftone_Size_Down_Z"),_float4(0.8f,0.8f,1.0f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z,TEXT("Player_C_Balalnce_LowpolySphere16_Size_Down_Z"),_float4(0.8f,0.8f,1.0f,0.5f),false,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z,TEXT("Player_C_Balalnce_stingerRing_Size_Up"),_float4(0.6f,0.8f,1.0f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z,TEXT("Player_C_Balalnce_stingerRing.001_Size_Up"),_float4(0.7f,0.7f,1.0f,0.5f) });

			_vector vPlayerLook = dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir();
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(m_pPlayer->Get_Position().x, m_pPlayer->Get_Position().y+1.f, m_pPlayer->Get_Position().z, 1.0f), -vPlayerLook);
			m_pWeapon->SetParticleCreate(true, CWeapon::WEAPON_PARTICLE_TYPE::BALANCE_C);

		}


	}
	if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_SPIN)
	{
		//for.Particle
		if (fCurPos >= 0.04 && fCurPos < 0.06)
		{
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_Technic_C_slashShockwave_Rotate"),_float4(0.1f,0.2f,1.0f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_Technic_C_slashCircleSword_Rotate"),_float4(0.2f,0.2f,1.0f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Y,TEXT("Player_Technic_C_slashCircle_Rotate"),_float4(0.3f,0.3f,1.0f,0.5f) });
	
			_vector vPlayerLook = dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir();
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(m_pPlayer->Get_Position().x, m_pPlayer->Get_Position().y+1.f, m_pPlayer->Get_Position().z, 1.0f), -vPlayerLook);
			m_pWeapon->SetParticleCreate(true, CWeapon::WEAPON_PARTICLE_TYPE::BALANCE_C);

		}


	}




}

HRESULT Matilda_Body_Player::Add_Components()
{
	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_AABB;
	ColliderDesc.vExtents = _float3(0.3f, 0.7f, 0.3f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);


	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;



	/* For.Com_Model */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, m_strModelName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_Mesh_Player_Matilda"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	






	m_pWeaponMatrix=m_pModelCom->Get_BoneCombinedTransformationMatrix("Bone Weapon003");






	return S_OK;
}

HRESULT Matilda_Body_Player::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

Matilda_Body_Player * Matilda_Body_Player::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	Matilda_Body_Player*		pInstance = new Matilda_Body_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Matilda_Body_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * Matilda_Body_Player::Clone(void * pArg)
{
	Matilda_Body_Player*		pInstance = new Matilda_Body_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Matilda_Body_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Matilda_Body_Player::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);


	Safe_Release(m_pRootNode);

}

NodeStates Matilda_Body_Player::DoIdle()
{
	//현재 상태가 아이들일 때만 true
	if (m_eCurState == CPlayer::STATE_DEFAULT_IDLE)
	{
		
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_IDLE;
		m_bLoop = true;
		m_AnimSpeed= 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);
		return NodeStates::SUCCESS;
	}
	else 
		return NodeStates::FAILURE;
}

NodeStates Matilda_Body_Player::DoRun()
{
	
	if (m_eCurState == CPlayer::STATE_RUN)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_RUN;
		m_bLoop = true;
		m_AnimSpeed = 1.5f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);
		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;

}

NodeStates Matilda_Body_Player::DoAttack_Left()
{
	if (m_eCurState == CPlayer::STATE_COMBO_ATTACK_LEFT)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_ATTACK_SLASH_LEFT;
		m_bLoop = false;
		m_AnimSpeed=1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(false);
		if (dNowFramePos >= 0.09 && dNowFramePos <= 0.13)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetIsCollision(false);
			
		}
	
		if (m_pModelCom->Get_AnimationIndex() != Player_Matilda_AnimationType::Matilda_ANIMATION_ATTACK_SLASH_LEFT)
		{
		
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimFinished())
		{	
			m_eCurState = CPlayer::STATE_ATTACK_DEFAULT_END;
		//	if (typeid(*m_pPlayer) == typeid(CPlayer))
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);

			return NodeStates::SUCCESS;
		}
		else
		{

			return NodeStates::RUNNING;
		}
	}


	
}

NodeStates Matilda_Body_Player::DoAttack_Right()
{
	if (m_eCurState == CPlayer::STATE_COMBO_ATTACK_RIGHT)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_ATTACK_SLASH_RIGHT;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(false);
		if (dNowFramePos >= 0.17 && dNowFramePos <= 0.19)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetIsCollision(false);
		}
		if (m_pModelCom->Get_AnimationIndex() != Player_Matilda_AnimationType::Matilda_ANIMATION_ATTACK_SLASH_RIGHT)
		{
			
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
			//if (typeid(*m_pPlayer) == typeid(CPlayer))
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);
				
			return NodeStates::FAILURE;
		}
		else
		{
			
			return NodeStates::RUNNING;
		}
	}
}

NodeStates Matilda_Body_Player::DoAttack_End()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_DEFAULT_END)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_ATTACK_SLASH_LEFT ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_UPPER||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_SPIN_END ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_STOMP||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_CRITICAL_GO)
		{
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
			//if (typeid(*m_pPlayer) == typeid(CPlayer))
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::FAILURE;
		}
		else
			return NodeStates::RUNNING;
			
	}

	//return NodeStates::RUNNING;
}

NodeStates Matilda_Body_Player::DoAttack_End2()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_DEFAULT_END)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_ATTACK_SLASH_LEFT ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_UPPER ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_SPIN_END ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_STOMP ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_CRITICAL_GO)
		{
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
			//if (typeid(*m_pPlayer) == typeid(CPlayer))
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::FAILURE;
		}
		else
			return NodeStates::RUNNING;

	}

	//return NodeStates::RUNNING;
}

NodeStates Matilda_Body_Player::DoHit()
{
	if (m_eCurState == CPlayer::STATE_HIT)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_HIT;
		m_bLoop = false;
		m_AnimSpeed=1.0f;
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DEFAULT_IDLE;

				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::FAILURE;
		}
		else
			return NodeStates::RUNNING;
	}




	
}

NodeStates Matilda_Body_Player::DoDash()
{
	if (m_eCurState==CPlayer::STATE_DASH)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_DASH;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.1 && dNowFramePos <= 0.14)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(2.0f);
		}
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT_CRITICAL)
		{
			return NodeStates::FAILURE;
		}

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_MOVE_AFTER_STOP;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);		
			return NodeStates::FAILURE;
		}
		else
		{
			return NodeStates::RUNNING;
		}
	}
	




	
}

NodeStates Matilda_Body_Player::DoAttack_After_Idle()
{
	if (m_eCurState == CPlayer::STATE_AFTER_ATTACK_IDLE)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_STOP_IDLE;
		m_bLoop = true;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		return NodeStates::RUNNING;

	}
	else 
		return NodeStates::FAILURE;

	
}

NodeStates Matilda_Body_Player::DoAttack_Before()
{
	if (m_eCurState == CPlayer::STATE_BEFORE_ATTACK_MOTION)
	{

		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_ATTACK_SLASH_RIGHT ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_DASH_BACK ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_ATTACK_STINGER ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT_CRITICAL||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLIDING_SHOT ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT_UPPER_POST ||
			m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT_CROUCH
			)
		{
			return NodeStates::FAILURE;
		}
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_AFTER_ATTACK_IDLE;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::FAILURE;
	
	}


}

NodeStates Matilda_Body_Player::DoMoveAfterStop()
{
	if (m_eCurState == CPlayer::STATE_MOVE_AFTER_STOP)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_MOVE_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_DASH)
		{
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DEFAULT_IDLE;

				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
		}
		else
			return NodeStates::RUNNING;

	}

	
}

NodeStates Matilda_Body_Player::DoBackDash()
{
	if (m_eCurState == CPlayer::STATE_BACKSTEP)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_DASH_BACK;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT_STOP)
		{
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT_CROUCH)
		{
					return NodeStates::RUNNING;
		}


		if (m_pModelCom->Get_AnimFinished())
		{
			switch (m_eCharacterType)
			{
			case Client::CPlayer::CHRACTER_SWORD_MASTER:
				switch (m_eSwordMasterType)
				{
				case Client::CPlayer::SWORD_BALANCE:
					m_eCurState = CPlayer::STATE_THROW_WEAPON;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					break;
				case Client::CPlayer::SWORD_TECHNNIC:
					m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					break;
				case Client::CPlayer::SWORD_POWER:
					break;
				case Client::CPlayer::SWORD_REVERSE:
					m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					break;
				case Client::CPlayer::SWORD_END:
					break;
				default:
					break;
				}
				break;
			case Client::CPlayer::CHRACTER_GUN_SLINGER:
				switch (m_eSwordMasterType)
				{
				case Client::CPlayer::GUN_BALANCE:
					m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					break;
				case Client::CPlayer::GUN_TECHNNIC:
					m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					break;
				case Client::CPlayer::GUN_POWER:
					m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					break;
				case Client::CPlayer::GUN_REVERSE:
					m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					break;
				case Client::CPlayer::GUN_END:
					break;
				default:
					break;
				}
				break;
			case Client::CPlayer::CHRACTER_END:
				break;
			default:
				break;
			}
			
		
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	if (m_eCurState == CPlayer::STATE_THROW_WEAPON)
	{
		return NodeStates::SUCCESS;
	}

}

NodeStates Matilda_Body_Player::DoThrowWeapon()
{

	if (m_eCurState == CPlayer::STATE_THROW_WEAPON)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_UPPER;
		m_bLoop = false;
		m_AnimSpeed = 3.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_DASH_BACK)
		{
			return NodeStates::RUNNING;
		}
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.03 && dNowFramePos <= 0.09)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
		}

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_ATTACK_DEFAULT_END;

				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}


	
	
}


NodeStates Matilda_Body_Player::DoMatildaHidden_D_Short()
{

	if(m_eCurState==CPlayer::STATE_MATILD_D_SHORT)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_HIDDEN_MATIDA_D_SHORT;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		double dNowFramePos= m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >=0.4&& dNowFramePos <=0.5 )
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(1.0f);
		}
	
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}


}

NodeStates Matilda_Body_Player::DoAttackStinger()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_STINGER)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_ATTACK_STINGER;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.03 && dNowFramePos <= 0.09)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetIsCollision(false);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(1.0f);
		}

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;



	}

}

NodeStates Matilda_Body_Player::DoAttakcSpin()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SPIN)
	{
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_DASH)
		{
			if (m_pModelCom->Get_AnimFinished())	//dash가 끝나면
			{
				m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_SPIN;
				m_bLoop = false;
				m_AnimSpeed = 1.0f;
				
				if (m_pModelCom->Get_AnimationIndex()== Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_SPIN&&m_pModelCom->Get_AnimFinished())
				{
					m_eCurState = CPlayer::STATE_ATTACK_SPIN_END;

						dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					return NodeStates::SUCCESS;
				}
				else
					return NodeStates::FAILURE;
				



			}
		}
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_SPIN)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetIsCollision(false);
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_ATTACK_SPIN_END;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::SUCCESS;
			}
			else
				return NodeStates::FAILURE;
		}


		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_DASH;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.1 && dNowFramePos <= 0.14)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
		}
		return NodeStates::FAILURE;



	}
	if (m_eCurState == CPlayer::STATE_ATTACK_SPIN_END)
	{
		return NodeStates::SUCCESS;
	}

}

NodeStates Matilda_Body_Player::DoAttackSpinEnd()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SPIN_END)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_SPIN_END;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SLASH_SPIN)
		{
			return NodeStates::RUNNING;
		}

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_ATTACK_DEFAULT_END;

			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	else
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);

	return NodeStates::SUCCESS;

}

NodeStates Matilda_Body_Player::DoAttackStomp()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_STOMP)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_STOMP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.02 && dNowFramePos <= 0.06)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetIsCollision(false);
		}
		

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_ATTACK_DEFAULT_END;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);

			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
	


}

NodeStates Matilda_Body_Player::DoAttackThrow()
{
	static _float3 fPrePlayerPos= dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();
	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();
	if (m_eCurState == CPlayer::STATE_ATTACK_THORW)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_ATTACK_THROWING;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		
			if (dNowFramePos >= 0.18 && dNowFramePos <= 0.2)
			{
				//총알 생성
				PlayerBulletdesc.fRadius = 0.5f;
				PlayerBulletdesc.strModelName = TEXT("swordThrowing");
				PlayerBulletdesc.vPosition = _float4(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f);
				PlayerBulletdesc.vDir = vPlayerLook;
				PlayerBulletdesc.fSpeedPerSec = 30.0f;
				PlayerBulletdesc.fLifeTime = 1.0f;
				PlayerBulletdesc.BulletState = &CBullet::Go_TargetPos;
				PlayerBulletdesc.vTargetPos = XMVectorSet(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z, 1.0f) + vPlayerLook * 10.0f;
				PlayerBulletdesc.eParticleType = CPlayerBullet::CPLAYER_BULLET_PARTICLE_TYPE::REVERSE_C;
				PlayerBulletdesc.bParticleCreate = true;
				PlayerBulletdesc.fDamage = 20.0f;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &PlayerBulletdesc);


				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Reverse_C_hitRing_Size_UP"),_float4(0.8f,0.8f,0.8f,0.5f) });
				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z, 1.0f), -vPlayerLook);
				dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
				dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(2.5f);
				fPrePlayerPos = vPlayerPos;
			}


			if (dNowFramePos >= 0.8 && dNowFramePos <= 1.0)
			{

				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Reverse_C_Matilda_Trace_Jump_Spread"),_float4(0.8f,0.8f,0.8f,0.5f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN,TEXT("Player_Reverse_C_LowpolyCylinder6_Size_Down"),_float4(0.8f,0.8f,0.8f,0.5f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Reverse_C_LowpolySphere16_Size_UP"),_float4(0.8f,0.8f,0.8f,0.5f) });

				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(fPrePlayerPos.x, fPrePlayerPos.y, fPrePlayerPos.z, 1.0f), -vPlayerLook);
				dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
				dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(3.0f);
			}
		
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_JUMP_END;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);

				return NodeStates::SUCCESS;
			}
			else
				return NodeStates::RUNNING;
		
	}



	return NodeStates();
}

NodeStates Matilda_Body_Player::DoAttackJumpEnd()
{
	if (m_eCurState == CPlayer::STATE_JUMP_END)
	{

		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_JUMP_END;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_ATTACK_THROWING)
		{
			return NodeStates::RUNNING;
		}
	

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
			if (typeid(*m_pPlayer) == typeid(CPlayer))
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;


	}

	
}



NodeStates Matilda_Body_Player::DoAttackUltimateBalance()
{
	static int iChangeCnt = 0;
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_BALANCE)
	{
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();

		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_DASH;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_DASH)
			{
				if (dNowFramePos >= 0.02 && dNowFramePos <= 0.05)
				{
					//1차 생성
					dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
					dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(5.5f);
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.001_Spread"),_float4(0.2f,0.2f,1.0f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.002_Spread"),_float4(0.4f,0.4f,1.0f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.003_Spread"),_float4(0.2f,0.2f,1.0f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.004_Spread"),_float4(0.0f,0.2f,1.0f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Balalnce_XC_Matilda_Trace_Ult.005_Spread"),_float4(0.0f,0.2f,1.0f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Balalnce_XC_Matilda_Trace_Ult_Spread"),_float4(0.7f,0.7f,0.9f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z_NONE_DISOLVE,TEXT("Player_Balalnce_XC_slashLine_Size_Down_Z"),_float4(0.8f,0.8f,0.9f,0.8f),true });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Balalnce_XC_swordGroundWind_Size_Up"),_float4(0.7f,0.7f,0.9f,0.5f),true,true });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION,TEXT("Player_Balalnce_XC_LowpolySphere8_Explosion"),_float4(0.9f,0.9f,0.9f,0.2f),true,true });


					_vector vPlayerLook = dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir();
					CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(m_pPlayer->Get_Position().x, m_pPlayer->Get_Position().y + 1.f, m_pPlayer->Get_Position().z, 1.0f), -vPlayerLook);



					m_pWeapon->SetParticleCreate(true, CWeapon::WEAPON_PARTICLE_TYPE::BALANCE_XC);

				}
				if (iChangeCnt == 0)
				{
					if (dNowFramePos >= 0.2)
					{
						vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP,TEXT("Player_Balalnce_XC_LowpolySphere16_Next_Pop"),_float4(0.2f,0.2f,1.0f,0.5f),false,true });
						

						_vector vPlayerLook = dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir();
						CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(m_pPlayer->Get_Position().x, m_pPlayer->Get_Position().y + 1.f, m_pPlayer->Get_Position().z, 1.0f), -vPlayerLook);
					
						
						//2차 생성
						dynamic_cast<CPlayer*>(m_pPlayer)->Set_Rotation(XMConvertToRadians(180.f));
						dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
						dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(5.5f);
						iChangeCnt++;
					}
				}

			}
			if (m_pModelCom->Get_AnimFinished())
			{
				iChangeCnt = 0;
				m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::SUCCESS;
			}
			else
				return NodeStates::RUNNING;
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_CROUCH_READY;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;

			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_ATTACK_SHOT_CROUCH;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);

				return NodeStates::FAILURE;

			}
			else
				return NodeStates::RUNNING;


			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			return NodeStates::FAILURE;
			break;
		}

		return NodeStates::FAILURE;

	}


}

NodeStates Matilda_Body_Player::DoAttackUltimateTechnic()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_TECHNIC)
	{

		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_HIDDEN_MATIDA_D_SHORT;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::SUCCESS;
			}
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_HIDDEN_MATIDA_D_SHORT;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::SUCCESS;
			}
			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			return NodeStates::RUNNING;
			break;
		}



	
	}
}

NodeStates Matilda_Body_Player::DoAttackUltimatePower()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_POWER)
	{
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_DASH;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
		
			if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_DASH)
			{
				if (dNowFramePos >= 0.02 && dNowFramePos <= 0.05)
				{
					dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
					dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(5.5f);
				}
			}
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_MOVE_AFTER_STOP;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::SUCCESS;
			}
			else
				return NodeStates::RUNNING;
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_UPPER_PRE;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_ATTACK_SHOT_UPPER;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::FAILURE;
			}

			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			break;
		}


		
	}



}

NodeStates Matilda_Body_Player::DoAttackUltimateReverse()
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	_vector vPlayerLook = dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir();
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_REVERSE)
	{
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
		

			if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_CRITICAL_GO)
			{
				double dNowFramePos = m_pModelCom->Get_CurrentPosition();
				if (dNowFramePos >= 0.08 && dNowFramePos <= 0.1)
				{
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Reverse_XC_LowpolySphere16_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z,TEXT("Player_Reverse_XC_stingerRing.001_Rotate_Z"),_float4(0.8f,0.8f,0.8f,0.5f) });
					vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z,TEXT("Player_Reverse_XC_stingerHalftone_Size_Up_Z"),_float4(0.8f,0.8f,0.8f,0.5f) });
					CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z, 1.0f), -vPlayerLook);
					
					PlayerBulletdesc.fRadius = 10.0f;
					PlayerBulletdesc.strModelName = TEXT("swordThrowing");
					PlayerBulletdesc.vPosition = _float4(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f);
					PlayerBulletdesc.vDir = vPlayerLook;
					PlayerBulletdesc.fSpeedPerSec = 50.0f;
					PlayerBulletdesc.fLifeTime = 3.0f;
					PlayerBulletdesc.BulletState = &CBullet::Go;
					PlayerBulletdesc.eParticleType = CPlayerBullet::CPLAYER_BULLET_PARTICLE_TYPE::REVERSE_XC;
					PlayerBulletdesc.bParticleCreate = true;
					PlayerBulletdesc.fDamage = 50.0f;
					m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &PlayerBulletdesc);


				}



				if (m_pModelCom->Get_AnimFinished())
				{
					m_eCurState = CPlayer::STATE_ATTACK_DEFAULT_END;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					return NodeStates::SUCCESS; 
				}
				else
					return NodeStates::RUNNING;

			}



			if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_REVERSE)
			{
				m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_CRITICAL_READY;
				m_bLoop = false;
				m_AnimSpeed = 1.0f;
				double dNowFramePos = m_pModelCom->Get_CurrentPosition();
				if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_CRITICAL_READY)
				{
					if (dNowFramePos >= 0.45 && dNowFramePos <= 0.5)
					{
						dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
						dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(1.5f);
					}
					if (m_pModelCom->Get_AnimFinished())
					{
						m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLASH_CRITICAL_GO;
						dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
						return NodeStates::SUCCESS;
					}
					else
						return NodeStates::RUNNING;
				}
			}
			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_CROUCH_READY;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			
			if (m_pModelCom->Get_AnimFinished())
			{
				
				m_eCurState = CPlayer::STATE_ATTACK_GHOST_CROUCH;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::FAILURE;

			}
			else
				return NodeStates::RUNNING;


			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			break;
		}


	}
	else
		return NodeStates::FAILURE;

}

NodeStates Matilda_Body_Player::DoAttack_Shot()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(false);
		if (dNowFramePos >= 0.04 && dNowFramePos <= 0.06)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetFire(true);

		}

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}
}

NodeStates Matilda_Body_Player::DoAttack_Shot_Critical()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_CRITICAL)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CRITICAL;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(false);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.04 && dNowFramePos <= 0.06)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);

		}

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}


}

NodeStates Matilda_Body_Player::DoAttack_Shot_Dash()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_DASH)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CRITICAL;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.04 && dNowFramePos <= 0.06)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);

		}


		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DASH;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);

			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}


}

NodeStates Matilda_Body_Player::DoAttack_Dash_Buck()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_DASH_BUCK)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_DASH_BUCK;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_ATTACK_DASH_BUCK_NEXT;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
		

		}
		else
			return NodeStates::RUNNING;



	}


	return NodeStates::FAILURE;
}

NodeStates Matilda_Body_Player::DoAttack_Dash_Buck_Next()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_DASH_BUCK_NEXT)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_DASH_BUCK_NEXT;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_DASH_BUCK)
		{
			return NodeStates::RUNNING;
		}


		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}


	return NodeStates::FAILURE;
}

NodeStates Matilda_Body_Player::DoAttack_Dash_Slide_Shot()
{

	if (m_eCurState == CPlayer::STATE_ATTACK_DASH_SLIDING_SHOT)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SLIDING_SHOT;
		m_bLoop = false;
		m_AnimSpeed = 2.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
		dynamic_cast<CPlayer*>(m_pPlayer)->SetIsCollision(false);

	
		if (m_pModelCom->Get_AnimationIndex() == Matilda_ANIMATION_SHOT_STOP)
		{
			return NodeStates::RUNNING;
		}

		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}


	
}

NodeStates Matilda_Body_Player::DoAttack_Shot_Crouch()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_CROUCH)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_CROUCH;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
		dynamic_cast<CPlayer*>(m_pPlayer)->SetIsCollision(false);
		if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_CROUCH_READY)
		{
			return NodeStates::RUNNING;
		}


		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_BACKSTEP;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetJump(true);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}






	
}

NodeStates Matilda_Body_Player::DoAttack_Shot_Upper()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_UPPER)
	{
		m_iAnimIndex = Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_UPPER_POST;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_UPPER_PRE)
		{
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimFinished())
		{
			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}


}

NodeStates Matilda_Body_Player::DoAttack_Shot_Ghost_Crouch()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_GHOST_CROUCH)
	{
		m_iAnimIndex= Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_CROUCH;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Player_Matilda_AnimationType::Matilda_ANIMATION_SHOT_CROUCH_READY)
		{
			return NodeStates::RUNNING;
		}

		if (m_pModelCom->Get_AnimFinished())
		{

			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;




	}

	
}
