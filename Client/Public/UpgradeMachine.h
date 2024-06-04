#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CUpgradeMachine final : public CGameObject
{
public:
	enum MACHINE_TYPE 
	{ 
		MACHINE_ADD_HP,
		MACHINE_ADD_SHIELD, 
		MACHINE_ADD_MOVE_SPEED, 
		MACHINE_ADD_ATTACK_SPEED, 
		MACHINE_ADD_ATTACK_DAMAGE,
		MACHINE_ADD_DASH_COUNT,
		MACHINE_ADD_EXP,
		MACHINE_ADD_COOL_TIME,
		MACHINE_TYPE_END };
public:
	typedef struct CUpgradeMachine_DESC : CGameObject::GAMEOBJECT_DESC
	{
		
		MACHINE_TYPE eMachineType;
		_matrix m_pWorldMatrix = {};
	}CUpgradeMachine_DESC;

private:
	CUpgradeMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUpgradeMachine(const CUpgradeMachine& rhs);
	virtual ~CUpgradeMachine() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Make_MachineTop();

private:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CCollider*						m_pColliderCom[COLLIDER_END] = { nullptr };



private:
	MACHINE_TYPE					m_eMachineType = MACHINE_TYPE_END;
	_bool                           m_bIsFoot = false;
	_bool							m_bIsGrow = false;
	_bool							m_bIsFullCharge = false;
	_float3							m_vOriginSize= {};
	_float3 						m_vTargetSize = {};
	_float							m_fGrowingTime = 0.0f;
	int 							m_iOnOff[4]= { 0,0,0,0 };


private:
	class							CUpgradeMachineTop* m_pMachineTop = nullptr;

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUpgradeMachine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END