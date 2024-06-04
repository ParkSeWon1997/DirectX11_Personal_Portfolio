#pragma once

#include "Base.h"

/* 애님에ㅣ션이ㅐ 사용하느 ㄴ뼈 하나의 정보. */
/* 정보 : 시간에 따른 이 뼈의 상태(스, 자, 이)들을 보고ㅜㅏㄴ한다. == 키프레임 */
/* */

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const aiNodeAnim*	pAIChannel, const vector<class CBone*>& Bones);
	void Update_TransformationMatrix(_double CurrentPosition, const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex);
	void Set_CurKeyFrameindex(_uint* pCurrentKeyFrameIndex) { pCurrentKeyFrameIndex = &m_iCurrentKeyFrameIndex; };
	KEYFRAME Get_KeyFrame(_uint iIndex) { return m_KeyFrames[iIndex]; }
	_uint Get_NumKeyFrames() { return m_iNumKeyFrames; }
	HRESULT Save_Data(ofstream& FileLoader);
	HRESULT Load_Data(ifstream& FileLoader);
private:
	_char			m_szName[MAX_PATH] = "";
	_uint			m_iBoneIndex = { 0 };

	_uint				m_iNumKeyFrames = { 0 };
	_uint				m_iCurrentKeyFrameIndex = { 0 };

	vector<KEYFRAME>	m_KeyFrames;

public:
	static CChannel* Create(const aiNodeAnim*	pAIChannel, const vector<class CBone*>& Bones);
	static CChannel* Load_Create(ifstream& FileLoader);
	virtual void Free() override;
};

END