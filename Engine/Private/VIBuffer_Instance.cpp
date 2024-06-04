#include "..\Public\VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance & rhs)
	: CVIBuffer{ rhs }	
	, m_iNumInstance  {rhs.m_iNumInstance }
	, m_iInstanceStride {rhs.m_iInstanceStride }
	, m_iIndexCountPerInstance { rhs.m_iIndexCountPerInstance }
	, m_InstanceBufferDesc { rhs.m_InstanceBufferDesc }
	, m_pSpeeds{ rhs.m_pSpeeds }
	, m_pOriginalPositions { rhs.m_pOriginalPositions }
	, m_pOriginalSizes { rhs.m_pOriginalSizes }
	, m_fOriginalSize_X{rhs.m_fOriginalSize_X}
	, m_fOriginalSize_Y{rhs.m_fOriginalSize_Y}
	, m_fOriginalSize_Z{rhs.m_fOriginalSize_Z}
	, m_InstanceDesc{ rhs.m_InstanceDesc }

{
	//m_pDevice->CreateBuffer(&m_InstanceBufferDesc, nullptr, &m_pVBInstance);
	//m_pContext->CopyResource(m_pVBInstance, rhs.m_pVBInstance);	
}

HRESULT CVIBuffer_Instance::Initialize_Prototype(const INSTANCE_DESC& InstanceDesc)
{
	m_iNumInstance = InstanceDesc.iNumInstance;

	m_RandomNumber = mt19937_64(m_RandomDevice());

	return S_OK;
}

HRESULT CVIBuffer_Instance::Initialize(void * pArg)
{





	

	return S_OK;
}

HRESULT CVIBuffer_Instance::Bind_Buffers()
{
	CVIBuffer_Instance::INSTANCE_DESC		InstanceDesc{};

	static int tmppp = 0;
	if (GetKeyState('H') & 0x8000)
	{
		tmppp++;
	}


	//InstanceDesc.iNumInstance = 40;
	//InstanceDesc.vOffsetPos = _float3(0.0f, 10.f, 0.0f);
	//InstanceDesc.vPivotPos = _float3(0.0f, 0.f, 0.0f);
	//InstanceDesc.vRange = _float3(0.5f, 0.5f, 0.5f);
	//InstanceDesc.vSize = _float2(0.2f, 0.4f);
	//InstanceDesc.vSpeed = _float2(1.f, 7.f);
	//InstanceDesc.vLifeTime = _float2(1.f, 3.f);
	//InstanceDesc.isLoop = true;
	//
	//m_InstanceDesc = InstanceDesc;
	//m_iNumInstance = InstanceDesc.iNumInstance;
	//
	//m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	//m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//m_InstanceBufferDesc.MiscFlags = 0;
	//m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	//
	//VTXMATRIX* pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	//ZeroMemory(pInstanceVertices, sizeof(VTXMATRIX) * m_iNumInstance);
	//
	////m_pSpeeds = new _float[m_iNumInstance];
	////ZeroMemory(m_pSpeeds, sizeof(_float) * m_iNumInstance);
	////
	////m_pOriginalPositions = new _float3[m_iNumInstance];
	////ZeroMemory(m_pOriginalPositions, sizeof(_float3) * m_iNumInstance);
	//
	////uniform_real_distribution<float>	RangeX(InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x * 0.5f, InstanceDesc.vPivotPos.x + InstanceDesc.vRange.x * 0.5f);
	////uniform_real_distribution<float>	RangeY(InstanceDesc.vPivotPos.y - InstanceDesc.vRange.y * 0.5f, InstanceDesc.vPivotPos.y + InstanceDesc.vRange.y * 0.5f);
	////uniform_real_distribution<float>	RangeZ(InstanceDesc.vPivotPos.z - InstanceDesc.vRange.z * 0.5f, InstanceDesc.vPivotPos.z + InstanceDesc.vRange.z * 0.5f);
	////
	////uniform_real_distribution<float>	Size(InstanceDesc.vSize.x, InstanceDesc.vSize.y);
	////uniform_real_distribution<float>	Speed(InstanceDesc.vSpeed.x, InstanceDesc.vSpeed.y);
	////
	////uniform_real_distribution<float>	LifeTime(InstanceDesc.vLifeTime.x, InstanceDesc.vLifeTime.y);
	//
	//
	//
	//for (size_t i = 0; i < m_iNumInstance; i++)
	//{
	//	
	//	pInstanceVertices[i].vRight = _float4(tmppp, 0.f, 0.f, 0.f);
	//	pInstanceVertices[i].vUp = _float4(0.f, tmppp, 0.f, 0.f);
	//	pInstanceVertices[i].vLook = _float4(0.f, 0.f, tmppp, 0.f);
	//	pInstanceVertices[i].vTranslation = _float4(tmppp, tmppp, tmppp, 1.f);
	//	m_pOriginalPositions[i] = _float3(pInstanceVertices[i].vTranslation.x, pInstanceVertices[i].vTranslation.y, pInstanceVertices[i].vTranslation.z);
	//	pInstanceVertices[i].vLifeTime.x = tmppp;
	//
	//	m_pSpeeds[i] = tmppp;
	//}
	//m_InitialData.pSysMem = pInstanceVertices;

	//if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InitialData, &m_pVBInstance)))
	//	return E_FAIL;


	//Safe_Delete_Array(pInstanceVertices);



	ID3D11Buffer*			pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint					iVertexStrides[] = {
		m_iVertexStride,
		m_iInstanceStride
	};

	_uint					iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_iIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}
