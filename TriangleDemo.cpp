#include "TriangleDemo.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "D3DCompiler.h"
#include "WICTextureLoader.h" 

namespace Rendering
{
    RTTI_DEFINITIONS(TriangleDemo)

    TriangleDemo::TriangleDemo(Game& game, Camera& camera)
        : DrawableGameComponent(game, camera),
          mEffect(nullptr), mTechnique(nullptr), mPass(nullptr), mWvpVariable(nullptr),
          mInputLayout(nullptr), mWorldMatrix(MatrixHelper::Identity), mVertexBuffer(nullptr), mIndexBuffer(nullptr), 
		  mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr), mAngle(0.0f)
    {
    }

    TriangleDemo::~TriangleDemo()
    {
        ReleaseObject(mWvpVariable);
        ReleaseObject(mPass);
        ReleaseObject(mTechnique);

		ReleaseObject(mTextureShaderResourceView);
		ReleaseObject(mColorTextureVariable);

        ReleaseObject(mEffect);		
        ReleaseObject(mInputLayout);
        ReleaseObject(mVertexBuffer);
    }

    void TriangleDemo::Initialize()
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
		
		//1. load the effect file (vertex and pixel shader)
		//insert code here
        
		//load the TextureMapping.fx instead of the previous BasicEffect.fx in the D3DCompileFromFile function.
		HRESULT hr = D3DCompileFromFile(L"Content\\Effects\\TextureMapping.fx", nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);
		if (FAILED(hr))
		{
			const char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileFromFile() failed");
			GameException ex(errorMessage, hr);
			ReleaseObject(errorMessages);
			throw ex;
		}
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
			throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified 				variable.", hr);
		}
		mColorTextureVariable = variable->AsShaderResource();
		if (mColorTextureVariable->IsValid() == false)
		{
			throw GameException("Invalid effect variable cast.");
		}

				

        // Create the input layout
        D3DX11_PASS_DESC passDesc;
        mPass->GetDesc(&passDesc);

		//2. create the vetex layout
		//insert code here
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			/* BasicEffect
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			*/

			//TextureEffect
			  { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	          { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

		};
		if (FAILED(hr = mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
		{
			throw GameException("ID3D11Device::CreateInputLayout() failed.", hr);
		}

    


        // 3. Create the vertex buffer
		//insert code here
		//Use TextureMappingVertex instead of BasicEffectVertex
		TextureMappingVertex vertices[] =
		{
//BasicEffectVertex(XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f,0.0f,1.0f)),//red
//BasicEffectVertex(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),//green
//BasicEffectVertex(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),//blue

		/*Vertices for a 3D gem
		BasicEffectVertex(XMFLOAT4(-1.25f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f,0.0f,1.0f)),//V0
		BasicEffectVertex(XMFLOAT4(1.25f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),//V1 
		BasicEffectVertex(XMFLOAT4(0.0f, 0.0f, -1.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)),//V2 
		BasicEffectVertex(XMFLOAT4(0.0f, 2.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),//V3
		BasicEffectVertex(XMFLOAT4(0.0f, -2.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),//V4
		*/
		TextureMappingVertex(XMFLOAT4(-40.0f, 0.0f, 40.0f, 1.0f), XMFLOAT2(0.0f, 15.0f)),
		TextureMappingVertex(XMFLOAT4(+40.0f, 0.0f, 40.0f, 1.0f), XMFLOAT2(15.0f, 15.0f)),
		TextureMappingVertex(XMFLOAT4(+40.0f, 0.0f, -40.0f, 1.0f), XMFLOAT2(15.0f, 0.0f)),
		TextureMappingVertex(XMFLOAT4(-40.0f, 0.0f, -40.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),

		};
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		//vertexBufferDesc.ByteWidth = sizeof(BasicEffectVertex) * ARRAYSIZE(vertices);
		vertexBufferDesc.ByteWidth = sizeof(TextureMappingVertex) * ARRAYSIZE(vertices);

		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		if (FAILED(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
		/*Indice for a gem
		UINT indices[] =
		{ 0, 1, 3,
			0, 3, 2,
			1, 2, 3,
			0, 4, 1,
			0, 2, 4,
			1, 4, 2,
		};
		*/

		UINT indices[] = {
			0, 3, 1,
			3, 2, 1, };


		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexSubResourceData;
		ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
		indexSubResourceData.pSysMem = indices;
		if (FAILED(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &mIndexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
    
		//load the texture map 
		std::wstring textureName = L"Content\\Textures\\Floor.jpeg";
		if (FAILED(hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, &mTextureShaderResourceView)))
		{
			throw GameException("CreateWICTextureFromFile() failed.", hr);
		}

    }

	void TriangleDemo::Update(const GameTime& gameTime)
	{

		mAngle += XM_PI * static_cast<float>(gameTime.ElapsedGameTime());
		//XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(mAngle));



	}

    void TriangleDemo::Draw(const GameTime& gameTime)
    {
		//4. draw function
		//insert the code here
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout);

		//UINT stride = sizeof(BasicEffectVertex);
		UINT stride = sizeof(TextureMappingVertex);

		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		//set Index buffer
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//load the texture into the rendering pipeline :
		mColorTextureVariable->SetResource(mTextureShaderResourceView);


		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mWvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));
		mPass->Apply(0, direct3DDeviceContext);

		//direct3DDeviceContext->Draw(3, 0);
		direct3DDeviceContext->DrawIndexed(18, 0, 0);
     
    }
}