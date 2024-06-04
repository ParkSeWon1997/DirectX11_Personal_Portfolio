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

	/* 1.�� ��ü�� ������������ ��ġ�� �ʿ��ϴ�. */
	/*: ��ü�� �������� �¿�����ؼ��� �÷��׾ ���ִ� ������ġ�� Y������ �÷��̾��� ������ġ Y�� ��ü�ϴ� �۾��� �Ƿ��ϴ�.  	*/
	/*: �÷��̷��� ������ ������ */

	return S_OK;
}



void CLandObject::Free()
{
	__super::Free();

}
