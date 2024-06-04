#include"stdafx.h"
#include "CSelector.h"

//template<typename T>
CSelector::CSelector()
   // : m_nodes(nodes)
{
}

template<typename T>
HRESULT CSelector::Initialize(T arg)
{
	m_nodes.emplace_back(arg);
	

	return S_OK;
}


NodeStates CSelector::Evaluate()
{
	for (auto& node : m_nodes)
	{
		switch (node->Evaluate())
		{
		case NodeStates::FAILURE:
			continue;
		case NodeStates::SUCCESS:
			m_eNodeState = NodeStates::SUCCESS;
			return m_eNodeState;             //자식 노드들 중 하나라도 ture이면 Selector도 true를 리턴. 그리고 바로 종료

		case NodeStates::RUNNING:
			m_eNodeState = NodeStates::RUNNING;
			return m_eNodeState;
		default:
			continue;
		}

	}
	m_eNodeState = NodeStates::FAILURE;
	return m_eNodeState;

}

NodeStates CSelector::Evaluate(_float fTimeDelta)
{
	for (auto& node : m_nodes)
	{
		switch (node->Evaluate(fTimeDelta))
		{
		case NodeStates::FAILURE:
			continue;
		case NodeStates::SUCCESS:
			m_eNodeState = NodeStates::SUCCESS;
			return m_eNodeState;             //자식 노드들 중 하나라도 ture이면 Selector도 true를 리턴. 그리고 바로 종료

		case NodeStates::RUNNING:
			m_eNodeState = NodeStates::RUNNING;
			return m_eNodeState;
		default:
			continue;
		}

	}
	m_eNodeState = NodeStates::FAILURE;
	return m_eNodeState;
}

template<typename... T>
CSelector* CSelector::Create(T... nodes)
{
	CSelector* pInstance = new CSelector();
	auto initializer= {(pInstance->Initialize(nodes),0)...};
	//if (FAILED(pInstance->Initialize(nodes)))
	//{
	//	Safe_Release(pInstance);
	//	MSG_BOX("Failed To Cloned : CSelector");
	//	return nullptr;
	//}
	return pInstance;


}

void CSelector::Free()
{
	for(auto& node : m_nodes)
	{
		Safe_Release(node);
	}
	m_nodes.clear();
}
