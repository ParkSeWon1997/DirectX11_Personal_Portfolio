
#include"ImGui/imgui.h"
#include"ImGui/imgui_impl_win32.h"
#include"ImGui/imgui_impl_dx11.h"
#include "CImGui.h"

#include"Terrain.h"
#include"Monster.h"
#include"ForkLift.h"
#include "GameInstance.h"
#include"Model.h"
#include"Particle_Mesh.h"


IMPLEMENT_SINGLETON(CImGui)

float CImGui::OBJPROPERTY::fScale[3] = {1.0f,1.0f,1.0f};
float CImGui::OBJPROPERTY::fRotateAxis[3] = {0.0f,1.0f,0.0f};
float CImGui::OBJPROPERTY::fRotateAngle = {0.f};
float CImGui::OBJPROPERTY::fPos[3] = {0.0f,0.0f,0.0f};
float CImGui::fPickpos[3] = { 0.0f,0.0f,0.0f };


int CImGui::PARTICLEPROPERTY::iNumInstance = {0};
int CImGui::PARTICLEPROPERTY::iNumMaxInstance = { 0 };
_float CImGui::PARTICLEPROPERTY::vOffsetPos[3] = { 0.0f,0.0f,0.0f };
_float CImGui::PARTICLEPROPERTY::vPivotPos[3] = { 0.0f,0.0f,0.0f };
_float CImGui::PARTICLEPROPERTY::vRange[3] = { 0.0f,0.0f,0.0f };
_float CImGui::PARTICLEPROPERTY::vSize[2] = { 0.0f,0.0f };
_float CImGui::PARTICLEPROPERTY::fSize[3] = { 0.0f,0.0f,0.0f };
_float CImGui::PARTICLEPROPERTY::vSpeed[2] = { 0.0f,0.0f };
_float CImGui::PARTICLEPROPERTY::vLifeTime[2] = { 0.0f,0.0f };
_float CImGui::PARTICLEPROPERTY::vTargetPos[3] = { 0.0f,0.0f,0.0f };
_uint CImGui::PARTICLEPROPERTY::iTypeIndex = { 0 };

_bool CImGui::PARTICLEPROPERTY::vIsLoop = { false };
char CImGui::PARTICLEPROPERTY::cInputMeshName[MAX_PATH] = "../Bin/Resources/Models/Object/Particle/";
char CImGui::PARTICLEPROPERTY::cSaveMeshName[MAX_PATH] = "";
char CImGui::PARTICLEPROPERTY::cFindParticleName[MAX_PATH] = "";

static CVIBuffer_Instance::INSTANCE_DESC g_Desc = {};

HRESULT CImGui::Start_ImGui()
{
	bool currentLBMouseState = m_pGameInstance->Get_DIMouseState(DIM_LB) & 0x8000;
	bool currentRBMouseState = m_pGameInstance->Get_DIMouseState(DIM_RB) & 0x8000;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (IsParticleMode)
	{
		ImGui_ParticleProperty();
		IsChageParticleMode = true;
		

	}
	else
	{
		IsChageParticleMode = false;
	}
	

	//bool bShow = true;
	//ImGui::ShowDemoWindow(&bShow);
	ImGui_Property();
	if (IsCreateMode)
	{
		if(!IsChageParticleMode)
			ImGui_ObjProperty();
	}
	if (GetAsyncKeyState('1'))
	{
		if(bChangeMode==false)
			bChangeMode = true;
		else if (bChangeMode== true)
		{
			bChangeMode = false;
		}
	}

	ImGui::Checkbox("MouseDrag Mode", &bisDragMode);
	if (bisDragMode == false)
	{
		ImGui::Checkbox("Change Mode", &bChangeMode);
	}
	
	if (GetAsyncKeyState('8') & 0x8000)
	{
		Delete_OBJ();
	}
	
	ImGui::Checkbox("Navigation Mode", &IsNavigationMode);
	if (ImGui::Checkbox("Change Particle Mode", &IsFiexdParticleMode))


	//왼쪽 마우스가 한 번만 눌린경우
	if (currentLBMouseState && !prevLBMouseState)
	{
		bool Ispick = false;
		fObjID=m_pGameInstance->PickingID(&Ispick);
		Select_OBJ();
	
		
	}
	if (IsNavigationMode)
	{
		Make_Navigation();

	}


	if (iNavivertexcount == 2)
	{
		iNavivertexcount = 0;
	}



	//오른쪽 마우스가 한 번만 눌린경우

	if (bisDragMode)
	{
		Clicked_Pick_Position();
	}
	else
	{
		if (currentRBMouseState && !prevRBMouseState)
		{
			Clicked_Pick_Position();
		}
	}
	















	ImGui::Render();

	prevLBMouseState = currentLBMouseState;
	prevRBMouseState = currentRBMouseState;



	return S_OK;

}

