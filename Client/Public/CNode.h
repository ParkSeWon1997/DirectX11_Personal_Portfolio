#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

END

BEGIN(Client)
template<typename... T>
class CNode abstract : public CBase
{
protected:
	CNode(){};
	virtual ~CNode() = default;

protected:
	NodeStates m_eNodeState = NONE;




public:
	virtual NodeStates Evaluate() = 0;
	virtual NodeStates Evaluate(_float fTimeDelta) = 0;

	


public:



	virtual void Free()override {};
};

END
