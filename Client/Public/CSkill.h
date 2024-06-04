#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

END

/* 플레이어를 구성하는 객체들을 들고 있는 객체이다. */



BEGIN(Client)
class CSkill abstract : public CGameObject
{
private:
	CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill(const CSkill& rhs);
	virtual ~CSkill() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _bool IsUse(_float fElapsedTime) const {
		return fElapsedTime>= m_CoolTime;
	}
	virtual void UseSkill() = 0;

protected:
	_float m_CoolTime;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
