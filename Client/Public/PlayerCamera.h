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

private:
	_float		m_fSensor = { 0.0f };


private:
	static bool bNoClick;
	const _float4x4* m_pParentMatrix = { nullptr };
public:
	static CPlayerCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END