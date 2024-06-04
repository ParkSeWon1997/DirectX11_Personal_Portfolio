#include "..\Public\GameObject.h"
#include "GameInstance.h"

const _tchar* CGameObject::m_pTransformTag = TEXT("Com_Transform");
float  CGameObject::g_fObjID_count = 0;

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice{ rhs.m_pDevice }
	, m_pContext{ rhs.m_pContext }
	, m_pGameInstance{ rhs.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CComponent* CGameObject::Get_Component(const wstring& strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag.c_str());
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}
HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void * pArg)
{
	m_fObjOwnderID=g_fObjID_count++;



	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_strModelName= static_cast<GAMEOBJECT_DESC*>(pArg)->strModelName;
		


		// m_iData = ((GAMEOBJECT_DESC*)pArg)->iData;
	}

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	m_Components.emplace(m_pTransformTag, m_pTransformCom);
	Safe_AddRef(m_pTransformCom);

	return S_OK;
}

void CGameObject::Priority_Tick(_float fTimeDelta)
{
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::Late_Tick(_float fTimeDelta)
{

}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::Set_Scaled(_float3 fScale)
{
	m_pTransformCom->Set_Scale(fScale.x, fScale.y, fScale.z);

}

void CGameObject::Set_Position(_float3 fPosition)
{
	_vector vPos = XMLoadFloat4(&_float4(fPosition.x, fPosition.y, fPosition.z, 1.0f));
	//m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CGameObject::Set_Rotation(_float fRotationAngle, _float3 fAxis)
{
	_vector vAxix= XMLoadFloat3(&fAxis);
	m_pTransformCom->Rotation(vAxix, XMConvertToRadians(fRotationAngle));

}

void CGameObject::Set_Rotation(_float fRotationAngle)
{
	m_pTransformCom->Rotation(fRotationAngle);
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, const wstring & strComponentTag, CComponent** ppOut, void* pArg)
{
	if (m_Components.end() != m_Components.find(strComponentTag))
		return E_FAIL;

	CComponent*	pComponent = m_pGameInstance->Clone_Component(iPrototypeLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;	

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Change_Component(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	Safe_Release(*ppOut);
	Safe_Release(m_Components.find(strComponentTag)->second);
	m_Components.erase(strComponentTag);
	//if (m_Components.end() != m_Components.find(strComponentTag))
	//	return E_FAIL;

	CComponent* pComponent = m_pGameInstance->Clone_Component(iPrototypeLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;
	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;
	//Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
