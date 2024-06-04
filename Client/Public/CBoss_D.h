#pragma once
#include"Monster.h"

BEGIN(Client)
class CBoss_D final : public CMonster
{
private:
	explicit CBoss_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBoss_D(const CBoss_D& rhs);
	virtual ~CBoss_D() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Add_Components() override;
public:
	static CBoss_D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
