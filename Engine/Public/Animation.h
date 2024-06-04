#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	_bool Get_Finished() const {
		return m_isFinished;
	}
	_double Get_Duration() const {
		return m_Duration;
	}
	_double Get_CurrentPosition() const {
		return m_CurrentPosition;
	}
public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones);
	void Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _bool isLoop,_uint* pCurKeyFrame,_double DurationRatio,_bool& IsAccept);
	_double Calculate_SelectedKeyFrameTime(_uint iFrame);
	void Reset();
	_uint Get_CurrentKeyFrameIndex(){
		return m_iCurrentKeyFrame;
	}

	HRESULT Save_Data(ofstream& FileLoader);
	HRESULT Load_Data(ifstream& FileLoader);

private:
	_char			m_szName[MAX_PATH] = "";
	_double			m_Duration = { 0.0 };
	_double			m_TickPerSecond = { 0.0 };
	_double			m_CurrentPosition = { 0.0 };
	_bool			m_isFinished = { false };

	_uint					m_iNumChannels = { 0 };
	
	vector<_uint>			m_CurrentKeyFrameIndices;
	_uint 					m_iCurrentKeyFrame = { 0 };
	vector<class CChannel*>	m_Channels;




public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones);
	static CAnimation* Load_Create(ifstream& FileLoader);
	CAnimation* Clone();
	virtual void Free() override;
};

END