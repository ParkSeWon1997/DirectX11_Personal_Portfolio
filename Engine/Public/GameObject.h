#pragma once

#include "Transform.h"

BEGIN(Engine)
//s
class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDER_END };
public:
	typedef struct GAMEOBJECT_DESC : public CTransform::TRANSFORM_DESC
	{
		_float3 vScale= {1.0f,1.0f,1.0f};
		_float3 vRotation= {0,0,0};
		_float  vRotationAngle= 0;
		_float4 vPosition= {0,0,0,1.0f};
		wstring strModelName = L"";
	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;


public:
	/*  지정한 이름의 컴포넌트를 가져온다. */
	class CComponent* Get_Component(const wstring& strComponentTag);


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Tick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_LightDepth() { return S_OK; }

	float Get_ObjID() {return m_fObjOwnderID;}
	_bool IsDead() { return this->m_bIsDead; }

	_float3 Get_Scaled() {return m_pTransformCom->Get_Scaled();}
	_float3 Get_Position() {
		_float3		fPosition = {};
		XMStoreFloat3(&fPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		return fPosition;
	}
	_vector Get_PositionVector() {
		return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	}


	void Set_Scaled(_float3 fScale);
	void Set_Position(_float3 fPosition);
	void Set_Rotation(_float fRotationAngle,_float3 fAxis);
	void Set_Rotation(_float fRotationAngle);
	void Set_Dead() { this->m_bIsDead = true; }

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };




protected:
	class CGameInstance*		m_pGameInstance = { nullptr };
	class CTransform*			m_pTransformCom = { nullptr };
	static const _tchar*		m_pTransformTag;
	wstring						m_strModelName = L"";
	_bool						m_bIsDead = false;
protected:
	map<const wstring, class CComponent*>		m_Components;

protected:
	float m_fObjOwnderID = 0;
	float m_fTime = 0;
	//

private:
	static float g_fObjID_count;

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, 
		CComponent** ppOut, void* pArg = nullptr);
	HRESULT Change_Component(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);



public:	
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END