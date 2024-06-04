#pragma once

#include "Component.h"

/* 객체의 월드 변환 행렬을 보관한다. */
/* 월드 변환 상태를 제어하기위한 기능. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct TRANSFORM_DESC
	{
		
	
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float3 Get_Scaled() {
		return _float3(XMVector3Length(Get_State(STATE_RIGHT)).m128_f32[0], 
			XMVector3Length(Get_State(STATE_UP)).m128_f32[0],
			XMVector3Length(Get_State(STATE_LOOK)).m128_f32[0]);
	}

	const _float4x4* Get_WorldFloat4x4() {
		return &m_WorldMatrix;
	}
	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}
	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

public:
	void Set_State(STATE eState, _fvector vState);
	void Set_Look(_fvector vLook);
	void Set_Up(_fvector vUp);
	void Set_Right(_fvector vRight);
	void Set_RotationSpeed(_float fRotationPerSec) {
		m_fRotationPerSec = XMConvertToRadians( fRotationPerSec);
	}
	void Set_Identity();
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

public:
	void Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);
	void Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);
	HRESULT Set_WorldMatrix(_fmatrix matWorld);
	HRESULT Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT Go_RandomStraight(_float fTimeDelta, _float fRadian,class CNavigation* pNavigation = nullptr);
	HRESULT Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT Go_Teleport(_vector vPosition, class CNavigation* pNavigation = nullptr);
	HRESULT Go_Up(_float fTimeDelta);
	HRESULT Go_Down(_float fTimeDelta);
	HRESULT Go_Jump(_float fTimeDelta,_float JumpPower,_float JumpHeight,_bool& Isfloor);
	HRESULT Up_And_Down(_float fTimeDelta, _float fInitialSpeed, _float fAngle, _float fElapsedTime);
	HRESULT Go_Left(_float fTimeDelta);
	HRESULT Go_Right(_float fTimeDelta);
	HRESULT Move_Look(_float fDistance);
	HRESULT LookAt(_fvector vTargetPosition);
	HRESULT LookAt_For_LandObject(_fvector vTargetPosition);
	HRESULT Turn(_fvector vAxis, _float fTimeDelta);
	HRESULT Rotation(_fvector vAxis, _float fRadian);
	HRESULT RotationUp_And_Go(_float fRadian, _float fTimeDelta);
	HRESULT Rotation( _float fRadian);
	HRESULT QuatRotation(_fvector vAxis, _float fRadian);
	HRESULT QuatRotation(_float3 vAxis);
	_bool	TurnToTarget(_float fTimeDelta, _fvector vTargetPosition);


private:
	/* row major */
	_float4x4				m_WorldMatrix;
	_float					m_fSpeedPerSec = { 0.0f };
	_float					m_fRotationPerSec = { 0.0f };



	
	_float					fScala={1.f};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END