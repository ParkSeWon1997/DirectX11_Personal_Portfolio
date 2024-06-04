#pragma once
#include"CBullet.h"

class CBoss_B_Bullet final: public CBullet
{

private:
	CBoss_B_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_B_Bullet(const CBullet& rhs);
	virtual ~CBoss_B_Bullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	virtual void Falling(_float fTimeDelta) override;

private:
	virtual HRESULT Add_Components() override;
	virtual HRESULT Bind_ShaderResources()override;


	_bool   m_Isfloor = false;
	_bool   m_IsAllCreate = false;
	_float  m_fHp = 0.f;


	const _float4x4* m_pBoneMatrix[3] = {nullptr,nullptr,nullptr};
	_matrix m_BoneMatrix[3] = { {},{},{} };

public:
	static CBoss_B_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;




};

