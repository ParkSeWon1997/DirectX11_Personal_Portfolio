#pragma once
#include"CNode.h"


class CActionNode final : public CNode<>
{
private:
	template<typename T>
	CActionNode(T action);


	//CActionNode(function<NodeStates()> actionNoArg);
	//CActionNode(function<NodeStates(_float fTimeDelta)> actionWithArg);
	virtual ~CActionNode() = default;

public:

	HRESULT Initialize();


private:
	
	std::function<NodeStates()> m_action;
	std::function<NodeStates(_float)> m_actionWithArg;


public:
	template<typename T>
	static CActionNode* create(T action);
	//static CActionNode* create(function<NodeStates()> action);



	virtual	NodeStates Evaluate( )override;
	virtual NodeStates Evaluate(_float fTimeDelta) override;


	//template<typename... T>
	//virtual NodeStates Evaluate(_float fTimeDelta) override;



	virtual void Free() override;


	

	
};

