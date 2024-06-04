#include"stdafx.h"
#include "CActionNode.h"




template<typename T>
CActionNode::CActionNode(T action)
	//:m_action(std::move(action))
	//, m_actionWithArg(std::move(action))
{
	if constexpr (is_invocable_v<T>)
	{
		m_action = action;
	}
	else if constexpr (is_invocable_v<T, _float>)
	{
		m_actionWithArg = action;
	}
	else
	{
		static_assert(false, "지원되지 않는 매개변수: Invalid ActionNode");
		
	}

	

	
}


HRESULT CActionNode::Initialize()
{
	return S_OK;
}



template<typename T>
CActionNode* CActionNode::create(T action)
{
	CActionNode* pInstance = new CActionNode(action);
		if (FAILED(pInstance->Initialize()))
		{
			Safe_Release(pInstance);
			MSG_BOX("Failed To Cloned : CActionNode");
			return nullptr;
		}
		return pInstance;


}


NodeStates CActionNode::Evaluate()
{
	switch (m_action())
	{
	case NodeStates::SUCCESS:
		m_eNodeState = NodeStates::SUCCESS;
		return m_eNodeState;
	case NodeStates::FAILURE:
		m_eNodeState = NodeStates::FAILURE;
		return m_eNodeState;
	case NodeStates::RUNNING:
		m_eNodeState = NodeStates::RUNNING;
		return m_eNodeState;
	default:
		m_eNodeState = NodeStates::FAILURE;
		return m_eNodeState;
	}

}

NodeStates CActionNode::Evaluate(_float fTimeDelta)
{
	switch (m_actionWithArg(fTimeDelta))
	{
	case NodeStates::SUCCESS:
		m_eNodeState = NodeStates::SUCCESS;
		return m_eNodeState;
	case NodeStates::FAILURE:
		m_eNodeState = NodeStates::FAILURE;
		return m_eNodeState;
	case NodeStates::RUNNING:
		m_eNodeState = NodeStates::RUNNING;
		return m_eNodeState;
	default:
		m_eNodeState = NodeStates::FAILURE;
		return m_eNodeState;
	}


	
}


void CActionNode::Free()
{
	if (m_action)
	{
		m_action = nullptr;
	}
	if (m_actionWithArg)
	{
		m_actionWithArg = nullptr;
	}
	//m_action = nullptr;
	//m_actionWithArg = nullptr;
}
