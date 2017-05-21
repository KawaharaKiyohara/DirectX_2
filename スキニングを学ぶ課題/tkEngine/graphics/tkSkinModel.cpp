#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkLight.h"
#include <map>
namespace tkEngine{
	extern UINT                 g_NumBoneMatricesMax;
	extern D3DXMATRIXA16*       g_pBoneMatrices ;
	
	
	namespace {
		std::map < LPD3DXMESH, char*> orgVertexBuffers;
		
		//���_�\���́B
		struct Vertex {
			D3DXVECTOR4	position;		//���W�B
			D3DXVECTOR4 blendWeight;	//���_�E�F�C�g�B
			D3DXVECTOR4 blendIndices;	//�{�[���C���f�b�N�X�B
			D3DXVECTOR3 normal;			//�@���B
			D3DXVECTOR3 tangent;		//�ڃx�N�g���B
			D3DXVECTOR2 uv;				//UV���W�B
		};
		void DrawMeshContainer(
			IDirect3DDevice9* pd3dDevice, 
			LPD3DXMESHCONTAINER pMeshContainerBase, 
			LPD3DXFRAME pFrameBase,
			ID3DXEffect* pEffect,
			D3DXMATRIX* worldMatrix,
			D3DXMATRIX* rotationMatrix,
			D3DXMATRIX* viewMatrix,
			D3DXMATRIX* projMatrix,
			CLight* light,
			CTexture* normalMap
		)
		{
			D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
			D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
			UINT iAttrib;
			LPD3DXBONECOMBINATION pBoneComb;

			D3DXMATRIXA16 matTemp;
			D3DCAPS9 d3dCaps;
			pd3dDevice->GetDeviceCaps(&d3dCaps);
			D3DXMATRIX viewProj;
			D3DXMatrixMultiply(&viewProj, viewMatrix, projMatrix);
			
			//�e�N�j�b�N��ݒ�B
			{
				if (pMeshContainer->pSkinInfo != NULL) {
					pEffect->SetTechnique("SkinModel");
				}
				else {
					pEffect->SetTechnique("NoSkinModel");
				}
			}
			//���ʂ̒萔���W�X�^��ݒ�
			{
				//�r���[�v���W�F�N�V����
				pEffect->SetMatrix("g_mViewProj", &viewProj);
				//���C�g
				pEffect->SetValue(
					"g_light",
					light,
					sizeof(CLight)
				);
				if (normalMap != nullptr) {
					pEffect->SetBool("g_isHasNormalMap", true);
					pEffect->SetTexture("g_normalTexture", normalMap->GetTextureDX());
				}
				else {
					pEffect->SetBool("g_isHasNormalMap", false);
				}
			}
			if (pMeshContainer->pSkinInfo != NULL)
			{
				//�X�L�����L��B
				pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
				for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
				{
					// first calculate all the world matrices
					for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
					{
						DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
						if (iMatrixIndex != UINT_MAX)
						{
							D3DXMatrixMultiply(
								&g_pBoneMatrices[iPaletteEntry],
								&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
								pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
							);
							//D3DXMatrixMultiply(&g_pBoneMatrices[iPaletteEntry], &matTemp, &g_matView);
						}
					}
					auto orgMesh = orgVertexBuffers.find(pMeshContainer->MeshData.pMesh);
					if (orgMesh == orgVertexBuffers.end()) {
						//NULL�B�I���W�i���̒��_�o�b�t�@���R�s�[�B
						LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
						pMeshContainer->MeshData.pMesh->GetVertexBuffer(&vertexBuffer);
						void* vertexPos;

						D3DVERTEXBUFFER_DESC desc;
						vertexBuffer->GetDesc(&desc);
				
						//���_�o�b�t�@�����b�N�B
						vertexBuffer->Lock(0, desc.Size, (void**)&vertexPos, D3DLOCK_DISCARD);
						//���_�o�b�t�@���R�s�[�B
						char* p = new char[desc.Size];
						memcpy(p, vertexPos, desc.Size);
						orgVertexBuffers.insert(std::pair<LPD3DXMESH, char*>(
							pMeshContainer->MeshData.pMesh,
							p
							));
						orgMesh = orgVertexBuffers.find(pMeshContainer->MeshData.pMesh);
						vertexBuffer->Unlock();

					}

					{///////////////////////////�@��������\�t�g�E�F�A�X�L�j���O�B


						//�����̒��_�o�b�t�@���擾����B
						LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
						pMeshContainer->MeshData.pMesh->GetVertexBuffer(&vertexBuffer);
						Vertex* vertex;
						Vertex* orgVertex = (Vertex*)orgMesh->second;	//���̃��f���̒��_�B
						
						D3DVERTEXBUFFER_DESC desc;
						vertexBuffer->GetDesc(&desc);

						//���_�o�b�t�@�����b�N�B
						vertexBuffer->Lock(0, desc.Size, (void**)&vertex, D3DLOCK_DISCARD);
						
						for (int vertNo = 0; vertNo < pMeshContainer->MeshData.pMesh->GetNumVertices(); vertNo++) {
							vertex->position = D3DXVECTOR4(.0f, 0.0f, 0.0f, 1.0f);
							int boneIndex[4];
							boneIndex[0] = orgVertex->blendIndices[2] * 255;
							boneIndex[1] = orgVertex->blendIndices[1] * 255;
							boneIndex[2] = orgVertex->blendIndices[0] * 255;
							boneIndex[3] = orgVertex->blendIndices[3] * 255;
							///////////////////////////////////////////////////////////////////
							//�����ɃX�L�j���O�̏������L�q����B
							//�I���W�i���ƂȂ郂�f���̒��_		orgVertex->position
							//�X�L�j���O��̒��_�̊i�[��		vertex->position
							//�{�[���s��̃A�N�Z�X�̎d��		g_pBoneMatrices[boneIndex[bone]]
							//�X�L���E�F�C�g					orgVertex->blendWeight[bone]
							///////////////////////////////////////////////////////////////////
			
							for (int bone = 0; bone < 2; bone++) {	//���͂Q�{�B
								D3DXVECTOR4 vTmp;
								//���̃��f���̒��_���{�[���s��ŕϊ����āA���̌��ʂ�vTmp�Ɋi�[����B
								D3DXVec4Transform(&vTmp, &orgVertex->position, &g_pBoneMatrices[boneIndex[bone]]);
								//��Ōv�Z�����A�j���[�V������̒��_���W�ɃX�L���E�F�C�g(�d��)����Z����B
								vertex->position += vTmp * orgVertex->blendWeight[bone];
							}
							vertex++;
							orgVertex++;
	
					
						}
						//���_�o�b�t�@���A�����b�N�B
						vertexBuffer->Unlock();
						vertexBuffer->Release();
					}
					
				
					// �f�B�t���[�Y�e�N�X�`���B
					pEffect->SetTexture("g_diffuseTexture", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);


					D3DXMATRIX viewRotInv;
					D3DXMatrixInverse(&viewRotInv, NULL, viewMatrix);
					viewRotInv.m[3][0] = 0.0f;
					viewRotInv.m[3][1] = 0.0f;
					viewRotInv.m[3][2] = 0.0f;
					viewRotInv.m[3][3] = 1.0f;
					D3DXMatrixTranspose(&viewRotInv, &viewRotInv);
					pEffect->SetMatrix("g_viewMatrixRotInv", &viewRotInv);

					D3DXMATRIX mWorld;
					D3DXMatrixIdentity(&mWorld);
					pEffect->SetMatrix("g_worldMatrix", &mWorld);
					pEffect->SetMatrix("g_rotationMatrix", rotationMatrix);

					pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
					pEffect->BeginPass(0);
					pEffect->CommitChanges();
					// draw the subset with the current world matrix palette and material state
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
					pEffect->EndPass();
					pEffect->End();

				}
			}
			else {
							
				D3DXMATRIX mWorld;
				if (pFrame != NULL) {
					mWorld = pFrame->CombinedTransformationMatrix;
				}
				else {
					mWorld = *worldMatrix;
				}

				pEffect->SetMatrix("g_worldMatrix", &mWorld);
				pEffect->SetMatrix("g_rotationMatrix", rotationMatrix);
				pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
				pEffect->BeginPass(0);

				for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++) {
					pEffect->SetTexture("g_diffuseTexture", pMeshContainer->ppTextures[i]);
					pEffect->CommitChanges();
					pMeshContainer->MeshData.pMesh->DrawSubset(i);
				}
				pEffect->EndPass();
				pEffect->End();
			}
		}
		void DrawFrame(
			IDirect3DDevice9* pd3dDevice, 
			LPD3DXFRAME pFrame, 
			ID3DXEffect* pEffect,
			D3DXMATRIX* worldMatrix,
			D3DXMATRIX* rotationMatrix,
			D3DXMATRIX* viewMatrix, 
			D3DXMATRIX* projMatrix,
			CLight* light,
			CTexture* normalMap)
		{
			LPD3DXMESHCONTAINER pMeshContainer;

			pMeshContainer = pFrame->pMeshContainer;
			while (pMeshContainer != NULL)
			{
				DrawMeshContainer(
					pd3dDevice, 
					pMeshContainer, 
					pFrame, 
					pEffect,
					worldMatrix,
					rotationMatrix,
					viewMatrix,
					projMatrix,
					light,
					normalMap
					);

				pMeshContainer = pMeshContainer->pNextMeshContainer;
			}

			if (pFrame->pFrameSibling != NULL)
			{
				DrawFrame(
					pd3dDevice, 
					pFrame->pFrameSibling, 
					pEffect,
					worldMatrix,
					rotationMatrix,
					viewMatrix,
					projMatrix,
					light,
					normalMap
					);
			}

			if (pFrame->pFrameFirstChild != NULL)
			{
				DrawFrame(
					pd3dDevice, 
					pFrame->pFrameFirstChild, 
					pEffect,
					worldMatrix,
					rotationMatrix,
					viewMatrix,
					projMatrix,
					light,
					normalMap
					);
			}
		}
	}
	CSkinModel::CSkinModel() :
		m_skinModelData(nullptr),
		m_worldMatrix(CMatrix::Identity),
		m_light(nullptr),
		m_normalMap(nullptr)
	{
	}
	CSkinModel::~CSkinModel()
	{

	}

	void CSkinModel::Init(CSkinModelData* modelData)
	{
		m_pEffect = EffectManager().LoadEffect("Assets/presetShader/skinModel.fx");
		m_skinModelData = modelData;
	}
	void CSkinModel::UpdateWorldMatrix(const CVector3& trans, const CQuaternion& rot, const CVector3& scale)
	{
		CMatrix mTrans, mScale;
		mTrans.MakeTranslation(trans);
		m_rotationMatrix.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_worldMatrix.Mul(mScale, m_rotationMatrix);
		m_worldMatrix.Mul(m_worldMatrix, mTrans);
		if (m_skinModelData) {
			m_skinModelData->UpdateBoneMatrix(m_worldMatrix);	//�{�[���s����X�V�B
		}
	}
	void CSkinModel::Draw(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		if (m_skinModelData) {
			renderContext.DrawSkinModel(this, viewMatrix, projMatrix);
		}
	}
	void CSkinModel::ImmidiateDraw(LPDIRECT3DDEVICE9 pd3ddevice, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix)
	{
		if (m_skinModelData) {
			DrawFrame(
				pd3ddevice, 
				m_skinModelData->GetFrameRoot(), 
				m_pEffect->GetD3DXEffect(),
				r_cast<D3DXMATRIX*>(&m_worldMatrix),
				r_cast<D3DXMATRIX*>(&m_rotationMatrix),
				viewMatrix,
				projMatrix,
				m_light,
				m_normalMap
				);
		}
	}
}