#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instance abstract : public CVIBuffer
{
public:
	typedef struct INSTANCE_DESC
	{
		INSTANCE_DESC() :
		vOffsetPos(_float3(0.f, 0.f, 0.f)),
		vPivotPos(_float3(0.f, 0.f, 0.f)), 
		vRange(_float3(0.f, 0.f, 0.f)), 
		vSize(_float2(0.f, 0.f)), 
		fSize_X(0.f),
		fSize_Y(0.f),
		fSize_Z(0.f),
		vSpeed(_float2(0.f, 0.f)), 
		vLifeTime(_float2(0.f, 0.f)),
		vTargetPos(_float3(0.f, 0.f, 0.f)),
		IsDead(false),
		isLoop(false) {}

		_uint			iNumInstance = { 0 };
		_float3			vOffsetPos;
		_float3			vPivotPos;
		_float3			vRange;
		_float2			vSize;	
		_float          fSize_X;
		_float          fSize_Y;
		_float          fSize_Z;
		_float2			vSpeed;
		_float2			vLifeTime;
		_float3		    vTargetPos;
		_bool			IsDead;
		_bool			isLoop;
		_uint			iType_Index = { 0 };
	}INSTANCE_DESC;
protected:
	CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instance(const CVIBuffer_Instance& rhs);
	virtual ~CVIBuffer_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC& InstanceDesc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Buffers() override;
	virtual HRESULT Render() override;

public:
	void Spread(_float fTimeDelta);
	void Drop(_float fTimeDelta);
	void Up_And_Down(_float fTimeDelta);
	void Explosion(_float fTimeDelta);
	void Pop(_float fTimeDelta);
	void RandomPop(_float fTimeDelta);
	void RandRotate_SizeUp(_float fTimeDelta);
	void SizeUp(_float fTimeDelta);
	void SizeDown(_float fTimeDelta);
	void SizeDown_Y(_float fTimeDelta);
	void SizeDown_X(_float fTimeDelta);
	void SizeDown_Z(_float fTimeDelta);
	void SizeUp_Y(_float fTimeDelta);
	void SizeUp_X(_float fTimeDelta);
	void SizeUp_Z(_float fTimeDelta);



	void Rotation_X(_float fTimeDelta);
	void Rotation_Y(_float fTimeDelta);
	void Rotation_Z(_float fTimeDelta);
	void Roation_Orbit(_float fTimeDelta);
	virtual HRESULT Set_ParticleProperty(const INSTANCE_DESC& InstanceDesc);
	virtual HRESULT Set_ParticleType(_uint iIndex);
	INSTANCE_DESC Get_InstanceDesc() { return m_InstanceDesc; }
protected:
	ID3D11Buffer*				m_pVBInstance = { nullptr };
	D3D11_BUFFER_DESC			m_InstanceBufferDesc = {};
	_uint						m_iNumInstance = { 0 };
	_uint						m_iInstanceStride = { 0 };
	_uint						m_iIndexCountPerInstance = { 0 };

	_float*						m_pSpeeds = { nullptr };
	_float3*					m_pOriginalPositions = { nullptr };
	_float3* 					m_pOriginalSizes = { nullptr };
	_float*                      m_fOriginalSize_X= { nullptr };
	_float*                      m_fOriginalSize_Y = { nullptr };
	_float*                      m_fOriginalSize_Z = { nullptr };
	_float                      m_fScalar = { 0.f };
	INSTANCE_DESC				m_InstanceDesc = {};

protected:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END