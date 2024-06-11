#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage_Change final : public CLevel
{
private:
	CLevel_Stage_Change(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Stage_Change() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;


private:
	HRESULT Ready_Lights();
	HRESULT Ready_UI(const wstring& strLayerTag);
	HRESULT Ready_Player_UI(const wstring& strLayerTag);
	HRESULT Ready_Player_SKill_UI(const wstring& strLayerTag);
	HRESULT Ready_UI_Changer(const wstring& strLayerTag);
	HRESULT Ready_Layer_FadeIn_Out(const wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);	
	HRESULT Ready_Layer_Chnager(const wstring& strLayerTag);	
	HRESULT Ready_Layer_Effect(const wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	HRESULT Ready_Layer_0_Stage(const wstring& strLayerTag);
	HRESULT Ready_Layer_2_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_Environment(const wstring& strLayerTag);
	HRESULT Ready_Layer_1_Segment(const wstring& strLayerTag);



public:
	static CLevel_Stage_Change* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END