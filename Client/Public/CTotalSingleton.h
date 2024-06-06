#pragma once

#include "Client_Defines.h"
#include "Base.h"



BEGIN(Client)
class CTotalSingleton final: public CBase
{
	DECLARE_SINGLETON(CTotalSingleton)
private:
	CTotalSingleton() {};
	CTotalSingleton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CTotalSingleton()noexcept = default;
public:
	void SetPotalOn(_bool bIsPotalOn) { m_bIsPotalOn = bIsPotalOn; }
	_bool GetPotalOn() { return m_bIsPotalOn; }


private:
	_bool m_bIsPotalOn = false;
	
public:
	
	virtual void Free() override;

};
END
