#pragma once
#include"Monster.h"

class CSequence;


BEGIN(Client)
class CBoss_B final : public CMonster
{
public:
	enum CBoss_B_STATES
	{
		STATES_ATTACK_A,
		STATES_HURT,
		STATES_IDLE,
		STATES_MOVE,
		STATES_MOVESTOP,
		STATES_SMASH,
		STATES_ATTACK_B,
		STATES_ATTACK_C,
		STATES_ATTACK_D,
		STATES_ATTACK_E,
		STATES_ATTACK_F,


		STATES_GO_RANDPOS,
		STATES_GO_JUMPATTACK,
		STATES_GO_POWERFULATTACK,
		STATES_GO_POWERFULATTACK2,
		STATES_GO_POWERFULATTACK3,


		STATES_END
	};

private:
	enum CBoss_Bone
	{
		LEFT_HAND_9,
		LEFT_HAND_10,
		RIGHT_HAND_9,
		RIGHT_HAND_10,
		HAND_END


	};

private:
	struct BulletSpawnDesc
	{
		_float fspawnTime;
		bool bisSpawned;
	};



private:
	explicit CBoss_B(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBoss_B(const CBoss_B& rhs);
	virtual ~CBoss_B() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	NodeStates			DoIdle(_float fTimeDelta);
	NodeStates          DoMove(_float fTimeDelta);
	NodeStates			DoPowerfulAttack1(_float fTimeDelta);
	NodeStates			DoPowerfulAttack2(_float fTimeDelta);
	NodeStates			DoPowerfulAttack3(_float fTimeDelta);
	NodeStates			DoJumpAttack(_float fTimeDelta);
	NodeStates          DoRightHandAttack(_float fTimeDelta);
	NodeStates          DoLeftHandAttack(_float fTimeDelta);
	NodeStates          DoPullUpAttack(_float fTimeDelta);



private:
	void Make_Falling_Bullet(_float fTimeDelta);
	CBoss_B_STATES GetRandomState();

	void				CreateEffect_By_Motion(_float fTimeDelta);
private:
	vector<class CGameObject*>		m_PartObjects;
	CSequence*						m_pRootNode = nullptr;
	CBoss_B_STATES					m_eCurState = CBoss_B_STATES::STATES_IDLE;

	_vector							m_vOriginPos;
	
	_vector							m_vMovePos[4];
	_float							fChangeTime = 0.f;

	_bool							m_bIsJumpAttack = false;
	_bool							m_bIsDetect = false;


	const _float4x4*				m_pHandMatrix[HAND_END] = {nullptr,nullptr,nullptr,nullptr};
	_matrix							m_HandMatrix[HAND_END] = { {},{},{},{} };


    vector<BulletSpawnDesc>			m_vBulletSpawnDesc;
	_float 							m_fBulletSpawnTime = 0.f;
	_float                          m_fHitTime = 0.f;
	_bool							m_bIsBulletSpawn = false;
public:
	HRESULT Add_PartObjects();

public:
	virtual HRESULT Add_Components() override;
public:
	static CBoss_B* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
