#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
public:
	class CComponent* Get_Component(const wstring& strComponentTag, _uint iIndex);
	class CGameObject* Get_Object(_uint iIndex);

public:
	HRESULT Initialize();
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject(class CGameObject* pGameObject);
	CGameObject** FIND_GameObject(class CGameObject* pGameObject);
	void Priority_Tick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	_uint Get_GameObjectSize() { return m_GameObjects.size(); }
private:
	list<class CGameObject* >		m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END