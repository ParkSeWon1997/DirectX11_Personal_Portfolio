#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class UI  : public CGameObject
{
public:
	typedef struct UI_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		_float            fX, fY, fSizeX, fSizeY;
		_uint             iPassIndex = 0;


	}UI_DESC;
protected:
	UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	UI(const UI& rhs);
	virtual ~UI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	

public:
	void Set_IsSelect(int iIsSelect) { m_iIsSelect = iIsSelect; }
	void Set_Dead(bool bIsDead) { m_bIsDead = bIsDead; }


	void Set_Hp(float fHp) { m_fHp = fHp; }
	void Set_MaxHp(float fMaxHp) { m_fMaxHp = fMaxHp; }
protected:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };	
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;


	_uint							m_iPassIndex = 0;
	_float                          m_fHp = 0.0f;
	_float                         m_fMaxHp = 0.0f;
	int 						   m_iIsSelect = 0;
	_bool                           m_bIsDead = false;

private:
	_tchar					m_szCoin[MAX_PATH] = TEXT("");
private:
	virtual HRESULT Add_Components();
	virtual HRESULT Bind_ShaderResources();



public:
	static UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END