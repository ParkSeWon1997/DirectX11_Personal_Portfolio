#pragma once
#include"CNode.h"


class CSequence final : public CNode<>
{
private:
	CSequence();
	virtual ~CSequence() = default;


public:
	template<typename T>
	HRESULT Initialize(T arg);

private:
	vector<CNode*> m_nodes;


public:
	template<typename... T>
	static CSequence* Create(T... nodes);

	virtual	NodeStates Evaluate() override;
	virtual NodeStates Evaluate(_float fTimeDelta) override;

	virtual void Free() override;
};

