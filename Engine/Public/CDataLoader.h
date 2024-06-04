#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CDataLoader final : public CBase
{
protected:
	CDataLoader();
	CDataLoader(const CDataLoader& rhs);
	virtual ~CDataLoader() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag);
	virtual HRESULT Initialize(void* pArg);

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	_uint m_iLevelIndex = 0;
	const wstring& m_strLayerTag= L"";
	const wstring& m_strPrototypeTag= L"";
	_float4x4 m_matWorld;
public:
	static CDataLoader* Create(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag );
	virtual CDataLoader* Clone(void* pArg);
	virtual void Free() override;
};


END