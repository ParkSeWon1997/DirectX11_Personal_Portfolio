#include "..\Public\Animation.h"
#include "Channel.h"



CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation & rhs)
	: m_Duration{rhs.m_Duration }
	, m_TickPerSecond{ rhs.m_TickPerSecond }
	, m_CurrentPosition{ rhs.m_CurrentPosition }
	, m_isFinished{ rhs.m_isFinished }
	, m_iNumChannels{ rhs.m_iNumChannels }
	, m_CurrentKeyFrameIndices{ rhs.m_CurrentKeyFrameIndices }
	, m_Channels{ rhs.m_Channels }
{
	strcpy_s(m_szName, rhs.m_szName);

	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;
	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	/* 이 애니메이션을 구동하는 필요한 뼈의 갯수 */
	m_iNumChannels = pAIAnimation->mNumChannels;

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel*		pChannel = CChannel::Create(pAIAnimation->mChannels[i], Bones);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.emplace_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _bool isLoop, _uint* pCurKeyFrame, _double DurationRatio, _bool& IsAccept)
{
	m_CurrentPosition += m_TickPerSecond * fTimeDelta;
	if (DurationRatio != -1)
	{
		if (m_CurrentPosition >= m_Duration * DurationRatio)
		{
			IsAccept= true;
		}
	}

	for (auto& pChannel : m_Channels)
	{
		pChannel->Set_CurKeyFrameindex(&m_iCurrentKeyFrame);
	}




	if (m_CurrentPosition >= m_Duration)
	{
		m_CurrentPosition = 0.0;

		if(false == isLoop)
			m_isFinished = true;		
	}

	if (false == m_isFinished)
	{
		m_iCurrentKeyFrame = 0;
		_uint		iChannelIndex = { 0 };
		for (auto& pChannel : m_Channels)
		{
			pChannel->Update_TransformationMatrix(m_CurrentPosition, Bones, &m_CurrentKeyFrameIndices[iChannelIndex++]);
			pChannel->Set_CurKeyFrameindex(&m_iCurrentKeyFrame);
			pCurKeyFrame = &iChannelIndex;
		}
		

	}
}

_double CAnimation::Calculate_SelectedKeyFrameTime(_uint iFrame)
{
	_uint iMaxKeyFrame{ 0 };
	_uint iChannelIndex{ 9999 };


	int count{ 0 };
	for (size_t i = 0; i < m_Channels.size(); i++)
	{
		if (m_Channels[i] == nullptr)
			continue;

		if (count == m_iNumChannels)
			break;

		if (iMaxKeyFrame <= m_Channels[i]->Get_NumKeyFrames())
		{
			iMaxKeyFrame = m_Channels[i]->Get_NumKeyFrames();
			iChannelIndex = static_cast<_uint>(i);
		}
		count++;
	}


	if (iChannelIndex == 9999)
	{
		MSG_BOX("Something Wrong...");
		return -9999.0;
	}
	KEYFRAME KeyFrame = m_Channels[iChannelIndex]->Get_KeyFrame(iFrame);
	
	return m_Channels[iChannelIndex]->Get_KeyFrame(iFrame).Time;
}






void CAnimation::Reset()
{
	m_CurrentPosition = 0.0;
	m_isFinished = false;
	ZeroMemory(&m_CurrentKeyFrameIndices.front(), sizeof(_uint) * m_iNumChannels);

}

HRESULT CAnimation::Save_Data(ofstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		FileLoader.write(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.write((char*)&m_Duration, sizeof(_double));
		FileLoader.write((char*)&m_TickPerSecond, sizeof(_double));
		FileLoader.write((char*)&m_CurrentPosition, sizeof(_double));
		FileLoader.write((char*)&m_isFinished, sizeof(_bool));
		FileLoader.write((char*)&m_iNumChannels, sizeof(_uint));



		

		for (auto& pChannels : m_Channels)
		{
			pChannels->Save_Data(FileLoader);
		}
		
		
	
		return S_OK;
	}
	else
		return E_FAIL;






	

}

HRESULT CAnimation::Load_Data(ifstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		FileLoader.read(m_szName, sizeof(_char) * MAX_PATH);
		FileLoader.read((char*)&m_Duration, sizeof(_double));
		FileLoader.read((char*)&m_TickPerSecond, sizeof(_double));
		FileLoader.read((char*)&m_CurrentPosition, sizeof(_double));
		FileLoader.read((char*)&m_isFinished, sizeof(_bool));
		FileLoader.read((char*)&m_iNumChannels, sizeof(_uint));

		m_CurrentKeyFrameIndices.resize(m_iNumChannels);
		m_Channels.reserve(m_iNumChannels);

		for (size_t i = 0; i < m_iNumChannels; i++)
		{
			CChannel* pChannel = CChannel::Load_Create(FileLoader);
			if (nullptr == pChannel)
				return E_FAIL;

			m_Channels.emplace_back(pChannel);
		}

	


		return S_OK;
	}
	else
		return E_FAIL;


}



CAnimation * CAnimation::Create(const aiAnimation * pAIAnimation, const vector<class CBone*>& Bones)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, Bones)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Load_Create(ifstream& FileLoader)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Load_Data(FileLoader)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