HRESULT CVIBuffer_Instance::Render()
{
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instance::Spread(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXMATRIX*		pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		

		pVertices[i].vLifeTime.y += fTimeDelta;
		
		_vector			vDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_InstanceDesc.vOffsetPos), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;
			}
		}

	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;

	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instance::Drop(_float fTimeDelta)
{	
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXMATRIX*		pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vTranslation.y -= m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vLifeTime.y += fTimeDelta;

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;
			}
		}
	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;

	}

	m_pContext->Unmap(m_pVBInstance, 0);	
}

void CVIBuffer_Instance::Up_And_Down(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	const _float gravity = -9.8f; // 중력 가속도 (단위: m/s^2)
	_float Angle =45.f;
	static _float JumpPower = 15.f;
	static _float InitialSpeed = 15.f; // 초기 속도 설정
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
	
		
	
		_vector vRandomPos = XMLoadFloat4(&pVertices[i].vTargetPos);
		_vector vDir=XMVectorSetW(vRandomPos - XMLoadFloat3(&m_InstanceDesc.vOffsetPos), 0.f);
		_float initialSpeed = m_pSpeeds[i];

		// 시간이 흐름에 따라 속도 갱신 (가속도 적용)
		pVertices[i].vLifeTime.y += fTimeDelta;

		// 위치 업데이트
		//XMStoreFloat(&pVertices[i].vTranslation.x, XMLoadFloat(&pVertices[i].vTranslation.x) + XMVector3Normalize(vDir) *JumpPower * initialSpeed * fTimeDelta* cos(XMConvertToRadians(Angle)) * pVertices[i].vLifeTime.y);
		//XMStoreFloat(&pVertices[i].vTranslation.z, XMLoadFloat(&pVertices[i].vTranslation.z) + XMVector3Normalize(vDir) *JumpPower * initialSpeed * fTimeDelta* cos(XMConvertToRadians(Angle)) * pVertices[i].vLifeTime.y);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir)*JumpPower * m_pSpeeds[i] * fTimeDelta* cos(XMConvertToRadians(Angle)) * pVertices[i].vLifeTime.y);

		//pVertices[i].vTranslation.x = initialSpeed * cos(XMConvertToRadians(Angle)) * pVertices[i].vLifeTime.y;
		pVertices[i].vTranslation.y = JumpPower*initialSpeed * sin(XMConvertToRadians(Angle)) * pVertices[i].vLifeTime.y + 0.5f * gravity * (pVertices[i].vLifeTime.y * pVertices[i].vLifeTime.y);

		// 파티클이 초기 위치 아래로 내려가면

		
		if (pVertices[i].vTranslation.y < 0.0f)
		{
			

			
			if (m_InstanceDesc.isLoop)
			{
				// Loop인 경우, 위치와 시간을 초기화
				pVertices[i].vTranslation = _float4(0.0f, 0.0f, 0.0f, 1.0f);
				pVertices[i].vLifeTime.y = 0.0f;
			}
			else
			{
				// Loop가 아닌 경우, 속도를 0으로 설정해서 더 이상 떨어지지 않도록 함
				//pVertices[i].vTranslation.y = 0.0f;
			}
		}
		

		

		
	}
	if (pVertices[m_iNumInstance-1].vLifeTime.y >= pVertices[m_iNumInstance-1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;

	}


	m_pContext->Unmap(m_pVBInstance, 0);



}

