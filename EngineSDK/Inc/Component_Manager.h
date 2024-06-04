#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"

#include "Collider.h"
#include "Navigation.h"
#include "Bounding_OBB.h"
#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"

#include "VIBuffer_Cube.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"

/* 컴포넌트의 원형들을 레벨별로 보관한다. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent* pPrototype);
	HRESULT Delete_Clone(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent* pClone);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	map<const wstring, CComponent*>*			m_pPrototypes = { nullptr };
	typedef map<const wstring, CComponent*>		PROTOTYPES;

	_uint									m_iNumLevels = { 0 };

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	static CComponent_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END