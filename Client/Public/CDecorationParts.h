#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CDecorationParts final : public CPartObject
{
public:
	typedef struct DECORATION_DESC : public CPartObject::PARTOBJ_DESC
	{
		const _float4x4* pCombinedTransformationMatrix;
	}DECORATION_DESC;

private:
	CDecorationParts(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDecorationParts(const CDecorationParts& rhs);
	virtual ~CDecorationParts() = default;

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

private:
	const _float4x4* m_pSocketMatrix = { nullptr };

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDecorationParts* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END