void CVIBuffer_Instance::Explosion(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{

		pVertices[i].vTranslation.y += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x += m_pSpeeds[i]*fTimeDelta;
		pVertices[i].vUp.y += m_pSpeeds[i]* fTimeDelta;
		pVertices[i].vLook.z += m_pSpeeds[i]* fTimeDelta;
		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			//pVertices[i].vLifeTime.y = 0.f;
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vRight.x=m_pOriginalSizes[i].x;
				pVertices[i].vUp.y=m_pOriginalSizes[i].y;
				pVertices[i].vLook.z=m_pOriginalSizes[i].z;
				
				pVertices[i].vLifeTime.y = 0.f;
			}
		
		
		
		}


	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;

	}


	m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Instance::Pop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
		
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;

			}
			//pVertices[i].vLifeTime.y = 0.f;
		}
	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;

	}



	m_pContext->Unmap(m_pVBInstance, 0);


}

HRESULT CVIBuffer_Instance::Set_ParticleProperty(const INSTANCE_DESC& InstanceDesc)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};
	_float tmep = InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x * 0.5f;
	_float	fThresholdX = min(InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x * 0.5f, InstanceDesc.vPivotPos.x + InstanceDesc.vRange.x * 0.5f);
	_float	fThresholdY = min(InstanceDesc.vPivotPos.y - InstanceDesc.vRange.y * 0.5f, InstanceDesc.vPivotPos.y + InstanceDesc.vRange.y * 0.5f);
	_float	fThresholdZ = min(InstanceDesc.vPivotPos.z - InstanceDesc.vRange.z * 0.5f, InstanceDesc.vPivotPos.z + InstanceDesc.vRange.z * 0.5f);

	//if (InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x * 0.5f > fThresholdX)
	//{
	//	MSG_BOX("RANGE.X 최대값이 너무 작습니다");
	//
	//	return E_FAIL;
	//}
	//if (InstanceDesc.vPivotPos.y - InstanceDesc.vRange.y * 0.5f > fThresholdY)
	//{
	//	MSG_BOX("RANGE.Y 최대값이 너무 작습니다");
	//
	//	return E_FAIL;
	//}
	//if (InstanceDesc.vPivotPos.z - InstanceDesc.vRange.z * 0.5f > fThresholdZ)
	//{
	//	MSG_BOX("RANGE.Z 최대값이 너무 작습니다");
	//
	//	return E_FAIL;
	//}
	if (InstanceDesc.vSize.x > InstanceDesc.vSize.y)
	{
		MSG_BOX("SIZE 최대값이 너무 작습니다");
	
		return E_FAIL;
	}
	if (InstanceDesc.vSpeed.x > InstanceDesc.vSpeed.y)
	{
		MSG_BOX("SPEED 최대값이 너무 작습니다");
	
		return E_FAIL;
	}
	if (InstanceDesc.vLifeTime.x > InstanceDesc.vLifeTime.y)
	{
		MSG_BOX("LIFETIME 최대값이 너무 작습니다");
	
		return E_FAIL;
	}
	

	uniform_real_distribution<float>	RangeX(InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x * 0.5f, InstanceDesc.vPivotPos.x + InstanceDesc.vRange.x * 0.5f);
	uniform_real_distribution<float>	RangeY(InstanceDesc.vPivotPos.y - InstanceDesc.vRange.y * 0.5f, InstanceDesc.vPivotPos.y + InstanceDesc.vRange.y * 0.5f);
	uniform_real_distribution<float>	RangeZ(InstanceDesc.vPivotPos.z - InstanceDesc.vRange.z * 0.5f, InstanceDesc.vPivotPos.z + InstanceDesc.vRange.z * 0.5f);

	uniform_real_distribution<float>	Size(InstanceDesc.vSize.x, InstanceDesc.vSize.y);
	uniform_real_distribution<float>	Speed(InstanceDesc.vSpeed.x, InstanceDesc.vSpeed.y);

	uniform_real_distribution<float>	LifeTime(InstanceDesc.vLifeTime.x, InstanceDesc.vLifeTime.y);

	m_iNumInstance= m_InstanceDesc.iNumInstance = InstanceDesc.iNumInstance;
	m_InstanceDesc.vOffsetPos = InstanceDesc.vOffsetPos;
	m_InstanceDesc.vPivotPos = InstanceDesc.vPivotPos;
	m_InstanceDesc.vRange = InstanceDesc.vRange;
	m_InstanceDesc.vSize = InstanceDesc.vSize;

	m_InstanceDesc.fSize_X = InstanceDesc.fSize_X;
	m_InstanceDesc.fSize_Y = InstanceDesc.fSize_Y;
	m_InstanceDesc.fSize_Z = InstanceDesc.fSize_Z;



	m_InstanceDesc.vSpeed = InstanceDesc.vSpeed;
	m_InstanceDesc.vLifeTime = InstanceDesc.vLifeTime;
	m_InstanceDesc.isLoop = InstanceDesc.isLoop;
	m_InstanceDesc.vTargetPos = InstanceDesc.vTargetPos;
	m_InstanceDesc.iType_Index = InstanceDesc.iType_Index;


	//InstanceDesc.iNumInstance = 1;
	//InstanceDesc.vOffsetPos = _float3(0.0f, 10.f, 0.0f);
	//InstanceDesc.vPivotPos = _float3(0.0f, 0.f, 0.0f);
	//InstanceDesc.vRange = _float3(0.5f, 0.5f, 0.5f);
	//InstanceDesc.vSize = _float2(0.2f, 0.4f);
	//InstanceDesc.vSpeed = _float2(1.f, 7.f);
	//InstanceDesc.vLifeTime = _float2(1.f, 3.f);
	//InstanceDesc.isLoop = true;

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	
	for (_uint i = 0; i < m_iNumInstance; i++)
	{
	
		_float	fSize = Size(m_RandomNumber);
		VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
		pVertices[i].vLifeTime.x	= LifeTime(m_RandomNumber);
		pVertices[i].vLifeTime.y	= InstanceDesc.vLifeTime.y;
		pVertices[i].vRight			= _float4(fSize, 0.f, 0.f, 0.f);
		pVertices[i].vUp			= _float4(0.f, fSize, 0.f, 0.f);
		pVertices[i].vLook			= _float4(0.f, 0.f, fSize, 0.f);
		pVertices[i].vTranslation	= _float4(RangeX(m_RandomNumber), RangeY(m_RandomNumber), RangeZ(m_RandomNumber), 1.f);
		pVertices[i].vTargetPos		= _float4(RandomNum<_float>(-InstanceDesc.vTargetPos.x, InstanceDesc.vTargetPos.x), 
											  RandomNum<_float>(-InstanceDesc.vTargetPos.y, InstanceDesc.vTargetPos.y),
											  RandomNum<_float>(-InstanceDesc.vTargetPos.z, InstanceDesc.vTargetPos.z),1.0f);
		m_pOriginalPositions[i]		= _float3(pVertices[i].vTranslation.x, pVertices[i].vTranslation.y, pVertices[i].vTranslation.z);
		m_pOriginalSizes[i] = _float3(fSize, fSize, fSize);
		m_fOriginalSize_X[i] = InstanceDesc.fSize_X;
		m_fOriginalSize_Y[i] = InstanceDesc.fSize_Y;
		m_fOriginalSize_Z[i] = InstanceDesc.fSize_Z;
		
		m_pSpeeds[i]				= Speed(m_RandomNumber);
		//pVertices[i].
	}
	
	
	
	
	m_pContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

