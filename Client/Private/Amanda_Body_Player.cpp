#include "stdafx.h"
#include "Amanda_Body_Player.h"

#include "GameInstance.h"
#include"Loader.h"


#include "CFsm_Anim.h"
#include "Player.h"




#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"

#include"CMyNode.h"


#include"Fade_In_Out.h"
#include"CPlayerBullet.h"
#include"Particle_Mesh.h"
#include"Monster.h"

Amanda_Body_Player::Amanda_Body_Player(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CPartObject{ pDevice, pContext }
{
}

Amanda_Body_Player::Amanda_Body_Player(const Amanda_Body_Player & rhs)
	: CPartObject{ rhs }
{
}

HRESULT Amanda_Body_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Amanda_Body_Player::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;	

	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(rand() % 20, true));	
	
	Amanda_BODY_DESC* pDesc = (Amanda_BODY_DESC*)pArg;
	m_pPlayer= pDesc->m_pPlayer;




	CActionNode* pIdle = CActionNode::create(bind(&Amanda_Body_Player::DoIdle, this));
	CActionNode* pHit = CActionNode::create(bind(&Amanda_Body_Player::DoHit, this));
	CActionNode* pRun = CActionNode::create(bind(&Amanda_Body_Player::DoRun, this));
	CActionNode* pDash = CActionNode::create(bind(&Amanda_Body_Player::DoDash, this));
	CActionNode* pMoveStop = CActionNode::create(bind(&Amanda_Body_Player::DoMoveAfterStop, this));
	CActionNode* pAttackLfet = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Left, this));
	CActionNode* pAttackRight = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Right, this));
	CActionNode* pAttackEnd = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_End, this));
	CActionNode* pAttackEnd2 = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_End2, this));
	CActionNode* pBackDash = CActionNode::create(bind(&Amanda_Body_Player::DoBackDash, this));
	CActionNode* pThrowWeapon = CActionNode::create(bind(&Amanda_Body_Player::DoThrowWeapon, this));
	CActionNode* pAttackBefore = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Before, this));
	CActionNode* pAfterAttackIdle = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_After_Idle, this));
	CActionNode* pMatildaHidden_D_Short = CActionNode::create(bind(&Amanda_Body_Player::DoMatildaHidden_D_Short, this));
	CActionNode* pAttackStinger = CActionNode::create(bind(&Amanda_Body_Player::DoAttackStinger, this));
	CActionNode* pAttackSpin = CActionNode::create(bind(&Amanda_Body_Player::DoAttakcSpin, this));
	CActionNode* pAttackSpinEnd = CActionNode::create(bind(&Amanda_Body_Player::DoAttackSpinEnd, this));
	CActionNode* pAttackStomp = CActionNode::create(bind(&Amanda_Body_Player::DoAttackStomp, this));
	CActionNode* pAttackSThrow = CActionNode::create(bind(&Amanda_Body_Player::DoAttackThrow, this));
	CActionNode* pAttackJumpEnd = CActionNode::create(bind(&Amanda_Body_Player::DoAttackJumpEnd, this));
	CActionNode* pAttackUltimateBalance = CActionNode::create(bind(&Amanda_Body_Player::DoAttackUltimateBalance, this));
	CActionNode* pAttackUltimateTechnic = CActionNode::create(bind(&Amanda_Body_Player::DoAttackUltimateTechnic, this));
	CActionNode* pAttackUltimatePower = CActionNode::create(bind(&Amanda_Body_Player::DoAttackUltimatePower, this));
	CActionNode* pAttackUltimateEnd = CActionNode::create(bind(&Amanda_Body_Player::DoAttackUltimateReverse, this));

	CActionNode* pDoAttack_Shot = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Shot, this));
	CActionNode* pDoAttack_Shot_Critical = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Shot_Critical, this));
	CActionNode* pDoAttack_Shot_Dash = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Shot_Dash, this));
	CActionNode* pDoAttack_Dash_Buck = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Dash_Buck, this));
	CActionNode* pDoAttack_Dash_Buck_Next = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Dash_Buck_Next, this));
	CActionNode* pDoAttack_Dash_Slide_Shot = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Dash_Slide_Shot, this));
	CActionNode* pDoAttack_Shot_Crouch = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Shot_Crouch, this));
	CActionNode* pDoAttack_Shot_Upper = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Shot_Upper, this));
	CActionNode* pDoAttack_Shot_Ghost_Crouch = CActionNode::create(bind(&Amanda_Body_Player::DoAttack_Shot_Ghost_Crouch, this));

	CSelector* pRunOrStop = CSelector::Create(pRun, pDash, pMoveStop);


	CSelector* pComboAttackSelector = CSelector::Create(pAttackLfet, pAttackRight, pDoAttack_Shot, pDoAttack_Shot_Critical);

	CSequence* pBackDashSeq = CSequence::Create(pBackDash, pThrowWeapon, pAttackEnd);

	CSequence* pAttackSpinSeq = CSequence::Create(pAttackSpin, pAttackSpinEnd);


	CSequence* pListAttackThrowSeq = CSequence::Create(pAttackSThrow, pAttackJumpEnd);

	CSelector* pUltimateSelector = CSelector::Create(pAttackUltimateBalance, pDoAttack_Shot_Crouch, pAttackUltimateTechnic, pAttackUltimatePower, pDoAttack_Shot_Upper, pAttackUltimateEnd, pDoAttack_Shot_Ghost_Crouch);

	CSelector* pAttackSelector = CSelector::Create(pComboAttackSelector, pBackDashSeq, pDoAttack_Shot_Dash, pDoAttack_Dash_Buck, pDoAttack_Dash_Buck_Next, pDoAttack_Dash_Slide_Shot, pMatildaHidden_D_Short, pAttackStinger, pAttackSpinSeq, pAttackStomp, pListAttackThrowSeq, pUltimateSelector);

	CSelector* pOtherAnimSelect = CSelector::Create(pRunOrStop, pAttackSelector);

	CSequence* pEndAttackSeq = CSequence::Create(pAttackBefore, pAfterAttackIdle);

	CSelector* pIdleOrOther = CSelector::Create(pIdle, pHit, pAttackEnd2, pEndAttackSeq, pOtherAnimSelect);


	m_pRootNode = CSequence::Create(pIdleOrOther);



	 
	//CActionNode* pIdle = CActionNode::create([this]() { return DoIdle(); });
	//CActionNode* pHit = CActionNode::create([this]() { return DoHit(); });
	//CActionNode* pRun = CActionNode::create([this]() { return DoRun(); });
	//CActionNode* pDash = CActionNode::create([this]() { return DoDash(); });
	//CActionNode* pMoveStop = CActionNode::create([this]() { return DoMoveAfterStop(); });
	//CActionNode* pAttackLfet = CActionNode::create([this]() { return DoAttack_Left(); });
	//CActionNode* pAttackRight = CActionNode::create([this]() { return DoAttack_Right(); });
	//CActionNode* pAttackEnd = CActionNode::create([this]() { return DoAttack_End(); });
	//CActionNode* pAttackEnd2 = CActionNode::create([this]() { return DoAttack_End2(); });
	//CActionNode* pBackDash = CActionNode::create([this]() { return DoBackDash(); });
	//CActionNode* pThrowWeapon = CActionNode::create([this]() { return DoThrowWeapon(); });
	//CActionNode* pAttackBefore = CActionNode::create([this]() { return DoAttack_Before(); });
	//CActionNode* pAfterAttackIdle = CActionNode::create([this]() { return DoAttack_After_Idle(); });
	//CActionNode* pMatildaHidden_D_Short = CActionNode::create([this]() { return DoMatildaHidden_D_Short(); });
	//CActionNode* pAttackStinger = CActionNode::create([this]() { return DoAttackStinger(); });
	//CActionNode* pAttackSpin = CActionNode::create([this]() { return DoAttakcSpin(); });
	//CActionNode* pAttackSpinEnd = CActionNode::create([this]() { return DoAttackSpinEnd(); });
	//CActionNode* pAttackStomp = CActionNode::create([this]() { return DoAttackStomp(); });
	//CActionNode* pAttackSThrow = CActionNode::create([this]() { return DoAttackThrow(); });
	//CActionNode* pAttackJumpEnd = CActionNode::create([this]() { return DoAttackJumpEnd(); });
	//CActionNode* pAttackUltimateBalance = CActionNode::create([this]() { return DoAttackUltimateBalance(); });
	//CActionNode* pAttackUltimateTechnic = CActionNode::create([this]() { return DoAttackUltimateTechnic(); });
	//CActionNode* pAttackUltimatePower = CActionNode::create([this]() { return DoAttackUltimatePower(); });
	//CActionNode* pAttackUltimateEnd = CActionNode::create([this]() { return DoAttackUltimateReverse(); });
	//
	//CActionNode* pDoAttack_Shot = CActionNode::create([this]() { return DoAttack_Shot(); });
	//CActionNode* pDoAttack_Shot_Critical = CActionNode::create([this]() { return DoAttack_Shot_Critical(); });
	//CActionNode* pDoAttack_Shot_Dash = CActionNode::create([this]() { return DoAttack_Shot_Dash(); });
	//CActionNode* pDoAttack_Shot_Dash_Buck = CActionNode::create([this]() { return DoAttack_Dash_Buck(); });
	//CActionNode* pDoAttack_Shot_Dash_Buck_Next = CActionNode::create([this]() { return DoAttack_Dash_Buck_Next(); });
	//CActionNode* pDoAttack_Shot_Dash_Slide = CActionNode::create([this]() { return DoAttack_Dash_Slide_Shot(); });
	//CActionNode* pDoAttack_Shot_Crouch = CActionNode::create([this]() { return DoAttack_Shot_Crouch(); });
	//CActionNode* pDoAttack_Shot_Upper = CActionNode::create([this]() { return DoAttack_Shot_Upper(); });
	//CActionNode* pDoAttack_Shot_Ghost_Crouch = CActionNode::create([this]() { return DoAttack_Shot_Ghost_Crouch(); });
	//
	//
	//CSelector* pRunOrStop = CSelector::Create(pRun,pDash,pMoveStop);
	//
	//
	//CSelector* pComboAttackSelector = CSelector::Create(pAttackLfet, pAttackRight, pDoAttack_Shot, pDoAttack_Shot_Critical);
	////CSelector*  
	//
	//CSequence* pBackDashSeq = CSequence::Create(pBackDash, pThrowWeapon, pAttackEnd);
	//
	//CSequence* pAttackSpinSeq= CSequence::Create(pAttackSpin, pAttackSpinEnd);
	//
	//
	//CSequence* pListAttackThrowSeq= CSequence::Create(pAttackSThrow, pAttackJumpEnd);
	//
	//CSelector* pUltimateSelector = CSelector::Create(pAttackUltimateBalance, pDoAttack_Shot_Crouch, pAttackUltimateTechnic, pAttackUltimatePower, pDoAttack_Shot_Upper, pAttackUltimateEnd, pDoAttack_Shot_Ghost_Crouch);
	//
	//CSelector* pAttackSelector = CSelector::Create(pComboAttackSelector, pBackDashSeq, pDoAttack_Shot_Dash, pDoAttack_Shot_Dash_Buck, pDoAttack_Shot_Dash_Buck_Next, pDoAttack_Shot_Dash_Slide,pMatildaHidden_D_Short, pAttackStinger, pAttackSpinSeq, pAttackStomp, pListAttackThrowSeq, pUltimateSelector);
	//
	//CSelector* pOtherAnimSelect = CSelector::Create(pRunOrStop, pAttackSelector);
	//
	//CSequence* pEndAttackSeq = CSequence::Create(pAttackBefore, pAfterAttackIdle);
	//
	//CSelector* pIdleOrOther = CSelector::Create(pIdle, pHit, pAttackEnd2, pEndAttackSeq, pOtherAnimSelect);
	//
	//
	//m_pRootNode = CSequence::Create(pIdleOrOther);
	//

	return S_OK;
}

