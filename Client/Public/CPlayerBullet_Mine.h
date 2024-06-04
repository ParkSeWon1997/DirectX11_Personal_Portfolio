#pragma once
#include"CBullet.h"
#include"Collider.h"
#include"Player.h"


BEGIN(Client)
class CPlayerBullet_Mine final : public CBullet
{
public:
	enum  CPLAYER_BULLET_PARTICLE_TYPE
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
		CPLAYER_BULLET_PARTICLE_TYPE_END


	};



public:
	typedef struct CPlayetBULLET_MINE_DESC : public CBullet::CBullet_DESC
	{
		CCollider::TYPE 		eColliderType = CCollider::TYPE_SPHERE;
		_float                  fRadius = 0.f;

		_float3                vExtents = _float3(0.5f, 0.5f, 0.5f);
		_float3                vRotation = _float3(0.f, 0.f, 0.f);

		_float                  fLifeTime = 0.f;
		_float                  fInitSpeed= 0.f;
		_bool                   bParticleCreate = false;
		_vector                 vTargetPos = {};
		const _float4x4*               pMatPlayerWorld = nullptr;
		CPLAYER_BULLET_PARTICLE_TYPE eParticleType = CPLAYER_BULLET_PARTICLE_TYPE::CPLAYER_BULLET_PARTICLE_TYPE_END;
		CPlayer::CHRACTER_TYPE       eCharacterType = CPlayer::CHRACTER_TYPE::CHRACTER_SWORD_MASTER;
		_float3                vCenter = _float3(0.f, 0.f, 0.f);
	};



private:
	explicit CPlayerBullet_Mine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPlayerBullet_Mine(const CPlayerBullet_Mine& rhs);
	virtual ~CPlayerBullet_Mine() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	virtual HRESULT Add_Components() override;
	virtual HRESULT Bind_ShaderResources()override;

private:

	virtual void Go(_float fTimeDelta) override;
	virtual void Go_TargetPos(_float fTimeDelta) override;
	virtual void ComeBack(_float fTimeDelta) override;
	virtual void Draven_Bullet(_float fTimeDelta)override;
	virtual void Spread(_float fTimeDelta) override;
	virtual void Pop(_float fTimeDelta) override;
	void MakeParticle();

	CCollider::COLLIDER_DESC* MakeColliderDesc(CCollider::TYPE eTpye);

private:
	_float m_fLifeTime = 0.f;
	_float m_fMaxLifeTime = 0.f;
	_float m_fLookAcc = 0.f;
	_float m_fSpeed = 0.f;
	CCollider::TYPE		m_eColliderType = CCollider::TYPE_SPHERE;
	CBounding_Sphere::BOUNDING_SPHERE_DESC		ColliderDescSphre{};
	CBounding_AABB::BOUNDING_AABB_DESC			ColliderDescAABB{};
	CBounding_OBB::BOUNDING_OBB_DESC			ColliderDescOBB{};


	_float									m_fRadius = 0.f;
	_float                                  m_fLastParticleCreateTime = 0.f;
	_float3									m_vExtents = _float3(0.5f, 0.5f, 0.5f);
	_float3									m_vRotation = _float3(0.f, 0.f, 0.f);
	_float3									m_vCenter = _float3(0.f, 0.f, 0.f);
	_vector									m_vTargetPos = {};
	_float                                  m_fInitSpeed = 0.f;
	const _float4x4*						m_pMatPlayerWorld = nullptr;
	_bool									m_bParticleCreate = false;
	CPLAYER_BULLET_PARTICLE_TYPE			m_eParticleType = CPLAYER_BULLET_PARTICLE_TYPE::CPLAYER_BULLET_PARTICLE_TYPE_END;
	CPlayer::CHRACTER_TYPE					m_eCharacterType = CPlayer::CHRACTER_TYPE::CHRACTER_END;
public:
	static CPlayerBullet_Mine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
