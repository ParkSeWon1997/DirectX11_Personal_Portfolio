
#pragma once
#include "Monster.h"

class CSequence;

BEGIN(Client)
class CHandBoss final : public CMonster
{
public:
	enum CHandBoss_STATES
	{
		STATES_ATTACK_A_0,
		STATES_SMASH,
		STATES_IDLE,
		STATES_ATTACK_B_0,
		STATES_ATTACK_C_0,
		STATES_ATTACK_D_0,
		STATES_ATTACK_A_1,
		STATES_ATTACK_B_1,
		STATES_ATTACK_C_1,
		STATES_ATTACK_D_1,
		STATES_ATTACK_A_2,
		STATES_ATTACK_B_2,
		STATES_ATTACK_C_2,
		STATES_ATTACK_D_2,
		STATES_ATTACK_GROGGY_START,
		STATES_ATTACK_GROGGY_END,
		STATES_ATTACK_E_0,
		STATES_ATTACK_E_1,
		STATES_ATTACK_C_C_1,
		STATES_ATTACK_C_B_1,
		STATES_ATTACK_C_A_1,
		STATES_END

	};

	enum CHandBoss_PARTS
	{
		PARTS_HAND_L,
		PARTS_HAND_R,
		PARTS_END
	};
public:
	typedef struct CHandBoss_DESC : CMonster::CMonster_DESC
	{

		CHandBoss_PARTS ePart;
	}CHandBoss_DESC;


private:
	explicit CHandBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHandBoss(const CHandBoss& rhs);
	virtual ~CHandBoss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	virtual HRESULT Add_Components() override;


private:
	NodeStates			DoIdle(_float fTimeDelta);
	NodeStates         DoAttack_A_0(_float fTimeDelta);
	NodeStates         DoAttack_A_1(_float fTimeDelta);
	NodeStates         DoAttack_A_2(_float fTimeDelta);
	NodeStates 			DoFollow(_float fTimeDelta);
	NodeStates			DoTakeDown(_float fTimeDelta);
	NodeStates			DoHandsUp(_float fTimeDelta);
	NodeStates			DoAttack_C_0(_float fTimeDelta);
	NodeStates			DoAttack_C_1(_float fTimeDelta);
	NodeStates			DoAttack_C_2(_float fTimeDelta);
	NodeStates			DoAttack_D_0(_float fTimeDelta);
	NodeStates			DoAttack_D_1(_float fTimeDelta);
	NodeStates			DoAttack_D_2(_float fTimeDelta);
	NodeStates			DoAttack_E_0(_float fTimeDelta);
	NodeStates			DoAttack_E_1(_float fTimeDelta);
	NodeStates			DoAttack_Groggy_Start(_float fTimeDelta);
	NodeStates			DoAttack_Groggy_End(_float fTimeDelta);
	NodeStates          DoIsAlive(_float fTimeDelta);


private:
	CHandBoss_STATES GetRandomAttack();

	
	void SetRandomData();
private:
	_vector							m_vOriginPos = {};
	_float                          m_fHitTime = 0.f;
	_float 							fChangeTime = 0.f;
	_float							fRandTime = 0.f;
	vector<class CGameObject*>		m_PartObjects;
	CHandBoss_STATES				m_eCurState = STATES_END;
	CHandBoss_PARTS					m_eCurPart = PARTS_END;

	CSequence* m_pRootNode = nullptr;
	const _float4x4* m_HandPalm= nullptr;
	_matrix m_HandPalmMatrix = {};


	_uint  m_iBulletCount = 0;
public:
	HRESULT Add_PartObjects();


public:
	static CHandBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END