void CVIBuffer_Instance::SizeUp(_float fTimeDelta)
{


	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vUp.y += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vLook.z += m_pSpeeds[i] * fTimeDelta;
		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{


		
			//pVertices[i].vLifeTime.y = 0.f;
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vRight.x = m_pOriginalSizes[i].x;
				pVertices[i].vUp.y = m_pOriginalSizes[i].y;
				pVertices[i].vLook.z = m_pOriginalSizes[i].z;
				pVertices[i].vLifeTime.y = 0.f;
			}



		}

	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;

	}


	m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Instance::RandomPop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		
		_vector vRandomPos =XMVectorSet(RandomNum<_float>(-m_InstanceDesc.vRange.x, m_InstanceDesc.vRange.x),
			RandomNum<_float>(-m_InstanceDesc.vRange.y, m_InstanceDesc.vRange.y),
			RandomNum<_float>(-m_InstanceDesc.vRange.z, m_InstanceDesc.vRange.z), 1.0f);
		_vector vDir = XMVectorSetW(XMLoadFloat3(&m_InstanceDesc.vOffsetPos)-vRandomPos, 0.f);
		//XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);

		XMStoreFloat4(&pVertices[i].vTranslation, vRandomPos);
		XMStoreFloat4(&pVertices[i].vLook, XMVector3Normalize(vDir) );
			//XMVector3Normalize(vDir);



		pVertices[i].vLifeTime.y += fTimeDelta;
		
	if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			//pVertices[i].vLifeTime.y = 0.f;
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;
			}
		}

	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;

	}


