#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class DirectionalLight;
	class Keyboard;
	class ProxyModel;
	class RenderStateHelper;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{

	class DiffuseLightingMaterial;

	class ObjectDiffuseLight : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ObjectDiffuseLight, DrawableGameComponent)

	public:
		ObjectDiffuseLight(Game& game, Camera& camera);
		~ObjectDiffuseLight();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		//add to support multiple model in the scene
		void SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ);

	private:
		ObjectDiffuseLight();
		ObjectDiffuseLight(const ObjectDiffuseLight& rhs);
		ObjectDiffuseLight& operator=(const ObjectDiffuseLight& rhs);

		void UpdateAmbientLight(const GameTime& gameTime);
		void UpdateDirectionalLight(const GameTime& gameTime);

		static const float LightModulationRate;
		static const XMFLOAT2 LightRotationRate;

		Effect* mEffect;
		DiffuseLightingMaterial* mMaterial;		
		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;
		
		XMCOLOR mAmbientColor;
		DirectionalLight* mDirectionalLight;
		Keyboard* mKeyboard;
		XMFLOAT4X4 mWorldMatrix;

		ProxyModel* mProxyModel;

		RenderStateHelper* mRenderStateHelper;
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;
	};
}
