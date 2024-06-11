#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class UI_PlayerSkill  : public UI
{

public:
	typedef struct UI_PlayerSkill_DESC : public UI::UI_DESC
	{
		


	}UI_PlayerSkill_DESC;

private:
	UI_PlayerSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	UI_PlayerSkill(const UI_PlayerSkill& rhs);
	virtual ~UI_PlayerSkill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
	void Set_SkillNum(int iSkillNum) { m_iSkillNum = iSkillNum; }
	void Set_SkillCoolTime(_float fCoolTime,_float fMaxCoolTime,_bool IsSkillOn);
private:
	virtual HRESULT Add_Components()override;
	virtual HRESULT Bind_ShaderResources()override;


	

	int       m_iSkillNum = 0;
	_float    m_fCoolTime = 0.f;
	_bool     m_bIsSkillOn = false;
public:
	static UI_PlayerSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END