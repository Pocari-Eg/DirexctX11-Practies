#include "ModelClass.h"

#include"TextureClass.h"


#include<fstream>
using namespace std;
ModelClass::ModelClass()
{
}

ModelClass::ModelClass(const ModelClass&)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device,char* modelfilename, WCHAR* Filename)
{

    //// 정점 및 인덱스 버퍼를 초기화합니다.
    //if (!Load(modelfilename))
    //{
    //    return false;
    //}

    // 정점 및 인덱스 버퍼를 초기화합니다.
    if (!InitializeBuffers(device))
    {
        return false;
    }

    // 이 모델의 텍스처를 로드합니다.
    return LoadTexture(device, Filename);
}

void ModelClass::Shutdown()
{
    ReleaseTexture();

    ShutdownBuffers();

    ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
    RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
    return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
    return  m_Texture->GetTexture();
}

bool ModelClass::Load(HWND hwnd, ID3D11Device* dev, ID3D11DeviceContext* devcon, std::string filename)
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filename,
        aiProcess_Triangulate |
        aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
        return false;

    this->directory_ = filename.substr(0, filename.find_last_of("/\\"));

    this->dev_ = dev;
    this->devcon_ = devcon;
    this->hwnd_ = hwnd;

    processNode(pScene->mRootNode, pScene);

    return true;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{


    // 정점 배열을 만듭니다.
    VertexType* vertices = new VertexType[m_vertexCount];
    if (!vertices)
    {
        return false;
    }

    // 인덱스 배열을 만듭니다.
    unsigned long* indices = new unsigned long[m_indexCount];
    if (!indices)
    {
        return false;
    }

    for (int i = 0; i < m_vertexCount; i++)
    {
        vertices[i].position = XMFLOAT3(m_vertices[i].X, m_vertices[i].Y, m_vertices[i].Z);
        vertices[i].texture = XMFLOAT2(m_vertices[i].texcoord.x, m_vertices[i].texcoord.y);
      //  vertices[i].noraml = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

        indices[i] = i;
    }


    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
    {
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
    {
        return false;
    }

    delete[] vertices;
    vertices = 0;

    delete[] indices;
    indices = 0;

    return true;
}

void ModelClass::ShutdownBuffers()
{
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{

    m_Texture = new TextureClass;
    if (!m_Texture)return false;

    return m_Texture->Initialize(device, filename);
}

void ModelClass::ReleaseTexture()
{
    if (m_Texture)
    {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = 0;
    }
}

bool ModelClass::LoadModel(char* filename)
{


    ifstream fin;
    fin.open(filename);

    if (fin.fail())
    {
        return false;
    }

    char input = 0;
    fin.get(input);
    while (input!=':')
    {
        fin.get(input);
    }

    fin >> m_vertexCount;
    m_indexCount = m_vertexCount;

    m_model = new ModelType[m_vertexCount];
    if (!m_model)
    {
        return false;
    }


    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    for (int i = 0; i < m_vertexCount; i++)
    {
        fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
        fin >> m_model[i].tu >> m_model[i].tv;
        fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
    }

    fin.close();




    return true;
}

void ModelClass::ReleaseModel()
{
    for (auto& t : textures_loaded_)
        t.Release();

    for (size_t i = 0; i < meshes_.size(); i++) {
        meshes_[i].Close();
    }

    if (m_model)
    {
        delete[] m_model;
        m_model = 0;
    }
}

void ModelClass::processNode(aiNode* node, const aiScene* scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
       this->processMesh(mesh, scene);
    }

    for (UINT i = 0; i < node->mNumChildren; i++) {
        this->processNode(node->mChildren[i], scene);
    }

}

void ModelClass::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // Data to fill
    std::vector<VERTEX> vertices;
    std::vector<UINT> indices;
    std::vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        VERTEX vertex;

        vertex.X = mesh->mVertices[i].x;
        vertex.Y = mesh->mVertices[i].y;
        vertex.Z = mesh->mVertices[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    m_vertices = vertices;
    m_indices = indices;
    m_textures = textures;

    m_vertexCount = m_vertices.size();
    m_indexCount = m_indices.size();
}

std::vector<Texture> ModelClass::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
    std::vector<Texture> textures;
    for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (UINT j = 0; j < textures_loaded_.size(); j++) {
            if (std::strcmp(textures_loaded_[j].path.c_str(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded_[j]);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip) {   // If texture hasn't been loaded already, load it
            HRESULT hr;
            Texture texture;

            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
            if (embeddedTexture != nullptr) {
                texture.texture = loadEmbeddedTexture(embeddedTexture);
            }
            else {
                std::string filename = std::string(str.C_Str());
              //  filename = directory_ + '/' + filename;
                std::wstring filenamews = std::wstring(filename.begin(), filename.end());
                hr = CreateWICTextureFromFile(dev_, devcon_, filenamews.c_str(), nullptr, &texture.texture);
                if (FAILED(hr))
                    MessageBox(hwnd_, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
            }
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            this->textures_loaded_.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

ID3D11ShaderResourceView* ModelClass::loadEmbeddedTexture(const aiTexture* embeddedTexture)
{
    HRESULT hr;
    ID3D11ShaderResourceView* texture = nullptr;

    if (embeddedTexture->mHeight != 0) {
        // Load an uncompressed ARGB8888 embedded texture
        D3D11_TEXTURE2D_DESC desc;
        desc.Width = embeddedTexture->mWidth;
        desc.Height = embeddedTexture->mHeight;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresourceData;
        subresourceData.pSysMem = embeddedTexture->pcData;
        subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
        subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

        ID3D11Texture2D* texture2D = nullptr;
        hr = dev_->CreateTexture2D(&desc, &subresourceData, &texture2D);
        if (FAILED(hr))
            MessageBox(hwnd_, L"CreateTexture2D failed!", L"Error!", MB_ICONERROR | MB_OK);

        hr = dev_->CreateShaderResourceView(texture2D, nullptr, &texture);
        if (FAILED(hr))
            MessageBox(hwnd_, L"CreateShaderResourceView failed!", L"Error!", MB_ICONERROR | MB_OK);

        return texture;
    }

    // mHeight is 0, so try to load a compressed texture of mWidth bytes
    const size_t size = embeddedTexture->mWidth;

    hr = CreateWICTextureFromMemory(dev_, devcon_, reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
    if (FAILED(hr))
        MessageBox(hwnd_, L"Texture couldn't be created from memory!", L"Error!", MB_ICONERROR | MB_OK);

    return texture;
}
