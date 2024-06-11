#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CItem final : public CGameObject
{
public:
	typedef struct CItem_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_vector vDirection = {};
		_vector vTargetPos = {};
	}CItem_DESC;

private:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	

private:
	

private:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CCollider*						m_pColliderCom = { nullptr };


	_vector  				   m_vDirection = {};
	_vector					   m_vTargetPos = {};

	_bool 						m_bIsDead = false;
	_bool						bClose = false;
	_float                     m_fLifeTime = 0.0f;

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END