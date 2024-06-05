#pragma once

#include "Base.h"

/* ���� ��ü���� �׸��� ������ �����ؾ��Ѵ�. */
/* ��κ��� ��ü���� ���� �׽�Ʈ�� ���ؼ� �˾Ƽ� ��������� �ǰ� �ִ�. */
/* ���̹����� ����� ���� �˾Ƽ� ���̰������ȴ� ������ ū �����̱��ѵ�. */
/* �̷� ���ؼ� ����� ���ۿ��� �����ϳ�. */
/* ���ۿ� 1. ��ī�̹ڽ�. : ��ī�̹ڽ��� ������ �۰� �׸���.  */
/* �ϴ� : ������ �ٸ� ��ü�鿡�� �������Ѵ�(��������׸���) + �ϴ��� ���̴� �ٸ���ü��� �񱳿����� �Ͼ���� �ȵȴ�.(�ϴ��� ���̸� ���̹��ۿ� ������� �ʴ´�.) */
/* ���ۿ� 2. ���ĺ����� :   */
/* ������ : ���� �׸������ϴ� ���� ������ �׷����ִ� ���� ���´�. */
/* ���ۿ� 3. UI.  */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_SHADOWOBJ, RENDER_MASK, RENDER_NONBLEND, RENDER_NONLIGHT, RENDER_BLEND,RENDER_EFFECT, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	void Clear();
	void Draw();

#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pComponent);
#endif

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	list<class CGameObject*>			m_RenderGroup[RENDER_END];

private:
	class CShader* m_pShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	class CTexture*  m_pTexture = { nullptr };

	_float4x4							m_WorldMatrix = {};
	_float4x4							m_ViewMatrix = {};
	_float4x4							m_ProjMatrix = {};

	ID3D11DepthStencilView* m_pLightDepthStencilView = { nullptr };

private:
	void  Render_Priority();
	void  Render_ShadowObjects();
	void  Render_Mask();
	void  Render_NonBlend();
	void  Render_LightAcc();
	void  Render_DeferredResult();
	void  Render_Bloom();
	void  Render_FinalBlend();
	void  Render_NonLight();
	void  Render_Blend();
	void  Render_Effect();
	void  Render_UI();
#ifdef _DEBUG
private:
	list<class CComponent*>				m_DebugComponents;
private:
	void Render_Debug();
#endif

private:

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END