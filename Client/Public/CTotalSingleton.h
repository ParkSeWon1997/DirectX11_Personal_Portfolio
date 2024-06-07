#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include"GameInstance.h"

#include <set>
BEGIN(Client)
class CTotalSingleton final: public CBase
{
	DECLARE_SINGLETON(CTotalSingleton)
private:
	CTotalSingleton();
	CTotalSingleton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CTotalSingleton()noexcept = default;
public:
	/*for.Potal */
	void SetPotalOn(_bool bIsPotalOn) { m_bIsPotalOn = bIsPotalOn; }
	_bool GetPotalOn() { return m_bIsPotalOn; }

public:
	/*for.Level */
	void SetVisitLevel(LEVEL eVisitLevel) 
	{
		m_VisitLevel.insert(eVisitLevel);
	}
	_bool GetVisitLevel(LEVEL eVisitLevel)
	{
		if (m_VisitLevel.find(eVisitLevel) != m_VisitLevel.end())
			return true;
		return false;
	}
	


private:
	class CGameInstance* m_pGameInstance = nullptr;

private:
	/*for.Potal */
	_bool m_bIsPotalOn = false;

private:
	/*for.Level */
	set<LEVEL> m_VisitLevel;
	
public:
	
	virtual void Free() override;

};
END
