#include "..\Public\Mesh_Particle.h"
#include "Bone.h"
#include"Mesh.h"

CMesh_Particle::CMesh_Particle(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance{ pDevice, pContext }
{
	
}

CMesh_Particle::CMesh_Particle(const CMesh_Particle & rhs)
	: CVIBuffer_Instance{ rhs }
{
}

HRESULT CMesh_Particle::Initialize(const CMesh* Meshs, _fmatrix PreTransformMatrix, const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc)
{
	if (FAILED(__super::Initialize_Prototype(InstanceDesc)))
		return E_FAIL;
	m_InstanceDesc = InstanceDesc;
	//TODO 
	// 메쉬 머터리얼 갯수 이름이랑 똑같은지 비교해서 찾기
	

	m_iMaterialIndex = Meshs->Get_MaterialIndex();

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iNumVertexBuffers = 2;
	m_iVertexStride = sizeof(VTXMESH);
	m_iNumVertices = Meshs->Get_VertexCount();
	m_iIndexStride = 4;
	m_iIndexCountPerInstance = Meshs->Get_VertexCount();
	m_iNumIndices = m_iIndexCountPerInstance * m_iNumInstance;
	m_iInstanceStride = sizeof(VTXMATRIX);


#pragma region VERTEX_BUFFER 



	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;


	_matrix matWorld = PreTransformMatrix;



	VTXMESH*pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);
	
	
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &Meshs->Get_Vertices()[i].vPosition, sizeof(_float3));
		//XMStoreFloat3(&pVertices[i].vPosition,
		//	XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), matWorld));
	
		memcpy(&pVertices[i].vNormal, &Meshs->Get_Vertices()[i].vNormal, sizeof(_float3));
		//XMStoreFloat3(&pVertices[i].vNormal,
		//	XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), matWorld)));
	
		
		memcpy(&pVertices[i].vTexcoord, &Meshs->Get_Vertices()[i].vTexcoord, sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &Meshs->Get_Vertices()[i].vTangent, sizeof(_float3));
	}
	
	
	
	//m_pNonAnimVertices= pVertices;
	m_InitialData.pSysMem = pVertices;
	
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;
	
	Safe_Delete_Array(pVertices);




#pragma endregion

#pragma region INDEX_BUFFER 
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;


	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };
	

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_uint iFaceCnt = Meshs->Get_NumIndices() / 3;
		_uint		iNumFacesIndices = { 0 };
		for (size_t j = 0; j < iFaceCnt; j++)
		{
			pIndices[iNumIndices++] = Meshs->Get_Indices()[iNumFacesIndices++];
			pIndices[iNumIndices++] = Meshs->Get_Indices()[iNumFacesIndices++];
			pIndices[iNumIndices++] = Meshs->Get_Indices()[iNumFacesIndices++];
		}
		int tmep = 0;
	

	}
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;
	Safe_Delete_Array(pIndices);

#pragma endregion

