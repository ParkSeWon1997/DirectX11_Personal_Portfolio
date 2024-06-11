#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END

/* 플레이어를 구성하는 객체들을 들고 있는 객체이다. */



BEGIN(Client)
class CPlayer final : public CGameObject
{
public:
	enum PART { PART_CAMERA, PART_BODY_MATILDA, PART_BODY_AMANDA, PART_WEAPON, PART_END };
	enum STATE {
		STATE_DEFAULT_IDLE = 0,
		STATE_BEFORE_ATTACK_MOTION,
		STATE_AFTER_ATTACK_IDLE,
		STATE_WALK,
		STATE_RUN,
		STATE_COMBO_ATTACK_LEFT,
		STATE_COMBO_ATTACK_RIGHT,
		STATE_ATTACK_DEFAULT_END,
		STATE_HIT,
		STATE_DIE,
		STATE_DASH,
		STATE_ATTACKIDLE,
		STATE_BACKSTEP,
		STATE_THROW_WEAPON,
		STATE_WHIRLWIND,
		STATE_MOVE_AFTER_STOP,
		STATE_MATILD_D_SHORT,
		STATE_ATTACK_STINGER,
		STATE_ATTACK_SPIN,
		STATE_ATTACK_SPIN_END,
		STATE_ATTACK_STOMP,
		STATE_ATTACK_THORW,
		STATE_ATTACK_ULTIMATE_BALANCE,
		STATE_ATTACK_ULTIMATE_TECHNIC,
		STATE_ATTACK_ULTIMATE_POWER,
		STATE_ATTACK_ULTIMATE_REVERSE,
		STATE_JUMP_START,
		STATE_JUMP_END,

		STATE_ATTACK_SHOT,
		STATE_ATTACK_SHOT_CRITICAL,
		STATE_ATTACK_SHOT_DASH,
		STATE_ATTACK_DASH_BUCK,
		STATE_ATTACK_DASH_BUCK_NEXT,
		STATE_ATTACK_DASH_SLIDING_SHOT,
		STATE_ATTACK_SLIDING,
		STATE_ATTACK_SHOT_CROUCH,
		STATE_ATTACK_SHOT_UPPER,
		STATE_ATTACK_GHOST_CROUCH,

		STATE_END
	};

	enum CHRACTER_TYPE
	{
		CHRACTER_SWORD_MASTER = 0,
		CHRACTER_GUN_SLINGER,
		CHRACTER_END
	};



	enum SWORD_MASTER_TYPE
	{
		SWORD_BALANCE = 0,
		SWORD_TECHNNIC,
		SWORD_POWER,
		SWORD_REVERSE,
		SWORD_END
	};
	enum GUN_SLINGER_TYPE
	{
		GUN_BALANCE = 0,
		GUN_TECHNNIC,
		GUN_POWER,
		GUN_REVERSE,
		GUN_END
	};



public:
	struct Player_Abililty
	{
		enum SKILL_COOL_TIME
		{
			COOL_TIME_SKILL_Z = 0,
			COOL_TIME_SKILL_X,
			COOL_TIME_SKILL_C,
			COOL_TIME_SKILL_X_C,
			COOL_TIME_SKILL_Z_SPACE,
			COOL_TIME_SKILL_END
		};


		float fDashPower = 0.f;
		float fMoveSpeed = 1.f;
		float fHp = 0.f;
		float fMaxHp = 0.f;
		float fDamage = 10.f;
		_uint iComboCount = 0;
		float fDashCoolTime = 1.f;
		_uint	  iDashCount = 0;
		float fMatildaSkillCoolTime[COOL_TIME_SKILL_END] = { 0.0f,0.f,0.f ,0.f ,0.f };
		_bool bMatildaSkillCoolTimeOn[COOL_TIME_SKILL_END] = { false,false,false,false,false };
		float fAmandaSkillCoolTime[COOL_TIME_SKILL_END] = { 0.0f,0.f,0.f ,0.f ,0.f };
		_bool bAmandaSkillCoolTimeOn[COOL_TIME_SKILL_END] = { false,false,false,false,false };

