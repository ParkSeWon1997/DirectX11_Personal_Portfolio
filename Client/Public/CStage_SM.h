#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CStage_SM final : public CGameObject
{
public:

	typedef struct CStage_SM_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_matrix m_pWorldMatrix = {};
	}CStage_SM_DESC;

private:
	CStage_SM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStage_SM(const CStage_SM& rhs);
	virtual ~CStage_SM() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CStage_SM* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END