void Amanda_Body_Player::Priority_Tick(_float fTimeDelta)
{
}

void Amanda_Body_Player::Tick(_float fTimeDelta)
{
	

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix()  * XMLoadFloat4x4(m_pParentMatrix));



 	CModel::ANIMATION_DESC		AnimDesc{ 0, true };
	m_pRootNode->Evaluate();
	AnimDesc.iAnimIndex = m_iAnimIndex;
	AnimDesc.isLoop = m_bLoop;


	//bChildRunning ? NodeStates::RUNNING : NodeStates::SUCCESS;
	//float t = m_eCurState == CPlayer::STATE_DASH ? 0.001 : 1;
	m_pModelCom->Set_AnimationIndex(AnimDesc);

	m_pModelCom->Play_Animation(fTimeDelta* m_AnimSpeed);

	
	m_pColliderCom->Tick(XMLoadFloat4x4(&m_WorldMatrix));
	CreateEffect_By_Motion(fTimeDelta);
}

void Amanda_Body_Player::Late_Tick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_SHADOWOBJ, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT Amanda_Body_Player::Render()
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

HRESULT Amanda_Body_Player::Render_LightDepth()
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

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void Amanda_Body_Player::CreateEffect_By_Motion(_float fTimeDelta)
{
	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();
	double fCurPos = m_pModelCom->Get_CurrentPosition();

	if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_UPPER_POST)
	{
		if (fCurPos >= 0.15 && fCurPos <= 0.17)
		{
			CMonster* pMonster = nullptr;
			for (int i = 0; i < 5; i++)
			{
				pMonster = static_cast<CMonster*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_2_Monster"), i));
				

				PlayerBulletdesc.strModelName = TEXT("swordThrowing");
				PlayerBulletdesc.eColliderType = CCollider::TYPE_OBB;
				PlayerBulletdesc.vExtents = _float3(1.1f, 20.1f, 2.1f);
				PlayerBulletdesc.vCenter = _float3(0.f, PlayerBulletdesc.vExtents.y, PlayerBulletdesc.vExtents.z);
				PlayerBulletdesc.vRotation = _float3(0.f, 0.f, 0.f);
				PlayerBulletdesc.vPosition = pMonster == nullptr ? _float4(vPlayerPos.x+i*2.0f, 0.0f, vPlayerPos.z, 1.0f) : _float4(pMonster->Get_Position().x+i * 2.0f, 0.0f, pMonster->Get_Position().z,1.0f);
				PlayerBulletdesc.vDir = vPlayerLook;
				PlayerBulletdesc.fLifeTime = 1.0f;
				PlayerBulletdesc.BulletState = &CBullet::Pop;
				PlayerBulletdesc.bParticleCreate = true;
				PlayerBulletdesc.eCharacterType = CPlayer::CHRACTER_GUN_SLINGER;
				PlayerBulletdesc.eParticleType= CPlayerBullet::CPLAYER_BULLET_PARTICLE_TYPE::POWER_XC;
				PlayerBulletdesc.fDamage = 40.f;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &PlayerBulletdesc);
			}
			
		}



	}





}

