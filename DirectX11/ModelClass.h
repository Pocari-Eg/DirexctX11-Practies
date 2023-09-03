#pragma once
#include "AlignedAllocationPolicy.h"
#include "framework.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Mesh.h"
#include "TextureLoader.h"

class TextureClass;

class ModelClass : public AlignedAllocationPolicy<16>
{

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 noraml;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*,char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	bool Load(HWND hwnd, ID3D11Device* dev, ID3D11DeviceContext* devcon, std::string filename);
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*,WCHAR*);
	void ReleaseTexture();


	bool LoadModel(char*);
	void ReleaseModel();


	//assimp
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* loadEmbeddedTexture(const aiTexture* embeddedTexture);
private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	int m_vertexCount = 0;
	int m_indexCount = 0;

	TextureClass* m_Texture = nullptr;
	ModelType* m_model = nullptr;


	std::vector<VERTEX> m_vertices;
	std::vector<UINT> m_indices;
	std::vector<Texture> m_textures;

	//assimp
	ID3D11Device* dev_;
	ID3D11DeviceContext* devcon_;
	std::vector<Mesh> meshes_;
	std::string directory_;
	std::vector<Texture> textures_loaded_;
	HWND hwnd_;

};