#pragma region INSTANCE_BUFFER
	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	
	VTXMATRIX* pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXMATRIX) * m_iNumInstance);
	
	m_pSpeeds = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeeds, sizeof(_float) * m_iNumInstance);
	
	m_pOriginalPositions = new _float3[m_iNumInstance];
	ZeroMemory(m_pOriginalPositions, sizeof(_float3) * m_iNumInstance);

	m_pOriginalSizes = new _float3[m_iNumInstance];
	ZeroMemory(m_pOriginalSizes, sizeof(_float3) * m_iNumInstance);


	
	m_fOriginalSize_X = new _float[m_iNumInstance];
	ZeroMemory(m_fOriginalSize_X, sizeof(_float) * m_iNumInstance);
	m_fOriginalSize_Y = new _float[m_iNumInstance];
	ZeroMemory(m_fOriginalSize_Y, sizeof(_float) * m_iNumInstance);
	m_fOriginalSize_Z = new _float[m_iNumInstance];
	ZeroMemory(m_fOriginalSize_Z, sizeof(_float) * m_iNumInstance);
	


	
	_float temp=InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x;
	_float	fThresholdX= min(InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x * 0.5f, InstanceDesc.vPivotPos.x + InstanceDesc.vRange.x * 0.5f);
	_float	fThresholdY = min(InstanceDesc.vPivotPos.y - InstanceDesc.vRange.y * 0.5f, InstanceDesc.vPivotPos.y + InstanceDesc.vRange.y * 0.5f);
	_float	fThresholdZ = min(InstanceDesc.vPivotPos.z - InstanceDesc.vRange.z * 0.5f, InstanceDesc.vPivotPos.z + InstanceDesc.vRange.z * 0.5f);

	if (InstanceDesc.vPivotPos.x - InstanceDesc.vRange.x * 0.5f > fThresholdX)
	{
		MSG_BOX("RANGE.X 최대값이 너무 작습니다");
	
		return E_FAIL;
	}
	if (InstanceDesc.vPivotPos.y - InstanceDesc.vRange.y * 0.5f > fThresholdY)
	{
		MSG_BOX("RANGE.Y 최대값이 너무 작습니다");
	
		return E_FAIL;
	}
	if (InstanceDesc.vPivotPos.z - InstanceDesc.vRange.z * 0.5f > fThresholdZ)
	{
		MSG_BOX("RANGE.Z 최대값이 너무 작습니다");
	
		return E_FAIL;
	}
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
	
	
	
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float	fSize = Size(m_RandomNumber);
		pInstanceVertices[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, fSize, 0.f);
		pInstanceVertices[i].vTranslation = _float4(RangeX(m_RandomNumber), RangeY(m_RandomNumber), RangeZ(m_RandomNumber), 1.f);
		m_pOriginalPositions[i] = _float3(pInstanceVertices[i].vTranslation.x, pInstanceVertices[i].vTranslation.y, pInstanceVertices[i].vTranslation.z);
		m_pOriginalSizes[i] = _float3(fSize, fSize, fSize);
		m_fOriginalSize_X[i] = InstanceDesc.fSize_X;
		m_fOriginalSize_Y[i] = InstanceDesc.fSize_Y;
		m_fOriginalSize_Z[i] = InstanceDesc.fSize_Z;

		pInstanceVertices[i].vLifeTime.x = LifeTime(m_RandomNumber);
		pInstanceVertices[i].vTargetPos = _float4(
			RandomNum<float>(-InstanceDesc.vTargetPos.x, InstanceDesc.vTargetPos.x), 
			RandomNum<float>(-InstanceDesc.vTargetPos.y, InstanceDesc.vTargetPos.y), 
			RandomNum<float>(-InstanceDesc.vTargetPos.z, InstanceDesc.vTargetPos.z), 1.0f);

		m_pSpeeds[i] = Speed(m_RandomNumber);
	}
	m_InitialData.pSysMem = pInstanceVertices;
	
	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InitialData, &m_pVBInstance)))
		return E_FAIL;
	
	
	Safe_Delete_Array(pInstanceVertices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh_Particle::Initialize(void* pArg)
{
	//D3D11_MAPPED_SUBRESOURCE		SubResource{};
	//
	////m_pDevice->CreateBuffer(&m_InstanceBufferDesc, nullptr, &m_pVBInstance);
	//
	//
	//m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	//
	//VTXMATRIX* pInstanceVertices = (VTXMATRIX*)SubResource.pData;
	//
	//for (size_t i = 0; i < m_iNumInstance; i++)
	//{
	//	uniform_real_distribution<float>	RangeX(m_InstanceDesc.vPivotPos.x - m_InstanceDesc.vRange.x * 0.5f, m_InstanceDesc.vPivotPos.x + m_InstanceDesc.vRange.x * 0.5f);
	//	uniform_real_distribution<float>	RangeY(m_InstanceDesc.vPivotPos.y - m_InstanceDesc.vRange.y * 0.5f, m_InstanceDesc.vPivotPos.y + m_InstanceDesc.vRange.y * 0.5f);
	//	uniform_real_distribution<float>	RangeZ(m_InstanceDesc.vPivotPos.z - m_InstanceDesc.vRange.z * 0.5f, m_InstanceDesc.vPivotPos.z + m_InstanceDesc.vRange.z * 0.5f);
	//
	//	uniform_real_distribution<float>	Size(m_InstanceDesc.vSize.x, m_InstanceDesc.vSize.y);
	//	uniform_real_distribution<float>	Speed(m_InstanceDesc.vSpeed.x, m_InstanceDesc.vSpeed.y);
	//
	//	uniform_real_distribution<float>	LifeTime(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);
	//
	//
	//
	//
	//	_float	fSize = Size(m_RandomNumber);
	//	
	//	pInstanceVertices[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
	//	pInstanceVertices[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
	//	pInstanceVertices[i].vLook = _float4(0.f, 0.f, fSize, 0.f);
	//	pInstanceVertices[i].vTranslation = _float4(RangeX(m_RandomNumber), RangeY(m_RandomNumber), RangeZ(m_RandomNumber), 1.f);
	//	m_pOriginalPositions[i] = _float3(pInstanceVertices[i].vTranslation.x, pInstanceVertices[i].vTranslation.y, pInstanceVertices[i].vTranslation.z);
	//	m_pOriginalSizes[i] = _float3(fSize, fSize, fSize);
	//	pInstanceVertices[i].vLifeTime.x = LifeTime(m_RandomNumber);
	//	pInstanceVertices[i].vTargetPos = _float4(
	//		RandomNum<float>(-m_InstanceDesc.vTargetPos.x, m_InstanceDesc.vTargetPos.x),
	//		RandomNum<float>(-m_InstanceDesc.vTargetPos.y, m_InstanceDesc.vTargetPos.y),
	//		RandomNum<float>(-m_InstanceDesc.vTargetPos.z, m_InstanceDesc.vTargetPos.z), 1.0f);
	//
	//	m_pSpeeds[i] = Speed(m_RandomNumber);
	//}
	//
	//
	//
	//m_pContext->Unmap(m_pVBInstance, 0);
	


	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	
	VTXMATRIX* pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXMATRIX) * m_iNumInstance);

	RandomNum<_float>(m_InstanceDesc.vPivotPos.x - m_InstanceDesc.vRange.x * 0.5f, m_InstanceDesc.vPivotPos.x + m_InstanceDesc.vRange.x * 0.5f);
	
	//uniform_real_distribution<float>	RangeX(m_InstanceDesc.vPivotPos.x - m_InstanceDesc.vRange.x * 0.5f, m_InstanceDesc.vPivotPos.x + m_InstanceDesc.vRange.x * 0.5f);
	//uniform_real_distribution<float>	RangeY(m_InstanceDesc.vPivotPos.y - m_InstanceDesc.vRange.y * 0.5f, m_InstanceDesc.vPivotPos.y + m_InstanceDesc.vRange.y * 0.5f);
	//uniform_real_distribution<float>	RangeZ(m_InstanceDesc.vPivotPos.z - m_InstanceDesc.vRange.z * 0.5f, m_InstanceDesc.vPivotPos.z + m_InstanceDesc.vRange.z * 0.5f);
	//
	//uniform_real_distribution<float>	Size(m_InstanceDesc.vSize.x, m_InstanceDesc.vSize.y);
	//uniform_real_distribution<float>	Speed(m_InstanceDesc.vSpeed.x, m_InstanceDesc.vSpeed.y);
	//
	//uniform_real_distribution<float>	LifeTime(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);
	
	
	
	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float	fSize = RandomNum<_float>(m_InstanceDesc.vSize.x, m_InstanceDesc.vSize.y);
		_float fSpeed = RandomNum<_float>(m_InstanceDesc.vSpeed.x, m_InstanceDesc.vSpeed.y);
		_float fRangeX = RandomNum<_float>(m_InstanceDesc.vPivotPos.x - m_InstanceDesc.vRange.x * 0.5f, m_InstanceDesc.vPivotPos.x + m_InstanceDesc.vRange.x * 0.5f);
		_float fRangeY = RandomNum<_float>(m_InstanceDesc.vPivotPos.y - m_InstanceDesc.vRange.y * 0.5f, m_InstanceDesc.vPivotPos.y + m_InstanceDesc.vRange.y * 0.5f);
		_float fRangeZ = RandomNum<_float>(m_InstanceDesc.vPivotPos.z - m_InstanceDesc.vRange.z * 0.5f, m_InstanceDesc.vPivotPos.z + m_InstanceDesc.vRange.z * 0.5f);
		_float fLifeTime = RandomNum<_float>(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);




		pInstanceVertices[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, fSize, 0.f);
		pInstanceVertices[i].vTranslation = _float4(fRangeX, fRangeY, fRangeZ, 1.f);
		m_pOriginalPositions[i] = _float3(pInstanceVertices[i].vTranslation.x, pInstanceVertices[i].vTranslation.y, pInstanceVertices[i].vTranslation.z);
		m_pOriginalSizes[i] = _float3(fSize, fSize, fSize);
		m_fOriginalSize_X[i] = m_InstanceDesc.fSize_X;
		m_fOriginalSize_Y[i] = m_InstanceDesc.fSize_Y;
		m_fOriginalSize_Z[i] = m_InstanceDesc.fSize_Z;

		pInstanceVertices[i].vLifeTime.x = fLifeTime;
		pInstanceVertices[i].vTargetPos = _float4(
			RandomNum<float>(-m_InstanceDesc.vTargetPos.x, m_InstanceDesc.vTargetPos.x),
			RandomNum<float>(-m_InstanceDesc.vTargetPos.y, m_InstanceDesc.vTargetPos.y),
			RandomNum<float>(-m_InstanceDesc.vTargetPos.z, m_InstanceDesc.vTargetPos.z), 1.0f);
	
		m_pSpeeds[i] = fSpeed;
	}
	m_InitialData.pSysMem = pInstanceVertices;
	
	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InitialData, &m_pVBInstance)))
		return E_FAIL;
	
	
	Safe_Delete_Array(pInstanceVertices);

	return S_OK;
}


