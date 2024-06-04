#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

BEGIN(Engine)
//enum class eKEY_STATE
//{
//	NONE,
//	TAP, //������
//	HOLD,//������ ������
//	AWAY, // �H����
//	KEY_END,
//};
//struct tKeyInfo
//{
//	eKEY_STATE	eState; //Ű�� ���°�
//	bool		bPrevPush;	//���������ӿ� �������� �ȴ�������
//};

class ENGINE_DLL CInput_Device : public CBase
{


private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)			{ return m_byKeyState[byKeyID]; }
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 	
	{ 	return m_tMouseState.rgbButtons[eMouse]; 	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	
	{	
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}
	
public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);
	void Update_MouseState(void);


	eKEY_STATE GetKeyState(_ubyte byKeyID);
	eKEY_STATE GetMouseState(MOUSEKEYSTATE eMouseState);


private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;

private:
	_byte					m_byKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
	DIMOUSESTATE			m_tMouseState;	

	vector<eKEY_STATE> m_vecMouseState;
	vector< tKeyInfo>		m_vecKey;

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};
END
#endif // InputDev_h__
