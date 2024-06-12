#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include"Player.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END
//class CPlayer;



class CSequence;
class CSelector;
class CActionNode;
class CInverter;



BEGIN(Client)

class Matilda_Body_Player final : public CPartObject
{
public:
	typedef struct Matilda_BODY_DESC : public CPartObject::PARTOBJ_DESC
	{
		class CGameObject* m_pPlayer = nullptr;


	}Matilda_BODY_DESC;
private:
	Matilda_Body_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Matilda_Body_Player(const Matilda_Body_Player& rhs);
	virtual ~Matilda_Body_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

	
	void SetAnimState(CPlayer::STATE eState) { m_eCurState = eState; }
	void SetAttackType(CPlayer::SWORD_MASTER_TYPE eType) { m_eSwordMasterType = eType; }
	void SetGunSlingerType(CPlayer::GUN_SLINGER_TYPE eType) { m_eGunSlingerType = eType; }
	void SetCharacterType(CPlayer::CHRACTER_TYPE eType) { m_eCharacterType = eType; }
	void SetAnimSpeed(_float fSpeed) { m_AnimSpeed = fSpeed; }
	void SetWeapon(class CWeapon* pWeapon) { m_pWeapon = pWeapon; }
	void SetShadowOn(_bool bIsShadowOn) { m_bIsShadowOn = bIsShadowOn; }
public:
	void CreateEffect_By_Motion(_float fTimeDelta);
	

private:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CCollider*						m_pColliderCom = { nullptr };

	_uint							m_iAnimIndex = 0;
	_bool							m_bLoop = false;	
	_bool                           m_bIsShadowOn = false;
private:
	_float							m_AnimSpeed = 1.0f;
	class CGameObject*				m_pPlayer = nullptr;
	class CWeapon*					m_pWeapon = nullptr;
	CPlayer::STATE					m_eCurState = CPlayer::STATE::STATE_END;
	CPlayer::SWORD_MASTER_TYPE		m_eSwordMasterType = CPlayer::SWORD_MASTER_TYPE::SWORD_END;
	CPlayer::GUN_SLINGER_TYPE       m_eGunSlingerType = CPlayer::GUN_SLINGER_TYPE::GUN_END;
	CPlayer::CHRACTER_TYPE			m_eCharacterType = CPlayer::CHRACTER_TYPE::CHRACTER_END;
	

	const _float4x4*                m_pWeaponMatrix = { nullptr };
	_matrix 					   m_matWeaponMatrix= {};
private:
	const _float4x4* m_pSocketMatrix = { nullptr };

public:
	NodeStates	 DoIdle();
	NodeStates	 DoRun();
	NodeStates	 DoAttack_Left();
	NodeStates	 DoAttack_Right();
	NodeStates	 DoAttack_End();
	NodeStates	 DoAttack_End2();
	NodeStates	 DoHit();
	NodeStates	 DoDash();
	NodeStates	 DoAttack_After_Idle();
	NodeStates   DoAttack_Before();
	NodeStates	 DoMoveAfterStop();
	NodeStates   DoBackDash();
	NodeStates   DoThrowWeapon();
	NodeStates   DoMatildaHidden_D_Short();
	NodeStates   DoAttackStinger();
	NodeStates   DoAttakcSpin();
	NodeStates   DoAttackSpinEnd();
	NodeStates   DoAttackStomp();
	NodeStates   DoAttackThrow();
	NodeStates   DoAttackJumpEnd();
	NodeStates   DoAttackUltimateBalance();
	NodeStates   DoAttackUltimateTechnic();
	NodeStates   DoAttackUltimatePower();
	NodeStates   DoAttackUltimateReverse();

	NodeStates   DoAttack_Shot();
	NodeStates   DoAttack_Shot_Critical();
	NodeStates   DoAttack_Shot_Dash();
	NodeStates   DoAttack_Dash_Buck();
	NodeStates   DoAttack_Dash_Buck_Next();
	NodeStates   DoAttack_Dash_Slide_Shot();
	NodeStates   DoAttack_Shot_Crouch();
	NodeStates   DoAttack_Shot_Upper();
	NodeStates   DoAttack_Shot_Ghost_Crouch();
	

private:
	CSequence* m_pRootNode = nullptr;

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static Matilda_Body_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END