#include "..\Public\Mesh.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer { pDevice, pContext }
{
	
}

CMesh::CMesh(const CMesh & rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CMesh::Initialize(CModel::MODELTYPE eModelType, const aiMesh * pAIMesh, _fmatrix PreTransformMatrix, const vector<class CBone*>& Bones)
{
	//TODO 
	// 메쉬 머터리얼 갯수 이름이랑 똑같은지 비교해서 찾기
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iNumVertexBuffers = 1;
	
	m_iNumVertices = pAIMesh->mNumVertices;

	m_iIndexStride = 4;
	m_iNumFaces = pAIMesh->mNumFaces;
	m_iNumIndices = pAIMesh->mNumFaces * 3;

	m_eModelType = eModelType;

#pragma region VERTEX_BUFFER 

	HRESULT			hr = m_eModelType == CModel::TYPE_NONANIM ? Ready_Vertices_For_NonAnimMesh(pAIMesh, PreTransformMatrix) : Ready_Vertices_For_AnimMesh(pAIMesh, Bones);

	if (FAILED(hr))
		return E_FAIL;

	

#pragma endregion

#pragma region INDEX_BUFFER 
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	m_pIndices= new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		m_pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
		m_pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
		m_pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
	}

	_uint temp = m_pIndices[0];
	_uint temp2 = m_pIndices[iNumIndices-1];
	
	m_InitialData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;


#pragma endregion

	return S_OK;
}

void CMesh::Fill_Matrices(vector<class CBone*>& Bones, _float4x4 * pMeshBoneMatrices)
{
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&pMeshBoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * XMLoadFloat4x4(Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix()));
	}	
}

HRESULT CMesh::Save_Data( ofstream& FileLoader)
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

HRESULT CMesh::Load_Data(ifstream& FileLoader)
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

HRESULT CMesh::Ready_Vertices_For_NonAnimMesh(const aiMesh * pAIMesh, _fmatrix PreTransformMatrix)
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

HRESULT CMesh::Ready_Vertices_For_AnimMesh(const aiMesh * pAIMesh, const vector<class CBone*>& Bones)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	m_MeshBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_MeshBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_MeshBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_MeshBufferDesc.CPUAccessFlags = 0;
	m_MeshBufferDesc.MiscFlags = 0;
	m_MeshBufferDesc.StructureByteStride = m_iVertexStride;

	m_pAnimVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(m_pAnimVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_pAnimVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&m_pAnimVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&m_pAnimVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&m_pAnimVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	/* 이 메시에 영향을 주는 뼈의 갯수를 가져온다. */
	m_iNumBones = pAIMesh->mNumBones;

	/* 뼈들를 순회하면서 각각의 뼈가 어떤 정점들에게 영향을 주는가를 캐치한다. */
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* pAIBone = i번째 뼈다 */
		aiBone*		pAIBone = pAIMesh->mBones[i];

		/*pAIBone->mOffsetMatrix*/
		_float4x4		OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		
		m_OffsetMatrices.emplace_back(OffsetMatrix);

		_uint		iBoneIndex = { 0 };

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
		{
			if (true == pBone->Compare_Name(pAIBone->mName.data))
				return true;

			++iBoneIndex;

			return false;
		});

		
		m_BoneIndices.emplace_back(iBoneIndex);

		/* pAIBone->mNumWeights : i번째 뼈가 몇개의 정점들에게 영향을 주는가? */
		/* j루프 : 이 뼈가 영향을 주는 정점들을 순회한다. */
		m_iNumWeights = pAIBone->mNumWeights;
		for (size_t j = 0; j < m_iNumWeights; j++)
		{
			aiVertexWeight		VertexWeight = pAIBone->mWeights[j];

			/* VertexWeight.mVertexId : 이 뼈가 영향을 주는 정점들 중, j번째 정점의 인덱스 */
			/* pVertices[VertexWeight.mVertexId].vBlendIndices: 이 정점에게 영햐응ㄹ 주는 뼈의 인덱스를 최대 네개 저장한다. */
			if (0.0f == m_pAnimVertices[VertexWeight.mVertexId].vBlendWeights.x)
			{
				m_pAnimVertices[VertexWeight.mVertexId].vBlendIndices.x =static_cast<uint32_t>(i);
				m_pAnimVertices[VertexWeight.mVertexId].vBlendWeights.x = VertexWeight.mWeight;
			}

			else if (0.0f == m_pAnimVertices[VertexWeight.mVertexId].vBlendWeights.y)
			{
				m_pAnimVertices[VertexWeight.mVertexId].vBlendIndices.y = static_cast<uint32_t>(i);
				m_pAnimVertices[VertexWeight.mVertexId].vBlendWeights.y = VertexWeight.mWeight;
			}

			else if (0.0f == m_pAnimVertices[VertexWeight.mVertexId].vBlendWeights.z)
			{
				m_pAnimVertices[VertexWeight.mVertexId].vBlendIndices.z = static_cast<uint32_t>(i);
				m_pAnimVertices[VertexWeight.mVertexId].vBlendWeights.z = VertexWeight.mWeight;
			}

			else
			{
				m_pAnimVertices[VertexWeight.mVertexId].vBlendIndices.w = static_cast<uint32_t>(i);
				m_pAnimVertices[VertexWeight.mVertexId].vBlendWeights.w = VertexWeight.mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;


		_uint		iBoneIndex = { 0 };

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
		{
			if (true == pBone->Compare_Name(m_szName))
				return true;

			++iBoneIndex;

			return false;
		});

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrices.emplace_back(OffsetMatrix);

		m_BoneIndices.emplace_back(iBoneIndex);
	}

//	m_pAnimVertices= pVertices;
	m_InitialData.pSysMem = m_pAnimVertices;

	

	if (FAILED(__super::Create_Buffer_For_Mesh(&m_pVB)))
		return E_FAIL;

	//Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh * CMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::MODELTYPE eModelType, const aiMesh * pAIMesh, _fmatrix PreTransformMatrix, const vector<class CBone*>& Bones)
{
	CMesh*		pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eModelType, pAIMesh, PreTransformMatrix, Bones)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Load_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& FileLoader)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Load_Data(FileLoader)))
	{
		MSG_BOX("Failed To Load : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CComponent * CMesh::Clone(void * pArg)
{
	return nullptr;
}

void CMesh::Free()
{

	Safe_Delete(m_pAnimVertices);
	Safe_Delete(m_pNonAnimVertices);
	Safe_Delete(m_pIndices);
	__super::Free();

}

