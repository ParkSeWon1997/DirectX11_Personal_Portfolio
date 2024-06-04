#pragma once
#include"Monster.h"


class CBossMonster abstract: public CMonster
{

protected:
	explicit CBossMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBossMonster(const CBossMonster& rhs);
	virtual ~CBossMonster() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;



};

