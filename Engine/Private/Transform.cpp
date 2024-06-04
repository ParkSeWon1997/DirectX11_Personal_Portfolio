#include "..\Public\Transform.h"
#include "Shader.h"

#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent{ pDevice, pContext }
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent{ rhs }
{
}





void CTransform::Set_State(STATE eState, _fvector vState)
{
	_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	WorldMatrix.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

void CTransform::Set_Look(_fvector vLook)
{
	if(XMVector3Equal(vLook, XMVectorZero()))
		return;


	_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	_float3 vScaled = Get_Scaled();

	_vector vpLook = XMVector3Normalize(vLook);

	_vector vpRight = XMVector3Cross(XMVectorSet(0.0f, 1.f, 0.f, 0.f), vpLook);

	_vector vpUp = XMVector3Cross(vpLook, vpRight);

	WorldMatrix.r[STATE_RIGHT] = vpRight * vScaled.x;

	WorldMatrix.r[STATE_UP] = vpUp * vScaled.y;

	WorldMatrix.r[STATE_LOOK] = vpLook * vScaled.z;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);



}

void CTransform::Set_Up(_fvector vUp)
{
	_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	_float3 vScaled = Get_Scaled();

	_vector vpUp = XMVector3Normalize(vUp);

	_vector vpRight = XMVector3Cross(vpUp, XMVectorSet(0.0f, 0.f, 1.f, 0.f));

	_vector vpLook = XMVector3Cross(vpRight, vpUp);

	WorldMatrix.r[STATE_RIGHT] = vpRight * vScaled.x;

	WorldMatrix.r[STATE_UP] = vpUp * vScaled.y;

	WorldMatrix.r[STATE_LOOK] = vpLook * vScaled.z;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);




}

void CTransform::Set_Right(_fvector vRight)
{

	_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	_float3 vScaled = Get_Scaled();

	_vector vpRight = XMVector3Normalize(vRight);

	_vector vpUp = XMVector3Cross(vpRight, XMVectorSet(0.0f, 0.f, 1.f, 0.f));

	_vector vpLook = XMVector3Cross(vpRight, vpUp);

	WorldMatrix.r[STATE_RIGHT] = vpRight * vScaled.x;

	WorldMatrix.r[STATE_UP] = vpUp * vScaled.y;

	WorldMatrix.r[STATE_LOOK] = vpLook * vScaled.z;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);






}

void CTransform::Set_Identity()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		TRANSFORM_DESC*		pTransformDesc = (TRANSFORM_DESC*)pArg;

	
		m_fSpeedPerSec = pTransformDesc->fSpeedPerSec;
		m_fRotationPerSec = pTransformDesc->fRotationPerSec;
	}

	return S_OK;
}

HRESULT CTransform::Bind_ShaderResource(CShader * pShader, const _char * pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);	
}

void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	Set_State(STATE_RIGHT, Get_State(STATE_RIGHT) * fScaleX);
	Set_State(STATE_UP, Get_State(STATE_UP) * fScaleY);
	Set_State(STATE_LOOK, Get_State(STATE_LOOK) * fScaleZ);
}

void CTransform::Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
}

HRESULT CTransform::Set_WorldMatrix(_fmatrix matWorld)
{

	XMStoreFloat4x4(&m_WorldMatrix, matWorld);

	return S_OK;
}

HRESULT CTransform::Go_Straight(_float fTimeDelta,CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);


	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
	if (nullptr == pNavigation ? true : pNavigation->isMove(vPosition))
	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_RandomStraight(_float fTimeDelta, _float fRadian,CNavigation* pNavigation)
{
	QuatRotation(Get_State(STATE_UP), fRadian);

	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
	if (nullptr == pNavigation ? true : pNavigation->isMove(vPosition))
		Set_State(STATE_POSITION, vPosition);



	return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
	if (nullptr == pNavigation ? true : pNavigation->isMove(vPosition))
		Set_State(STATE_POSITION, vPosition);


	return S_OK;
}

HRESULT CTransform::Go_Teleport(_vector vPosition, CNavigation* pNavigation)
{
	if (nullptr == pNavigation ? true : pNavigation->isMove(vPosition))
		Set_State(STATE_POSITION, vPosition);

	return S_OK;
}


