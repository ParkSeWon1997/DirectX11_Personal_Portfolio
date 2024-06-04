#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"
#include"VIBuffer_Instance.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)

class CParticle_Mesh final : public CGameObject
{
public:
	enum  PARTICLE_TYPE
	{
		PARTICLE_TYPE_NONE = 0,
		PARTICLE_TYPE_SPREAD,
		PARTICLE_TYPE_DROP,
		PARTICLE_TYPE_UP_AND_DOWN,
		PARTICLE_TYPE_EXPLOSION,
		PARTICLE_TYPE_POP,
		PARTICLE_TYPE_RANDOM_POP,
		PARTICLE_TYPE_SIZE_UP,
		PARTICLE_TYPE_SIZE_DOWN,
		PARTICLE_TYPE_SIZE_DOWN_X,
		PARTICLE_TYPE_SIZE_DOWN_Y,
		PARTICLE_TYPE_SIZE_DOWN_Z,
		PARTICLE_TYPE_SIZE_UP_X,
		PARTICLE_TYPE_SIZE_UP_Y,
		PARTICLE_TYPE_SIZE_UP_Z,
		PARTICLE_TYPE_MAX,
		PARTICLE_TYPE_ROTATION_X,
		PARTICLE_TYPE_ROTATION_Y,
		PARTICLE_TYPE_ROTATION_Z,
		PARTICLE_TYPE_END

	};
public:
	typedef struct PARTICLE_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		_bool           bIsRandome = false;
		_float4			vStartPos;
	}PARTICLE_DESC;
private:
	CParticle_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Mesh(const CParticle_Mesh& rhs);
	virtual ~CParticle_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_ParticleType(const PARTICLE_TYPE eParticleType);
	void Set_ParticleProperty(const CVIBuffer_Instance::INSTANCE_DESC& ParticleDesc);
	const CVIBuffer_Instance::INSTANCE_DESC Get_InstanceBufferDesc();

private:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };	
	CModel*							m_pModelCom = { nullptr };

	PARTICLE_TYPE					m_eParticleType = PARTICLE_TYPE::PARTICLE_TYPE_END;

private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CParticle_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END