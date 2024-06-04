#include "..\Public\Picking.h"

#include "GameInstance.h"
#include "PipeLine.h"

CPicking::CPicking(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	_uint		iNumViews = { 1 };

	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViews, &ViewportDesc);

	m_iWinSizeX = ViewportDesc.Width;
	m_iWinSizeY = ViewportDesc.Height;

	D3D11_TEXTURE2D_DESC		TextureDesc{};

	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = m_iWinSizeX;
	TextureDesc.Height = m_iWinSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_STAGING;
	TextureDesc.BindFlags = 0;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	return S_OK;
}

_vector CPicking::Picking(_Out_ _bool* isSuccess)
{
	*isSuccess = true;

	POINT		ptMouse;

	GetCursorPos(&ptMouse);

	ScreenToClient(m_hWnd, &ptMouse);

	if (ptMouse.x < 0)
		ptMouse.x = 0;
	if (ptMouse.x >= m_iWinSizeX)
		ptMouse.x = m_iWinSizeX;
	if (ptMouse.y < 0)
		ptMouse.y = 0;
	if (ptMouse.y >= m_iWinSizeY)
		ptMouse.y = m_iWinSizeY;

	_vector		vMousePos = XMVectorZero();

	/* 투영공간상의 좌표다. = 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
	vMousePos = XMVectorSetX(vMousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	vMousePos = XMVectorSetY(vMousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);
	vMousePos = XMVectorSetZ(vMousePos, m_pGameInstance->Compute_ProjZ(ptMouse));
	vMousePos = XMVectorSetW(vMousePos, 1.f);

	if (XMVectorGetZ(vMousePos) < 0.0f)
		*isSuccess = false;

	


	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ));
	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW));

	return vMousePos;
}

_float CPicking::PickID(_Out_ _bool* isSuccess)
{
	*isSuccess = true;
	POINT		ptMouse;

	GetCursorPos(&ptMouse);

	ScreenToClient(m_hWnd, &ptMouse);

	if (ptMouse.x < 0)
		ptMouse.x = 0;
	if (ptMouse.x >= m_iWinSizeX)
		ptMouse.x = m_iWinSizeX;
	if (ptMouse.y < 0)
		ptMouse.y = 0;
	if (ptMouse.y >= m_iWinSizeY)
		ptMouse.y = m_iWinSizeY;

	_vector		vMousePos = XMVectorZero();

	/* 투영공간상의 좌표다. = 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
	vMousePos = XMVectorSetX(vMousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	vMousePos = XMVectorSetY(vMousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);
	vMousePos = XMVectorSetZ(vMousePos, m_pGameInstance->Compute_ProjZ(ptMouse));
	vMousePos = XMVectorSetW(vMousePos, 1.f);

	if (XMVectorGetZ(vMousePos) < 0.0f)
		*isSuccess = false;


	return m_pGameInstance->Compute_ProjID(ptMouse);
}

_float CPicking::PickMaskMap(const _float3& vPos)
{
	// 1. 플레이어의 월드 좌표를 뷰-투영 좌표로 변환
	XMVECTOR vPlayerWorldPos =XMLoadFloat3(&vPos);
	XMMATRIX matView = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	XMMATRIX matProj = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);


	XMVECTOR vPlayerViewPos = XMVector3TransformCoord(vPlayerWorldPos, matView);
	XMVECTOR vPlayerProjPos = XMVector3TransformCoord(vPlayerViewPos, matProj);


	//2 w분할 처리 NDC좌표로 변환
	XMFLOAT4 playerProjPosF;
	XMStoreFloat4(&playerProjPosF, vPlayerProjPos);
	if (playerProjPosF.w != 0.0f)
	{
		playerProjPosF.x /= playerProjPosF.w;
		playerProjPosF.y /= playerProjPosF.w;
		playerProjPosF.z /= playerProjPosF.w;
	}

	//3 NDC좌표를 스크린 좌표로 변환

	_float fX= ((playerProjPosF.x+1.0f)*0.5f)*m_iWinSizeX;
	_float fY = ((1.0f - playerProjPosF.y) * 0.5f) * m_iWinSizeY;

	if(fX<0.0f)
		fX=0.0f;
	if(fY<0.0f)
		fY=0.0f;
	if(fX>=m_iWinSizeX)
		fX=(_float)m_iWinSizeX-1.0f;
	if(fY>=m_iWinSizeY)
		fY=(_float)m_iWinSizeY-1.0f;

	m_pGameInstance->Copy_Resource(TEXT("Target_MapMask"), m_pTexture2D);
	if (nullptr == m_pTexture2D)
		return -1.0f;
	D3D11_MAPPED_SUBRESOURCE SubResources{};
	if (FAILED(m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResources)))
		return -1.0f;

	_uint iIndex = static_cast<_uint>(fY) * m_iWinSizeX + static_cast<_uint>(fX);
	_float4* pTexels = reinterpret_cast<_float4*>(SubResources.pData);
	_float4 vResult = pTexels[iIndex];

	m_pContext->Unmap(m_pTexture2D, 0);



	if (vResult.x == 1.0f && vResult.y == 1.0f && vResult.z == 1.0f)
		return 1.0f; // 흰색
	else if (vResult.x == 0.0f && vResult.y == 0.0f && vResult.z == 0.0f)
		return 0.0f; // 검은색

	// 다른 색상일 경우 -1.0f 반환
	return -1.0f;

	
}



CPicking * CPicking::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HWND hWnd)
{
	CPicking*		pInstance = new CPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("Failed to Created : CInput_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	Safe_Release(m_pTexture2D);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