void CMesh_Particle::Fill_Matrices(vector<class CBone*>& Bones, _float4x4 * pMeshBoneMatrices)
{
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&pMeshBoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * XMLoadFloat4x4(Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix()));
	}	
}

HRESULT CMesh_Particle::Save_Data( ofstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		FileLoader.write(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.write((char*)&m_iMaterialIndex, sizeof(_uint));

		FileLoader.write((char*)&m_ePrimitiveTopology, sizeof(D3D11_PRIMITIVE_TOPOLOGY));
		FileLoader.write((char*)&m_iIndexFormat, sizeof(DXGI_FORMAT));
		FileLoader.write((char*)&m_iNumVertexBuffers, sizeof(_uint));
		
		FileLoader.write((char*)&m_iNumVertices, sizeof(_uint));//156
		FileLoader.write((char*)&m_iVertexStride, sizeof(_uint));//76

		FileLoader.write((char*)&m_iNumIndices, sizeof(_uint));//156
		FileLoader.write((char*)&m_iIndexStride, sizeof(_uint));//4

		FileLoader.write((char*)&m_eModelType, sizeof(CModel::MODELTYPE));

		if (m_eModelType == CModel::MODELTYPE::TYPE_NONANIM)
		{
			FileLoader.write((char*)&m_MeshBufferDesc, sizeof(D3D11_BUFFER_DESC));//11856
			for (size_t i = 0; i < m_iNumVertices; i++)
			{
				FileLoader.write((char*)&m_pNonAnimVertices[i].vPosition, sizeof(_float3));
				FileLoader.write((char*)&m_pNonAnimVertices[i].vNormal, sizeof(_float3));
				FileLoader.write((char*)&m_pNonAnimVertices[i].vTexcoord, sizeof(_float2));
				FileLoader.write((char*)&m_pNonAnimVertices[i].vTangent, sizeof(_float3));

			}


		}
		//정점정보 저장
		else if (m_eModelType == CModel::MODELTYPE::TYPE_ANIM)
		{

			//버텍스(메쉬 전용) 버퍼 구조체 정보 저장 
			FileLoader.write((char*)&m_MeshBufferDesc, sizeof(D3D11_BUFFER_DESC));//11856
			for (size_t i = 0; i < m_iNumVertices; i++)
			{
				FileLoader.write((char*)&m_pAnimVertices[i].vPosition, sizeof(_float3));
				FileLoader.write((char*)&m_pAnimVertices[i].vNormal, sizeof(_float3));
				FileLoader.write((char*)&m_pAnimVertices[i].vTexcoord, sizeof(_float2));
				FileLoader.write((char*)&m_pAnimVertices[i].vTangent, sizeof(_float3));
				FileLoader.write((char*)&m_pAnimVertices[i].vBlendIndices, sizeof(XMUINT4));
				FileLoader.write((char*)&m_pAnimVertices[i].vBlendWeights, sizeof(XMFLOAT4));

			}

			FileLoader.write((char*)&m_iNumBones, sizeof(_uint));
			FileLoader.write((char*)&m_iNumWeights, sizeof(_uint));

			//본 인덱스들 벡터 사이즈 저장
			size_t m_BoneIndicesSize = m_BoneIndices.size();	//1
			FileLoader.write((char*)&m_BoneIndicesSize, sizeof(size_t));

			//본 인덱스 저장
			for (auto& pBonIndices : m_BoneIndices)
			{
				FileLoader.write((char*)&pBonIndices, sizeof(_uint));
			}


			//오프셋 매트릭스들 벡터 사이즈 저장
			size_t m_OffsetMatricesSize = m_OffsetMatrices.size();	//1
			FileLoader.write((char*)&m_OffsetMatricesSize, sizeof(size_t));

			//오프셋 매트릭스 저장
			for (auto& pffsetMatrices : m_OffsetMatrices)
			{
				FileLoader.write((char*)&pffsetMatrices, sizeof(_float4x4));
			}

		}





	
		
		
		//인덱스 버퍼 구조체 정보 저장 
		FileLoader.write((char*)&m_BufferDesc.ByteWidth, sizeof(UINT));
		FileLoader.write((char*)&m_BufferDesc.Usage, sizeof(D3D11_USAGE));
		FileLoader.write((char*)&m_BufferDesc.CPUAccessFlags, sizeof(UINT));
		FileLoader.write((char*)&m_BufferDesc.BindFlags, sizeof(D3D11_BIND_FLAG));
		FileLoader.write((char*)&m_BufferDesc.MiscFlags, sizeof(UINT));
		FileLoader.write((char*)&m_BufferDesc.StructureByteStride, sizeof(UINT));


		//인덱스 정점 정보 저장
		_uint		iNumIndices = { 0 };
		int Face = 0;
		Face = m_iNumIndices / 3;
		for (size_t i = 0; i < Face; i++)
		{
			FileLoader.write((char*)&m_pIndices[iNumIndices++], sizeof(_uint));
			FileLoader.write((char*)&m_pIndices[iNumIndices++], sizeof(_uint));
			FileLoader.write((char*)&m_pIndices[iNumIndices++], sizeof(_uint));
		
		}

		return S_OK;
	}
	else
		return E_FAIL;


}

