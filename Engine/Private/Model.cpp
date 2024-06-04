#include "..\Public\Model.h"

#include "Bone.h"
#include "Mesh.h"
#include"Mesh_Particle.h"
#include "Shader.h"
#include "Texture.h"
#include"VIBuffer_Instance.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _char* pModelFilePath)
	: CComponent { pDevice, pContext }
	, m_sFilePath{ pModelFilePath }
{

}

CModel::CModel(const CModel & rhs)
	: CComponent{ rhs }
	, m_iNumMeshes{ rhs.m_iNumMeshes }
	, m_Meshes { rhs.m_Meshes } 
	, m_iNumMaterials{ rhs.m_iNumMaterials }
	, m_Materials { rhs.m_Materials }
	, m_eModelType{ rhs.m_eModelType }
	/*, m_Bones{ rhs.m_Bones }*/
	, m_PreTransformMatrix{ rhs.m_PreTransformMatrix }
	, m_iNumAnimations{rhs.m_iNumAnimations }
	/*, m_Animations{ rhs.m_Animations }	*/
	, m_sFilePath{rhs.m_sFilePath }
	//, m_ParticleMeshes{rhs.m_ParticleMeshes}
	
{
	
	

	for (auto& pPrototypeAnimation : rhs.m_Animations)
		m_Animations.emplace_back(pPrototypeAnimation->Clone());

	for (auto& pPrototypeBone : rhs.m_Bones)
		m_Bones.emplace_back(pPrototypeBone->Clone());		

	for (auto& MaterialDesc: m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(MaterialDesc.MaterialTextures[i]);
	}

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for(auto& pPrototypeParticleMesh : rhs.m_ParticleMeshes)
		m_ParticleMeshes.emplace_back(pPrototypeParticleMesh->Clone(nullptr));
	//for (auto& pMesh : m_ParticleMeshes)
	//	Safe_AddRef(pMesh);
	

}

const _float4x4* CModel::Get_BoneCombinedTransformationMatrix(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			return pBone->Compare_Name(pBoneName);
		});
	
	if (iter == m_Bones.end())
		return nullptr;

	return (*iter)->Get_CombinedTransformationMatrix();
}

HRESULT CModel::Initialize_Prototype(MODELTYPE eModelType, const _char * pModelFilePath,  _fmatrix PreTransformMatrix, vector<const _char*>* vpDeletename)
{	
	m_vpDeleteName = vpDeletename;



	if (load_Data())
	{
	
		return S_OK;
	}
	else
	{
		//MSG_BOX("Failed To Load_Data");
		//MSG_BOX("Start Save_Data");

		_uint		iFlag = 0;

		if (TYPE_NONANIM == eModelType)
			iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
		else
			iFlag = aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
			
			//iFlag = aiProcess_ConvertToLeftHanded |  aiProcess_GenNormals | aiProcess_CalcTangentSpace |
			//aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType | 0;
		
		//	_uint		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
		//
		//	if (TYPE_NONANIM == eModelType)
		//		iFlag |= aiProcess_PreTransformVertices;	
		//

		/* 메시들을 적당한뼈의 위치에 미리 다 배치시킨다. */
		m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

		if (nullptr == m_pAIScene)
			return E_FAIL;

		m_eModelType = eModelType;

		XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

		if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
			return E_FAIL;
	
		if (FAILED(Ready_Meshes()))
			return E_FAIL;
		
		//else
		//{
		//	if (FAILED(Ready_Meshes_Particle()))
		//		return E_FAIL;
		//}


		

		if (FAILED(Ready_Materials(pModelFilePath)))
			return E_FAIL;

		if (FAILED(Ready_Animations()))
			return E_FAIL;

		Save_Data();


		return S_OK;
	}
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath, const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc)
{
	if (load_Data())
	{
		if (FAILED(Ready_Meshes_Particle(InstanceDesc)))
			return E_FAIL;
	}
	else
		return E_FAIL;


	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	


	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{	
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();
	
	return S_OK;
}

HRESULT CModel::Render_Instance(_uint iMeshIndex)
{
	m_ParticleMeshes[iMeshIndex]->Bind_Buffers();
	m_ParticleMeshes[iMeshIndex]->Render();

	return S_OK;
}

const CVIBuffer_Instance::INSTANCE_DESC CModel::Get_ParticleProperty()
{
	for(auto& iter: m_ParticleMeshes)
		return iter->Get_InstanceBufferDesc();
}

void CModel::Set_ParticleProperty(const CVIBuffer_Instance::INSTANCE_DESC& ParticleDesc)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Set_ParticleProperty(ParticleDesc);
}

void CModel::Set_ParticleTpye(_uint IndexType)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Set_ParticleType(IndexType);

}

