
#pragma once

#include "Client_Defines.h"

#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CSwordThow final : public CGameObject
{
public:
	typedef struct CSwordThow_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_matrix m_pWorldMatrix = {};
		_vector vDir = {};
		wstring m_strObjName = L"";
	}CSwordThow_DESC;

private:
	CSwordThow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSwordThow(const CSwordThow& rhs);
	virtual ~CSwordThow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };


	_bool m_bIsDead = false;

	_float m_fLifeTime = 0.f;
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();
	_bool Intersect( const wstring& strComponetTag, CCollider* pTargetCollider);
	void Set_Dead() { m_bIsDead = true; }
public:
	static CSwordThow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END