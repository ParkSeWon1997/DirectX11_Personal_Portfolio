#include "CBossMonster.h"

CBossMonster::CBossMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CBossMonster::CBossMonster(const CBossMonster& rhs)
	: CMonster{ rhs }
{
}

HRESULT CBossMonster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBossMonster::Initialize(void* pArg)
{
    return S_OK;
}

void CBossMonster::Priority_Tick(_float fTimeDelta)
{
}

void CBossMonster::Tick(_float fTimeDelta)
{
}

void CBossMonster::Late_Tick(_float fTimeDelta)
{
}

HRESULT CBossMonster::Render()
{
    return S_OK;
}

HRESULT CBossMonster::Add_Components()
{
    return S_OK;
}

HRESULT CBossMonster::Bind_ShaderResources()
{

    return S_OK;
}

void CBossMonster::Free()
{
    __super::Free();
}
