#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;


	HRESULT Save_Data(ofstream& FileLoader);
	HRESULT Load_Data(ifstream& FileLoader);

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, _uint iTextureIndex);
	HRESULT Bind_ShaderResources(class CShader* pShader, const _char* pConstantName);

private:
	_uint													m_iNumTextures = { 0};
	vector<ID3D11ShaderResourceView*>						m_Textures;

	TCHAR	textureFileName[MAX_PATH] = L"";
	

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures);
	static CTexture* Load_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumTextures, ifstream& FileLoader);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END