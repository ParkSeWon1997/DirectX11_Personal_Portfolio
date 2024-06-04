#pragma once
#include"CBullet.h"

class CNoneTexBullet final: public CBullet
{

private:
	CNoneTexBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNoneTexBullet(const CBullet& rhs);
	virtual ~CNoneTexBullet() = default;
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
	static CNoneTexBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;




};

