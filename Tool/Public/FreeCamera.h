#pragma once

#include "Tool_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CFreeCamera final : public CCamera
{
public:
	typedef struct FREE_CAMERA_DESC : public CCamera::CAMERA_DESC
	{
		_float		fSensor = { 0.f };
	};
private:
	CFreeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFreeCamera(const CFreeCamera& rhs);
	virtual ~CFreeCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float		m_fSensor = { 0.0f };
	static bool bNoClick;

public:
	static CFreeCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END