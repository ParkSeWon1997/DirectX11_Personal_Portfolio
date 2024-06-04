

#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include"Player.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CDrone final : public CGameObject
{

public:
	typedef struct CDrone_DESC : CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4*    pPlayerWorldMatrix = nullptr;
		_vector				vDirLook = {};
		_float              fCurAngle = 0.f;
		CPlayer::CHRACTER_TYPE eType = CPlayer::CHRACTER_TYPE::CHRACTER_END;
	}CDrone_DESC;

private:
	CDrone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDrone(const CDrone& rhs);
	virtual ~CDrone() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetBoxColliderSize(_float3 vScale)
	{
		m_pColliderCom->SetBoxSize(vScale);
	}
private:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	
	const _float4x4*   m_pPlayerWorldMatrix = nullptr;

	_float              m_fOrbitRadius = 0.f;
	_float              m_fOrbitSpeed = 0.f;
	_float              m_fLifeTime = 0.f;
	_float             m_fCurrentAngle = 0.f;
	_float             m_fElapsedTime = 0.f;
	_float             m_fMakeParticleTime = 0.f;
	_bool              m_bIsDead = false;
	CPlayer::CHRACTER_TYPE m_eChracterType = CPlayer::CHRACTER_TYPE::CHRACTER_END;
private:
	void Orbit(_float fTimeDelta);
	void Make_Particle(_float fTimeDelta);
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDrone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END