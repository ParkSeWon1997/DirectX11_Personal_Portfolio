#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CRank final : public CGameObject
{

private:
	CRank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRank(const CRank& rhs);
	virtual ~CRank() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	

private:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CCollider*						m_pColliderCom = { nullptr };


	_float							m_fUpAcc = 0.0f;
	_float							m_fSpeed = 50.0f;
	_float                          m_fTargetDownPos_Y = 0.0f;


public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CRank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END