#pragma once
#include"CBullet.h"



class CMold_CBullet final : public CBullet
{
private:
		explicit CMold_CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		explicit CMold_CBullet(const CMold_CBullet& rhs);
		virtual ~CMold_CBullet() = default;


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

	virtual void Go(_float fTimeDelta) override;

private:
	_float m_fLifeTime = 0.f;
public:
	static CMold_CBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

