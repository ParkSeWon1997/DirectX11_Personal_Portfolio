
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CBullet  : public CGameObject
{

public:
	typedef void (CBullet::* HAND_BULLET_STATE)(_float fTimeDelta);




public:
	typedef struct CBullet_DESC : CGameObject::GAMEOBJECT_DESC
	{
		_matrix m_pWorldMatrix = {};
		_vector vDir = {};
		_float fDamage = 0.f;
		wstring m_strObjName = L"";
		_float fRotatedAngle = 0.f;
		_vector vTargetPos;
		HAND_BULLET_STATE BulletState;
	}CBullet_DESC;

protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_bool Intersect( const wstring& strComponetTag, CCollider* pTargetCollider);
	_float Get_Damage() { return m_fDamage; }

	void Set_IsCollision(_bool bIsCollision) { m_bIsCollision = bIsCollision; }
	_bool Get_IsCollision() { return m_bIsCollision; }
public:
	virtual void Falling(_float fTimeDelta);
	virtual void Spread(_float fTimeDelta);
	virtual void Go(_float fTimeDelta);
	virtual void Go_TargetPos(_float fTimeDelta);
	virtual void Draven_Bullet(_float fTimeDelta);
	virtual void Pop(_float fTimeDelta);
	virtual void ComeBack(_float fTimeDelta);

protected:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };


	_vector             m_vTargetPos;

	_float				m_fElapsedTime = 0.f;
	_float              m_fDamage = 0.f;
	_bool               m_bIsCollision = false;
	HAND_BULLET_STATE	m_pBulletAction;
	
public:
	virtual HRESULT Add_Components();
	virtual HRESULT Bind_ShaderResources();
	
public:
	static CBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END