m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Instance::RandRotate_SizeUp(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{

		pVertices[i].vLifeTime.y += fTimeDelta;

		// 크기 증가
		pVertices[i].vRight.x += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vUp.y += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vLook.z += m_pSpeeds[i] * fTimeDelta;

		// 회전 각도 설정 (랜덤)
		float fRandomAngle = RandomNum<float>(0.0f, XM_2PI);

		// 회전 축 설정 (이 예제에서는 y축을 기준으로 회전)
		XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // up 방향
		XMMATRIX mRotation = XMMatrixRotationAxis(vUp, fRandomAngle);

		// 기존 벡터를 로드
		XMVECTOR vRight = XMLoadFloat4(&pVertices[i].vRight);
		XMVECTOR vUpVector = XMLoadFloat4(&pVertices[i].vUp);
		XMVECTOR vLook = XMLoadFloat4(&pVertices[i].vLook);

		// 회전 적용
		vRight = XMVector3TransformNormal(vRight, mRotation);
		vLook = XMVector3TransformNormal(vLook, mRotation);

		// 회전된 벡터를 정점에 저장
		XMStoreFloat4(&pVertices[i].vRight, vRight);
		XMStoreFloat4(&pVertices[i].vLook, vLook);

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			if (m_InstanceDesc.isLoop)
			{
				pVertices[i].vRight.x = m_pOriginalSizes[i].x;
				pVertices[i].vUp.y = m_pOriginalSizes[i].y;
				pVertices[i].vLook.z = m_pOriginalSizes[i].z;
				pVertices[i].vLifeTime.y = 0.f;
			}
		}
	}

	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);

}



