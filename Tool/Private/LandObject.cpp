#include "stdafx.h"
#include "..\Public\LandObject.h"

#include "GameInstance.h"

CLandObject::CLandObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject{ pDevice, pContext }
{

}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void * pArg)
{
	return S_OK;
}

void CLandObject::Priority_Tick(_float fTimeDelta)
{
}

void CLandObject::Tick(_float fTimeDelta)
{
}

void CLandObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

HRESULT CLandObject::Compute_Height()
{

	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_matrix		ViewMatrix = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	_matrix		ProjMatrix = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	/* 1.내 객체의 투영공간상의 위치가 필요하다. */
	/*: 객체를 지형위에 태우기위해서는 플레잉어가 서있는 월드위치에 Y값으로 플레이어의 월드위치 Y를 교체하는 작업이 피룡하다.  	*/
	/*: 플레이러를 제외한 공간상에 */

	return S_OK;
}



void CLandObject::Free()
{
	__super::Free();

}