HRESULT Amanda_Body_Player::Add_Components()
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
	if (FAILED(__super::Add_Component(CLoader::m_eNextLevel, TEXT("Prototype_Component_Shader_Mesh_Player_Amanda"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;	

	return S_OK;
}

HRESULT Amanda_Body_Player::Bind_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	return S_OK;
}

Amanda_Body_Player * Amanda_Body_Player::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	Amanda_Body_Player*		pInstance = new Amanda_Body_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Amanda_Body_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * Amanda_Body_Player::Clone(void * pArg)
{
	Amanda_Body_Player*		pInstance = new Amanda_Body_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Amanda_Body_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Amanda_Body_Player::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);


	Safe_Release(m_pRootNode);

}

NodeStates Amanda_Body_Player::DoIdle()
{
	//현재 상태가 아이들일 때만 true
	if (m_eCurState == CPlayer::STATE_DEFAULT_IDLE)
	{
		
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_IDLE;
		m_bLoop = true;
		m_AnimSpeed= 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);
		return NodeStates::SUCCESS;
	}
	else 
		return NodeStates::FAILURE;
}

NodeStates Amanda_Body_Player::DoRun()
{
	
	if (m_eCurState == CPlayer::STATE_RUN)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_RUN;
		m_bLoop = true;
		m_AnimSpeed = 1.5f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(true);
		return NodeStates::SUCCESS;
	}
	else
		return NodeStates::FAILURE;

}

