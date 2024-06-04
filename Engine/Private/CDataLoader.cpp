#include "CDataLoader.h"

CDataLoader::CDataLoader()
{
	
}

CDataLoader::CDataLoader(const CDataLoader& rhs)
{
	
}

HRESULT CDataLoader::Initialize_Prototype(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag)
{



	return E_NOTIMPL;
}

HRESULT CDataLoader::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

CDataLoader* CDataLoader::Create(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag)
{
	CDataLoader* pInstance = new CDataLoader();

	if (FAILED(pInstance->Initialize_Prototype(iLevelIndex, strLayerTag,strPrototypeTag)))
	{
		MSG_BOX("Failed To Created : CDataLoader");

		Safe_Release(pInstance);
	}

	return pInstance;
}


CDataLoader* CDataLoader::Clone(void* pArg)
{
	CDataLoader* pInstance = new CDataLoader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CDataLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDataLoader::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
