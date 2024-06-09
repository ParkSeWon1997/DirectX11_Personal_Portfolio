#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CPlayerCamera final : public CCamera
{
public:
	typedef struct Player_CAMERA_DESC : public CCamera::CAMERA_DESC
	{
		const _float4x4* pParentMatrix;
		_float		fSensor = { 0.f };
		_float      fAngle = { 0.f };
		_vector     vTargetPos = {};
	}Player_CAMERA_DESC;
private:
	CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerCamera(const CPlayerCamera& rhs);
	virtual ~CPlayerCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


	void Set_Shake(_bool fShake,_float fShakeTime) {
		m_bShake = fShake; 
		m_fShakeMaxTime = fShakeTime;
	}
	void Set_TargetPos(_vector vTargetPos) {
		m_vTargetPos = vTargetPos;
	}

private:
	_float		m_fSensor = { 0.0f };
	_float      m_fShakeTime = 0.0f;
	_float      m_fShakeMaxTime = 0.3f;

	_bool       m_bShake = false;

	
	_vector     m_vTargetPos = {};
private:
	static bool bNoClick;
	const _float4x4* m_pParentMatrix = { nullptr };
public:
	static CPlayerCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END