void CVIBuffer_Instance::SizeDown(_float fTimeDelta)
{

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
	_float fRotationAngle = 0.f;
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta * 0.5f;
	

	
		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vRight.x += m_pSpeeds[i] * fTimeDelta*0.5f;
			pVertices[i].vUp.y += m_pSpeeds[i] * fTimeDelta*0.5f;
			pVertices[i].vLook.z += m_pSpeeds[i] * fTimeDelta*0.5f;
			//pVertices[i].vLifeTime.y = 0.f;

			//if (pVertices[i].vRight.x < 0.0f)
			//{
			//	pVertices[i].vRight.x = m_pOriginalSizes[i].x;
			//	pVertices[i].vUp.y = m_pOriginalSizes[i].y;
			//	pVertices[i].vLook.z = m_pOriginalSizes[i].z;
			//	pVertices[i].vLifeTime.y = 0.f;
			//}
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vRight.x = m_pOriginalSizes[i].x;
				pVertices[i].vUp.y = m_pOriginalSizes[i].y;
				pVertices[i].vLook.z = m_pOriginalSizes[i].z;
				pVertices[i].vLifeTime.y = 0.f;
				
			}



		}
		if (fRotationAngle > 90.f)
		{
			int temp= 0;

		}


		fRotationAngle+= fTimeDelta;
		XMMATRIX rotationMatrix = XMMatrixRotationY(fRotationAngle);
		XMVECTOR rightVec = XMLoadFloat4(&pVertices[i].vRight);
		XMVECTOR upVec = XMLoadFloat4(&pVertices[i].vUp);
		XMVECTOR lookVec = XMLoadFloat4(&pVertices[i].vLook);
		rightVec = XMVector3TransformNormal(rightVec, rotationMatrix);
		upVec = XMVector3TransformNormal(upVec, rotationMatrix);
		lookVec = XMVector3TransformNormal(lookVec, rotationMatrix);
		XMStoreFloat4(&pVertices[i].vRight, rightVec);
		XMStoreFloat4(&pVertices[i].vUp, upVec);
		XMStoreFloat4(&pVertices[i].vLook, lookVec);

	}
	//if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	//{
	//	m_InstanceDesc.IsDead = true;
	//
	//}


	m_pContext->Unmap(m_pVBInstance, 0);



}

void CVIBuffer_Instance::SizeDown_Y(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;








	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x = m_fOriginalSize_X[i];
		pVertices[i].vUp.y -= m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vLook.z = m_fOriginalSize_Z[i];

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			
				pVertices[i].vRight.x = m_fOriginalSize_X[i];
				pVertices[i].vUp.y = m_fOriginalSize_Y[i];
				pVertices[i].vLook.z = m_fOriginalSize_Z[i];
			
				if(m_InstanceDesc.isLoop)
					pVertices[i].vLifeTime.y = 0.f;
			
		}


	}

	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instance::SizeDown_X(_float fTimeDelta)
{

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;








	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x -= m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vUp.y = m_fOriginalSize_Y[i];
		pVertices[i].vLook.z = m_fOriginalSize_Z[i];

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{

			pVertices[i].vRight.x = m_fOriginalSize_X[i];
			pVertices[i].vUp.y = m_fOriginalSize_Y[i];
			pVertices[i].vLook.z = m_fOriginalSize_Z[i];
			if (m_InstanceDesc.isLoop)
				pVertices[i].vLifeTime.y = 0.f;

		}


	}

	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);






}

void CVIBuffer_Instance::SizeDown_Z(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x = m_fOriginalSize_X[i];
		pVertices[i].vUp.y = m_fOriginalSize_Y[i];
		pVertices[i].vLook.z -= m_pSpeeds[i] * fTimeDelta;

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{

			pVertices[i].vRight.x = m_fOriginalSize_X[i];
			pVertices[i].vUp.y = m_fOriginalSize_Y[i];
			pVertices[i].vLook.z = m_fOriginalSize_Z[i];
			if (m_InstanceDesc.isLoop)
				pVertices[i].vLifeTime.y = 0.f;

		}
	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);




}

void CVIBuffer_Instance::Rotation_X(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;


		_vector vRight = XMLoadFloat4(&pVertices[i].vRight);
		_vector vUp = XMLoadFloat4(&pVertices[i].vUp);
		_vector vLook = XMLoadFloat4(&pVertices[i].vLook);

		_matrix		mQuat = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vRight, XMConvertToRadians(60.f) * fTimeDelta * m_pSpeeds[i]));

		vRight = XMVector3TransformNormal(vRight, mQuat);
		vUp = XMVector3TransformNormal(vUp, mQuat);
		vLook = XMVector3TransformNormal(vLook, mQuat);

		XMStoreFloat4(&pVertices[i].vRight, vRight);
		XMStoreFloat4(&pVertices[i].vUp, vUp);
		XMStoreFloat4(&pVertices[i].vLook, vLook);

		if (m_InstanceDesc.isLoop == true)
		{

			if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
			{
				pVertices[i].vRight = _float4(m_pOriginalSizes[i].x, 0.f, 0.f, 0.f);
				pVertices[i].vUp = _float4(0.f, m_pOriginalSizes[i].y, 0.f, 0.f);
				pVertices[i].vLook = _float4(0.f, 0.f, m_pOriginalSizes[i].z, 0.f);
				pVertices[i].vLifeTime.y = 0.f;
			}

		}


	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);


}

