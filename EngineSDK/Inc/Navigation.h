#pragma once

#include "Component.h"

/* 1. ��ü�� ������ �� �ִ� ���� �ﰢ���� �������� ǥ������. */
/* 2. ������ ������ ��������ؼ� ������ �� �ֳ�? ����? �˻縦 �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct NAVIGATION_DESC
	{
		_int		iCurrentCellIndex = { -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strNavigationDataFile);
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Update(const _float4x4* pWorldMatrix);
	_bool isMove(_fvector vPosition);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif
	
private:
	vector<class CCell*>		m_Cells;
	static _float4x4			m_WorldMatrix;
	_int						m_iCurrentCellIndex = { -1 };

#ifdef _DEBUG
	class CShader*				m_pShader = { nullptr };	
#endif

private:
	HRESULT SetUp_Neighbors();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationDataFile);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();

};

END