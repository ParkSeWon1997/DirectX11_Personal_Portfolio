#pragma once
#include"Client_Defines.h"
#include"Base.h"

#include"Player.h"

class CSequence;
class CSelector;
class CActionNode;
class CInverter;




BEGIN(Client)
class CPlayer;
class CBehavior : public CBase
{

	
private:
	CBehavior();
	virtual ~CBehavior() = default;

public:
	HRESULT Initialize();
	void Update(CPlayer::STATE NowState);

private:
	CSequence* m_pRootNode = nullptr;
	CActionNode* m_pMove= nullptr;
	CActionNode* m_pAttack = nullptr;

	CSelector* m_pSelector = nullptr;

	CActionNode* m_pIdle = nullptr;
	CInverter* m_pInverter = nullptr;


	/* for.detect*/
	CActionNode* m_pDetect = nullptr;
	CSelector* m_pDetectSelector = nullptr;
	CActionNode* m_pDetectPlayer = nullptr;
	CInverter* m_pInverterDetect = nullptr;


private:
	_uint m_iCurrentState = 0;

	CPlayer::STATE m_eState = CPlayer::STATE::STATE_END;
private:
	NodeStates DoIdle();

	
public:
	static CBehavior* Create();
	virtual void Free() override;
};

END