HRESULT CImGui::End_ImGui()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

HRESULT CImGui::Init_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);


	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	

	//ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.MousePos;
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);



	return S_OK;
}

void CImGui::Create_Obj()
{
	CMonster::CMonster_DESC dec = {};
	CForkLift::CForkLift_DESC dec2 = {};
	dec2.fPosition.x=dec.fPosition.x= fPickpos[0];
	dec2.fPosition.y=dec.fPosition.y= fPickpos[1];
	dec2.fPosition.z	=dec.fPosition.z= fPickpos[2];
	dec2.fPosition.w =dec.fPosition.w = 1.0f;
	dec2.fScale.x=dec.fScale.x = OBJPROPERTY::fScale[0];
	dec2.fScale.y=dec.fScale.y = OBJPROPERTY::fScale[1];
	dec2.fScale.z=dec.fScale.z = OBJPROPERTY::fScale[2];
	dec2.fRotateAxis.x=	dec.fRotateAxis.x = OBJPROPERTY::fRotateAxis[0];
	dec2.fRotateAxis.y=	dec.fRotateAxis.y = OBJPROPERTY::fRotateAxis[1];
	dec2.fRotateAxis.z=	dec.fRotateAxis.z = OBJPROPERTY::fRotateAxis[2];
	dec2.fRotateAngle=dec.fRotateAngle = OBJPROPERTY::fRotateAngle;
	dec2.fPosition.w=	dec.fPosition.w = 1.0f;
	//dec.m_strObjName = TEXT("Test");

	//char 형의 문자열을 wchar_t형으로 변환하는 코드
	size_t nLen = std::strlen(InputName);
	dec.m_strObjName.resize(nLen);
	std::copy(InputName, InputName + nLen, dec.m_strObjName.begin());

	if (IsAnimObject)
	{
		m_pGameInstance->Add_CopyCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_2_Monster"), TEXT("Prototype_GameObject_AnimObject"), &m_pGameObj, &dec);
	}
	else
	{
		m_pGameInstance->Add_CopyCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_2_Monster"), TEXT("Prototype_GameObject_NoAnimObject"), &m_pGameObj, &dec2);
	}
	CTransform* pGameObjTransCom= dynamic_cast<CTransform*>(m_pGameObj->Get_Component(TEXT("Com_Transform")));
	_matrix fWorldMatrix = pGameObjTransCom->Get_WorldMatrix();

	dec.m_strObjName+=std::to_wstring(static_cast<int>(m_pGameObj->Get_ObjID()));

	//Save_Data(L"Layer_2_Monster", fWorldMatrix, dec.m_strObjName);

	

	m_mapGameObj.emplace(m_pGameObj->Get_ObjID(), m_pGameObj);
	//m_vcGameObjs.push_back(m_pGameObj);


}

void CImGui::Clicked_Pick_Position()
{

	_bool		isPick = { false };
	_vector		vTargetPos = {};
	_float3		fpickedPos = {};

	vTargetPos = m_pGameInstance->Picking(&isPick);
	XMStoreFloat3(&fpickedPos, vTargetPos);
	fPickpos[0]= fpickedPos.x;
	fPickpos[1]= fpickedPos.y;
	fPickpos[2]= fpickedPos.z;


}

HRESULT CImGui::Make_Navigation()
{
	_bool		isPick = { false };
	_vector		vTargetPos = {};
	_float3		fpickedPos = {};

	vTargetPos = m_pGameInstance->Picking(&isPick);
	XMStoreFloat3(&fpickedPos, vTargetPos);
	fPickpos[0] = fpickedPos.x;
	fPickpos[1] = fpickedPos.y;
	fPickpos[2] = fpickedPos.z;

	_float3		vPoints[3] = {};

	//iNavivertexcount

	ofstream outFile("../Bin/DataFiles/Navigation.dat", ios::out | std::ios::binary);

	if (outFile.is_open())
	{
	

	}

	return S_OK;
}

HRESULT CImGui::Make_Particle(wstring wstrModelName, const char* cstr, void* arg)
{
	CVIBuffer_Instance::INSTANCE_DESC*  Instance_desc = static_cast<CVIBuffer_Instance::INSTANCE_DESC*>(arg);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, wstrModelName, CModel::Create(m_pDevice, m_pContext, cstr, (*Instance_desc)))))
		return E_FAIL;

	iProtoTypeParticleCount++;


	return S_OK;
}

