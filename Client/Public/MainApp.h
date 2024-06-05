#pragma once

#include "Client_Defines.h"
#include "Base.h"


class CPotalSingleton;

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CGameInstance*			m_pGameInstance = { nullptr };
	CPotalSingleton*		m_pPotalSingleton = { nullptr };
public:
	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Navigation();
	HRESULT Ready_Fonts();
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Prototype_Component();


public:
	static CMainApp* Create();
	virtual void Free() override;
};

END