#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Rendering
{
    class TriangleDemo : public DrawableGameComponent
    {
        RTTI_DECLARATIONS(TriangleDemo, DrawableGameComponent)

    public:
        TriangleDemo(Game& game, Camera& camera);
        ~TriangleDemo();

        virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

    private:
        /*typedef struct _BasicEffectVertex
        {
            XMFLOAT4 Position;
            XMFLOAT4 Color;

            _BasicEffectVertex() { }

            _BasicEffectVertex(XMFLOAT4 position, XMFLOAT4 color)
                : Position(position), Color(color) { }
        } BasicEffectVertex;
		*/

		typedef struct _TextureMappingVertex
		{
			XMFLOAT4 Position;
			XMFLOAT2 TextureCoordinates;
			_TextureMappingVertex() { }
			_TextureMappingVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates)
				: Position(position), TextureCoordinates(textureCoordinates) { }
		} TextureMappingVertex;


        TriangleDemo();
        TriangleDemo(const TriangleDemo& rhs);
        TriangleDemo& operator=(const TriangleDemo& rhs);

        ID3DX11Effect* mEffect;
        ID3DX11EffectTechnique* mTechnique;
        ID3DX11EffectPass* mPass;
        ID3DX11EffectMatrixVariable* mWvpVariable;

        ID3D11InputLayout* mInputLayout;		
        ID3D11Buffer* mVertexBuffer;

		ID3D11Buffer* mIndexBuffer;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;


        XMFLOAT4X4 mWorldMatrix;
		float mAngle;
    };
}