		SWORD_MASTER_TYPE eSwordMasterType = SWORD_END;
		GUN_SLINGER_TYPE eGunSlingerType = GUN_END;

		_bool bIsFirstData = false;
	};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;	


public:
	_bool IsAttacking() { return m_bIsAttacking; }

	void SetNextAttack(_bool bNextAttack) { m_bNextAttack = bNextAttack; }
	void SetState(STATE eState) { m_eCurState = eState; }
	void SetJump(_bool bJump) { m_bIsJump = bJump; }
	void SetAttackEndTime(_bool bIsAttacking) { m_bIsAttacking = bIsAttacking; }
	void SetMove_After_Skill(_bool bMatilda_D_Short) { m_bIsMove_After_Skill = bMatilda_D_Short; }
	void SetMovePower(_float fPower) { m_fMovePower = fPower; }
	void SetFire(_bool bFire) { m_bIsFire = bFire; }

	void SetPlayerSwordType(SWORD_MASTER_TYPE eType) { m_Ability.eSwordMasterType = eType; }
	void SetPlayerGunType(GUN_SLINGER_TYPE eType) { m_Ability.eGunSlingerType = eType; }

	
public:
		_vector GetLookDir() { return m_pTransformCom->Get_State(CTransform::STATE_LOOK); }
public:
	HRESULT Add_Components();	
	HRESULT Add_PartObjects();
	_bool Intersect(PART ePartObjID, const wstring& strComponetTag,  CCollider* pTargetCollider);
	_float GetDamage() { return m_Ability.fDamage; }

	void SetIsCollision(_bool bIsCollision) { m_bIsCollision = bIsCollision; }
	_bool GetIsCollision() { return m_bIsCollision; }


	void Set_CameraShake(_bool bIsCutScene,_float fShakeTime);
	void Set_CameraTargetPos(_vector vTargetPos);
	void Set_CameraRotation(_float fAngle);



private:
	void Move(_float fTimeDelta);
	void SetStatePressZ(_float fTimeDelta);
	void SetStatePressX(_float fTimeDelta);
	void SetStatePressC(_float fTimeDelta);
	void SetStatePress_X_C(_float fTimeDelta);


	void JumpingState(_float fTimeDelta);


	void DoMove_After_Skill();

	void MakeDrone(_int DroneCount, _float fDistance, wstring DroneName);

	void MakeChangeparticle(_float fTimeDelta);
	void CheckCoolTime(_float fTimeDelta);



	void CloseTargetPos(_float fTimeDelta);



	template<typename T,typename U>
	_bool CoolTimeCheck(T val,U fTimedelta);
	

private:
	vector<class CGameObject*>		m_PartObjects;

	class CNavigation* m_pNavigationCom = { nullptr };


private:
	Player_Abililty					m_Ability = {};
	STATE							m_eCurState = STATE_DEFAULT_IDLE;
	CHRACTER_TYPE					m_eCharacterType = CHRACTER_GUN_SLINGER;
	SWORD_MASTER_TYPE				m_eSwordMasterType = SWORD_END;
	GUN_SLINGER_TYPE				m_eGunSlingerType = GUN_END;

	_vector                         m_vCloseTargetPos = {};
	_float							m_fMovePower = 0.f;
	_float                          m_fQuestTime = 0.f;
	
	
	_bool                           m_bIsMonsterHave = false;
	_bool                           m_bIsCollision = false;
	_bool							m_bIsFire = false;
	_bool							m_bNextAttack = false;
	_bool							m_bIsJump = false;
	_bool							m_bIsMove_After_Skill = false;
	_bool    						m_bIsAttacking = false;
	_bool                           m_bIsMonsterDetact = false;
	_bool                           m_bIsChange = false;
public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

