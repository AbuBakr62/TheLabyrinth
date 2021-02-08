#include "Rocks.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "D3DCompiler.h"
#include "Model.h"
#include "Mesh.h"
#include <WICTextureLoader.h>

using namespace DirectX;

namespace Rendering
{
    RTTI_DEFINITIONS(Rocks)

    Rocks::Rocks(Game& game, Camera& camera, const std::string modelFilename)
        : DrawableGameComponent(game, camera),  
          mEffect(nullptr), mTechnique(nullptr), mPass(nullptr), mWvpVariable(nullptr), mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr),
          mInputLayout(nullptr), mWorldMatrix(MatrixHelper::Identity), mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0), modelFile(modelFilename)
    {
		//we don't use the model description and model value for this constructor
		mModelValue = 0;
    }

	Rocks::Rocks(Game& game, Camera& camera, const std::string modelFilename, const std::wstring ModelDes, int ModelValue)
		: DrawableGameComponent(game, camera),
		mEffect(nullptr), mTechnique(nullptr), mPass(nullptr), mWvpVariable(nullptr), mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr),
		mInputLayout(nullptr), mWorldMatrix(MatrixHelper::Identity), mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0), modelFile(modelFilename), modelDes(ModelDes), mModelValue(ModelValue) 
	{

	}

    Rocks::~Rocks()
    {
        ReleaseObject(mColorTextureVariable);
        ReleaseObject(mTextureShaderResourceView);
        ReleaseObject(mWvpVariable);
        ReleaseObject(mPass);
        ReleaseObject(mTechnique);
        ReleaseObject(mEffect);		
        ReleaseObject(mInputLayout);
        ReleaseObject(mVertexBuffer);
        ReleaseObject(mIndexBuffer);
    }



    void Rocks::Initialize()
    {
        SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

        // Compile the shader
        UINT shaderFlags = 0;

	#if defined( DEBUG ) || defined( _DEBUG )
	    shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

        ID3D10Blob* compiledShader = nullptr;
        ID3D10Blob* errorMessages = nullptr;		
        HRESULT hr = D3DCompileFromFile(L"Content\\Effects\\TextureMapping.fx", nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);

		/*
		if (FAILED(hr))
		{
			char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileFromFile() failed");
			GameException ex(errorMessage, hr);
			ReleaseObject(errorMessages);

			throw ex;
		}*/

        // Create an effect object from the compiled shader
        hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, mGame->Direct3DDevice(), &mEffect);
        if (FAILED(hr))
        {
            throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
        }

        ReleaseObject(compiledShader);

        // Look up the technique, pass, and WVP variable from the effect
        mTechnique = mEffect->GetTechniqueByName("main11");
        if (mTechnique == nullptr)
        {
            throw GameException("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.", hr);
        }

        mPass = mTechnique->GetPassByName("p0");
        if (mPass == nullptr)
        {
            throw GameException("ID3DX11EffectTechnique::GetPassByName() could not find the specified pass.", hr);
        }

        ID3DX11EffectVariable* variable = mEffect->GetVariableByName("WorldViewProjection");
        if (variable == nullptr)
        {
            throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
        }

        mWvpVariable = variable->AsMatrix();
        if (mWvpVariable->IsValid() == false)
        {
            throw GameException("Invalid effect variable cast.");
        }

        variable = mEffect->GetVariableByName("ColorTexture");
        if (variable == nullptr)
        {
            throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
        }
        
        mColorTextureVariable = variable->AsShaderResource();
        if (mColorTextureVariable->IsValid() == false)
        {
            throw GameException("Invalid effect variable cast.");
        }

        // Create the input layout
        D3DX11_PASS_DESC passDesc;
        mPass->GetDesc(&passDesc);

        D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        if (FAILED(hr = mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
        {
            throw GameException("ID3D11Device::CreateInputLayout() failed.", hr);
        }

        // Load the model
        std::unique_ptr<Model> model(new Model(*mGame, modelFile, true));
        
        // Create the vertex and index buffers
        Mesh* mesh = model->Meshes().at(0);
        CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
        mesh->CreateIndexBuffer(&mIndexBuffer);
        mIndexCount = mesh->Indices().size();


		
		
        // Load the texture
       // std::wstring textureName = L"Content\\Textures\\EarthComposite.jpg";

		std::wstring textureName = L"Content\\Textures\\rocks.jpg"; 
        
		if (FAILED(hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, &mTextureShaderResourceView)))
        {
            throw GameException("CreateWICTextureFromFile() failed.", hr);
        }


        //position model in the world space, the issue here is that models are from different sources need adjustment for scaling, rotation,
		/*
		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX RotationZ = XMMatrixRotationZ(-1.57f);
	    XMMATRIX RotationX = XMMatrixRotationX(-1.57f);
		XMMATRIX RotationY = XMMatrixRotationY(-1.57f);
		XMMATRIX Scale = XMMatrixScaling( 0.05f, 0.05f, 0.05f );
	    XMMATRIX Translation = XMMatrixTranslation( 0.0f, 0.0f, 0.0f );
	    worldMatrix= RotationZ*RotationX *RotationY* Scale * Translation;

	    XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
*/
    }


	void Rocks::SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ)
	{
		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX RotationZ = XMMatrixRotationZ(rotateZ);
		XMMATRIX RotationX = XMMatrixRotationX(rotateX);
		XMMATRIX RotationY = XMMatrixRotationY(rotateY);
		XMMATRIX Scale = XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);
		XMMATRIX Translation = XMMatrixTranslation(translateX, translateY, translateZ);
		worldMatrix = RotationZ*RotationX *RotationY* Scale * Translation;

		XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	}



	void Rocks::Update(const GameTime& gameTime)
	{
	//	XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
	//	mAngle += XM_PI * static_cast<float> (gameTime.ElapsedGameTime());
		
	//	mAngle = 0.0002 * XM_PI;

	//	worldMatrix =   worldMatrix * XMMatrixRotationY(mAngle);
	//	XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationX(mAngle));

	//	XMStoreFloat4x4(&mWorldMatrix, worldMatrix);

		
	}


    void Rocks::Draw(const GameTime& gameTime)
    {
        ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();        
        direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        direct3DDeviceContext->IASetInputLayout(mInputLayout);

        UINT stride = sizeof(TextureMappingVertex);
        UINT offset = 0;
        direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);		
        direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
                
        XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
        XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
        mWvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

		
        mColorTextureVariable->SetResource(mTextureShaderResourceView);

        mPass->Apply(0, direct3DDeviceContext);

        direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
    }

    void Rocks::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
    {
        const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();

        std::vector<TextureMappingVertex> vertices;
        vertices.reserve(sourceVertices.size());
        
        std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
        assert(textureCoordinates->size() == sourceVertices.size());
          

		//generate the bounding box
		float min = -1e38f;
		float max = 1e38f;

		XMFLOAT3 vMinf3(max, max, max);
		XMFLOAT3 vMaxf3(min,min,min);


		XMVECTOR vMin = XMLoadFloat3(&vMinf3);
		XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

		//end


        for (UINT i = 0; i < sourceVertices.size(); i++)
        {
            XMFLOAT3 position = sourceVertices.at(i);
            XMFLOAT3 uv = textureCoordinates->at(i);
            vertices.push_back(TextureMappingVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y)));


			//create the bounding box from the list of vertices
			XMVECTOR P = XMLoadFloat3(&position);
			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
			//the end

        }
		
	    //final step to generate the bounding box
	
		XMStoreFloat3(const_cast<XMFLOAT3*>(&mBoundingBox.Center),  0.5f*(vMin + vMax));
		XMStoreFloat3(const_cast<XMFLOAT3*>(&mBoundingBox.Extents), 0.5f*(vMax - vMin));

	

		
        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.ByteWidth = sizeof(TextureMappingVertex) * vertices.size();
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;		
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexSubResourceData;
        ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
        vertexSubResourceData.pSysMem = &vertices[0];
        if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
        {
            throw GameException("ID3D11Device::CreateBuffer() failed.");
        }
    }
}