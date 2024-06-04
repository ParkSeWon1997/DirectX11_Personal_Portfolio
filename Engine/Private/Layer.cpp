#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{

}

CComponent* CLayer::Get_Component(const wstring& strComponentTag, _uint iIndex)
{
	if (iIndex >= m_GameObjects.size())
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_Component(strComponentTag);
	
}

CGameObject* CLayer::Get_Object(_uint iIndex)
{
	if (iIndex >= m_GameObjects.size())
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return *iter;
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	for (auto& tmp : m_GameObjects)
	{
		if (tmp == pGameObject)
		{
			Safe_Release(pGameObject);
			m_GameObjects.remove(tmp);
			break;
		}

	}

	return S_OK;
}

CGameObject** CLayer::FIND_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return nullptr;

	for (auto& tmp : m_GameObjects)
	{
		if (tmp == pGameObject)
		{

			return &tmp;
		}

	}

	return nullptr;
}

void CLayer::Priority_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (pGameObject == nullptr)
			return;
		pGameObject->Priority_Tick(fTimeDelta);
	}
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Tick(fTimeDelta);
}

void CLayer::Late_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Late_Tick(fTimeDelta);
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}