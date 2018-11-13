#include "���.h"

// Vertex(����)

// 3D �׷��ȿ��� �ּ� ó�� ����
// ���(�ﰢ��)�� �����ϴ� 3���� ����

// 3�����󿡼� ��� ������Ʈ�� ǥ���ϱ����� ,,, �ּ�(�⺻) ������        ( 2���������� ǥ���ϱ����� �⺻������ ��ǥ�ΰ� ó�� )
// ������, �ؽ������� ���� �����Ҽ� �ִ�.
// �ϳ��� ǥ�������� �⺻�� �ﰢ��,,, 3���� ���ؽ�
// ��鿡 ���� ó�� -> rasterize

// ���Ƿ� ����� = Ŀ���� ���ؽ�





// 11.13
//���� ���� ��ȭ ���� ���� : OPENFILENAME


// Matrix (���)



// DirectX ���� ���� ����

LPDIRECT3D9				g_pD3D			= NULL;			// D3D ��ü ������					���̷�Ʈ3D ��ü
LPDIRECT3DDEVICE9		g_pD3DDevice	= NULL;			// D3D Device ��ü ������			���̷�Ʈ�� �����ϴ� 3D Device ��ü
LPDIRECT3DVERTEXBUFFER9	g_pVB			= NULL;			// D3D Vertex Buffer ��ü ������

//
struct CUSTOMVERTEX
{
	FLOAT				x, y, z;				// ��ȯ���� ���� 3���� ��ǥ
	DWORD				color;
};

#define FVF				(D3DFVF_XYZ | D3DFVF_DIFFUSE) // ���������� ����ִ°�


// Window ���� ���� ����
HWND					g_hWnd;
HINSTANCE				g_hInstance;

HRESULT					InitD3D(HWND hwnd);
HRESULT					InitVB();

VOID					SetupMetrices();
VOID					CleanUp();
VOID					Render();

LRESULT WINAPI	MsgProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI	WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Tutorial", NULL
	};

	RegisterClassEx(&wc);

	g_hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial", WS_OVERLAPPEDWINDOW,
		100, 100, 1000, 800, NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(g_hWnd)))
	{
		if (SUCCEEDED(InitVB()))
		{
			ShowWindow(g_hWnd, SW_SHOWDEFAULT);
			UpdateWindow(g_hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					Render();
				}
			}
		}
	}

	UnregisterClass("D3D Tutorial", wc.hInstance);
	
	return 0;
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		Render();
		ValidateRect(hWnd, NULL);
		return 0;
	case WM_DESTROY:
		CleanUp();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HRESULT InitD3D(HWND hWnd) // ���̷�Ʈ     �ʱ�ȭ �۾�
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) // ��������� DLL������ �´��� Ȯ��
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp; // PRESENT = ȭ�鿡 ǥ���Ѵ�.
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // ���۹� ���� ���� ����Ʈ,, DISCARD = ����°�,,
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,  // �ϵ���� ���� �Ծ��� ���ִ���? Ȯ��
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, // ���ý��� �����ϴ°��� �ϵ�������� ����Ʈ��������
		&d3dpp,
		&g_pD3DDevice)))
	{
		return E_FAIL;
	}

	// �������ҋ� ��� ó���Ұ���
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // �����̽� �ø��� ���ϰٴٴ°�.
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ��,, ���� ó��

	return S_OK;
}

HRESULT InitVB()
{
	CUSTOMVERTEX vertices[] =						// ���ؽ��� ���� �迭�� �ֱ�
	{
		//{ 150.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, },
		//{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
		//{ 50.0f, 250.0f, 0.5f, 1.0f, 0xff0000ff, },

		{ -1.0f, -1.0f, 0.0f, 0xffff0000, },
		{ 1.0f, -1.0f, 0.0f, 0xff00ff00, },
		{ 0.0f, 1.0f, 0.0f, 0xff0000ff, },
	};

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0,
		FVF, D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pVertices;

	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();


	return S_OK;
}

#define ROTATIONSPEED 10000 //�⺻ 1000 ,, % ���̴ϱ� Ŭ���� ������,,

VOID SetupMetrices() // ��� �ٽ�,, ���� ó��
{
	// ���� ��ȯ ,, ��ǥ�� ȭ����� ��ǥ�� ��ȯ�ϴ°�
	D3DXMATRIX matWorld;
	
	UINT iTime = timeGetTime() % ROTATIONSPEED;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / ROTATIONSPEED;
	D3DXMatrixRotationZ(&matWorld, fAngle);
//	D3DXMatrixRotationY(&matWorld, fAngle);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld); // ���� ��ȯ ����

	// �� ��ȯ = ��ġ
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // ��� ����

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView); // �� ��ȯ ����

	// ���� ��ȯ = 2���������� 3���������� �ٲ��ִ°� ( ī�޶� ������ �������� ),, 3���������� 2���� ������ ��ȯ
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.f); // �޼� ��ǥ��� ����Ѵٴ� ��,,, ���̷�Ʈx�� �⺻������ �޼���ǥ�踦 �����.
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj); // ���� ��ȯ ����


	//return VOID();
}

VOID CleanUp() // ���������� �������� ����,, g_pD3D�� ������� g_pD3DDevice�� ��������ϱ�,
{
	if (g_pVB != NULL)
		g_pVB->Release();
	if (g_pD3DDevice != NULL)
		g_pD3DDevice->Release();
	if (g_pD3D != NULL)
		g_pD3D->Release();
}

VOID Render()
{
	if (NULL == g_pD3DDevice) // �ִ��� ������ Ȯ�� ,, �긦 ���� ������ �̷�����ϱ�
		return;

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene())) // ��信 �����Ҽ��ְ� ����
	{
		//  �� ���̿� ������ �۾��ؾߵ�

		// ��ȯ �۾�
		SetupMetrices();

		// �׸��� �۾�
		g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX)); // vertex ������ ��� �о����
		g_pD3DDevice->SetFVF(FVF);										// vertex ������ ����
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);			// vertex ������ ������ �׸���, ���۹��ؽ� ( 0�̸� ó�� ), ��� �׸���

		//

		//
		g_pD3DDevice->EndScene(); //
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL); // ȭ�����
}