#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class UI_PlayerIcon  : public UI
{

public:
	typedef struct UI_PlayerIcon_DESC : public UI::UI_DESC
	{
		


	}UI_PlayerIcon_DESC;

private:
	UI_PlayerIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	UI_PlayerIcon(const UI_PlayerIcon& rhs);
	virtual ~UI_PlayerIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	

	void Set_TexIndex(int iIndex) { m_iTexIndex = iIndex; }
private:
	virtual HRESULT Add_Components()override;
	virtual HRESULT Bind_ShaderResources()override;



	int m_iTexIndex = 0;
public:
	static UI_PlayerIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END