NodeStates Amanda_Body_Player::DoAttack_Left()
{
	if (m_eCurState == CPlayer::STATE_COMBO_ATTACK_LEFT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_ATTACK_SLASH_LEFT;
		m_bLoop = false;
		m_AnimSpeed=1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.09 && dNowFramePos <= 0.13)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
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
			return NodeStates::FAILURE;
		}
		else
			return NodeStates::RUNNING;
		
	}


	
}

NodeStates Amanda_Body_Player::DoAttack_Right()
{
	if (m_eCurState == CPlayer::STATE_COMBO_ATTACK_RIGHT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_ATTACK_SLASH_RIGHT;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(false);
		if (dNowFramePos >= 0.17 && dNowFramePos <= 0.19)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
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
				return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;

	}
}

NodeStates Amanda_Body_Player::DoAttack_End()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_DEFAULT_END)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_ATTACK_SLASH_LEFT ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_UPPER||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_SPIN_END ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_STOMP||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_CRITICAL_GO)
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

NodeStates Amanda_Body_Player::DoAttack_End2()
{


	if (m_eCurState == CPlayer::STATE_ATTACK_DEFAULT_END)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_ATTACK_SLASH_LEFT ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_UPPER ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_SPIN_END ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_STOMP ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_CRITICAL_GO)
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

NodeStates Amanda_Body_Player::DoHit()
{
	if (m_eCurState == CPlayer::STATE_HIT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_HIT;
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

NodeStates Amanda_Body_Player::DoDash()
{
	if (m_eCurState==CPlayer::STATE_DASH)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_DASH;
		m_bLoop = false;
		m_AnimSpeed = 1.5f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.1 && dNowFramePos <= 0.14)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(2.0f);
		}
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT_CRITICAL)
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