void CModel::Spread(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Spread(fTimeDelta);

}

void CModel::Drop(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Drop(fTimeDelta);
}

void CModel::Explosion(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Explosion(fTimeDelta);
}

void CModel::Up_And_Down(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Up_And_Down(fTimeDelta);
}

void CModel::Pop(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Pop(fTimeDelta);
}

void CModel::RandomPop(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->RandomPop(fTimeDelta);
}

void CModel::RandRotate_SizeUp(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->RandRotate_SizeUp(fTimeDelta);
}

void CModel::SizeUp(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeUp(fTimeDelta);
}

void CModel::SizeDown(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeDown(fTimeDelta);
}

void CModel::SizeDown_Y(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeDown_Y(fTimeDelta);

}

void CModel::SizeDown_X(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeDown_X(fTimeDelta);
}

void CModel::SizeDown_Z(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeDown_Z(fTimeDelta);
}

void CModel::SizeUp_Y(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeUp_Y(fTimeDelta);
}

void CModel::SizeUp_X(_float fTimeDelta)
{
for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeUp_X(fTimeDelta);
}

void CModel::SizeUp_Z(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->SizeUp_Z(fTimeDelta);
}

void CModel::Rotation_Y(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Rotation_Y(fTimeDelta);
}

void CModel::Rotation_X(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Rotation_X(fTimeDelta);
}

void CModel::Rotation_Z(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Rotation_Z(fTimeDelta);
}

void CModel::Roation_Orbit(_float fTimeDelta)
{
	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Roation_Orbit(fTimeDelta);
}

CVIBuffer_Instance::INSTANCE_DESC CModel::Get_ParticleDesc()
{
	for (auto& pMesh : m_ParticleMeshes)
		return pMesh->Get_InstanceBufferDesc();
}

HRESULT CModel::Render_Particle()
{

	for (auto& pMesh : m_ParticleMeshes)
		pMesh->Render();
	return S_OK;
}

HRESULT CModel::Bind_Buffer()
{
	for(auto& pMesh : m_ParticleMeshes)
		pMesh->Bind_Buffers();
	return S_OK;
}

HRESULT CModel::Bind_Material(CShader * pShaderCom, const _char * pConstantName, _uint iMeshIndex, aiTextureType eMaterialType)
{
	return m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].MaterialTextures[eMaterialType]->Bind_ShaderResource(pShaderCom, pConstantName, 0);
	
}

HRESULT CModel::Bind_Material_Instance(CShader* pShaderCom, const _char* pConstantName, _uint iMeshIndex, aiTextureType eMaterialType)
{
	return m_Materials[m_ParticleMeshes[iMeshIndex]->Get_MaterialIndex()].MaterialTextures[eMaterialType]->Bind_ShaderResource(pShaderCom, pConstantName, 0);
}

HRESULT CModel::Bind_BoneMatrices(CShader * pShaderCom, const _char * pConstantName, _uint iMeshIndex)
{
	ZeroMemory(m_MeshBoneMatrices, sizeof(_float4x4) * 512);

	m_Meshes[iMeshIndex]->Fill_Matrices(m_Bones, m_MeshBoneMatrices);

	return pShaderCom->Bind_Matrices(pConstantName, m_MeshBoneMatrices, 512);		
}

void CModel::Play_Animation(_float fTimeDelta)
{
	/* 특정 애니메이션을 재생한다. == 특정 애니메이션에서 사용하는 뼈들의 TransformationMatrix를 갱신해준다. */
	/* 현재 애니메이션의 상태에 맞도록 뼈들의 상태행렬(TransformationMatrix)을 만들고 갱신해준다. */ 
	/* m_Animations[m_iCurrentAnimIndex] : 이 애니메이션에서 사용ㅇ하는 뼈들의 상태정보다 */
	_uint tmpiAnimIndex = 0;
	if(!m_Animations.empty())
		m_Animations[m_AnimDesc.iAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, m_AnimDesc.isLoop, &tmpiAnimIndex, m_dDuration_Ratio, m_IsAccept);
	
	/* 전체뼈를 순회하면서 모든 뼈의 CombinedTransformationMatrix를 갱신한다. */
	for (auto& pBone : m_Bones)
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
}

