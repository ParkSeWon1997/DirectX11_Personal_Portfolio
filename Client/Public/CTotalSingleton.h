#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include"GameInstance.h"
#include "Player.h"

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
	
public:
	/*for.Player */
	void SetPlayerAbility(CPlayer::Player_Abililty PlayerAbility) { m_PlayerAbility = PlayerAbility; }
	CPlayer::Player_Abililty GetPlayerAbility() { return m_PlayerAbility; }






public:
	/*for.Ui_open*/
	void SetUiOpen(_bool bIsUiOpen) { m_bIsUiOpen = bIsUiOpen; }
	_bool GetUiOpen() { return m_bIsUiOpen; }










private:
	class CGameInstance* m_pGameInstance = nullptr;

private:
	/*for.Potal */
	_bool m_bIsPotalOn = false;

private:
	/*for.Level */
	set<LEVEL> m_VisitLevel;
	
private:
	/*for.Player */
	CPlayer::Player_Abililty m_PlayerAbility;

private:
	/*for.Ui_open*/
	_bool m_bIsUiOpen = false;
public:
	
	virtual void Free() override;

};
END
