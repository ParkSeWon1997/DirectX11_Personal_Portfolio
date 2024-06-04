#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CForkLift final : public CGameObject
{
public:
	typedef struct CForkLift_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_float3 fScale;
		_float3 fRotateAxis;
		_float fRotateAngle;
		_float4 fPosition;
		wstring m_strObjName = L"";
	}CForkLift_DESC;
private:
	CForkLift(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CForkLift(const CForkLift& rhs);
	virtual ~CForkLift() = default;

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
	CTexture*						m_pTextureCom = nullptr;

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CForkLift* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END