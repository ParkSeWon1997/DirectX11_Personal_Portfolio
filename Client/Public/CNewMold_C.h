
#pragma once
#include"Monster.h"

class CSequence;


BEGIN(Client)
class CNewMold_C final : public CMonster
{
	enum CNewMold_C_STATES
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
	explicit CNewMold_C(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNewMold_C(const CNewMold_C& rhs);
	virtual ~CNewMold_C() = default;

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
	NodeStates			DoSmash(_float fTimeDelta);
	NodeStates			DoBigSmash(_float fTimeDelta);
	_bool				DoDetact(_float fTimeDelta);
	NodeStates			DoIsAlive();



private:

	_bool m_bCanMove = false;


	const _float4x4*				m_pHandMatrix = { nullptr };
	_matrix							m_HandMatrix = {};

	CSequence* 					m_pRootNode = nullptr;
	CNewMold_C_STATES				m_eCurState = STATES_IDLE;
public:
	virtual HRESULT Add_Components() override;
	HRESULT Add_PartObjects();
public:
	static CNewMold_C* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