NodeStates Amanda_Body_Player::DoAttack_After_Idle()
{
	if (m_eCurState == CPlayer::STATE_AFTER_ATTACK_IDLE)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_STOP_IDLE;
		m_bLoop = true;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		return NodeStates::RUNNING;

	}
	else 
		return NodeStates::FAILURE;

	
}

NodeStates Amanda_Body_Player::DoAttack_Before()
{
	if (m_eCurState == CPlayer::STATE_BEFORE_ATTACK_MOTION)
	{

		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_ATTACK_SLASH_RIGHT ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_DASH_BACK ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_ATTACK_STINGER ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT_CRITICAL||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLIDING_SHOT ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT_UPPER_POST ||
			m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT_CROUCH
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

NodeStates Amanda_Body_Player::DoMoveAfterStop()
{
	if (m_eCurState == CPlayer::STATE_MOVE_AFTER_STOP)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_MOVE_STOP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_DASH)
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

NodeStates Amanda_Body_Player::DoBackDash()
{
	if (m_eCurState == CPlayer::STATE_BACKSTEP)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_DASH_BACK;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT_STOP)
		{
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT_CROUCH)
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
				case Client::CPlayer::SWORD_END:
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

NodeStates Amanda_Body_Player::DoThrowWeapon()
{

	if (m_eCurState == CPlayer::STATE_THROW_WEAPON)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_UPPER;
		m_bLoop = false;
		m_AnimSpeed = 3.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_DASH_BACK)
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


NodeStates Amanda_Body_Player::DoMatildaHidden_D_Short()
{

	if(m_eCurState==CPlayer::STATE_MATILD_D_SHORT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_HIDDEN_AMANDA_D_SHORT;
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

NodeStates Amanda_Body_Player::DoAttackStinger()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_STINGER)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_ATTACK_STINGER;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.03 && dNowFramePos <= 0.09)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
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

NodeStates Amanda_Body_Player::DoAttakcSpin()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SPIN)
	{
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_DASH)
		{
			if (m_pModelCom->Get_AnimFinished())	//dash가 끝나면
			{
				m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_SPIN;
				m_bLoop = false;
				m_AnimSpeed = 1.0f;
				
				if (m_pModelCom->Get_AnimationIndex()== Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_SPIN&&m_pModelCom->Get_AnimFinished())
				{
					m_eCurState = CPlayer::STATE_ATTACK_SPIN_END;

						dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
					return NodeStates::SUCCESS;
				}
				else
					return NodeStates::FAILURE;
				



			}
		}
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_SPIN)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_ATTACK_SPIN_END;
					dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::SUCCESS;
			}
			else
				return NodeStates::FAILURE;
		}


		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_DASH;
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

NodeStates Amanda_Body_Player::DoAttackSpinEnd()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_SPIN_END)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_SPIN_END;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SLASH_SPIN)
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

NodeStates Amanda_Body_Player::DoAttackStomp()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_STOMP)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_STOMP;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.02 && dNowFramePos <= 0.06)
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);
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

NodeStates Amanda_Body_Player::DoAttackThrow()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_THORW)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_ATTACK_THROWING;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		
			if (dNowFramePos >= 0.18 && dNowFramePos <= 0.2)
			{
				dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
				dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(2.5f);
			}


			if (dNowFramePos >= 0.8 && dNowFramePos <= 1.0)
			{
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

NodeStates Amanda_Body_Player::DoAttackJumpEnd()
{
	if (m_eCurState == CPlayer::STATE_JUMP_END)
	{

		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_JUMP_END;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_ATTACK_THROWING)
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



NodeStates Amanda_Body_Player::DoAttackUltimateBalance()
{
	static int iChangeCnt = 0;
	
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_BALANCE)
	{
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();



		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_DASH;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_DASH)
			{
				if (dNowFramePos >= 0.02 && dNowFramePos <= 0.05)
				{
					dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
					dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(5.5f);
				}
				if (iChangeCnt == 0)
				{
					if (dNowFramePos >= 0.2)
					{
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
			m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CROUCH_READY;
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

NodeStates Amanda_Body_Player::DoAttackUltimateTechnic()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_TECHNIC)
	{
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_HIDDEN_AMANDA_D_SHORT;
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
			m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_HIDDEN_AMANDA_D_SHORT;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_DEFAULT_IDLE;
				dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
				return NodeStates::SUCCESS;
			}
			break;

			break;
		case Client::CPlayer::CHRACTER_END:
			break;
		default:
			return NodeStates::RUNNING;
			break;
		}
			
	}
}

NodeStates Amanda_Body_Player::DoAttackUltimatePower()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_POWER)
	{
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_DASH;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;

			if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_DASH)
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
			m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_UPPER_PRE;
			m_bLoop = false;
			m_AnimSpeed = 1.0f;
			if (m_pModelCom->Get_AnimFinished())
			{
				m_eCurState = CPlayer::STATE_ATTACK_SHOT_UPPER;
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
}
	

