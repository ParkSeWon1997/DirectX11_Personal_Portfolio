#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CLandObject abstract : public CGameObject
{
protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Compute_Height();

public:	
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END