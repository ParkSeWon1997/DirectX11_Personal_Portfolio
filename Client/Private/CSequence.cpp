#include "stdafx.h"
#include "CSequence.h"


CSequence::CSequence()
{
}

template<typename T>
HRESULT CSequence::Initialize(T arg)
{
	m_nodes.emplace_back(arg);
	return S_OK;
}

//NodeStates CSequence::Evaluate()
//{
//	_bool bChildRunning = false;
//	for(auto& node : m_nodes)
//	{
//		switch (node->Evaluate())
//		{
//		case NodeStates::SUCCESS:
//			continue;
//		case NodeStates::FAILURE:
//			m_eNodeState = NodeStates::FAILURE;
//			return m_eNodeState;
//		case NodeStates::RUNNING:
//			bChildRunning = true;
//			continue;
//		default:
//			m_eNodeState = NodeStates::SUCCESS;
//			return m_eNodeState;
//		}
//
//	}
//	m_eNodeState = bChildRunning ? NodeStates::RUNNING : NodeStates::SUCCESS;
//
//return m_eNodeState;
//	
//}

NodeStates CSequence::Evaluate()
{
	_bool bChildRunning = false;
	for (auto& node : m_nodes)
	{
		
		switch (node->Evaluate())
		{
		case NodeStates::SUCCESS:
			continue;
		case NodeStates::FAILURE:
			m_eNodeState = NodeStates::FAILURE;
			return m_eNodeState;
		case NodeStates::RUNNING:
			bChildRunning = true;
			continue;
		default:
			m_eNodeState = NodeStates::SUCCESS;
			return m_eNodeState;
		}

	}
	m_eNodeState = bChildRunning ? NodeStates::RUNNING : NodeStates::SUCCESS;

	return m_eNodeState;

}
NodeStates CSequence::Evaluate(_float fTimeDelta)
{
	_bool bChildRunning = false;
	for (auto& node : m_nodes)
	{

		switch (node->Evaluate(fTimeDelta))
		{
		case NodeStates::SUCCESS:
			continue;
		case NodeStates::FAILURE:
			m_eNodeState = NodeStates::FAILURE;
			return m_eNodeState;
		case NodeStates::RUNNING:
			bChildRunning = true;
			continue;
		default:
			m_eNodeState = NodeStates::SUCCESS;
			return m_eNodeState;
		}

	}
	m_eNodeState = bChildRunning ? NodeStates::RUNNING : NodeStates::SUCCESS;

	return m_eNodeState;

}
template<typename... T>
CSequence* CSequence::Create(T... nodes)
{
	CSequence* pInstance = new CSequence();
	auto initializer = { (pInstance->Initialize(nodes),0)... };
	//if (FAILED(pInstance->Initialize(nodes)))
	//{
	//	Safe_Release(pInstance);
	//	MSG_BOX("Failed To Cloned : CSequence");
	//	return nullptr;
	//}
	return pInstance;


}

void CSequence::Free()
{
	for (auto& node : m_nodes)
	{
		Safe_Release(node);
	}
	m_nodes.clear();
}
