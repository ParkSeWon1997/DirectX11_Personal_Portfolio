#include "stdafx.h"
#include "..\Public\PlayerCamera.h"

#include "GameInstance.h"

#include"Loader.h"

bool CPlayerCamera::bNoClick = false;
CPlayerCamera::CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CPlayerCamera::CPlayerCamera(const CPlayerCamera& rhs)
	: CCamera{ rhs }
{
}

HRESULT CPlayerCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerCamera::Initialize(void* pArg)
{
	Player_CAMERA_DESC* pDesc = static_cast<Player_CAMERA_DESC*>(pArg);

	m_fSensor = pDesc->fSensor;
	m_pParentMatrix = pDesc->pParentMatrix;
	m_vTargetPos = pDesc->vTargetPos;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(pDesc->fAngle));

	return S_OK;
}

void CPlayerCamera::Priority_Tick(_float fTimeDelta)
{
}

void CPlayerCamera::Tick(_float fTimeDelta)
{

	


	if (!m_bCutScene)
	{
		_matrix pPlayerMatrix = XMLoadFloat4x4(m_pParentMatrix);
		_vector vPlayerPos = pPlayerMatrix.r[CTransform::STATE_POSITION];

		_vector vPlayerToFarHeigt = vPlayerPos + m_vTargetPos;
		_vector vCamerTargetPos = vPlayerToFarHeigt - (pPlayerMatrix.r[CTransform::STATE_LOOK] * m_fSensor);

		_vector vCurrentCameraPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_vector vCameraPos = XMVectorLerp(vCurrentCameraPos, vCamerTargetPos, fTimeDelta * 2.0f);



		if (m_bShake)
		{
			m_fShakeTime += fTimeDelta;

			if (m_fShakeTime > m_fShakeMaxTime)
			{
				m_fShakeTime = 0.0f;
				m_bShake = false;
			}
			float fshake = 0.1f;
			float fshakeX = RandomNum<_float>(-1.0, 1.0) * fshake;
			float fshakeY = RandomNum<_float>(-1.0, 1.0) * fshake;
			float fshakeZ = RandomNum<_float>(-1.0, 1.0) * fshake;

			_vector vShake = XMVectorSet(fshakeX, fshakeY, fshakeZ, 0.f);

			vCameraPos += vShake;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (vCameraPos));
	}
	else
	{/*
		if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
			m_pTransformCom->Go_Left(fTimeDelta);
		if (GetKeyState('D') & 0x8000)
			m_pTransformCom->Go_Right(fTimeDelta);
		if (GetKeyState('W') & 0x8000)
			m_pTransformCom->Go_Straight(fTimeDelta);
		if (GetKeyState('S') & 0x8000)
			m_pTransformCom->Go_Backward(fTimeDelta);

		_long		MouseMove = { 0 };
		if (GetAsyncKeyState(VK_TAB) & 0x8000)
		{
			if (bNoClick == false)
			{
				bNoClick = true;
			}
			else
				bNoClick = false;
		}

		if (bNoClick)
		{
			if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
			{
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);
			}

			if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
			{
				m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_fSensor * MouseMove);
			}
		}*/



		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (m_vTargetPos));

	}
	_vector vCameraPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vCameraPos;
	//m_fFovy= XMConvertToRadians(90.f);

	__super::Tick(fTimeDelta);
}

void CPlayerCamera::Late_Tick(_float fTimeDelta)
{
	

	__super::Late_Tick(fTimeDelta);
}

HRESULT CPlayerCamera::Render()
{
	return S_OK;
}

CPlayerCamera* CPlayerCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerCamera* pInstance = new CPlayerCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayerCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerCamera::Clone(void* pArg)
{
	CPlayerCamera* pInstance = new CPlayerCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayerCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerCamera::Free()
{
	__super::Free();

}
