
#pragma once
#include"Monster.h"

class CSequence;


BEGIN(Client)
class CNewMold_D final : public CMonster
{
	enum CNewMold_D_STATES
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
	explicit CNewMold_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNewMold_D(const CNewMold_D& rhs);
	virtual ~CNewMold_D() = default;

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
	

	CSequence* 					m_pRootNode = nullptr;
	CNewMold_D_STATES				m_eCurState = STATES_IDLE;
public:
	virtual HRESULT Add_Components() override;
	HRESULT Add_PartObjects();
public:
	static CNewMold_D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
