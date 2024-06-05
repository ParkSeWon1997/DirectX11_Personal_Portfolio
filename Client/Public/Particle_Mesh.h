#pragma once

#include"Client_Defines.h""
#include "GameObject.h"
#include"VIBuffer_Instance.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)

class CParticle_Mesh final : public CGameObject
{
public:
	enum class PARTICLE_TYPE
	{
		PARTICLE_TYPE_NONE = 0,
		PARTICLE_TYPE_SPREAD,
		PARTICLE_TYPE_DROP,
		PARTICLE_TYPE_UP_AND_DOWN,
		PARTICLE_TYPE_EXPLOSION,
		PARTICLE_TYPE_POP,
		PARTICLE_TYPE_RANDOM_POP,
		PARTICLE_TYPE_SIZE_UP,
		PARTICLE_TYPE_SIZE_DOWN,
		PARTICLE_TYPE_SIZE_DOWN_X,
		PARTICLE_TYPE_SIZE_DOWN_Y,
		PARTICLE_TYPE_SIZE_DOWN_Z,
		PARTICLE_TYPE_SIZE_UP_X,
		PARTICLE_TYPE_SIZE_UP_Y,
		PARTICLE_TYPE_SIZE_UP_Z,
		PARTICLE_TYPE_HALF_ALHPA,
		PARTICLE_TYPE_COLOR_CHANGE,
		PARTICLE_TYPE_TEXTURE,
		PARTICLE_TYPE_ROTATE_X,
		PARTICLE_TYPE_ROTATE_Y,
		PARTICLE_TYPE_ROTATE_Z,

		PARTICLE_TYPE_SPREAD_NONE_DISOLVE,
		PARTICLE_TYPE_SIZE_DOWN_Z_NONE_DISOLVE,
		PARTICLE_TYPE_PARTICLE_TYPE_EXPLOSION_NONE_DISOLVE,
		PARTICLE_TYPE_SIZE_UP_NONE_DISOLVE,
		PARTICLE_TYPE_POP_NONE_DISOLVE,
		PARTICLE_TYPE_ROTATE_X_NONE_DISOLVE,
		PARTICLE_TYPE_ROTATE_Y_NONE_DISOLVE,
		PARTICLE_TYPE_ROTATE_Z_NONE_DISOLVE,
		PARTICLE_TYPE_POP_TEXTURE,
		PARTICLE_TYPE_UP_AND_DOWN_TEXTURE,
		PARTICLE_TYPE_ROTATE_X_TEXTURE,
		PARTICLE_TYPE_SIZE_UP_Z_TEXTURE,
		PARTICLE_TYPE_END

	};
public:
	typedef struct PARTICLE_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		PARTICLE_TYPE 	eParticleType;
		_float4			vStartPos;
		_vector			vLookDir = {};
		_vector 		vOffsetPos = {};
	
		_float4			fColor = {};
		_bool           bIsRandome = false;
		_bool           bBloom = false;
		_float          fDisolveSpeed = 1.5f;
		CGameObject*	 pGameCallObject=nullptr;
		PARTICLE_DESC() :eParticleType(PARTICLE_TYPE::PARTICLE_TYPE_NONE), vStartPos(0, 0, 0, 0) {};
		PARTICLE_DESC(PARTICLE_TYPE type, const std::wstring& modelName, _float4 fColor=_float4(), _bool bIsRandom=false,_bool bBloom =false, CGameObject& pCallObject= *static_cast<CGameObject*>(nullptr), _vector vOffsetPos=_vector())
			:eParticleType(type), fColor(fColor), bIsRandome(bIsRandom), bBloom(bBloom),pGameCallObject(&pCallObject),vOffsetPos(vOffsetPos)
		{
			Safe_AddRef(pGameCallObject);
			this->strModelName= modelName;
		}
	}PARTICLE_DESC;
private:
	CParticle_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Mesh(const CParticle_Mesh& rhs);
	virtual ~CParticle_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_ParticlePos(CGameObject* pCallObject)
	{
	}
	void Set_ParticleProperty(const CVIBuffer_Instance::INSTANCE_DESC& ParticleDesc);
	const CVIBuffer_Instance::INSTANCE_DESC Get_InstanceBufferDesc();

private:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };	
	CModel*							m_pModelCom = { nullptr };

	PARTICLE_TYPE					m_eParticleType = PARTICLE_TYPE::PARTICLE_TYPE_END;
	_float4                        m_vColor = {};

	_float                         m_fDisolveTime = 0.f;
	_float                         m_fDisolveSpeed = 1.5f;
	_vector                        m_vPos = {};
	_bool                          m_bIsBloom = false;
	CGameObject*                   m_pGameCallObject = nullptr;

private:
	void RandomPosAndDirection(_float3* pOutPosition, _float3* pOutDirection, const _float4& vStartPos);

private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CParticle_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
	
public:
	
	static void inline Make_Particle(const vector< PARTICLE_DESC>& pvParticleInfo, const _vector& vStartPos,const _vector& vLookDir=_vector(),_float fDisolveSpeed=1.0f)
	{

		//캐시 적중률을 높이기 위해 배열을 사용? 필요한 작업인가?
		//그럴려면 메모리를 또 할당해야 하는데....
		PARTICLE_DESC desc = {};
		desc.vLookDir = vLookDir;
		desc.fDisolveSpeed = fDisolveSpeed;
		XMStoreFloat4(&desc.vStartPos, vStartPos);
		
		//const int tmep = pvParticleInfo.size();
		//PARTICLE_DESC* descArray=new PARTICLE_DESC[tmep];
		//for (int i = 0; i < pvParticleInfo.size(); ++i)
		//{
		//	
		//}


		for (const auto& pInfo : pvParticleInfo)
		{
			//cout<<&pInfo<<endl;
			desc.strModelName= pInfo.strModelName;
			desc.eParticleType = pInfo.eParticleType;
			desc.fColor = pInfo.fColor;
			desc.bIsRandome = pInfo.bIsRandome;
			desc.bBloom = pInfo.bBloom;
			desc.pGameCallObject = pInfo.pGameCallObject;
			desc.vOffsetPos= pInfo.vOffsetPos;
			CGameInstance::GetInstance()->Add_CloneObject(CLoader::m_eNextLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Particle"), &desc);
		}


	}
};

END