void CImGui::Find_Particle_Files(const char* cstr)
{
	string inputMeshName = cstr;
	wstring wstr(inputMeshName.begin(), inputMeshName.end());
	decltype(auto) load_data	= Load_Data_Text<wstring, wstring,
		wstring,_uint,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float, 
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float, 
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float,
		wstring,float>(wstr);

	PARTICLEPROPERTY::iNumInstance = get<3>(load_data);
	PARTICLEPROPERTY::vOffsetPos[0] = get<5>(load_data);
	PARTICLEPROPERTY::vOffsetPos[1] = get<7>(load_data);
	PARTICLEPROPERTY::vOffsetPos[2] = get<9>(load_data);
	PARTICLEPROPERTY::vPivotPos[0] = get<11>(load_data);
	PARTICLEPROPERTY::vPivotPos[1] = get<13>(load_data);
	PARTICLEPROPERTY::vPivotPos[2] = get<15>(load_data);
	PARTICLEPROPERTY::vRange[0] = get<17>(load_data);
	PARTICLEPROPERTY::vRange[1] = get<19>(load_data);
	PARTICLEPROPERTY::vRange[2] = get<21>(load_data);

	PARTICLEPROPERTY::vSize[0] = get<23>(load_data);
	PARTICLEPROPERTY::vSize[1] = get<25>(load_data);


	PARTICLEPROPERTY::vSpeed[0] = get<27>(load_data);
	PARTICLEPROPERTY::vSpeed[1] = get<29>(load_data);
	PARTICLEPROPERTY::vLifeTime[0] = get<31>(load_data);
	PARTICLEPROPERTY::vLifeTime[1] = get<33>(load_data);
	PARTICLEPROPERTY::vIsLoop = get<35>(load_data);
	PARTICLEPROPERTY::vTargetPos[0] = get<37>(load_data);
	PARTICLEPROPERTY::vTargetPos[1] = get<39>(load_data);
	PARTICLEPROPERTY::vTargetPos[2] = get<41>(load_data);


	PARTICLEPROPERTY::fSize[0] = get<43>(load_data);
	PARTICLEPROPERTY::fSize[1] = get<45>(load_data);
	PARTICLEPROPERTY::fSize[2] = get<47>(load_data);



	//
	//
	//
	//
	//
	//
	////decltype(auto) loadedData = Load_Data<_matrix, wstring, _matrix, wstring>(strLayerTag);	//타입과 갯수를 지정해줘야함
	////_matrix pWorldMatrix = get<0>(loadedData);
	////wstring pObjname = get<1>(loadedData);
	////_matrix pWorldMatri1 = get<2>(loadedData);
	////wstring pObjname1 = get<3>(loadedData);
	//
	//
	//
	
}

tuple<int, bool, CGameObject*> CImGui::Find_Particle(_float fObjId)
{

	auto iter =m_mapGameObj.find(fObjId);
	if(iter==m_mapGameObj.end())
		return make_tuple(0, false,nullptr);
	if (typeid(*iter->second) == typeid(CParticle_Mesh))
	{
		return make_tuple((int)dynamic_cast<CParticle_Mesh*>(iter->second)->Get_ObjID(), true, iter->second);
	}
	
}


void CImGui::ImGui_Property()
{
	ImGui::Begin("Property", nullptr, ImGuiWindowFlags_HorizontalScrollbar/*| ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize*/);
	ImGui::Text("Frame:%f", ImGui::GetIO().Framerate);
	ImGui::Text("Mouse:%.2f ,%.2f", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "PickPosX:%.2f", fPickpos[0]);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "PickPosY:%.2f", fPickpos[1]);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "PickPosZ:%.2f", fPickpos[2]);
	//ImGui::Text("PickPos:%.2f,%.2f,%.2f", fPickpos[0], fPickpos[1], fPickpos[2]);
	ImGui::Text("Press  8 :Delete Obj ");
	ImGui::Checkbox("Create Mode", &IsCreateMode);
	ImGui::Checkbox("Anim object", &IsAnimObject);
	ImGui::Checkbox("Particle Mode", &IsParticleMode);
	ImGui::End();

}

