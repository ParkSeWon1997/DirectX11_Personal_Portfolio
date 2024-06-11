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
	void SetPlayerAbillty_AddHp(_float fHp)
	{ 
		if(m_PlayerAbility.fHp + fHp > m_PlayerAbility.fMaxHp)
			m_PlayerAbility.fHp = m_PlayerAbility.fMaxHp;
		else
			m_PlayerAbility.fHp += fHp;
	}
	CPlayer::Player_Abililty GetPlayerAbility() { return m_PlayerAbility; }


	void AddCoin(int fCoin) { m_fCoin += fCoin; }
	int GetCoin() { return m_fCoin; }

	void SetAnimSpeed(_float fAnimSpeed) { m_fAnimSpeed += fAnimSpeed; }
	_float GetAnimSpeed() { return m_fAnimSpeed; }

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
	int                 m_fCoin = 0.f;

	_float              m_fAnimSpeed = 1.f;
private:
	/*for.Ui_open*/
	_bool m_bIsUiOpen = false;
public:
	
	virtual void Free() override;

};
END
