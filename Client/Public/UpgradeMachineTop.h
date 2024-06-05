#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CUpgradeMachineTop final : public CGameObject
{
public:

public:

private:
	CUpgradeMachineTop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUpgradeMachineTop(const CUpgradeMachineTop& rhs);
	virtual ~CUpgradeMachineTop() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;


public:
	void Set_Activated(_bool bActivated) { m_bIsActivated = bActivated; }
	_bool Get_Activated() { return m_bIsActivated; }
private:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };




private:
	_bool							m_bIsActivated = false;
	_float							m_fSpeed = 50.0f;
	_bool							m_bIsDown = false;
	_float							m_fUpAcc = 0.0f;
	_float                          m_fTargetDownPos_Y = 3.0f;
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUpgradeMachineTop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END