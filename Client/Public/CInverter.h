#pragma once
#include"CNode.h"

class CInverter final : public CNode<>
{
private:
	template<typename T>
	CInverter(T node);
	virtual ~CInverter() = default;


public:
	HRESULT Initialize();
	CNode* GetNode() const { return m_Node; }


private:
	CNode* m_Node;


public:
	template<typename T>
	static CInverter* Create(T node);


	virtual NodeStates Evaluate()override;
	virtual NodeStates Evaluate(_float fTimeDelta) override;

	virtual void Free() override;
};


