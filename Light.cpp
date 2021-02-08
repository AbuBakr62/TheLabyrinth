#include "Light.h"
//#include "ColorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(Light)

	Light::Light(Game& game)
		: GameComponent(game), mColor(1.0f,1.0f,1.0f,1.0f)
	{
	}

	Light::~Light()
	{
	}

	const XMCOLOR& Light::Color() const
	{
		return mColor;
	}

	XMVECTOR Light::ColorVector() const
	{
		return XMLoadColor(&mColor);
	}

	void Light::SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
	{
		XMCOLOR color(r, g, b, a);
		SetColor(color);
	}

	void Light::SetColor(XMCOLOR color)
	{
		mColor = color;
	}

	void Light::SetColor(FXMVECTOR color)
	{
		XMStoreColor(&mColor, color);
	}
}