HRESULT CTransform::Go_Up(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vUp = Get_State(STATE_UP);

	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec * fTimeDelta;
	
	Set_State(STATE_POSITION, vPosition);


	return S_OK;
}

HRESULT CTransform::Go_Down(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vUp = Get_State(STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Jump(_float fTimeDelta, _float JumpPower, _float JumpHeight,_bool& Isfloor)
{


	// 현재 위치에 점프에 따른 높이 적용
	_float4 vPosition = {};
	XMStoreFloat4(&vPosition, Get_State(STATE_POSITION));

	if (vPosition.y >= JumpHeight)
	{
		fScala= -1.f;
	}

	//vPosition.y += JumpPower * fTimeDelta* fScala; // 수직 이동
	//Set_State(STATE_POSITION, XMLoadFloat4(&vPosition));

	vPosition.y += JumpPower * fTimeDelta * fScala; // 수직 이동
	if (vPosition.y <= 0.0f)
	{
		Isfloor = true;
		vPosition.y = 0.0f;
		fScala = 1.f;

	}
	else
	{
		Isfloor = false;
	}

	Set_State(STATE_POSITION, XMLoadFloat4(&vPosition));

	return S_OK;
}



HRESULT CTransform::Up_And_Down(_float fTimeDelta, _float fInitialSpeed, _float fAngle, _float fElapsedTime)
{
	const _float fGravity = 9.8f;
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);
	_float4 vPos = {};
	XMStoreFloat4(&vPos, vPosition);

	_vector vVelocityXZ = XMVector3Normalize(vLook) * fInitialSpeed * cos(XMConvertToRadians(fAngle));

	_float fYVelocity = fInitialSpeed * sin(XMConvertToRadians(fAngle));
	_float fPosY = fYVelocity * fElapsedTime - 0.5f * fGravity * pow(fElapsedTime, 2);

	_vector vNewPosXZ = vPosition + vVelocityXZ * fTimeDelta;

	_float4 vNewPos = {};
	XMStoreFloat4(&vNewPos, vNewPosXZ);
	vNewPos.y = XMVectorGetY(vPosition) + fPosY;

	Set_State(STATE_POSITION, XMLoadFloat4(&vNewPos));

	return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
	

	return S_OK;
}

HRESULT CTransform::Move_Look(_float fDistance)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	// 바라보는 방향을 정규화하여 이동 벡터 계산
	_vector vMoveVector = XMVector3Normalize(vLook) * fDistance;

	// 이동 벡터를 현재 위치에 더하여 새로운 위치 설정
	Set_State(STATE_POSITION, vPosition + vMoveVector);

	return S_OK;





}



HRESULT CTransform::LookAt(_fvector vTargetPosition)
{
	_float3 vScaled = Get_Scaled();

	_vector	vLook = vTargetPosition - Get_State(STATE_POSITION);

	_vector	vRight = XMVector3Cross(XMVectorSet(0.0f, 1.f, 0.f, 0.f), vLook);

	_vector vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

	return S_OK;
}

HRESULT CTransform::LookAt_For_LandObject(_fvector vTargetPosition)
{
	_float3 vScaled = Get_Scaled();

	_vector	vLook = vTargetPosition - Get_State(STATE_POSITION);

	_vector	vRight = XMVector3Cross(XMVectorSet(0.0f, 1.f, 0.f, 0.f), vLook);

	vLook = XMVector3Cross(vRight, XMVectorSet(0.0f, 1.f, 0.f, 0.f));

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);	
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

	// XMVector3Dot();

	return S_OK;
}

HRESULT CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);


	_matrix		mQuat = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta));
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);
	
	vRight = XMVector3TransformNormal(vRight, mQuat);
	vUp = XMVector3TransformNormal(vUp, mQuat);
	vLook = XMVector3TransformNormal(vLook, mQuat);
	
	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
	




	//_vector vQuat = XMQuaternionRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);
	//_matrix mRotaion = XMLoadFloat4x4(&m_WorldMatrix);
	//_matrix mQuat = XMMatrixRotationQuaternion(vQuat);
	//
	//mRotaion = XMMatrixMultiply(mRotaion, mQuat);
	//
	//XMStoreFloat4x4(&m_WorldMatrix, mRotaion);




	return S_OK;
}

