#pragma once

#pragma warning (disable : 4251)

#include "document.h"
#include "writer.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "pointer.h"
#include "rapidjson.h"
using namespace rapidjson;

#include <fstream>
#include <iostream>
#include <tuple>
#include<type_traits>
#include<variant>
#include <d3d11.h>
#include<random>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>
#include "fx11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"
#include "DirectXTK\Keyboard.h"
#include "DirectXTK\Mouse.h"

#include "DirectXTK\ScreenGrab.h"
#include "DirectXTK\PrimitiveBatch.h"
#include "DirectXTK\VertexTypes.h"
#include "DirectXTK\SpriteBatch.h"
#include "DirectXTK\SpriteFont.h"
#include "DirectXTK\Effects.h"



#include "assimp\scene.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"


using namespace DirectX;
using namespace TriangleTests;

#include <vector>
#include <list>
#include <map>
#include <string>
#include<sstream>
#include<iomanip>
#include <algorithm>
#include <functional>
#include<locale>
#include<string_view>

#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")
#include <io.h>


namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
	enum CHANNELID { SOUND_EFFECT, SOUND_PLAYER, SOUND_MONSTER, SOUND_BOSS, SOUND_BGM, MAXCHANNEL };
}

#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"



#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace Engine;
using namespace std;

