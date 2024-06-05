#pragma once

#include "Base.h"

/* assimp������ ���� ǥ���ϱ����� Ÿ������ ������ Ÿ���� �������ش�. */
/* aiNode, aiBone, aiNodeAnim */

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _float4x4* Get_CombinedTransformationMatrix() const {
		return &m_CombinedTransformationMatrix;
	}

public:
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	_bool Compare_Name(const _char* pName) {
		return !strcmp(m_szName, pName);
	}

public:
	HRESULT Initialize(const aiNode* pBoneNode, _int iParentIndex);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

	HRESULT Save_Data( ofstream& FileLoader);
	HRESULT Load_Data( ifstream& FileLoader);



private:
	_char				m_szName[MAX_PATH] = "";
	_int				m_iParentBoneIndex = { 0 };
	_float4x4			m_TransformationMatrix;
	_float4x4			m_CombinedTransformationMatrix;


public:
	static CBone* Create(const aiNode* pBoneNode, _int iParentIndex);
	static CBone* Load_Create(ifstream& FileLoader);
	CBone* Clone();
	virtual void Free() override;
};

END