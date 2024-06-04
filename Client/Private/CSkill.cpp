#include "CSkill.h"

CSkill::CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
    , m_CoolTime{0.0f}

{
}

CSkill::CSkill(const CSkill& rhs)
    :CGameObject(rhs)
    , m_CoolTime(rhs.m_CoolTime)
{
}

HRESULT CSkill::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CSkill::Initialize(void* pArg)
{
    return S_OK;
}

void CSkill::Priority_Tick(_float fTimeDelta)
{
}

void CSkill::Tick(_float fTimeDelta)
{
}

void CSkill::Late_Tick(_float fTimeDelta)
{
}

HRESULT CSkill::Render()
{
    return S_OK;
}

void CSkill::UseSkill()
{

}

void CSkill::Free()
{
    __super::Free();
}