HRESULT CModel::Ready_Meshes()
{
	/* 하나의 모델은 여러개의 메시로 구성되어있다. */
	/* 메시 : 폴리곤의 집합. */
	/* 폴리곤 : 정점 세개. */

	/* -> 메시를 만든다 == 정점버퍼를 만든다. + 인덱스 */
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		_bool bSkipMesh = false;

		if (m_vpDeleteName != nullptr)
		{
			_char			m_Test[MAX_PATH] = "";
			strcpy_s(m_Test, m_pAIScene->mMeshes[i]->mName.data);
			for (auto& pDeleteName : *m_vpDeleteName)
			{
				if (!strcmp(m_Test, pDeleteName))
				{
					bSkipMesh = true;
					break;
				}
			}
		}
		//_char			m_Test[MAX_PATH] = "";
		//strcpy_s(m_Test, m_pAIScene->mMeshes[i]->mName.data);
		////	"Bip001 Head"
		//// "Amanda_Face"
		//if (!strcmp(m_Test, "Amanda_Face")|| !strcmp(m_Test, "Matilda_Face"))
		//{
		//	
		//	m_iNumMeshes--;
		// m_pAIScene->mMeshes[i]->mName.data = 0x000001277692c4d0 "Matilda_Body_Outline"
		//	continue;
		//}
		if (bSkipMesh)
		{
			m_iNumMeshes--;
			continue;
		}
	
		CMesh*		pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix), m_Bones);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes_Particle(const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc)
{

	
	for (size_t i = 0; i < m_iNumMeshes; i++)
	{

	
		CMesh_Particle* pMesh = CMesh_Particle::Create(m_pDevice, m_pContext, m_Meshes[i], XMLoadFloat4x4(&m_PreTransformMatrix), InstanceDesc);
		if (nullptr == pMesh)
			return E_FAIL;
	
		m_ParticleMeshes.emplace_back(pMesh);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;


	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		aiMaterial*		pAIMaterial = m_pAIScene->mMaterials[i];

		MESH_MATERIAL		MeshMaterial{};
		int nullTextureCount(0);
		

		for (size_t j = aiTextureType_NONE; j < AI_TEXTURE_TYPE_MAX; j++)
		{
		
			

			aiString		strTextureFilePath;
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
			{
				nullTextureCount++;
				continue;
			}
				

			_char		szFileName[MAX_PATH] = "";
			_char		szExt[MAX_PATH] = "";

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);			

			_char		szDrive[MAX_PATH] = "";
			_char		szDirectory[MAX_PATH] = "";

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			_char		szFullPath[MAX_PATH] = "";

			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDirectory);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar		szRealFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szRealFullPath, MAX_PATH);
			//strcpy_s(m_szRealFullPath, MAX_PATH, szRealFullPath);
			MeshMaterial.MaterialTextures[j] = CTexture::Create(m_pDevice, m_pContext, szRealFullPath, 1);
			if (nullptr == MeshMaterial.MaterialTextures[j])
				return E_FAIL;
		}


		//디퓨즈 강제로 적용
		if (nullTextureCount == AI_TEXTURE_TYPE_MAX)
		{
			MeshMaterial.MaterialTextures[aiTextureType_DIFFUSE] = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/DumpPng/Dummy_Alpha.png"), 1);
			//MeshMaterial.MaterialTextures[aiTextureType_DIFFUSE] = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/DumpPng/Rect.png"), 1);
			
		//	continue;
		}
		
		m_Materials.emplace_back(MeshMaterial);
		
	}

	//m_iNumMaterials -= nullMaterialCount;


	return S_OK;
}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentIndex)
{
	_char			pszNotLoadBonename[MAX_PATH] = "";
	strcpy_s(pszNotLoadBonename, pAINode->mName.data);
	//	"Bip001 Head"
	// "Amanda_Face"
	if (!strcmp(pszNotLoadBonename, "Amanda_Face")|| !strcmp(pszNotLoadBonename, "Matilda_Face"))
	{
		return S_OK;
	}
	if (!strcmp(pszNotLoadBonename, "Bone Weapon003"))
	{
		int tmep = 0;
	}

	CBone*		pBone = CBone::Create(pAINode, iParentIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.emplace_back(pBone);

	_uint		iParent = m_Bones.size() - 1;

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
	{
		
		Ready_Bones(pAINode->mChildren[i], iParent);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{	
	/* 어떤 애니메이션을 재생했을 때, 이 애니메이션에서 사용하고 있는 뼈들의 각각의 변환 상태(행렬)들을 로드한다.  */
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CAnimation*		pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], m_Bones);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.emplace_back(pAnimation);
	}

	return S_OK;
}








