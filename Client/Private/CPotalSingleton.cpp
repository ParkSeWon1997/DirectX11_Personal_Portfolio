#include "stdafx.h"
#include "..\Public\CPotalSingleton.h"

#include "GameInstance.h"
#include"Loader.h"



IMPLEMENT_SINGLETON(CPotalSingleton)

CPotalSingleton::CPotalSingleton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{


}

void CPotalSingleton::Free()
{
}
