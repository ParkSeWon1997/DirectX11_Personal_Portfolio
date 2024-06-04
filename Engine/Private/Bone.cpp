#include "..\Public\Bone.h"




CBone::CBone()
{

}

HRESULT CBone::Initialize(const aiNode * pBoneNode, _int iParentIndex)
{

	//= 0x000002216247e980 {mName = { length = 13 data = 0x000002216247e984 "PlayerManager" } mTransformation = { a1 = 1.00000000 ... } ...}
//- 0x00000221627520b0 {mName = { length = 12 data = 0x00000221627520b4 "Actor_Player" } mTransformation = { a1 = -1.00000000 ... } ...}	aiNode*
//	+0x0000022162753dd0 {mName = { length = 19 data = 0x0000022162753dd4 "ActorBody_NewPlayer" } mTransformation = ...}	aiNode*
//	+0x00000221627552e0 {mName = { length = 9 data = 0x00000221627552e4 "newAmanda" } mTransformation = { a1 = 1.00000000 ... } ...}	aiNode*
//	+0x00000221627567f0 {mName = { length = 7 data = 0x00000221627567f4 "_Avatar" } mTransformation = { a1 = 0.000150600943 ... } ...}	aiNode*
//	+0x0000022162758dd0 {mName = { length = 8 data = 0x0000022162758dd4 "BoneRoot" } mTransformation = { a1 = 1.00000000 ... } ...}	aiNode*

	strcpy_s(m_szName, pBoneNode->mName.data);
	
	m_iParentBoneIndex = iParentIndex;
	//"RootNode"
	/*  */
	memcpy(&m_TransformationMatrix, &pBoneNode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	if (Compare_Name("Actor_Player"))
	{
		m_TransformationMatrix._41 = 0.0f;
		m_TransformationMatrix._42 = 0.0f; 
		m_TransformationMatrix._43 = 0.0f;
	}

	
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	if(-1 == m_iParentBoneIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);

	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	}
}

HRESULT CBone::Save_Data( ofstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		FileLoader.write(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.write((char*)&m_iParentBoneIndex, sizeof(_int));
		FileLoader.write((char*)&m_TransformationMatrix, sizeof(_float4x4));

		return S_OK;
	}
	else
		return E_FAIL;

}

HRESULT CBone::Load_Data( ifstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		FileLoader.read(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.read((char*)&m_iParentBoneIndex, sizeof(_uint));
		FileLoader.read((char*)&m_TransformationMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
		return S_OK;
	}
	else
		return E_FAIL;
}


CBone * CBone::Create(const aiNode * pBoneNode, _int iParentIndex)
{
	CBone*		pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pBoneNode, iParentIndex)))
	{
		MSG_BOX("Failed To Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Load_Create(ifstream& FileLoader)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Load_Data(FileLoader)))
	{
		MSG_BOX("Failed To Load: CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone * CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
}

