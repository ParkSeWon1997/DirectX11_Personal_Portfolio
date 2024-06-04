#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	_vector Picking(_Out_ _bool* isSuccess);
	_float  PickID(_Out_ _bool* isSuccess);
	_float  PickMaskMap(const  _float3& vPos);

private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };
	_uint							m_iWinSizeX, m_iWinSizeY;

	class CGameInstance*			m_pGameInstance = { nullptr };
	HWND							m_hWnd = {};

	ID3D11Texture2D* m_pTexture2D = { nullptr };

public:
	static CPicking * Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HWND hWnd);
	virtual void Free() override;
};

END