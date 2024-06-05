#pragma once

#include "Client_Defines.h"
#include "Base.h"



BEGIN(Client)
class CPotalSingleton final: public CBase
{
	DECLARE_SINGLETON(CPotalSingleton)
private:
	CPotalSingleton() {};
	CPotalSingleton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CPotalSingleton()noexcept = default;
public:
	void SetPotalOn(_bool bIsPotalOn) { m_bIsPotalOn = bIsPotalOn; }
	_bool GetPotalOn() { return m_bIsPotalOn; }


private:
	_bool m_bIsPotalOn = false;
	
public:
	
	virtual void Free() override;

};
END