HRESULT CMesh_Particle::Load_Data(ifstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		
		FileLoader.read(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.read((char*)&m_iMaterialIndex, sizeof(_uint));

		FileLoader.read((char*)&m_ePrimitiveTopology, sizeof(D3D11_PRIMITIVE_TOPOLOGY));
		FileLoader.read((char*)&m_iIndexFormat, sizeof(DXGI_FORMAT));
		FileLoader.read((char*)&m_iNumVertexBuffers, sizeof(_uint));

		FileLoader.read((char*)&m_iNumVertices, sizeof(_uint));
		FileLoader.read((char*)&m_iVertexStride, sizeof(_uint));

		FileLoader.read((char*)&m_iNumIndices, sizeof(_uint));
		FileLoader.read((char*)&m_iIndexStride, sizeof(_uint));


		FileLoader.read((char*)&m_eModelType, sizeof(CModel::MODELTYPE));

		if (m_eModelType == CModel::MODELTYPE::TYPE_NONANIM)
		{
			FileLoader.read((char*)&m_MeshBufferDesc, sizeof(D3D11_BUFFER_DESC));
			m_pNonAnimVertices = new VTXMESH[m_iNumVertices];
			ZeroMemory(m_pNonAnimVertices, sizeof(VTXMESH) * m_iNumVertices);
			for (size_t i = 0; i < m_iNumVertices; i++)
			{
				FileLoader.read((char*)&m_pNonAnimVertices[i].vPosition, sizeof(_float3));
				FileLoader.read((char*)&m_pNonAnimVertices[i].vNormal, sizeof(_float3));
				FileLoader.read((char*)&m_pNonAnimVertices[i].vTexcoord, sizeof(_float2));
				FileLoader.read((char*)&m_pNonAnimVertices[i].vTangent, sizeof(_float3));

			}
			m_InitialData.pSysMem = m_pNonAnimVertices;

			if (FAILED(__super::Create_Buffer_For_Mesh(&m_pVB)))
				return E_FAIL;

		}

		//정점 정보 불러오기
		else if (m_eModelType == CModel::MODELTYPE::TYPE_ANIM)
		{
			FileLoader.read((char*)&m_MeshBufferDesc, sizeof(D3D11_BUFFER_DESC));

			m_pAnimVertices = new VTXANIMMESH[m_iNumVertices];
			ZeroMemory(m_pAnimVertices, sizeof(VTXANIMMESH) * m_iNumVertices);
			for (size_t i = 0; i < m_iNumVertices; i++)
			{
				FileLoader.read((char*)&m_pAnimVertices[i].vPosition, sizeof(_float3));
				FileLoader.read((char*)&m_pAnimVertices[i].vNormal, sizeof(_float3));
				FileLoader.read((char*)&m_pAnimVertices[i].vTexcoord, sizeof(_float2));
				FileLoader.read((char*)&m_pAnimVertices[i].vTangent, sizeof(_float3));
				FileLoader.read((char*)&m_pAnimVertices[i].vBlendIndices, sizeof(XMUINT4));
				FileLoader.read((char*)&m_pAnimVertices[i].vBlendWeights, sizeof(XMFLOAT4));


			}

			FileLoader.read((char*)&m_iNumBones, sizeof(_uint));
			FileLoader.read((char*)&m_iNumWeights, sizeof(_uint));


			m_InitialData.pSysMem = m_pAnimVertices;
			if (FAILED(__super::Create_Buffer_For_Mesh(&m_pVB)))
				return E_FAIL;
			//본 인덱스들 벡터 사이즈 불러오기
			size_t m_BoneIndicesSize = 0;
			FileLoader.read((char*)&m_BoneIndicesSize, sizeof(size_t));
			m_BoneIndices.reserve(m_BoneIndicesSize);

			//본 인덱스 불러오기
			for (size_t i = 0; i < m_BoneIndicesSize; i++)
			{
				_uint pBonIndices = 0;
				FileLoader.read((char*)&pBonIndices, sizeof(_uint));
				m_BoneIndices.push_back(pBonIndices);
			}

			//오프셋 매트릭스들 벡터 사이즈 불러오기
			size_t m_OffsetMatricesSize = 0;
			FileLoader.read((char*)&m_OffsetMatricesSize, sizeof(size_t));
			m_BoneIndices.reserve(m_OffsetMatricesSize);

			//오프셋 매트릭스 불러오기
			for (size_t i = 0; i < m_OffsetMatricesSize; i++)
			{
				_float4x4 pffsetMatrices = {};
				FileLoader.read((char*)&pffsetMatrices, sizeof(_float4x4));
				m_OffsetMatrices.push_back(pffsetMatrices);
			}
		}




		
		
		


		//인덱스 정점 정보 불러오기
#pragma region INDEX_BUFFER 
		//인덱스 구조체 정보 불러오기
		FileLoader.read((char*)&m_BufferDesc.ByteWidth, sizeof(UINT));
		FileLoader.read((char*)&m_BufferDesc.Usage, sizeof(D3D11_USAGE));
		FileLoader.read((char*)&m_BufferDesc.CPUAccessFlags, sizeof(UINT));
		FileLoader.read((char*)&m_BufferDesc.BindFlags, sizeof(D3D11_BIND_FLAG));
		FileLoader.read((char*)&m_BufferDesc.MiscFlags, sizeof(UINT));
		FileLoader.read((char*)&m_BufferDesc.StructureByteStride, sizeof(UINT));


		m_pIndices = new _uint[m_iNumIndices];
		ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

		_uint		iNumIndices = { 0 };
		int Face = 0;
		Face = m_iNumIndices / 3;
		for (size_t i = 0; i < Face; i++)
		{
			FileLoader.read((char*)&m_pIndices[iNumIndices++], sizeof(_uint));
			FileLoader.read((char*)&m_pIndices[iNumIndices++], sizeof(_uint));
			FileLoader.read((char*)&m_pIndices[iNumIndices++], sizeof(_uint));
		}

		m_InitialData.pSysMem = m_pIndices;

		if (FAILED(__super::Create_Buffer(&m_pIB)))
			return E_FAIL;
	
#pragma endregion

		return S_OK;
	}


	else 
		return E_FAIL;
}

