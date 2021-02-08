#pragma once

#include "DrawableGameComponent.h"

#include <DirectXCollision.h>

using namespace Library;

namespace Library
{
	class Mesh;
}

namespace Rendering
{
	class Clouds : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Clouds, DrawableGameComponent)

	public:
		Clouds(Game& game, Camera& camera, const std::string modelFilename);
		Clouds(Game& game, Camera& camera, const std::string modelFilename, const std::wstring modelDes, int modelValue);
		~Clouds();

		virtual void Update(const GameTime& gameTime) override;


		//add to support multiple model in the scene, remove this function

		void SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ);

		//bounding box require to access the world matrix
		
		XMFLOAT4X4* WorldMatrix() { return &mWorldMatrix; }

		//need to access this , make this public for simplicity
		DirectX::BoundingBox mBoundingBox;
		const std::wstring GetModelDes() { return modelDes; }
		int const ModelValue() { return mModelValue; }

		
		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;






	private:
		typedef struct _TextureMappingVertex
		{
			XMFLOAT4 Position;
			XMFLOAT2 TextureCoordinates;

			_TextureMappingVertex() { }

			_TextureMappingVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates)
				: Position(position), TextureCoordinates(textureCoordinates) { }
		} TextureMappingVertex;

		Clouds();
		Clouds(const Clouds& rhs);
		Clouds& operator=(const Clouds& rhs);

		void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;

		ID3DX11Effect* mEffect;
        ID3DX11EffectTechnique* mTechnique;
        ID3DX11EffectPass* mPass;
        ID3DX11EffectMatrixVariable* mWvpVariable;
		
		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;		

        ID3D11InputLayout* mInputLayout;		
        ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		XMFLOAT4X4 mWorldMatrix;	
		float mAngle;

		const std::string modelFile;

		std::wstring modelDes;
		int mModelValue;
		
	

	};
}
