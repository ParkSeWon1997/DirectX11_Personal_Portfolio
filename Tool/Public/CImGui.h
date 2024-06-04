#pragma once


#include "Tool_Defines.h"
#include "Base.h"
#include"Particle_Mesh.h"

class CParticle_Mesh;

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CParticle_Mesh;

END

BEGIN(Client)
class CImGui final : public CBase
{
public:
	typedef struct OBJPROPERTY
	{
	static float fScale[3];
	static float fRotateAxis[3];
	static float fRotateAngle;
	static float fPos[3];
	};
	typedef struct PARTICLEPROPERTY
	{
		static int iNumInstance;
		static int iNumMaxInstance;
		static _float vOffsetPos[3];
		static _float vPivotPos[3];
		static _float vRange[3];
		static _float vSize[2];
		static _float fSize[3];
		static _float vSpeed[2];
		static _float vLifeTime[2];
		static _float vTargetPos[3];
		static _uint iTypeIndex;

		static _bool vIsLoop;
		static char cInputMeshName[MAX_PATH];
		static char cSaveMeshName[MAX_PATH];
		static char cFindParticleName[MAX_PATH];

	};
	


	DECLARE_SINGLETON(CImGui)
private:
	CImGui() {};
	CImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CImGui()noexcept = default;



public:
	HRESULT Start_ImGui();
	HRESULT End_ImGui();
	HRESULT Init_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);



	void Create_Obj();
	void Clicked_Pick_Position();
	HRESULT Make_Navigation();
	HRESULT Make_Particle(wstring wstrModelName, const char* cstr, void* arg);
	void    Find_Particle_Files(const char* cstr);
	tuple<int,bool,CGameObject*> Find_Particle(_float fObjId);

	//int Find_Particle(_float fObjId);


private:
	/* For.ImGui*/
	void ImGui_Property();
	void ImGui_ObjProperty();
	void ImGui_ParticleProperty();


private:
	void Select_OBJ();
	void Delete_OBJ();
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	CGameInstance* m_pGameInstance = { nullptr };


	CGameObject* m_pGameObj= { nullptr };
	CGameObject* m_pGameObjParticle = { nullptr };
	map<float, CGameObject*> m_mapGameObj;
	map<const char* , CGameObject*> m_mapGameObjParticle;

private:
	static float fPickpos[3];
	int iProtoTypeParticleCount = 0;

private:
	_bool		bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_END] = { };
	
	//IMGUI전용 멤버 변수
	bool		bChangeMode = false;
	bool		bisDragMode = false;
	bool		IsCreateMode = false;
	bool		bDeleteButton = false;
	bool		IsCreate=false;
	bool		IsSave = false;
	bool 		IsNavigationMode = false;
	_bool		IsParticleMode = false;
	_bool		IsChageParticleMode = false;
	_bool		IsChangeSlideParticleMode = false;
	_bool 	    IsFiexdParticleMode = false;
	bool		IsAnimObject = false;
	float fObjID = 0;;


	bool prevLBMouseState = false;
	bool prevRBMouseState = false;



	int PreiNumInstance = 0;
	 _float PrevOffsetPos[3];
	 _float PrevPivotPos[3];
	 _float PrevRange[3];
	 _float PrevSize[2];
	 _float PrefSize[3];
	 _float PrevSpeed[2];
	 _float PrevLifeTime[2];
	 _bool PrevIsLoop;
	 _float PrevTargetPos[3];
	 _uint PreTypeIndex = 0;

	 _bool bCheckLoop = false;



	char InputName[MAX_PATH] = { 0};


	_uint iNavivertexcount = 0;
public:
	//static CImGui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;



};

END