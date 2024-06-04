#include"stdafx.h"
#include "CBehavior.h"

#include"CActionNode.h"
#include"CSelector.h"
#include"CSequence.h"
#include"CInverter.h"





CBehavior::CBehavior()
{
}

HRESULT CBehavior::Initialize()
{
	


	


    return S_OK;
}

void CBehavior::Update(CPlayer::STATE NowState)
{
	NodeStates state = NodeStates::FAILURE;;
	m_eState = NowState;
	if (nullptr != m_pRootNode)
	{
		state = m_pRootNode->Evaluate();
	}
}

NodeStates CBehavior::DoIdle()
{

	return NodeStates::SUCCESS;
}

CBehavior* CBehavior::Create()
{
	CBehavior* pInstance = new CBehavior();
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed To Cloned : CBehavior");
		return nullptr;
	}
	return pInstance;


}

void CBehavior::Free()
{
	Safe_Release(m_pSelector);
	Safe_Release(m_pIdle);
	Safe_Release(m_pRootNode);
}
