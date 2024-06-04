#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent{ pDevice, pContext }
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent{ rhs }
	, m_iNumTextures { rhs.m_iNumTextures }
	, m_Textures{ rhs.m_Textures }
{
	
	for (auto& pSRV : m_Textures)
		Safe_AddRef(pSRV);	
}

HRESULT CTexture::Initialize_Prototype(const wstring & strTextureFilePath, _uint iNumTextures)
{
	//if (strTextureFilePath.length() < MAX_PATH)
	//{
	//	// wstring을 _char* 형식으로 변환
	//	std::string strTextureFilePathA(strTextureFilePath.begin(), strTextureFilePath.end());
	//
	//	// _char* 형식 문자열을 textureFileName에 복사
	//	strcpy_s(textureFileName, MAX_PATH, strTextureFilePathA.c_str());
	//}
	
	_tchar			szTextureFilePath[MAX_PATH] = TEXT("");
	_tchar			szEXT[MAX_PATH] = TEXT("");

	m_Textures.reserve(iNumTextures);

	m_iNumTextures = iNumTextures;

	for (size_t i = 0; i < iNumTextures; i++)
	{
		/* ..\Bin\Resources\Textures\Default0.jpg */
		wsprintf(szTextureFilePath, strTextureFilePath.c_str(), i);
		wsprintf(textureFileName, szTextureFilePath, i);
		

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

		ID3D11ShaderResourceView*		pSRV = { nullptr };

		HRESULT			hr{};

		if (false == lstrcmp(szEXT, TEXT(".dds")))		
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

		// else if(false == lstrcmp(szEXT, TEXT(".tga")))

		else
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

		

 		if (FAILED(hr))
			return E_FAIL;

		


		

		m_Textures.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Save_Data(ofstream& FileLoader)
{
	if (FileLoader.is_open())
	{
		//textureFileName

		FileLoader.write((char*)textureFileName, sizeof(TCHAR)* MAX_PATH);
		FileLoader.write((char*)&m_iNumTextures, sizeof(_uint));
		



		

		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CTexture::Load_Data(ifstream& FileLoader)
{

	if (FileLoader.is_open())
	{
		FileLoader.read((char*)textureFileName, sizeof(TCHAR) * MAX_PATH);
		FileLoader.read((char*)&m_iNumTextures, sizeof(_uint));

		Initialize_Prototype(textureFileName, m_iNumTextures);
	
		

		return S_OK;
	}
		
	else
		return E_FAIL;
}

HRESULT CTexture::Bind_ShaderResource(CShader * pShader, const _char * pConstantName, _uint iTextureIndex)
{
	//if(this==nullptr)
	//	return E_FAIL;

 	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_Textures[iTextureIndex]);	
}

HRESULT CTexture::Bind_ShaderResources(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_SRVs(pConstantName, &m_Textures.front(), m_iNumTextures);
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed To Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture* CTexture::Load_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumTextures, ifstream& FileLoader)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Load_Data(FileLoader)))
	{
		MSG_BOX("Failed To Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_Textures)
		Safe_Release(pSRV);	
	m_Textures.clear();
}