NodeStates Amanda_Body_Player::DoAttackUltimateReverse()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_ULTIMATE_REVERSE)
	{
		switch (m_eCharacterType)
		{
		case Client::CPlayer::CHRACTER_SWORD_MASTER:
			if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_CRITICAL_GO)
			{
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
				m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_CRITICAL_READY;
				m_bLoop = false;
				m_AnimSpeed = 1.0f;
				double dNowFramePos = m_pModelCom->Get_CurrentPosition();
				if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_CRITICAL_READY)
				{
					if (dNowFramePos >= 0.45 && dNowFramePos <= 0.5)
					{
						dynamic_cast<CPlayer*>(m_pPlayer)->SetMove_After_Skill(true);
						dynamic_cast<CPlayer*>(m_pPlayer)->SetMovePower(1.5f);
					}
					if (m_pModelCom->Get_AnimFinished())
					{
						m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLASH_CRITICAL_GO;
						dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
						return NodeStates::SUCCESS;
					}
					else
						return NodeStates::RUNNING;
				}
			}

			break;
		case Client::CPlayer::CHRACTER_GUN_SLINGER:
			m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CROUCH_READY;
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


}

NodeStates Amanda_Body_Player::DoAttack_Shot()
{
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();

	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		//m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(false);
		if (dNowFramePos >= 0.04 && dNowFramePos <= 0.06)
		{
			PlayerBulletdesc.strModelName = TEXT("swordThrowing");
			PlayerBulletdesc.eColliderType = CCollider::TYPE_OBB;
			PlayerBulletdesc.vExtents = _float3(1.1f, 1.1f, 20.1f);
			PlayerBulletdesc.vCenter= _float3(0.f, PlayerBulletdesc.vExtents.y, PlayerBulletdesc.vExtents.z);
			PlayerBulletdesc.vRotation= _float3(0.f, 0.f, 0.f);
			PlayerBulletdesc.vPosition = _float4(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f);
			PlayerBulletdesc.vDir = vPlayerLook;
			PlayerBulletdesc.fLifeTime = 0.1f;
			PlayerBulletdesc.BulletState = &CBullet::Pop;
			PlayerBulletdesc.fDamage = 5.0f;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &PlayerBulletdesc);

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN_TEXTURE,TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"),_float4(1.0f,0.2f,0.8f,0.5f) });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y+1.0f, vPlayerPos.z, 1.0f), -vPlayerLook);
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

NodeStates Amanda_Body_Player::DoAttack_Shot_Critical()
{

	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();
	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_CRITICAL)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CRITICAL;
		m_bLoop = false;
		//m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetNextAttack(false);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.04 && dNowFramePos <= 0.06)
		{

			PlayerBulletdesc.strModelName = TEXT("swordThrowing");
			PlayerBulletdesc.eColliderType = CCollider::TYPE_OBB;
			PlayerBulletdesc.vExtents = _float3(1.1f, 1.1f, 20.1f);
			PlayerBulletdesc.vCenter = _float3(0.f, PlayerBulletdesc.vExtents.y, PlayerBulletdesc.vExtents.z);
			PlayerBulletdesc.vRotation = _float3(0.f, 0.f, 0.f);
			PlayerBulletdesc.vPosition = _float4(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f);
			PlayerBulletdesc.vDir = vPlayerLook;
			PlayerBulletdesc.fLifeTime = 0.1f;
			PlayerBulletdesc.BulletState = &CBullet::Pop;
			PlayerBulletdesc.fDamage=7.0f;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &PlayerBulletdesc);

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Default_Attack_Beam_Size_Up"),_float4(1.0f,0.8f,0.8f,0.7f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Default_Attack_Muzzle_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE,TEXT("Player_Amanda_Default_Attack_LowpolySphere16_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X,TEXT("Player_Amanda_Default_Attack_hit001_Size_Up_X"),_float4(1.0f,0.2f,0.8f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN_TEXTURE,TEXT("Player_Amanda_Default_Attack_Shell001_Up_And_Down_Tex"),_float4(1.0f,0.2f,0.8f,0.5f)});

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f), -vPlayerLook);


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

