/*!
 *@brief	�L�����N�^�𓮂����T���v���v���O�����B���K�p�B
 */
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>
#include "sub.h"


//-----------------------------------------------------------------------------
// ����̎��K�ŋC�ɂ���K�v������O���[�o���ϐ��B
//-----------------------------------------------------------------------------
D3DXMATRIX				g_worldMatrix;										//�Ղ̃��[���h�s��B���f�����[�J����Ԃ���A���[���h��Ԃɕϊ�����s��B
D3DXVECTOR3				g_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�Ղ̍��W�B
float					g_currentAngleY = 0.0f;
/*!
*@brief	�Ղ̍X�V�����B
* ����̎��K�ł͂��̊֐���ҏW����B
*/
void UpdateTiger()
{
	//��L�[�������ꂽ�Ƃ��̔��蕶�͂���Ȋ����B
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		g_position.z += 0.1f;
		//180�x��������B
		g_currentAngleY = D3DXToRadian(180.0f);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		g_position.z -= 0.1f;
		//���ʂ���������B
		g_currentAngleY = D3DXToRadian(0.0f);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		g_position.x += 0.1f;
		//�E��������������B
		g_currentAngleY = D3DXToRadian(-90.0f);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000){
		//����������������
		g_currentAngleY = D3DXToRadian(90.0f);
		g_position.x -= 0.1f;
	}
	//���s�ړ��s����쐬����B
	D3DXMATRIX mTrans;
	D3DXMatrixTranslation(&mTrans, g_position.x, g_position.y, g_position.z);
	//��]�s����쐬����B
	D3DXMATRIX mRot;
	D3DXMatrixRotationY(&mRot, g_currentAngleY);
	g_worldMatrix = mRot * mTrans;
}
//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	// Clear the backbuffer to a blue color
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	static int renderCount = 0;
	
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		UpdateTiger();
		//���C�g���X�V
		UpdateLight();
		// Turn on the zbuffer
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		
		//�V�F�[�_�[�K�p�J�n�B
		g_pEffect->SetTechnique("SkinModel");
		g_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		g_pEffect->BeginPass(0);

		//�萔���W�X�^�ɐݒ肷��J���[�B
		D3DXVECTOR4 color( 1.0f, 0.0f, 0.0f, 1.0f);
		//���[���h�s��̓]���B
		g_pEffect->SetMatrix("g_worldMatrix", &g_worldMatrix);
		//�r���[�s��̓]���B
		g_pEffect->SetMatrix("g_viewMatrix", &g_viewMatrix);
		//�v���W�F�N�V�����s��̓]���B
		g_pEffect->SetMatrix("g_projectionMatrix", &g_projectionMatrix);
		//��]�s���]���B
		g_pEffect->SetMatrix( "g_rotationMatrix", &g_worldMatrix);
		//���C�g�̌�����]���B
		g_pEffect->SetVectorArray("g_diffuseLightDirection", g_diffuseLightDirection, LIGHT_NUM );
		//���C�g�̃J���[��]���B
		g_pEffect->SetVectorArray("g_diffuseLightColor", g_diffuseLightColor, LIGHT_NUM );
		//������ݒ�B
		g_pEffect->SetVector("g_ambientLight", &g_ambientLight);

		g_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
		
		// Meshes are divided into subsets, one for each material. Render them in
        // a loop
        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
			g_pEffect->SetTexture("g_diffuseTexture", g_pMeshTextures[i]);
            // Draw the mesh subset
            g_pMesh->DrawSubset( i );
        }
        
		g_pEffect->EndPass();
		g_pEffect->End();

		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*!
 *@brief	X�t�@�C����ǂݍ���ŃW�I���g�����������B
 */
HRESULT InitGeometry()
{
    LPD3DXBUFFER pD3DXMtrlBuffer;

    // Load the mesh from the specified file
    if( FAILED( D3DXLoadMeshFromX( "Tiger.x", D3DXMESH_SYSTEMMEM,
                                   g_pd3dDevice, NULL,
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
                                   &g_pMesh ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( "..\\Tiger.x", D3DXMESH_SYSTEMMEM,
                                       g_pd3dDevice, NULL,
                                       &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
                                       &g_pMesh ) ) )
        {
            MessageBox( NULL, "Could not find tiger.x", "Meshes.exe", MB_OK );
            return E_FAIL;
        }
    }
	//�@�������݂��邩���ׂ�B
	if ((g_pMesh->GetFVF() & D3DFVF_NORMAL) == 0) {
		//�@�����Ȃ��̂ō쐬����B
		ID3DXMesh* pTempMesh = NULL;

		g_pMesh->CloneMeshFVF(g_pMesh->GetOptions(),
			g_pMesh->GetFVF() | D3DFVF_NORMAL, g_pd3dDevice, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		g_pMesh->Release();
		g_pMesh = pTempMesh;

	}
    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
   
    g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
    if( g_pMeshTextures == NULL )
        return E_OUTOFMEMORY;

    for( DWORD i = 0; i < g_dwNumMaterials; i++ )
    {
        g_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFileA( g_pd3dDevice,
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &g_pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const CHAR* strPrefix = "..\\";
                CHAR strTexture[MAX_PATH];
                strcpy_s( strTexture, MAX_PATH, strPrefix );
                strcat_s( strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFileA( g_pd3dDevice,
                                                        strTexture,
                                                        &g_pMeshTextures[i] ) ) )
                {
                    MessageBox( NULL, "Could not find texture map", "Meshes.exe", MB_OK );
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Shader Tutorial", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow("Shader Tutorial", "Shader Tutorial 00",
		WS_OVERLAPPEDWINDOW, 100, 100, 1380, 820,
		NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the vertex buffer
		if (SUCCEEDED(InitGeometry()))
		{
			ZeroMemory( g_diffuseLightDirection, sizeof(g_diffuseLightDirection) );
			ZeroMemory( g_diffuseLightColor, sizeof(g_diffuseLightColor) );
			
			InitProjectionMatrix();
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass("Shader Tutorial", wc.hInstance);
	return 0;
}
