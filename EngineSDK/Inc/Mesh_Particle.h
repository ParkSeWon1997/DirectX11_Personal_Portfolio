#pragma once

#include "VIBuffer_Instance.h"
#include "Model.h"

BEGIN(Engine)

class CMesh_Particle final : public CVIBuffer_Instance
{
private:
	CMesh_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh_Particle(const CMesh_Particle& rhs);
	virtual ~CMesh_Particle() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

/// <MyData>
public:
	const INSTANCE_DESC Get_InstanceBufferDesc() const {
		return m_InstanceDesc;
	}

/// </MyData>

public:
	HRESULT Initialize(const CMesh* Meshs, _fmatrix PreTransformMatrix, const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc);
	virtual HRESULT Initialize(void* pArg) override;

	void Fill_Matrices(vector<class CBone*>& Bones, _float4x4* pMeshBoneMatrices);
	HRESULT Save_Data( ofstream& FileLoader);
	HRESULT Load_Data(ifstream& FileLoader);
private:
	_char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = { 0 };
	_uint				m_iNumBones = { 0 }; /* 이 메시에 영향을 주는 뼈의 갯수 */
	_uint				m_iNumWeights = { 0 }; 
	vector<_uint>		m_BoneIndices;
	vector<_float4x4>	m_OffsetMatrices;



	//for.Loader
	CModel::MODELTYPE			m_eModelType = CModel::TYPE_END;
	VTXANIMMESH* m_pAnimVertices = nullptr;
	VTXMESH* m_pNonAnimVertices = nullptr;
	_uint* m_pIndices = nullptr;

private:
	HRESULT Ready_Vertices_For_NonAnimMesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
public:
	static CMesh_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const CMesh* Meshs, _fmatrix PreTransformMatrix,const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc);
	static CMesh_Particle* Load_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& FileLoader);
	CMesh_Particle* Clone(void* pArg);
	virtual void Free() override;
};

END