#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CEnvironmentObj final : public CGameObject
{
public:
	typedef struct CEnvironmentObj_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_matrix m_pWorldMatrix = {};
	}CEnvironmentObj_DESC;

private:
	CEnvironmentObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnvironmentObj(const CEnvironmentObj& rhs);
	virtual ~CEnvironmentObj() = default;

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
	CCollider*						m_pColliderCom[COLLIDER_END] = { nullptr };
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CEnvironmentObj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END