void CVIBuffer_Instance::SizeUp_Y(_float fTimeDelta)
{

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;








	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x = m_fOriginalSize_X[i];
		pVertices[i].vUp.y += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vLook.z = m_fOriginalSize_Z[i];

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{

			pVertices[i].vRight.x = m_fOriginalSize_X[i];
			pVertices[i].vUp.y = m_fOriginalSize_Y[i];
			pVertices[i].vLook.z = m_fOriginalSize_Z[i];

			if (m_InstanceDesc.isLoop)
				pVertices[i].vLifeTime.y = 0.f;

		}


	}

	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);



}



void CVIBuffer_Instance::Rotation_Y(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;

		
		_vector vRight = XMLoadFloat4(&pVertices[i].vRight);
		_vector vUp = XMLoadFloat4(&pVertices[i].vUp);
		_vector vLook = XMLoadFloat4(&pVertices[i].vLook);

		_matrix		mQuat = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vUp, XMConvertToRadians(60.f)* fTimeDelta*m_pSpeeds[i]));

		vRight = XMVector3TransformNormal(vRight, mQuat);
		vUp = XMVector3TransformNormal(vUp, mQuat);
		vLook = XMVector3TransformNormal(vLook, mQuat);

		XMStoreFloat4(&pVertices[i].vRight, vRight);
		XMStoreFloat4(&pVertices[i].vUp, vUp);
		XMStoreFloat4(&pVertices[i].vLook, vLook);

		if (m_InstanceDesc.isLoop == true)
		{

			if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
			{
				pVertices[i].vRight = _float4(m_pOriginalSizes[i].x, 0.f, 0.f, 0.f);
				pVertices[i].vUp = _float4(0.f, m_pOriginalSizes[i].y, 0.f, 0.f);
				pVertices[i].vLook = _float4(0.f, 0.f, m_pOriginalSizes[i].z, 0.f);
				pVertices[i].vLifeTime.y = 0.f;
			}

		}


	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);


}

void CVIBuffer_Instance::SizeUp_X(_float fTimeDelta)
{

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;








	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vUp.y = m_fOriginalSize_Y[i];
		pVertices[i].vLook.z = m_fOriginalSize_Z[i];

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{

			pVertices[i].vRight.x = m_fOriginalSize_X[i];
			pVertices[i].vUp.y = m_fOriginalSize_Y[i];
			pVertices[i].vLook.z = m_fOriginalSize_Z[i];

			if (m_InstanceDesc.isLoop)
				pVertices[i].vLifeTime.y = 0.f;

		}


	}

	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);



}

void CVIBuffer_Instance::Rotation_Z(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;


		_vector vRight = XMLoadFloat4(&pVertices[i].vRight);
		_vector vUp = XMLoadFloat4(&pVertices[i].vUp);
		_vector vLook = XMLoadFloat4(&pVertices[i].vLook);

		_matrix		mQuat = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vLook, XMConvertToRadians(60.f) * fTimeDelta * m_pSpeeds[i]));

		vRight = XMVector3TransformNormal(vRight, mQuat);
		vUp = XMVector3TransformNormal(vUp, mQuat);
		vLook = XMVector3TransformNormal(vLook, mQuat);

		XMStoreFloat4(&pVertices[i].vRight, vRight);
		XMStoreFloat4(&pVertices[i].vUp, vUp);
		XMStoreFloat4(&pVertices[i].vLook, vLook);

		if (m_InstanceDesc.isLoop == true)
		{

			if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
			{
				pVertices[i].vRight = _float4(m_pOriginalSizes[i].x, 0.f, 0.f, 0.f);
				pVertices[i].vUp = _float4(0.f, m_pOriginalSizes[i].y, 0.f, 0.f);
				pVertices[i].vLook = _float4(0.f, 0.f, m_pOriginalSizes[i].z, 0.f);
				pVertices[i].vLifeTime.y = 0.f;
			}

		}


	}
	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);


}

