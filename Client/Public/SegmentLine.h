#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CSegmentLine final : public CGameObject
{
public:
	typedef struct CSegmentLine_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_matrix m_pWorldMatrix = {};
		wstring m_strObjName = L"";
	}CSegmentLine_DESC;

private:
	CSegmentLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSegmentLine(const CSegmentLine& rhs);
	virtual ~CSegmentLine() = default;

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
	static CSegmentLine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END