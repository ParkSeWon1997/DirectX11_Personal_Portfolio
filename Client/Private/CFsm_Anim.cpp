#include "stdafx.h"
#include "CFsm_Anim.h"

#include"GameInstance.h"


CFsm_Anim::CFsm_Anim()
{
	
}
HRESULT CFsm_Anim::Initialize(int AnimIndex)
{
	m_vecStateFunction.reserve(AnimIndex);

	return S_OK;
}



void CFsm_Anim::BindFunction(function<void(void)> func)
{
	if(func)
		m_vecStateFunction.push_back((STATE_ANIMFUNCTION)func);
	else
		assert(false);

}

void CFsm_Anim::SetAnimations(STATE_ANIMFUNCTION func)
{
	//���� �Ű������� �Լ������Ϳ� Ÿ���� ������ üũ
	//į���� Ÿ�ӿ� �߷а���
	//#include<type_traits>�־�� ��
	if constexpr (std::is_same_v<STATE_ANIMFUNCTION, decltype(func)>)
	{
		m_vecStateFunction.push_back(func);

	}
	else
	{
		assert(false);
	}
}

void CFsm_Anim::SetState(int iAnimIndex)
{
	this->iAnimIndex = iAnimIndex;

}
void CFsm_Anim::Update()
{
	if(false==bIsAnim_runing)	//���� �ִϸ��̼��� �������� �ƴ� ���� ���� �ִϸ��̼��� ���� ����
		return(this->m_vecStateFunction[iAnimIndex])();

	
}

CFsm_Anim* CFsm_Anim::Create( int AnimIndex)
{
	CFsm_Anim* pInstance = new CFsm_Anim();
	if (FAILED(pInstance->Initialize(AnimIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;

	
}

void CFsm_Anim::Free()
{
	m_vecStateFunction.clear();
}
