#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
class CTexture;
class CShader;
class CModel;
END

BEGIN(Client)

class CMonster abstract: public CGameObject
{


public:
	typedef struct CMonster_DESC : CGameObject::GAMEOBJECT_DESC
	{
		wstring strDeconModelTag;
		_matrix m_pWorldMatrix = {};
		_float fHp = 0.f;
	}CMonster_DESC;


protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;




protected:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CCollider*						m_pColliderCom[COLLIDER_END] = {nullptr,nullptr ,nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CNavigation*					m_pNavigationCom = { nullptr };
	const _float4x4*				m_pParentMatrix= nullptr;
	const _float4x4*				m_vpSocketMatrix = nullptr;
	_uint							m_iNumSocketIndex = 0;

	_bool							m_bIsDead = false;
	_bool							m_bIsHit = false;
	_bool							m_bIsDetact = false;
	_float							m_fHp = 0.f;
	_float							m_fHpMax = 0.f;
	_float                          m_fAnimSpeed = 1.f;

	CPlayer*						m_pPlayer = nullptr;



	_float                         m_fDisolveTime = 0.f;
	_float                         m_fDisolveSpeed = 0.5f;
protected:
	wstring 						m_strDeconModelTag = L"";
public:
	virtual HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	//static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END