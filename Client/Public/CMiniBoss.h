
#pragma once
#include"Monster.h"

class CSequence;


BEGIN(Client)
class CMiniBoss final : public CMonster
{
	enum CMiniBoss_STATES
	{
		STATES_ATTACK,
		STATES_HURT,
		STATES_IDLE,
		STATES_MOVE,
		STATES_MOVESTOP,
		STATES_SMASH,
		STATES_BIGSMASH,
		STATES_END
	};

public:

private:
	explicit CMiniBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMiniBoss(const CMiniBoss& rhs);
	virtual ~CMiniBoss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:


	NodeStates			DoIdle(_float fTimeDelta);
	NodeStates			DoAttack(_float fTimeDelta);
	NodeStates			DoMove(_float fTimeDelta);
	NodeStates			DoHit(_float fTimeDelta);
	_bool				DoDetact(_float fTimeDelta);
	NodeStates			DoIsAlive();



private:
	_bool m_bCanMove = false;


	CSequence* m_pRootNode = nullptr;
	CMiniBoss_STATES				m_eCurState = STATES_IDLE;
public:
	virtual HRESULT Add_Components() override;
	HRESULT Add_PartObjects();
public:
	static CMiniBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
