#pragma once
#include"CBullet.h"

class CHandBullet final: public CBullet
{

private:
	CHandBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHandBullet(const CBullet& rhs);
	virtual ~CHandBullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;



private:
	virtual HRESULT Add_Components() override;
	virtual HRESULT Bind_ShaderResources()override;

private:




	//_matrix m_
public:
	static CHandBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;




};

