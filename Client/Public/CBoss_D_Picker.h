#pragma once
#include "Monster.h"



BEGIN(Client)
class CBoss_D_Picker final : public CMonster
{
private:
	explicit CBoss_D_Picker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBoss_D_Picker(const CBoss_D_Picker& rhs);
	virtual ~CBoss_D_Picker() = default;

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
	static CBoss_D_Picker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END
