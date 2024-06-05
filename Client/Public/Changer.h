#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CChanger final : public CGameObject
{
public:
	typedef void (CChanger::* Chnager_Func)(_float fTimeDelta);

public:
	typedef struct CChanger_DESC : CGameObject::GAMEOBJECT_DESC
	{

		Chnager_Func ChangerState;
		CGameObject* pChanger = nullptr;

	}CChanger_DESC;



private:
	CChanger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChanger(const CChanger& rhs);
	virtual ~CChanger() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	


	void   Rotate(_float fTimeDelta);
	void   Up_And_Down(_float fTimeDelta);
private:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CCollider*						m_pColliderCom = { nullptr };


	_float							m_fUpAcc = 0.0f;
	_float							m_fSpeed = 50.0f;
	_float                          m_fTargetDownPos_Y = 0.0f;
	Chnager_Func					m_ChangerState = nullptr;
	CGameObject*					m_pChanger = nullptr;
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CChanger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END