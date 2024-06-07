#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class Fade_In_Out final : public CGameObject
{
public:
	typedef struct Fade_In_Out_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		_float            fX, fY, fSizeX, fSizeY;


	}Fade_In_Out_DESC;
private:
	Fade_In_Out(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Fade_In_Out(const Fade_In_Out& rhs);
	virtual ~Fade_In_Out() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Start_FadeIn() {
		//if(m_fCurrentTime == 0.f)
		m_bIsFadeStart = true;
	};
	_bool IsFade() {
		return m_bIsFadeEnd;
	};
	void Set_FadeDuration(float fDuration) {
		m_fFadeDuration = fDuration;
	};
private:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };	
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;


	_bool  m_bIsFadeStart = false;
	_bool  m_bIsFadeEnd = false;
	_bool m_bIsFade = false;
	float m_fFadeDuration = 0.1f;
	float m_fFadeStartTime = 0.f;
	float m_fCurrentTime= 0.f;


private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static Fade_In_Out* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END