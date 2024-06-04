
#pragma once
#include"Monster.h"

class CSequence;


BEGIN(Client)
class CNewMold final : public CMonster
{
	enum CNewMold_STATES
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
	explicit CNewMold(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNewMold(const CNewMold& rhs);
	virtual ~CNewMold() = default;

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
	//_bool				DoDetact(_float fTimeDelta);
	NodeStates			DoIsAlive();

	CNewMold_STATES GetRandomState();
	void Make_particle_Bullet(_float3 PlayerPos);
private:
	vector<class CGameObject*>		m_PartObjects;
	_bool m_bCanMove = false;
	

	CSequence* 					m_pRootNode = nullptr;
	CNewMold_STATES				m_eCurState = STATES_IDLE;
	_float						m_fChangeTime = 0.f;
	_float						m_fTime = 0.f;
	_float                      m_fRandomRotateTimer = 0.f;
	_float                      m_fRandomMoveTimer = 0.f;
	_float3						m_fPlayerPos = {};
public:
	virtual HRESULT Add_Components() override;
	HRESULT Add_PartObjects();
public:
	static CNewMold* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