HRESULT CMesh_Particle::Ready_Vertices_For_NonAnimMesh(const aiMesh * pAIMesh, _fmatrix PreTransformMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);
	
	m_MeshBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_MeshBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_MeshBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_MeshBufferDesc.CPUAccessFlags = 0;
	m_MeshBufferDesc.MiscFlags = 0;
	m_MeshBufferDesc.StructureByteStride = m_iVertexStride;




	

	m_pNonAnimVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(m_pNonAnimVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_pNonAnimVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&m_pNonAnimVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&m_pNonAnimVertices[i].vPosition), PreTransformMatrix));

		memcpy(&m_pNonAnimVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&m_pNonAnimVertices[i].vNormal,
			XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_pNonAnimVertices[i].vNormal), PreTransformMatrix)));

		memcpy(&m_pNonAnimVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&m_pNonAnimVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}



	//m_pNonAnimVertices= pVertices;
	m_InitialData.pSysMem = m_pNonAnimVertices;

	if (FAILED(__super::Create_Buffer_For_Mesh(&m_pVB)))
		return E_FAIL;

	//Safe_Delete_Array(pVertices);

	return S_OK;
}


CMesh_Particle * CMesh_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CMesh* Meshs, _fmatrix PreTransformMatrix, const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc)
{
	CMesh_Particle*		pInstance = new CMesh_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Meshs, PreTransformMatrix, InstanceDesc)))
	{
		MSG_BOX("Failed To Created : CMesh_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh_Particle* CMesh_Particle::Load_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& FileLoader)
{
	CMesh_Particle* pInstance = new CMesh_Particle(pDevice, pContext);

	if (FAILED(pInstance->Load_Data(FileLoader)))
	{
		MSG_BOX("Failed To Load : CMesh_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CMesh_Particle* CMesh_Particle::Clone(void * pArg)
{

	CMesh_Particle* pInstance = new CMesh_Particle(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CMesh_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMesh_Particle::Free()
{

	Safe_Delete(m_pAnimVertices);
	Safe_Delete(m_pNonAnimVertices);
	Safe_Delete(m_pIndices);
	__super::Free();

}