void CImGui::ImGui_ObjProperty()
{
	ImGui::Begin("OBJ_Property", nullptr, ImGuiWindowFlags_HorizontalScrollbar/*| ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize*/);
	ImGui::InputFloat3("Scale", OBJPROPERTY::fScale);
	ImGui::InputFloat3("RotateAxis", OBJPROPERTY::fRotateAxis);
	ImGui::InputFloat("RotateAngle", &(OBJPROPERTY::fRotateAngle));
	ImGui::InputFloat3("Pos", OBJPROPERTY::fPos);
	ImGui::Text("ObjID:%f", fObjID);
	ImGui::InputText("ObjName", InputName, sizeof(InputName));
	
	if (ImGui::Button("Create Obj"))
	{
		Create_Obj();
	}

	IsSave=ImGui::Button("Save");
	
	ImGui::End();
}

void CImGui::ImGui_ParticleProperty()
{
	ImGui::Begin("Particle_Property", nullptr, ImGuiWindowFlags_HorizontalScrollbar/*| ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize*/);
	CVIBuffer_Instance::INSTANCE_DESC desc= {};

	if (IsChangeSlideParticleMode)
	{
		PreiNumInstance = PARTICLEPROPERTY::iNumInstance;
		PrevOffsetPos[0] = PARTICLEPROPERTY::vOffsetPos[0];
		PrevOffsetPos[1] = PARTICLEPROPERTY::vOffsetPos[1];
		PrevOffsetPos[2] = PARTICLEPROPERTY::vOffsetPos[2];
		PrevPivotPos[0] = PARTICLEPROPERTY::vPivotPos[0];
		PrevPivotPos[1] = PARTICLEPROPERTY::vPivotPos[1];
		PrevPivotPos[2] = PARTICLEPROPERTY::vPivotPos[2];
		PrevRange[0] = PARTICLEPROPERTY::vRange[0];
		PrevRange[1] = PARTICLEPROPERTY::vRange[1];
		PrevRange[2] = PARTICLEPROPERTY::vRange[2];
		PrevSize[0] = PARTICLEPROPERTY::vSize[0];
		PrevSize[1] = PARTICLEPROPERTY::vSize[1];

		PrefSize[0] = PARTICLEPROPERTY::fSize[0];
		PrefSize[1] = PARTICLEPROPERTY::fSize[1];
		PrefSize[2] = PARTICLEPROPERTY::fSize[2];

		PrevSpeed[0] = PARTICLEPROPERTY::vSpeed[0];
		PrevSpeed[1] = PARTICLEPROPERTY::vSpeed[1];
		PrevLifeTime[0] = PARTICLEPROPERTY::vLifeTime[0];
		PrevLifeTime[1] = PARTICLEPROPERTY::vLifeTime[1];
		PrevTargetPos[0] = PARTICLEPROPERTY::vTargetPos[0];
		PrevTargetPos[1] = PARTICLEPROPERTY::vTargetPos[1];
		PrevTargetPos[2] = PARTICLEPROPERTY::vTargetPos[2];
		PrevIsLoop = PARTICLEPROPERTY::vIsLoop;
		PreTypeIndex = PARTICLEPROPERTY::iTypeIndex;



		ImGui::SliderInt("NumInstance", &PARTICLEPROPERTY::iNumInstance, 1, (PARTICLEPROPERTY::iNumMaxInstance));
		ImGui::SliderFloat3("OffsetPos", PARTICLEPROPERTY::vOffsetPos, 0.0f, 100.0f);
		ImGui::SliderFloat3("PivotPos", PARTICLEPROPERTY::vPivotPos, 0.0f, 100.0f);
		ImGui::SliderFloat3("Range", PARTICLEPROPERTY::vRange, 0.0f, 100.0f);
		ImGui::InputFloat("X_Size", &PARTICLEPROPERTY::fSize[0]);
		ImGui::InputFloat("Y_Size", &PARTICLEPROPERTY::fSize[1]);
		ImGui::InputFloat("Z_Size", &PARTICLEPROPERTY::fSize[2]);

		ImGui::SliderFloat2("Size", PARTICLEPROPERTY::vSize, 0.0f, 10.0f);
		ImGui::SliderFloat2("Speed", PARTICLEPROPERTY::vSpeed, 0.0f, 10.0f);
		ImGui::SliderFloat2("LifeTime", PARTICLEPROPERTY::vLifeTime, 0.0f, 10.0f);
		ImGui::SliderFloat3("TargetDir", PARTICLEPROPERTY::vTargetPos, 0.0f, 100.0f);
		ImGui::Checkbox("IsLoop", &PARTICLEPROPERTY::vIsLoop);
		desc.iNumInstance = PARTICLEPROPERTY::iNumInstance;
		desc.vOffsetPos.x = PARTICLEPROPERTY::vOffsetPos[0];
		desc.vOffsetPos.y = PARTICLEPROPERTY::vOffsetPos[1];
		desc.vOffsetPos.z = PARTICLEPROPERTY::vOffsetPos[2];
		desc.vPivotPos.x = PARTICLEPROPERTY::vPivotPos[0];
		desc.vPivotPos.y = PARTICLEPROPERTY::vPivotPos[1];
		desc.vPivotPos.z = PARTICLEPROPERTY::vPivotPos[2];
		desc.vRange.x = PARTICLEPROPERTY::vRange[0];
		desc.vRange.y = PARTICLEPROPERTY::vRange[1];
		desc.vRange.z = PARTICLEPROPERTY::vRange[2];

		desc.vSize.x = PARTICLEPROPERTY::vSize[0];
		desc.vSize.y = PARTICLEPROPERTY::vSize[1];

		desc.fSize_X= PARTICLEPROPERTY::fSize[0];
		desc.fSize_Y = PARTICLEPROPERTY::fSize[1];
		desc.fSize_Z = PARTICLEPROPERTY::fSize[2];


		desc.vSpeed.x = PARTICLEPROPERTY::vSpeed[0];
		desc.vSpeed.y = PARTICLEPROPERTY::vSpeed[1];
		desc.vLifeTime.x = PARTICLEPROPERTY::vLifeTime[0];
		desc.vLifeTime.y = PARTICLEPROPERTY::vLifeTime[1];
		desc.vTargetPos.x = PARTICLEPROPERTY::vTargetPos[0];
		desc.vTargetPos.y = PARTICLEPROPERTY::vTargetPos[1];
		desc.vTargetPos.z = PARTICLEPROPERTY::vTargetPos[2];
		desc.iType_Index= PARTICLEPROPERTY::iTypeIndex;
		desc.isLoop = PARTICLEPROPERTY::vIsLoop;


	

		//Find를 통해 찾았다면
		if (m_pGameObjParticle != nullptr)
		{
			//이전 값이랑 틀릴 때만 갱신
			if (PreiNumInstance != PARTICLEPROPERTY::iNumInstance
				|| PrevOffsetPos[0] != PARTICLEPROPERTY::vOffsetPos[0] 
				|| PrevOffsetPos[1] != PARTICLEPROPERTY::vOffsetPos[1] 
				|| PrevOffsetPos[2] != PARTICLEPROPERTY::vOffsetPos[2] 
				|| PrevPivotPos[0] != PARTICLEPROPERTY::vPivotPos[0] 
				|| PrevPivotPos[1] != PARTICLEPROPERTY::vPivotPos[1] 
				|| PrevPivotPos[2] != PARTICLEPROPERTY::vPivotPos[2] 
				|| PrevRange[0] != PARTICLEPROPERTY::vRange[0] 
				|| PrevRange[1] != PARTICLEPROPERTY::vRange[1] 
				|| PrevRange[2] != PARTICLEPROPERTY::vRange[2] 
				|| PrevSize[0] != PARTICLEPROPERTY::vSize[0] 
				|| PrevSize[1] != PARTICLEPROPERTY::vSize[1]

				|| PrefSize[0] != PARTICLEPROPERTY::fSize[0]
				|| PrefSize[1] != PARTICLEPROPERTY::fSize[1]
				|| PrefSize[2] != PARTICLEPROPERTY::fSize[2]

				|| PrevSpeed[0] != PARTICLEPROPERTY::vSpeed[0] 
				|| PrevSpeed[1] != PARTICLEPROPERTY::vSpeed[1] 
				|| PrevLifeTime[0] != PARTICLEPROPERTY::vLifeTime[0] 
				|| PrevLifeTime[1] != PARTICLEPROPERTY::vLifeTime[1] 
				|| PrevIsLoop != PARTICLEPROPERTY::vIsLoop
				|| PrevTargetPos[0] != PARTICLEPROPERTY::vTargetPos[0]
				|| PrevTargetPos[1] != PARTICLEPROPERTY::vTargetPos[1]
				|| PrevTargetPos[2] != PARTICLEPROPERTY::vTargetPos[2]
				|| PreTypeIndex != PARTICLEPROPERTY::iTypeIndex

				)
			{

				dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleProperty(desc);
				g_Desc = desc;


			}

		}
	}
	else
	{
		ImGui::InputInt("NumInstance", &PARTICLEPROPERTY::iNumInstance);
		ImGui::InputFloat3("OffsetPos", PARTICLEPROPERTY::vOffsetPos);
		ImGui::InputFloat3("PivotPos", PARTICLEPROPERTY::vPivotPos);
		ImGui::InputFloat3("Range", PARTICLEPROPERTY::vRange);
		ImGui::InputFloat2("Size", PARTICLEPROPERTY::vSize);

		ImGui::InputFloat("X_Size", &PARTICLEPROPERTY::fSize[0]);
		ImGui::InputFloat("Y_Size", &PARTICLEPROPERTY::fSize[1]);
		ImGui::InputFloat("Z_Size", &PARTICLEPROPERTY::fSize[2]);


		



		ImGui::InputFloat2("Speed", PARTICLEPROPERTY::vSpeed);
		ImGui::InputFloat2("LifeTime", PARTICLEPROPERTY::vLifeTime);
		ImGui::InputFloat3("TargetDir", PARTICLEPROPERTY::vTargetPos);

		ImGui::Checkbox("IsLoop", &PARTICLEPROPERTY::vIsLoop);
	}


	if (IsFiexdParticleMode)
	{
		IsChangeSlideParticleMode = true;
		static int iFindParticleNum = 0;
		ImGui::Text("Particle Create Num:%d ", iProtoTypeParticleCount);
		ImGui::InputInt("ParticleID", &iFindParticleNum);
		
		if (ImGui::Button("Find Particle"))
		{
			auto [iobjid,bFound, pParticleObject]=Find_Particle(iFindParticleNum);
			if (bFound)
			{
				if (pParticleObject == nullptr)
					return;
				m_pGameObjParticle = pParticleObject;
				desc =dynamic_cast<CParticle_Mesh*>(pParticleObject)->Get_InstanceBufferDesc();
				PARTICLEPROPERTY::iNumInstance = desc.iNumInstance;
				PARTICLEPROPERTY::iNumMaxInstance = desc.iNumInstance;
				PARTICLEPROPERTY::vOffsetPos[0] = desc.vOffsetPos.x;
				PARTICLEPROPERTY::vOffsetPos[1] = desc.vOffsetPos.y;
				PARTICLEPROPERTY::vOffsetPos[2] = desc.vOffsetPos.z;
				PARTICLEPROPERTY::vPivotPos[0] = desc.vPivotPos.x;
				PARTICLEPROPERTY::vPivotPos[1] = desc.vPivotPos.y;
				PARTICLEPROPERTY::vPivotPos[2] = desc.vPivotPos.z;
				PARTICLEPROPERTY::vRange[0] = desc.vRange.x;
				PARTICLEPROPERTY::vRange[1] = desc.vRange.y;
				PARTICLEPROPERTY::vRange[2] = desc.vRange.z;

				PARTICLEPROPERTY::vSize[0] = desc.vSize.x;
				PARTICLEPROPERTY::vSize[1] = desc.vSize.y;

				PARTICLEPROPERTY::fSize[0] = desc.fSize_X;
				PARTICLEPROPERTY::fSize[1] = desc.fSize_Y;
				PARTICLEPROPERTY::fSize[2] = desc.fSize_Z;


				PARTICLEPROPERTY::vSpeed[0] = desc.vSpeed.x;
				PARTICLEPROPERTY::vSpeed[1] = desc.vSpeed.y;
				PARTICLEPROPERTY::vLifeTime[0] = desc.vLifeTime.x;
				PARTICLEPROPERTY::vLifeTime[1] = desc.vLifeTime.y;
				PARTICLEPROPERTY::vTargetPos[0] = desc.vTargetPos.x;
				PARTICLEPROPERTY::vTargetPos[1] = desc.vTargetPos.y;
				PARTICLEPROPERTY::vTargetPos[2] = desc.vTargetPos.z;
				PARTICLEPROPERTY::iTypeIndex = desc.iType_Index;
				PARTICLEPROPERTY::vIsLoop = desc.isLoop;
				iFindParticleNum = iobjid;

			}
			else
				iFindParticleNum= 0;


		
		}

		ImGui::SameLine();
		ImGui::Text("ParticleID:%d ", iFindParticleNum);
		ImGui::InputText("SaveFileName", PARTICLEPROPERTY::cSaveMeshName, sizeof(PARTICLEPROPERTY::cSaveMeshName));
		ImGui::SameLine();
		if (ImGui::Button("Save Particle"))
		{
		_uint tmep=dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().iNumInstance;
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vOffsetPos.x;

			string inputMeshName = PARTICLEPROPERTY::cInputMeshName;
			string inputString = CImGui::PARTICLEPROPERTY::cSaveMeshName;


			// 자를 부분의 시작 인덱스와 끝 인덱스 설정
			std::size_t startIndex = inputMeshName.find_last_of('/') + 1; // 마지막 '/'의 다음 인덱스부터 시작
			std::size_t endIndex = inputMeshName.find_last_of('.') - startIndex; // '.' 이전까지가 자를 범위
			
			// 잘린 문자열 추출
			std::string extractedString = inputMeshName.substr(startIndex, endIndex);

			wstring wstrMesh(extractedString.begin(), extractedString.end());
			wstring wstr(inputString.begin(), inputString.end());
			
			_uint temp= dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().iType_Index;

			Save_Data_Text(wstr,
				L"MeshName :",wstrMesh,
				L"Instance :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().iNumInstance,
				L"OffsetPos.x :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vOffsetPos.x,
				L"OffsetPos.y :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vOffsetPos.y,
				L"OffsetPos.z :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vOffsetPos.z,
				L"PivotPos.x :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vPivotPos.x,
				L"PivotPos.y :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vPivotPos.y,
				L"PivotPos.z :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vPivotPos.z,
				L"Range.x :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vRange.x,
				L"Range.y :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vRange.y,
				L"Range.z :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vRange.z,

				L"Size.x :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vSize.x,
				L"Size.y :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vSize.y,

		
				L"Speed.x :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vSpeed.x,
				L"Speed.y :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vSpeed.y,
				L"LifeTime.x :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vLifeTime.x,
				L"LifeTime.y :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vLifeTime.y,
				L"IsLoop :", false,
				L"TargetPos.x :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vTargetPos.x,
				L"TargetPos.y :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vTargetPos.y,
				L"TargetPos.z :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().vTargetPos.z,
				L"TypeIndex :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().iType_Index,
				L"X_Size :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().fSize_X,
				L"Y_Size :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().fSize_Y,
				L"Z_Size :", dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Get_InstanceBufferDesc().fSize_Z

				);
		}
		if (ImGui::Checkbox("SPREAD", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SPREAD);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("DROP", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_DROP]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_DROP);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("UP_AND_DOWN", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN]))
		{
		dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_UP_AND_DOWN);

		}
		ImGui::SameLine();
		if (ImGui::Checkbox("EXPLOSION", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION]))
		{
			 dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_EXPLOSION);
		}
		if (ImGui::Checkbox("POP", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP]))
		{
			 dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_POP);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("SIZE_UP", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP]))
		{
			 dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("SIZE_DOWN", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN]))
		{
			 dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN);
		}
		if (ImGui::Checkbox("SIZE_DOWN_X", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_X]))
		{
			 dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_X);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("SIZE_DOWN_Y", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Y]))
		{
			 dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Y);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("SIZE_DOWN_Z", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z]))
		{
			 dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_DOWN_Z);
		}
		if (ImGui::Checkbox("SIZE_UP_X", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_X);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("SIZE_UP_Y", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Y]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Y);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("SIZE_UP_Z", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_SIZE_UP_Z);
		}
		


		if (ImGui::Checkbox("ROTATE_X", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_X]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_X);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("ROTATE_Y", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_Y]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_Y);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("ROTATE_Z", &bIsCreate[CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_Z]))
		{
			dynamic_cast<CParticle_Mesh*>(m_pGameObjParticle)->Set_ParticleType(CParticle_Mesh::PARTICLE_TYPE::PARTICLE_TYPE_ROTATION_Z);
		}
		
	}
	

	//PARTICLEPROPERTY::strMeshName.c_str
	if (!IsFiexdParticleMode)
	{
		IsChangeSlideParticleMode = false;
		ImGui::InputText("MeshName", PARTICLEPROPERTY::cInputMeshName, sizeof(PARTICLEPROPERTY::cInputMeshName));

		if (ImGui::Button("Create Particle"))
		{
			
				CVIBuffer_Instance::INSTANCE_DESC desc1 = {};
				desc1.iNumInstance = PARTICLEPROPERTY::iNumInstance;
				desc1.vOffsetPos.x = PARTICLEPROPERTY::vOffsetPos[0];
				desc1.vOffsetPos.y = PARTICLEPROPERTY::vOffsetPos[1];
				desc1.vOffsetPos.z = PARTICLEPROPERTY::vOffsetPos[2];
				desc1.vPivotPos.x = PARTICLEPROPERTY::vPivotPos[0];
				desc1.vPivotPos.y = PARTICLEPROPERTY::vPivotPos[1];
				desc1.vPivotPos.z = PARTICLEPROPERTY::vPivotPos[2];
				desc1.vRange.x = PARTICLEPROPERTY::vRange[0];
				desc1.vRange.y = PARTICLEPROPERTY::vRange[1];
				desc1.vRange.z = PARTICLEPROPERTY::vRange[2];

				desc1.vSize.x = PARTICLEPROPERTY::vSize[0];
				desc1.vSize.y = PARTICLEPROPERTY::vSize[1];

				desc1.fSize_X = PARTICLEPROPERTY::fSize[0];
				desc1.fSize_Y = PARTICLEPROPERTY::fSize[1];
				desc1.fSize_Z = PARTICLEPROPERTY::fSize[2];


				desc1.vSpeed.x = PARTICLEPROPERTY::vSpeed[0];
				desc1.vSpeed.y = PARTICLEPROPERTY::vSpeed[1];
				desc1.vLifeTime.x = PARTICLEPROPERTY::vLifeTime[0];
				desc1.vLifeTime.y = PARTICLEPROPERTY::vLifeTime[1];
				desc1.isLoop = PARTICLEPROPERTY::vIsLoop;


				string str(CImGui::PARTICLEPROPERTY::cInputMeshName);
				wstring wstr(str.begin(), str.end());
				const char* cstr = str.c_str();

				CParticle_Mesh::PARTICLE_DESC GameDesc = {};
				GameDesc.strModelName = wstr + to_wstring(iProtoTypeParticleCount);
				GameDesc.vStartPos.x = fPickpos[0];
				GameDesc.vStartPos.y = fPickpos[1];
				GameDesc.vStartPos.z = fPickpos[2];
				GameDesc.vStartPos.w = 1.0f;
				Make_Particle(GameDesc.strModelName, cstr, (&desc1));

				//m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_2_Monster"), TEXT("Prototype_GameObject_Particle_Mesh"), &GameDesc);
				m_pGameInstance->Add_CopyCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_2_Monster"), TEXT("Prototype_GameObject_Particle_Mesh"), &m_pGameObj, &GameDesc);
				if (m_pGameObj != nullptr)
				{
					m_mapGameObj.emplace(m_pGameObj->Get_ObjID(), m_pGameObj);
				}

		}
	}



	ImGui::End();
	ImGui::Begin("Particle_Test", nullptr, ImGuiWindowFlags_HorizontalScrollbar/*| ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize*/);
	ImGui::InputText("FindFileName", PARTICLEPROPERTY::cFindParticleName, sizeof(PARTICLEPROPERTY::cFindParticleName));
	ImGui::SameLine();
	if (ImGui::Button("Find Particle File"))
	{
		Find_Particle_Files(PARTICLEPROPERTY::cFindParticleName);
	}

	ImGui::End();
}

