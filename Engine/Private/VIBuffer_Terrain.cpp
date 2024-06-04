#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer{ rhs }
	, m_iNumVerticesX{ rhs.m_iNumVerticesX }
	, m_iNumVerticesZ{ rhs.m_iNumVerticesZ }
{
	m_pos = new XMFLOAT3[rhs.m_iNumVerticesX * rhs.m_iNumVerticesZ];

	// 원본 데이터 복사
	for (size_t i = 0; i < m_iNumVerticesX * m_iNumVerticesZ; ++i) {
		m_pos[i] = rhs.m_pos[i];
	}
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const wstring& strHeightMapFilePath)
{
	_ulong			dwByte = { 0 };
	
	HANDLE			hFile = CreateFile(strHeightMapFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);


	BITMAPINFOHEADER		ih;
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	_uint*					pPixel = new _uint[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);	

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumVertices = ih.biWidth * ih.biHeight;
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iIndexStride = 4;
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;


	m_pos = new XMFLOAT3[m_iNumVertices];
#pragma region VERTEX_BUFFER 
	VTXNORTEX*		pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);
	
	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, 0.0f/*(pPixel[iIndex] & 0x000000ff) / 10.f*/, i);
			m_pos[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vNormal = _float3(0.0f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}				
	}
#pragma endregion

#pragma region INDEX_BUFFER 


	_uint*		pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1, 
				iIndex + 1, 
				iIndex		
			};

			_vector		vSour, vDest, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)	
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pPixel);
	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	return S_OK;
}

_float CVIBuffer_Terrain::Compute_Height(const _float3& vLocalPos)
{
	_uint			iIndex = _uint(vLocalPos.z) * m_iNumVerticesX + _uint(vLocalPos.x);

	_uint			iIndices[] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	_float		fWidth = vLocalPos.x - m_pVertexPositions[iIndices[0]].x;
	_float		fDepth = m_pVertexPositions[iIndices[0]].z - vLocalPos.z;

	_vector		vPlane = XMVectorZero();

	/* 오른쪽 위 삼각형 안. */
	if (fWidth > fDepth)
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat4(&m_pVertexPositions[iIndices[0]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[1]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[2]]));
	}
	/* 왼쪽 아래 삼각형 안. */
	else
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat4(&m_pVertexPositions[iIndices[0]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[2]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[3]]));
	}

	/*
	ax + by + cz + d = 0
	y = (-ax - cz - d) / b
	*/
	return (-XMVectorGetX(vPlane) * vLocalPos.x - XMVectorGetZ(vPlane) * vLocalPos.z - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
}

_vector CVIBuffer_Terrain::Picking_OnTerrain(HWND hWnd, _fmatrix InverseViewMat, _fmatrix InverseProjMat, _fmatrix InverseWorldMat)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);



	_float3		vMousePos = {};

	vMousePos.x = ptMouse.x / (1280 * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(720 * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_vector MP = {};

	// 투영 -> 뷰스페이스
	XMMATRIX fInverseProjwmat = InverseProjMat;

	MP = XMVector3TransformCoord(XMLoadFloat3(&vMousePos), fInverseProjwmat);
	//
	//
	//
	//
	//// 뷰 스페이스 -> 월드
	XMMATRIX fInverseViewwmat = InverseViewMat;

	_vector vRayDir, vRayPos;
	vRayPos = { 0.f };
	vRayDir = MP - vRayPos;


	vRayPos = XMVector3TransformCoord(vRayPos, fInverseViewwmat);
	vRayDir = XMVector3TransformNormal(vRayDir, fInverseViewwmat);
	vRayDir = XMVector3Normalize(vRayDir);


	XMMATRIX fInverseWorld = InverseWorldMat;

	vRayPos = XMVector3TransformCoord(vRayPos, fInverseWorld);
	vRayDir = XMVector3TransformNormal(vRayDir, fInverseWorld);
	vRayDir = XMVector3Normalize(vRayDir);



	const XMFLOAT3* pTerrainVtxPos = Get_Pos();

	_ulong	dwVtxIdx[3]{};
	_float	fU(0.f), fV(0.f), fDist(0.f);
	float distToCollision = FLT_MAX;

	for (_ulong i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_ulong j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_ulong	dwIndex = i * m_iNumVerticesX + j;

			dwVtxIdx[0] = dwIndex + m_iNumVerticesX;
			dwVtxIdx[1] = dwIndex + m_iNumVerticesX + 1;
			dwVtxIdx[2] = dwIndex + 1;


			if (Intersects(vRayPos, XMVector3Normalize(vRayDir),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[0]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[1]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[2]]),
				fDist))
			{

				return vRayPos + (XMVector3Normalize(vRayDir) * fDist);
			}


			dwVtxIdx[0] = dwIndex + m_iNumVerticesX;
			dwVtxIdx[1] = dwIndex + m_iNumVerticesX + 1;
			dwVtxIdx[2] = dwIndex;

			if (Intersects(vRayPos, XMVector3Normalize(vRayDir),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[0]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[1]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[2]]),
				fDist))
			{

				return vRayPos + (XMVector3Normalize(vRayDir) * fDist);
			}

		}


	}



	return _vector();
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strHeightMapFilePath)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strHeightMapFilePath)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Terrain::Free()
{
	__super::Free();
	Safe_Delete(m_pos);
}