NodeStates Amanda_Body_Player::DoAttack_Shot_Dash()
{
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	double fCurPos = m_pModelCom->Get_CurrentPosition();
	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();
	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_DASH)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CRITICAL;

		if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CRITICAL)
		{
			if (fCurPos >= 0.02 && fCurPos < 0.04)
			{
				PlayerBulletdesc.strModelName = TEXT("swordThrowing");
				PlayerBulletdesc.eColliderType = CCollider::TYPE_OBB;
				PlayerBulletdesc.vExtents = _float3(1.1f, 1.1f, 5.1f);
				PlayerBulletdesc.vCenter = _float3(0.f, PlayerBulletdesc.vExtents.y, PlayerBulletdesc.vExtents.z);
				PlayerBulletdesc.vRotation = _float3(0.f, 0.f, 0.f);
				PlayerBulletdesc.vPosition = _float4(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f);
				PlayerBulletdesc.vDir = vPlayerLook;
				PlayerBulletdesc.fLifeTime = 0.1f;
				PlayerBulletdesc.BulletState = &CBullet::Pop;
				PlayerBulletdesc.fDamage = 20.f;
				m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &PlayerBulletdesc);
				
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z_TEXTURE,TEXT("Player_Amanda_Balance_C_Grappling_Pop_Texture"),_float4(0.8f,0.8f,0.8f,0.5f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Balance_C_LowpolySphere16_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Balance_C_ShotRing_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Balance_C_BulletBuck_Size_Up"),_float4(1.0f,0.8f,0.0f,0.6f) });
				vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z,TEXT("Player_Amanda_Balance_C_MuzzleBuck.001_Size_Up_Z"),_float4(0.9f,0.0f,0.1f,0.7f) });

				CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y+1.0f, vPlayerPos.z, 1.0f), vPlayerLook);
			}
		}



		m_bLoop = false;
		m_AnimSpeed = 1.0f;
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

	return NodeStates::FAILURE;
}

NodeStates Amanda_Body_Player::DoAttack_Dash_Buck()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_DASH_BUCK)
	{

		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_DASH_BUCK;
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

NodeStates Amanda_Body_Player::DoAttack_Dash_Buck_Next()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_DASH_BUCK_NEXT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_DASH_BUCK_NEXT;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_DASH_BUCK)
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

NodeStates Amanda_Body_Player::DoAttack_Dash_Slide_Shot()
{

	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	double fCurPos = m_pModelCom->Get_CurrentPosition();

	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();

	if (m_eCurState == CPlayer::STATE_ATTACK_DASH_SLIDING_SHOT)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SLIDING_SHOT;
		m_bLoop = false;
		m_AnimSpeed = 2.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(true);


		if (fCurPos >= 0.02 && fCurPos < 0.04)
		{
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_ShotRing_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"),_float4(1.0f,0.1f,0.1f,0.7f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"),_float4(1.0f,0.5f,0.0f,0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_LowpolySphere16_Size_Up"),_float4(1.0f,0.8f,0.9f,0.2f) });
			

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f), vPlayerLook);
			
		}
		if (fCurPos >= 0.21 && fCurPos < 0.24)
		{
			
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"),_float4(0.9f,0.0f,0.0f,0.5f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"),_float4(0.9f,0.9f,0.0f,0.2f) });



			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f), vPlayerLook);
		}
		if (fCurPos >= 0.40 && fCurPos < 0.42)
		{
			
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_MuzzleBuck.001_Size_Up"),_float4(0.6f,0.0f,0.0f,0.2f) });
			//vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_C_BulletBuck_Size_Up"),_float4(0.8f,0.8f,0.8f,0.5f) });



			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f), vPlayerLook);
		}



		if (m_pModelCom->Get_AnimationIndex() == Amanda_ANIMATION_SHOT_STOP)
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

