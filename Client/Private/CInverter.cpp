#include"stdafx.h"
#include "CInverter.h"

template<typename T>
CInverter::CInverter(T node)
{
	m_Node = node;
}

HRESULT CInverter::Initialize()
{
    return S_OK;
}

//NodeStates CInverter::Evaluate()
//{
//	switch (m_Node->Evaluate())
//	{
//		// 자식노드의 반대로 리턴
//	case NodeStates::FAILURE:
//		m_eNodeState = NodeStates::SUCCESS;
//		return m_eNodeState;
//	case NodeStates::SUCCESS:
//		m_eNodeState = NodeStates::FAILURE;
//		return m_eNodeState;
//	case NodeStates::RUNNING:
//		m_eNodeState = NodeStates::RUNNING;
//		return m_eNodeState;
//	}
//	m_eNodeState= NodeStates::SUCCESS;
//	return m_eNodeState;
//
//}


NodeStates CInverter::Evaluate()
{
	switch (m_Node->Evaluate())
	{
		// 자식노드의 반대로 리턴
	case NodeStates::FAILURE:
		m_eNodeState = NodeStates::SUCCESS;
		return m_eNodeState;
	case NodeStates::SUCCESS:
		m_eNodeState = NodeStates::FAILURE;
		return m_eNodeState;
	case NodeStates::RUNNING:
		m_eNodeState = NodeStates::RUNNING;
		return m_eNodeState;
	}
	m_eNodeState = NodeStates::SUCCESS;
	return m_eNodeState;
}

NodeStates CInverter::Evaluate(_float fTimeDelta)
{
	switch (m_Node->Evaluate(fTimeDelta))
	{
		// 자식노드의 반대로 리턴
	case NodeStates::FAILURE:
		m_eNodeState = NodeStates::SUCCESS;
		return m_eNodeState;
	case NodeStates::SUCCESS:
		m_eNodeState = NodeStates::FAILURE;
		return m_eNodeState;
	case NodeStates::RUNNING:
		m_eNodeState = NodeStates::RUNNING;
		return m_eNodeState;
	}
	m_eNodeState = NodeStates::SUCCESS;
	return m_eNodeState;
}

template<typename T>
CInverter* CInverter::Create(T node)
{
	CInverter* pInstance = new CInverter(node);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed To Cloned : CInverter");
		return nullptr;
	}
	return pInstance;

}


void CInverter::Free()
{
	Safe_Release(m_Node);
}
