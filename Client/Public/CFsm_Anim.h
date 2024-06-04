#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

END


BEGIN(Client)
class CFsm_Anim final :  public  CBase
{

public:
	typedef std::function<void(void)> STATE_ANIMFUNCTION;
	
	//typedef void(CFsm_Anim::* STATE_ANIMFUNCTION)();
private:
	CFsm_Anim();
	~CFsm_Anim()=default;
	HRESULT Initialize(int AnimIndex);


private:
	int iAnimIndex = 0;
	_bool		bIsAnimEnd = false;
	_bool		bIsAnim_runing = false;
	vector<STATE_ANIMFUNCTION> m_vecStateFunction;

	
public:
	int Get_CurrentState() { return iAnimIndex; }
	_bool	IsAnimEnd() { return bIsAnimEnd; }
	_bool	IsAnimRuning() { return bIsAnim_runing; }
	_bool	IsAnimNotEqulPreState() { return iAnimIndex != iAnimIndex; }
	void SetAnimEnd(_bool bIsEnd) { bIsAnimEnd = bIsEnd; }
	void SetAnimRuning(_bool bIsRuning) { bIsAnim_runing = bIsRuning; }
	void BindFunction(function<void(void)> func);
	void SetState(int iAnimIndex);
	void Update();



	void SetAnimations(STATE_ANIMFUNCTION func);			//¾È ¾¸
public:
	static CFsm_Anim* Create(int AnimIndex);
	virtual void Free() override;
};

END