void CImGui::Select_OBJ()
{
	auto iter=m_mapGameObj.find(fObjID);

	if (iter == m_mapGameObj.end())
		return;

	

	if (bChangeMode)
	{
		iter->second->Set_Scaled(_float3(OBJPROPERTY::fScale[0], OBJPROPERTY::fScale[1], OBJPROPERTY::fScale[2]));
		iter->second->Set_Position(_float3(OBJPROPERTY::fPos[0], OBJPROPERTY::fPos[1], OBJPROPERTY::fPos[2]));
		iter->second->Set_Rotation(OBJPROPERTY::fRotateAngle, _float3(OBJPROPERTY::fRotateAxis[0], OBJPROPERTY::fRotateAxis[1], OBJPROPERTY::fRotateAxis[2]));
		
	}
	else
	{
		OBJPROPERTY::fScale[0] = iter->second->Get_Scaled().x;
		OBJPROPERTY::fScale[1] = iter->second->Get_Scaled().y;
		OBJPROPERTY::fScale[2] = iter->second->Get_Scaled().z;
		OBJPROPERTY::fPos[0] = iter->second->Get_Position().x;
		OBJPROPERTY::fPos[1] = iter->second->Get_Position().y;
		OBJPROPERTY::fPos[2] = iter->second->Get_Position().z;

	}


}

void CImGui::Delete_OBJ()
{
	auto iter = m_mapGameObj.find(fObjID);

	if (iter == m_mapGameObj.end())
		return;

	//iter->second->Get_ObjID();
	m_pGameInstance->Delete_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_2_Monster"), iter->second);
	m_mapGameObj.erase(iter);




}




void CImGui::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	

}
