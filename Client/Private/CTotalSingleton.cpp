#include "stdafx.h"
#include "..\Public\CTotalSingleton.h"

#include "GameInstance.h"
#include"Loader.h"



IMPLEMENT_SINGLETON(CTotalSingleton)

CTotalSingleton::CTotalSingleton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{


}

void CTotalSingleton::Free()
{
}
