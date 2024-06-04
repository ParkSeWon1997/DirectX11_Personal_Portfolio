#pragma once
#include"CNode.h"


class CSelector final : public CNode<>
{
private:
	//template<typename T>
	CSelector();
	virtual ~CSelector()=default;


public:
	template<typename T>
	HRESULT Initialize(T arg);
private:
	vector<CNode*> m_nodes;

public:
	template<typename... T>
	static CSelector* Create(T... nodes);

	virtual	NodeStates Evaluate()override;	
	virtual NodeStates Evaluate(_float fTimeDelta) override;


	virtual void Free() override;
};

