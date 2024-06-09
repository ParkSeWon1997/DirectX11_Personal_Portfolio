#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class UI_Changer  : public UI
{

public:
	typedef struct UI_Changer_DESC : public UI::UI_DESC
	{
		int iChangerType = 0;


	}UI_Changer_DESC;

private:
	UI_Changer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	UI_Changer(const UI_Changer& rhs);
	virtual ~UI_Changer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_IsOpen(_bool bIsOpen) { b_bIsOpen = bIsOpen; }
	void Set_ChangerType(int iType) { i_ChreaterType = iType; }
	
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

	void    Make_Changer_Ui();
private:
	_bool    b_bIsOpen = false;
	_bool    b_bIsChoose = false;
	int      i_ChreaterType = 0;
	_bool   b_IsCreate = false;

	int    m_iUiSwordIndex = 1;
	int    m_iUiGunIndex = 1;


	_bool m_bIsDead = false;
public:
	static UI_Changer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END