HRESULT CTransform::Rotation(_fvector vAxis, _float fRadian)
{

	if (XMVector3Equal(vAxis, XMVectorZero()))
		return S_OK;


	_float3		vScaled = Get_Scaled();
	
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;
	
	_matrix		mQuat = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vAxis, fRadian));
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);
	
	vRight = XMVector3TransformNormal(vRight, mQuat);
	vUp = XMVector3TransformNormal(vUp, mQuat);
	vLook = XMVector3TransformNormal(vLook, mQuat);


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	//XMQuaternionRotationRollPitchYaw




	return S_OK;
}

HRESULT CTransform::RotationUp_And_Go(_float fRadian, _float fTimeDelta)
{


	_vector vRight = Get_State(STATE_RIGHT); // 현재 오른쪽 벡터 가져오기
	_vector vUp = Get_State(STATE_UP); // 현재 위 벡터 가져오기

	// 각 축을 기준으로 회전시킬 행렬 생성
	_matrix qRotationMatrix = XMMatrixRotationAxis(vRight, fRadian);

	// 회전 행렬 적용
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, qRotationMatrix)); // 위 벡터 회전
	Set_State(STATE_LOOK, XMVector3TransformNormal(Get_State(STATE_LOOK), qRotationMatrix)); // 보이는 방향 벡터 회전

	return S_OK;

}

HRESULT CTransform::QuatRotation( _fvector vAxis, _float fRadian)
{
	if (XMVector3Equal(vAxis, XMVectorZero()))
		return S_OK;
	
	_vector vQuat = XMQuaternionRotationAxis(vAxis, XMConvertToRadians(fRadian));


	_vector vRight = Get_State(STATE_RIGHT);
	_vector vUp = Get_State(STATE_UP);
	_vector vLook = Get_State(STATE_LOOK);

	vRight= XMVector3Rotate(vRight, vQuat);
	vUp = XMVector3Rotate(vUp, vQuat);
	vLook = XMVector3Rotate(vLook, vQuat);


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);


	
	return S_OK;
}

HRESULT CTransform::QuatRotation(_float3 vAxis)
{
	_float4		vQuaternion;

	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(vAxis.x, vAxis.y, vAxis.z));

	_matrix		mRotation = XMMatrixRotationQuaternion(XMLoadFloat4(&vQuaternion));

	_matrix		mWorld = XMLoadFloat4x4(&m_WorldMatrix);

	mWorld = XMMatrixMultiply(mWorld, mRotation);

	XMStoreFloat4x4(&m_WorldMatrix, mWorld);

	return S_OK;

}

_bool CTransform::TurnToTarget(_float fTimeDelta, _fvector vTargetPosition)
{
	_float3 vScaled = Get_Scaled();

	_vector vPos = Get_State(STATE_POSITION);
	_vector   vUp = Get_State(STATE_UP);
	_vector vLook = Get_State(STATE_LOOK);


	_vector vTargetLook = XMVectorSetY(XMVectorSubtract(vTargetPosition, vPos), 0.f);
	_float fLerp = fTimeDelta * m_fRotationPerSec;

	_vector vCurLook = XMVectorLerp(vLook, vTargetLook, fLerp);

	_float angle = XMVectorGetX(XMVector3Dot(vLook, XMVector3Normalize(vTargetLook)));

	if (angle > 0.99f)
	{
		return true;
	}
	//_float angle = XMVectorGetX(XMVector3AngleBetweenNormals(vCurLook, vTargetLook));
	//cout << angle << endl;

	_vector   vRight = XMVector3Cross(XMVectorSet(0.0f, 1.f, 0.f, 0.f), vCurLook);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_LOOK, XMVector3Normalize(vCurLook) * vScaled.z);


	//플레이어를 정확히 바라볼땐 true를 반환하게 리턴해줘야함
	return false;

}

HRESULT CTransform::Rotation(_float fRadian)
{
	_vector vUp = Get_State(STATE_UP);

	_matrix      qRotationMatrix = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(vUp, fRadian));

	Set_State(STATE_RIGHT, XMVector3TransformNormal(Get_State(STATE_RIGHT), qRotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(Get_State(STATE_UP), qRotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(Get_State(STATE_LOOK), qRotationMatrix));


	return S_OK;
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