void CVIBuffer_Instance::SizeUp_Z(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;








	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vRight.x = m_fOriginalSize_X[i];
		pVertices[i].vUp.y = m_fOriginalSize_Y[i];
		pVertices[i].vLook.z += m_pSpeeds[i] * fTimeDelta;

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{

			pVertices[i].vRight.x = m_fOriginalSize_X[i];
			pVertices[i].vUp.y = m_fOriginalSize_Y[i];
			pVertices[i].vLook.z = m_fOriginalSize_Z[i];

			if (m_InstanceDesc.isLoop)
				pVertices[i].vLifeTime.y = 0.f;

		}


	}

	if (pVertices[m_iNumInstance - 1].vLifeTime.y >= pVertices[m_iNumInstance - 1].vLifeTime.x)
	{
		m_InstanceDesc.IsDead = true;
	}



	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instance::Roation_Orbit(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};
	bool allInstancesDead = true;
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		// Calculate the current angle
		float angle = atan2(pVertices[i].vTranslation.z - m_InstanceDesc.vOffsetPos.z,
			pVertices[i].vTranslation.x - m_InstanceDesc.vOffsetPos.x);

		// Increase the angle over time to create a swirl effect
		angle += m_pSpeeds[i] * fTimeDelta;

		// Use vRange.z as the base radius and scale it with the y position
		float fBaseRadius = m_InstanceDesc.vRange.z;
		float currentRadius = fBaseRadius + pVertices[i].vTranslation.y * 0.1f; // Adjust the factor as needed

		// Calculate the new position
		float x = m_InstanceDesc.vOffsetPos.x + currentRadius * cos(angle);
		float z = m_InstanceDesc.vOffsetPos.z + currentRadius * sin(angle);

		// Update the position
		pVertices[i].vTranslation.x = x;
		pVertices[i].vTranslation.z = z;

		// Update the height based on gravity


		// Update direction vectors (Right, Up, Look)
		XMVECTOR vDir = XMVectorSet(pVertices[i].vTranslation.x - m_InstanceDesc.vOffsetPos.x,
			pVertices[i].vTranslation.y - m_InstanceDesc.vOffsetPos.y,
			pVertices[i].vTranslation.z - m_InstanceDesc.vOffsetPos.z, 0.0f);
		XMVECTOR vLook = XMVector3Normalize(vDir);
		XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(vLook, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		XMVECTOR vUp = XMVector3Normalize(XMVector3Cross(vRight, vLook));

		XMStoreFloat4(&pVertices[i].vRight, vRight * XMLoadFloat3(&m_pOriginalSizes[i]));
		XMStoreFloat4(&pVertices[i].vUp, vUp * XMLoadFloat3(&m_pOriginalSizes[i]));
		XMStoreFloat4(&pVertices[i].vLook, vLook * XMLoadFloat3(&m_pOriginalSizes[i]));

		// Update lifetime
		pVertices[i].vLifeTime.y += fTimeDelta;
		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			if (m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;
				//m_pSize[i] = m_pOriginalSize[i];
			}
			else
			{
				pVertices[i].vLifeTime.y = pVertices[i].vLifeTime.x;
			}
		}

		if (pVertices[i].vLifeTime.y < pVertices[i].vLifeTime.x)
		{
			allInstancesDead = false;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);






}

HRESULT CVIBuffer_Instance::Set_ParticleType(_uint iIndex)
{
	m_InstanceDesc.iType_Index = iIndex;
	return S_OK;
}

void CVIBuffer_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pSpeeds);
		Safe_Delete_Array(m_pOriginalPositions);
		Safe_Delete_Array(m_pOriginalSizes);
		Safe_Delete_Array(m_fOriginalSize_X);
		Safe_Delete_Array(m_fOriginalSize_Y);
		Safe_Delete_Array(m_fOriginalSize_Z);
	}
	
	Safe_Release(m_pVBInstance);
}
