#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}
	_uint Get_VertexCount() const {
		return m_iNumVertices;
	}
	_uint Get_NumFaces() const {
		return m_iNumFaces;
	}
	VTXMESH* Get_Vertices() const {
		return m_pNonAnimVertices;
	}
	_uint* Get_Indices() const {
		return m_pIndices;
	}
	_uint Get_NumIndices() const {
		return m_iNumIndices;
	}

public:
	HRESULT Initialize(CModel::MODELTYPE eModelType, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, const vector<class CBone*>& Bones);

	void Fill_Matrices(vector<class CBone*>& Bones, _float4x4* pMeshBoneMatrices);
	HRESULT Save_Data( ofstream& FileLoader);
	HRESULT Load_Data(ifstream& FileLoader);
private:
	_char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = { 0 };
	_uint				m_iNumBones = { 0 }; /* 이 메시에 영향을 주는 뼈의 갯수 */
	_uint				m_iNumWeights = { 0 }; 
	_uint				m_iNumFaces = { 0 };
	vector<_uint>		m_BoneIndices;
	vector<_float4x4>	m_OffsetMatrices;



	//for.Loader
	CModel::MODELTYPE			m_eModelType = CModel::TYPE_END;
	VTXANIMMESH* m_pAnimVertices = nullptr;
	VTXMESH* m_pNonAnimVertices = nullptr;
	_uint* m_pIndices = nullptr;

private:
	HRESULT Ready_Vertices_For_NonAnimMesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Ready_Vertices_For_AnimMesh(const aiMesh* pAIMesh, const vector<class CBone*>& Bones);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::MODELTYPE eModelType, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, const vector<class CBone*>& Bones);
	static CMesh* Load_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& FileLoader);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END