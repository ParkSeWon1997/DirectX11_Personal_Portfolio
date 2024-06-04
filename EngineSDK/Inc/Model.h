#pragma once

#include "Component.h"
#include "Animation.h"
#include"VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODELTYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

	typedef struct tagAnimationDesc
	{
		tagAnimationDesc() {};
		tagAnimationDesc(_uint iAnimIndex, _bool isLoop)
			: iAnimIndex{ iAnimIndex }, isLoop{ isLoop } {}
		_uint		iAnimIndex = { 0 };
		_uint		iPreAnimIndex = { 0 };
		_bool		isLoop = { false };
	}ANIMATION_DESC;
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_bool Get_AnimFinished() const {
		return m_Animations[m_AnimDesc.iAnimIndex]->Get_Finished();
	}

	const _float4x4* Get_BoneCombinedTransformationMatrix(const _char* pBoneName) const;

	int Get_AnimationCount() {
		return m_Animations.size();
	}
	_uint Get_AnimationIndex() {
		return m_AnimDesc.iAnimIndex;
	}
	_double Get_FrameRatio(_uint Frame) {
		return m_Animations[m_AnimDesc.iAnimIndex]->Calculate_SelectedKeyFrameTime(Frame);
	}
	_double Get_CurrentPosition() {
		return m_Animations[m_AnimDesc.iAnimIndex]->Get_CurrentPosition();
	}


public:
	virtual HRESULT	Initialize_Prototype(MODELTYPE eModelType, const _char* pModelFilePath,_fmatrix PreTransformMatrix, vector<const _char*>* vpDeletename);
	virtual HRESULT	Initialize_Prototype( const _char* pModelFilePath, const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc);
	virtual HRESULT	Initialize(void* pArg) override;
	HRESULT Render(_uint iMeshIndex);
	HRESULT Render_Instance(_uint iMeshIndex);



	const CVIBuffer_Instance::INSTANCE_DESC Get_ParticleProperty();
	void Set_ParticleProperty(const CVIBuffer_Instance::INSTANCE_DESC& ParticleDesc);
	void Set_ParticleTpye(_uint IndexType);
	void Spread(_float fTimeDelta);
	void Drop(_float fTimeDelta);
	void Explosion(_float fTimeDelta);
	void Up_And_Down(_float fTimeDelta);
	void Pop(_float fTimeDelta);
	void RandomPop(_float fTimeDelta);
	void RandRotate_SizeUp(_float fTimeDelta);
	void SizeUp(_float fTimeDelta);
	void SizeDown(_float fTimeDelta);
	void SizeDown_Y(_float fTimeDelta);
	void SizeDown_X(_float fTimeDelta);
	void SizeDown_Z(_float fTimeDelta);
	void SizeUp_Y(_float fTimeDelta);
	void SizeUp_X(_float fTimeDelta);
	void SizeUp_Z(_float fTimeDelta);



	void Rotation_Y(_float fTimeDelta);
	void Rotation_X(_float fTimeDelta);
	void Rotation_Z(_float fTimeDelta);
	void Roation_Orbit(_float fTimeDelta);


	CVIBuffer_Instance::INSTANCE_DESC Get_ParticleDesc();
	HRESULT Render_Particle();
	HRESULT Bind_Buffer();





	HRESULT Bind_Material(class CShader* pShaderCom, const _char* pConstantName, _uint iMeshIndex, aiTextureType eMaterialType);
	HRESULT Bind_Material_Instance(class CShader* pShaderCom, const _char* pConstantName, _uint iMeshIndex, aiTextureType eMaterialType);
	HRESULT Bind_BoneMatrices(class CShader* pShaderCom, const _char* pConstantName, _uint iMeshIndex);
	void Play_Animation(_float fTimeDelta);
	void Set_AnimationIndex(const ANIMATION_DESC& AnimDesc) {
		if (m_AnimDesc.iAnimIndex == AnimDesc.iAnimIndex)
			return;
		m_AnimDesc = AnimDesc;
		m_Animations[m_AnimDesc.iAnimIndex]->Reset();
	}
	void Set_AnimationIndex(_uint iAnimIndex) {
		m_AnimDesc.iAnimIndex = iAnimIndex;
		m_Animations[m_AnimDesc.iAnimIndex]->Reset();
	}
	void Set_Duration_Ratio(_double dDuration_Ratio) {
		m_dDuration_Ratio = dDuration_Ratio;
	}
	_bool IsAccept() {
		return m_IsAccept;
	}
	void Reset_Animation() {
		m_Animations[m_AnimDesc.iAnimIndex]->Reset();
	}

private:
	const aiScene*				m_pAIScene = { nullptr };
	Assimp::Importer			m_Importer;
	_float4x4					m_PreTransformMatrix;
	MODELTYPE					m_eModelType = { TYPE_END };
	_float4x4					m_MeshBoneMatrices[512];

	string						m_sFilePath= nullptr ;

	vector<const _char*>*				m_vpDeleteName = nullptr;	
private:
	_uint						m_iNumMeshes = { 0 };	
	vector<class CMesh*>		m_Meshes;
	vector<class CMesh_Particle*>		m_ParticleMeshes;
private:
	_uint						m_iNumMaterials = { 0 };
	vector<MESH_MATERIAL>		m_Materials;

private:
	vector<class CBone*>		m_Bones;

private:
	_uint						m_iNumAnimations = { 0 };
	ANIMATION_DESC				m_AnimDesc{0, false};
	vector<class CAnimation*>	m_Animations;
	
	_double						m_dDuration_Ratio = -1.0;
	_bool						m_IsAccept = false;
	void Save_Data();
	bool load_Data();


private:
	HRESULT Ready_Meshes();
	HRESULT Ready_Meshes_Particle(const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc);

	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(const aiNode* pAINode, _int iParentIndex);
	HRESULT Ready_Animations();


public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix = _fmatrix(), vector<const _char*>* vpDeletename = nullptr);
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, const  CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END