void CModel::Save_Data()
{

	ofstream outFile(m_sFilePath,ios::out| std::ios::binary);

	if (outFile.is_open())
	{
		outFile.write((char*)&m_PreTransformMatrix, sizeof(_float4x4));
		outFile.write((char*)&m_AnimDesc, sizeof(ANIMATION_DESC));
		//본 벡터 사이즈 저장
		size_t SaveBonesSize = m_Bones.size();
		outFile.write((char*)&SaveBonesSize, sizeof(size_t));


		//본 정보 저장
		for (auto& pBones : m_Bones)
		{
			pBones->Save_Data(outFile);
		}


		//메쉬 갯수 저장
		outFile.write((char*)&m_iNumMeshes, sizeof(_uint));

		//메쉬 정보 저장
		for (auto& pMeshes : m_Meshes)
		{
			pMeshes->Save_Data(outFile);
		}


		//머터리얼 갯수 저장
		outFile.write((char*)&m_iNumMaterials, sizeof(_uint));

		//머터리얼 정보 저장
		for (auto& pMaterials : m_Materials)
		{
			for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
			{
				bool IsNull = false;
				if (nullptr== pMaterials.MaterialTextures[i])
				{
					IsNull = true;
					outFile.write((char*)&IsNull, sizeof(bool));
					continue;
				}
				else
				{
					IsNull = false;
					outFile.write((char*)&IsNull, sizeof(bool));
					pMaterials.MaterialTextures[i]->Save_Data(outFile);
				}

			}
		}

		//애니메이션 갯수 저장
		outFile.write((char*)&m_iNumAnimations, sizeof(_uint));

		//애니메이션 정보 저장
		for (auto& pAnimation : m_Animations)
		{

			pAnimation->Save_Data(outFile);
		}



		outFile.close();
	}
	

}

bool CModel::load_Data()
{

	size_t tmp = m_sFilePath.find(".fbx");

	m_sFilePath = m_sFilePath.substr(0, tmp) + ".bin";

	ifstream  inFile(m_sFilePath, ios::in | std::ios::binary);
	if (inFile.is_open())
	{

		inFile.read((char*)&m_PreTransformMatrix, sizeof(_float4x4));
		inFile.read((char*)&m_AnimDesc, sizeof(ANIMATION_DESC));
		//본 벡터 사이즈 읽기
		size_t LoadBonesSize = 0;
		inFile.read((char*)&LoadBonesSize, sizeof(size_t));
		m_Bones.reserve(LoadBonesSize);
		
		//본 정보 읽기
		for (size_t i = 0; i < LoadBonesSize; i++)
		{
			CBone* pBone = CBone::Load_Create(inFile);
			if (nullptr == pBone)
				return false;

			m_Bones.emplace_back(pBone);
		}
		
		//메쉬 개수 읽기
		inFile.read((char*)&m_iNumMeshes, sizeof(_uint));
		

		//메쉬 정보 읽기
		m_Meshes.reserve(m_iNumMeshes);
		for (size_t i = 0; i < m_iNumMeshes; i++)
		{

			CMesh* pMesh = CMesh::Load_Create(m_pDevice, m_pContext, inFile);
			if (nullptr == pMesh)
				return false;

			m_Meshes.emplace_back(pMesh);

		}
		

		//머터리얼 갯수 읽기
		inFile.read((char*)&m_iNumMaterials, sizeof(_uint));
		m_Materials.reserve(m_iNumMaterials);
		for (size_t i = 0; i < m_iNumMaterials; i++)
		{
			MESH_MATERIAL		MeshMaterial{};
			for (size_t j = aiTextureType_NONE; j < AI_TEXTURE_TYPE_MAX; j++)
			{
				bool IsNull = false;
				inFile.read((char*)&IsNull, sizeof(bool));
				if (IsNull)
				{
					continue;
				}
				MeshMaterial.MaterialTextures[j] = CTexture::Load_Create(m_pDevice, m_pContext,1, inFile);
			}
			
			m_Materials.emplace_back(MeshMaterial);
		}

		//애니메이션 갯수 불러오기
		inFile.read((char*)&m_iNumAnimations, sizeof(_uint));
		m_Animations.reserve(m_iNumAnimations);
		for (size_t i = 0; i < m_iNumAnimations; i++)
		{
			CAnimation* pAnimation = CAnimation::Load_Create(inFile);
			if (nullptr == pAnimation)
				return false;
			m_Animations.push_back(pAnimation);
		}



		inFile.close();

		return true;
	}
	else
	{
		return false;
	}

	
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, vector<const _char*>* vpDeletename)
{
	CModel* pInstance = new CModel(pDevice, pContext, pModelFilePath);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix, vpDeletename)))
	{
		MSG_BOX("Failed To Created : CModel");

		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, const CVIBuffer_Instance::INSTANCE_DESC& InstanceDesc)
{
	CModel* pInstance = new CModel(pDevice, pContext, pModelFilePath);

	if (FAILED(pInstance->Initialize_Prototype( pModelFilePath, InstanceDesc)))
	{
		MSG_BOX("Failed To Created : CModel");

		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (auto& MaterialDesc : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(MaterialDesc.MaterialTextures[i]);
	}

	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

	for (auto& pMesh : m_ParticleMeshes)
		Safe_Release(pMesh);
	m_ParticleMeshes.clear();
	

	m_Importer.FreeScene();
}
