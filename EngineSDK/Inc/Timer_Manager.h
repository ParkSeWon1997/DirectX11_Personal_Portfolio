#pragma once
#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class CTimer_Manager : public CBase
{	
private:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	_float		Get_TimeDelta(const _tchar* pTimerTag);
	_float Get_FPS(const _tchar* pTimerTag) {
		CTimer* pTimer = Find_Timer(pTimerTag);
		if (nullptr == pTimer)
			return 0.f;

		return pTimer->Get_FPS();
	}
	void		Update_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT		Ready_Timer(const _tchar* pTimerTag);

private:
	class CTimer*		Find_Timer(const _tchar* pTimerTag);

private:
	map<const _tchar*, class CTimer*>		m_mapTimers;

public:
	static CTimer_Manager* Create();
	virtual void		Free(void);
};

END