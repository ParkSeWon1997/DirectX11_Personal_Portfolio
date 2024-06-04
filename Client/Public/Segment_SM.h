#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CSegment_SM final : public CGameObject
{
public:
	typedef struct CSegment_SM_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_matrix m_pWorldMatrix = {};
		wstring m_strObjName = L"";
	}CSegment_SM_DESC;

private:
	CSegment_SM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSegment_SM(const CSegment_SM& rhs);
	virtual ~CSegment_SM() = default;

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

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CSegment_SM* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END