NodeStates Amanda_Body_Player::DoAttack_Shot_Crouch()
{
	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();



	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_CROUCH)
	{
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CROUCH;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;

		if (dNowFramePos >= 0.02 && dNowFramePos <= 0.04)
		{
			_uint iFadeLayerSize = m_pGameInstance->Get_LayerSize(CLoader::m_eNextLevel, TEXT("Layer_Fade_In_Out"));

			for (_uint i = 0; i < iFadeLayerSize; ++i)
			{
				Fade_In_Out* pFadeInOut = static_cast<Fade_In_Out*>(m_pGameInstance->Get_Object(CLoader::m_eNextLevel, TEXT("Layer_Fade_In_Out"), i));
				pFadeInOut->Start_FadeIn();
			}
			
			


			PlayerBulletdesc.strModelName = TEXT("swordThrowing");
			PlayerBulletdesc.eColliderType = CCollider::TYPE_OBB;
			PlayerBulletdesc.vExtents = _float3(1.1f, 1.1f, 20.1f);
			PlayerBulletdesc.vCenter = _float3(0.f, PlayerBulletdesc.vExtents.y, PlayerBulletdesc.vExtents.z);
			PlayerBulletdesc.vRotation = _float3(0.f, 0.f, 0.f);
			PlayerBulletdesc.vPosition = _float4(vPlayerPos.x, vPlayerPos.y + 1.0f, vPlayerPos.z, 1.0f);
			PlayerBulletdesc.vDir = vPlayerLook;
			PlayerBulletdesc.fLifeTime = 0.5f;
			PlayerBulletdesc.BulletState = &CBullet::Pop;
			PlayerBulletdesc.fDamage=100.f;
			m_pGameInstance->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_2_Player_Bullet"), TEXT("Prototype_GameObject_PlayerBullet"), &PlayerBulletdesc);



			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z_NONE_DISOLVE,TEXT("Player_Amanda_Balance_XC_Ult_Spring_Rotate_Z"),_float4(0.9f,0.4f,0.1f,1.0f),false,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z,TEXT("Player_Amanda_Balance_XC_Ult_Ring_Rotate_Z"),_float4(1.0f,0.1f,0.1f,0.1f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATE_Z,TEXT("Player_Amanda_Balance_XC_Ult_Beam_Rotate_Z"),_float4(1.0f,1.0f,0.0f,1.0f),false,true });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Amanda_Balance_XC_LowpolySphere16_1_Spread"),_float4(0.8f,0.9f,0.8f,0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD,TEXT("Player_Amanda_Balance_XC_LowpolySphere16_2_Spread"),_float4(0.9f,0.8f,0.9f,0.2f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X,TEXT("Player_Amanda_Balance_XC_hit001_Size_Up_X"),_float4(0.9f,0.8f,0.9f,0.2f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"),_float4(0.9f,0.8f,0.9f,1.0f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN,TEXT("Player_Amanda_Balance_XC_ExplosionFrag_Up_And_Down"),_float4(1.0f,1.0f,1.0f,0.5f) });

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN_TEXTURE,TEXT("Player_Amanda_Balance_XC_Shell.002_Up_And_Down_Texture"),_float4(1.0f,0.2f,0.8f,0.5f) });

			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z, 1.0f), vPlayerLook);
		}



		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CROUCH_READY)
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

NodeStates Amanda_Body_Player::DoAttack_Shot_Upper()
{
	CPlayerBullet::CPlayetBULLET_DESC PlayerBulletdesc = {};
	vector<CParticle_Mesh::PARTICLE_DESC> vecDesc = {};
	_vector vPlayerLook = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pPlayer)->GetLookDir());
	_float3 vPlayerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Position();



	if (m_eCurState == CPlayer::STATE_ATTACK_SHOT_UPPER)
	{
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_UPPER_POST;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		double dNowFramePos = m_pModelCom->Get_CurrentPosition();
		if (dNowFramePos >= 0.08 && dNowFramePos <= 0.10)
		{

			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_XC_ShotRing_Size_Up"),_float4(0.8f,0.8f,0.1f,0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_XC_MuzzleBuck_Size_Up"),_float4(0.6f,0.6f,0.f,0.4f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_XC_LowpolySphere16_Size_Up"),_float4(0.2f,0.2f,0.0f,0.1f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Y,TEXT("Player_Amanda_Power_XC_Flame_Size_UP_Y"),_float4(0.8f,0.4f,0.0f,0.3f) });
			vecDesc.push_back({ CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP,TEXT("Player_Amanda_Power_XC_BulletBuck_Size_Up"),_float4(1.0f,0.1f,0.1f,0.4f) });
			CParticle_Mesh::Make_Particle(vecDesc, XMVectorSet(vPlayerPos.x, vPlayerPos.y+3.0f, vPlayerPos.z, 1.0f), vPlayerLook);
		}




		if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_UPPER_PRE)
		{
			return NodeStates::RUNNING;
		}
		if (m_pModelCom->Get_AnimFinished())
		{
			

			//총알 생성
			m_eCurState = CPlayer::STATE_BEFORE_ATTACK_MOTION;
			dynamic_cast<CPlayer*>(m_pPlayer)->SetState(m_eCurState);
			return NodeStates::SUCCESS;
		}
		else
			return NodeStates::RUNNING;
	}

}

NodeStates Amanda_Body_Player::DoAttack_Shot_Ghost_Crouch()
{
	if (m_eCurState == CPlayer::STATE_ATTACK_GHOST_CROUCH)
	{
	
		m_iAnimIndex = Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CROUCH;
		m_bLoop = false;
		m_AnimSpeed = 1.0f;
		dynamic_cast<CPlayer*>(m_pPlayer)->SetAttackEndTime(false);
		if (m_pModelCom->Get_AnimationIndex() == Player_Amanda_AnimationType::Amanda_ANIMATION_SHOT_CROUCH_READY)
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

