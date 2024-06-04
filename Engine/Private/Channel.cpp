#include "..\Public\Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim * pAIChannel, const vector<class CBone*>& Bones)
{	
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	_uint		iBoneIndex = { 0 };

	auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
	{
		if (true == pBone->Compare_Name(m_szName))
			return true;

		++iBoneIndex;

		return false;
	});

	m_iBoneIndex = iBoneIndex;

	/* 이 뼈의 전체 애니메이션 과정 중, 특정 시간대에 취해야할 상태들을 모두 다 읽어서 저장해놓는다.*/

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	KEYFRAME			KeyFrame{};
	
	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&KeyFrame.vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pAIChannel->mScalingKeys[i].mTime;
		}		
		if (pAIChannel->mNumRotationKeys > i)
		{
			KeyFrame.vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			KeyFrame.vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			KeyFrame.vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			KeyFrame.vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.Time = pAIChannel->mRotationKeys[i].mTime;
		}
		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&KeyFrame.vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pAIChannel->mPositionKeys[i].mTime;
		}

		m_KeyFrames.emplace_back(KeyFrame);
	}
	
	return S_OK;
}

void CChannel::Update_TransformationMatrix(_double CurrentPosition, const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex)
{
	if (0.0 == CurrentPosition)
		*pCurrentKeyFrameIndex = 0;

	


	KEYFRAME		LastKeyFrame = m_KeyFrames.back();
	

	
	_vector			vScale, vRotation, vTranslation;
	_float ftmpRatio = 0.f;
	/* 현재 재생위치가 마지막 키프레임의 위치를 넘어가게되면 무조건 마지막 키프레임의 상태를 취한다.  */
	if (CurrentPosition >= LastKeyFrame.Time)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vPosition), 1.f);		
	}
	/* 특정 키프레임들 사이에 있을거다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	else
	{
		while (CurrentPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].Time)
		{
			++*pCurrentKeyFrameIndex;
		}

		m_iCurrentKeyFrameIndex = *pCurrentKeyFrameIndex;
	


		/* 현재 위치에서 왼쪽에 있는 키프렝미의 위치를 뺀다. / 내 오른쪽 키르렝미의 위치 - 내 왼쪽 키프렝밍의 위치. */
		_float		fRatio = (float)(CurrentPosition - m_KeyFrames[*pCurrentKeyFrameIndex].Time) / (float)(m_KeyFrames[*pCurrentKeyFrameIndex + 1].Time - m_KeyFrames[*pCurrentKeyFrameIndex].Time);
		//_float      fMatrixRatio= (0.2 - m_KeyFrames[*pPreKeyFrameIndex].Time) / 0.2f;

		vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation), fRatio);
		vTranslation = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vPosition), XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vPosition), fRatio);
		vTranslation = XMVectorSetW(vTranslation, 1.f);
	}

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}



HRESULT CChannel::Save_Data(ofstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		FileLoader.write(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.write((char*)&m_iBoneIndex, sizeof(_uint));

		FileLoader.write((char*)&m_iNumKeyFrames, sizeof(_uint));
		for(auto& pKeyFrames : m_KeyFrames)
			FileLoader.write((char*)&pKeyFrames, sizeof(KEYFRAME));





		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CChannel::Load_Data(ifstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		FileLoader.read(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.read((char*)&m_iBoneIndex, sizeof(_uint));

		FileLoader.read((char*)&m_iNumKeyFrames, sizeof(_uint));
		m_KeyFrames.reserve(m_iNumKeyFrames);

		KEYFRAME			KeyFrame{};
		for (size_t i = 0; i < m_iNumKeyFrames; i++)
		{
			FileLoader.read((char*)&KeyFrame, sizeof(KEYFRAME));
			m_KeyFrames.push_back(KeyFrame);
		}

		//for (auto& pKeyFrames : m_KeyFrames)
		//	FileLoader.read((char*)&pKeyFrames, sizeof(KEYFRAME));
		




		return S_OK;
	}
	else
		return E_FAIL;
}

CChannel * CChannel::Create(const aiNodeAnim * pAIChannel, const vector<class CBone*>& Bones)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, Bones)))
	{
		MSG_BOX("Failed To Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Load_Create(ifstream& FileLoader)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Load_Data(FileLoader)))
	{
		MSG_BOX("Failed To Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CChannel::Free()
{
	m_KeyFrames.clear();
}