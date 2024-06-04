#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CWeapon final : public CPartObject
{
public:
	enum class WEAPON_PARTICLE_TYPE
	{
		BALANCE_X,
		BALANCE_C,
		BALANCE_XC,
		TECHNIC_X,
		TECHNIC_C,
		TECHNIC_XC,
		POWER_X,
		POWER_C,
		POWER_XC,
		REVERSE_X,
		REVERSE_C,
		REVERSE_XC,
		WEAPON_PARTICLE_TYPE_END


	};



public:
	typedef struct WEAPON_DESC : public CPartObject::PARTOBJ_DESC
	{
		class CGameObject* pPlayer = nullptr;
		const _float4x4*			pCombinedTransformationMatrix;
	}WEAPON_DESC;

private:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

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
	void SetParticleCreate(_bool bCreate, WEAPON_PARTICLE_TYPE eParticleType)
	{
		m_bParticleCreate = bCreate;
		m_eParticleType = eParticleType;
	}

private:
	void MakeParticle();
private:
	CCollider*						m_pColliderCom = { nullptr };
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };

private:
	class CGameObject* m_pPlayer = nullptr;
	const _float4x4*				m_pSocketMatrix = { nullptr };
	_bool							m_bParticleCreate= false;
	WEAPON_PARTICLE_TYPE           m_eParticleType = WEAPON_PARTICLE_TYPE::WEAPON_PARTICLE_TYPE_END;
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END