// ��ɾ�			�ɼ�		���� ������ �ִ� ��ġ					�纻 ������ ������ ��ġ

xcopy			/y		.\Engine\Public\*.*					.\EngineSDK\Inc\
xcopy			/y/s	.\Engine\Public\*.*					.\EngineSDK\Inc\

xcopy			/y		.\Engine\Bin\Engine.dll				.\Client\Bin\
xcopy			/y		.\Engine\Bin\Engine.dll				.\Tool\Bin\
xcopy			/y		.\Engine\Bin\Engine.dll				.\FBXLoad_Proj\Bin\
xcopy			/y		.\Engine\Bin\Engine.lib				.\EngineSDK\Lib\
xcopy			/y		.\Engine\Bin\ShaderFiles\*.hlsl		.\Client\Bin\ShaderFiles\
xcopy			/y		.\Engine\Bin\ShaderFiles\*.hlsli	.\Client\Bin\ShaderFiles\



 xcopy			/y/s		.\Tool\Bin\Resources\Models\*.*			.\Client\Bin\Resources\Models\
