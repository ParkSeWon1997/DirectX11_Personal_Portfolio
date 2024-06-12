#include "stdafx.h"
#include "..\Public\CTotalSingleton.h"

#include "GameInstance.h"
#include"Loader.h"



IMPLEMENT_SINGLETON(CTotalSingleton)

CTotalSingleton::CTotalSingleton()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
}

CTotalSingleton::CTotalSingleton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{


}


void CTotalSingleton::UpdateElapsedTime()
{
	auto elapsed = GetElapsedTime();
	m_Hours = chrono::duration_cast<chrono::hours>(elapsed).count();
	m_Minutes = chrono::duration_cast<chrono::minutes>(elapsed).count() % 60;
	m_Seconds = chrono::duration_cast<chrono::seconds>(elapsed).count() % 60;
}

void CTotalSingleton::PauseTimer()
{
	
		if (!m_bIsTimerPaused)
		{
			m_PauseTime = chrono::high_resolution_clock::now();
			m_bIsTimerPaused = true;
		}

}

void CTotalSingleton::ResumeTimer()
{
	if (m_bIsTimerPaused)
	{
		auto pauseDuration = chrono::high_resolution_clock::now() - m_PauseTime;
		m_StartTime += pauseDuration;
		m_bIsTimerPaused = false;
	}
}

void CTotalSingleton::ResetTimer()
{
	m_StartTime = chrono::high_resolution_clock::now();
	m_bIsTimerPaused = false;
	m_Hours = 0;
	m_Minutes = 0;
	m_Seconds = 0;
}

void CTotalSingleton::Free()
{
	Safe_Release(m_pGameInstance);
}
