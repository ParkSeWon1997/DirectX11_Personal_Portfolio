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
	//들어온 매개변수가 함수포인터와 타입이 같은지 체크
	//캄파일 타임에 추론가능
	//#include<type_traits>있어야 함
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
	if(false==bIsAnim_runing)	//현재 애니메이션이 실행중이 아닐 때만 다음 애니메이션이 실행 가능
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
