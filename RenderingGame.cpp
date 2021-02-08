#include "RenderingGame.h"
#include "GameException.h"
#include "FirstPersonCamera.h"
#include "TriangleDemo.h"

#include "Keyboard.h"
#include "Mouse.h"

#include "ModelFromFile.h"
#include "Trees.h"
#include "Clouds.h"
#include "Rocks.h"
#include "Door.h"
#include "Spikes.h"

#include "FpsComponent.h"
#include "RenderStateHelper.h"
#include "ObjectDiffuseLight.h"
#include "SamplerStates.h"
#include "RasterizerStates.h"
#include <iostream>
#include <Windows.h> 

//display score
#include <SpriteFont.h>
#include <sstream>

namespace Rendering
{;

	const XMFLOAT4 RenderingGame::BackgroundColor = { 0.1f, 0.7f, 1.0f, 1.0f };

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand),
		mDemo(nullptr), mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr), mModel1(nullptr), mModel2(nullptr), mModel3(nullptr), mModel4(nullptr), mModel5(nullptr), 
		mModel6(nullptr), mModel7(nullptr), mModel8(nullptr), mModel9(nullptr), mModel10(nullptr), mModel11(nullptr), mModel12(nullptr), mModel13(nullptr), mModel14(nullptr), mModel15(nullptr), mModel16(nullptr),
		mFpsComponent(nullptr), mRenderStateHelper(nullptr), mObjectDiffuseLight(nullptr), mSpriteFont(nullptr), mSpriteBatch(nullptr)
    {
        mDepthStencilBufferEnabled = true;
        mMultiSamplingEnabled = true;
    }

    RenderingGame::~RenderingGame()
    {
    }

    void RenderingGame::Initialize()
    {

        mCamera = new FirstPersonCamera(*this);
        mComponents.push_back(mCamera);
        mServices.AddService(Camera::TypeIdClass(), mCamera);
    
        mDemo = new TriangleDemo(*this, *mCamera);
        mComponents.push_back(mDemo);

		//Remember that the component is a management class for all objects in the D3D rendering engine. 
		//It provides a centralized place to create and release objects. 
	    //NB: In C++ and other similar languages, to instantiate a class is to create an object.
		if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
		{
			throw GameException("DirectInput8Create() failed");
		}

		mKeyboard = new Keyboard(*this, mDirectInput);
		mComponents.push_back(mKeyboard);
		mServices.AddService(Keyboard::TypeIdClass(), mKeyboard);

		mMouse = new Mouse(*this, mDirectInput);
		mComponents.push_back(mMouse);
		mServices.AddService(Mouse::TypeIdClass(), mMouse);

		//COINS:
		mModel1 = new ModelFromFile(*this, *mCamera, "Content\\Models\\coin.obj", L"A Coin", +10);
		mModel1->SetPosition(0.0f, 1.57f, -0.0f, 0.1f, -10.5f, 1.5f, -5.0f);
		mComponents.push_back(mModel1);

		mModel2 = new ModelFromFile(*this, *mCamera, "Content\\Models\\coin.obj", L"A Coin", +10);
		mModel2->SetPosition(0.0f, 1.57f, -0.0f, 0.1f, -17.0f, 1.5f, 8.0f);
		mComponents.push_back(mModel2);

		mModel3 = new ModelFromFile(*this, *mCamera, "Content\\Models\\coin.obj", L"A Coin", +10);
		mModel3->SetPosition(0.0f, 0.0f, -0.0f, 0.1f, 10.0f, 1.5f, 9.0f);
		mComponents.push_back(mModel3);

		mModel4 = new ModelFromFile(*this, *mCamera, "Content\\Models\\coin.obj", L"A Coin", +10);
		mModel4->SetPosition(0.0f, 0.0f, -0.0f, 0.1f, -31.0f, 1.5f, 28.5f);
		mComponents.push_back(mModel4);

		mModel5 = new ModelFromFile(*this, *mCamera, "Content\\Models\\coin.obj", L"A Coin", +10);
		mModel5->SetPosition(0.0f, 1.57f, -0.0f, 0.1f, 3.0f, 1.5f, -4.0f);
		mComponents.push_back(mModel5);

		mModel6 = new ModelFromFile(*this, *mCamera, "Content\\Models\\coin.obj", L"A Coin", +10);
		mModel6->SetPosition(0.0f, 0.0f, -0.0f, 0.1f, -17.0f, 1.5f, -17.5f);
		mComponents.push_back(mModel6);

		mModel7 = new ModelFromFile(*this, *mCamera, "Content\\Models\\coin.obj", L"A Coin", +10);
		mModel7->SetPosition(0.0f, 1.57f, -0.0f, 0.1f, 22.5f, 1.5f, -17.0f);
		mComponents.push_back(mModel7);

		
		//TREES:
		mModel8 = new Trees(*this, *mCamera, "Content\\Models\\forest.obj");
		mModel8->SetPosition(6.28f, 3.14f, -0.0f, 0.09f, -0.50f, -3.0f, -1.0f);
		mComponents.push_back(mModel8);

		//CLOUDS:
		mModel9 = new Clouds(*this, *mCamera, "Content\\Models\\clouds.obj");
		mModel9->SetPosition(6.28f, 3.14f, -0.0f, 0.09f, -0.50f, -3.0f, -1.0f);
		mComponents.push_back(mModel9);

		//ROCKS:
		mModel10 = new Rocks(*this, *mCamera, "Content\\Models\\rocks.obj");
		mModel10->SetPosition(6.28f, 3.14f, -0.0f, 0.09f, -0.50f, -2.85f, -0.2f);
		mComponents.push_back(mModel10);

		//KEY:
		mModel11 = new ModelFromFile(*this, *mCamera, "Content\\Models\\key.obj", L"A Key", 50);
		mModel11->SetPosition(6.28f, 3.14f, -0.0f, 0.4f, -25.0f, 1.5f, 15.5f);
		mComponents.push_back(mModel11);

		//DOOR:
		mModel12 = new ModelFromFile(*this, *mCamera, "Content\\Models\\door.obj", L"A Door, do you want to open?", 0);
		mModel12->SetPosition(6.28f, 3.14f, -0.0f, 0.09f, -30.0f, -3.5f, -21.5f);
		mComponents.push_back(mModel12);

		//Spikes:
		mModel13 = new Spikes(*this, *mCamera, "Content\\Models\\spikes.obj");
		mModel13->SetPosition(6.28f, 3.14f, -0.0f, 0.12f, 15.7f, 0.0f, 23.5f);
		mComponents.push_back(mModel13);

		mModel14 = new Spikes(*this, *mCamera, "Content\\Models\\spikes.obj");
		mModel14->SetPosition(6.28f, 3.14f, -0.0f, 0.12f, -23.5f, 0.0f, -8.0f);
		mComponents.push_back(mModel14);

		mModel15 = new Spikes(*this, *mCamera, "Content\\Models\\spikes.obj");
		mModel15->SetPosition(6.28f, 3.14f, -0.0f, 0.12f, -17.0f, 0.0f, 10.0f);
		mComponents.push_back(mModel15);

		mModel16 = new Spikes(*this, *mCamera, "Content\\Models\\spikes.obj");
		mModel16->SetPosition(6.28f, 3.14f, -0.0f, 0.12f, 2.7f, 0.0f, -24.5f);
		mComponents.push_back(mModel16);

		//house object with diffuse lighting effect:
		mObjectDiffuseLight = new ObjectDiffuseLight(*this, *mCamera);
		mObjectDiffuseLight->SetPosition(6.28f, 3.14f, -0.0f, 0.09f, -0.50f, -3.0f, -1.0f);
		mComponents.push_back(mObjectDiffuseLight);
		RasterizerStates::Initialize(mDirect3DDevice);
		SamplerStates::Initialize(mDirect3DDevice);

	
		mFpsComponent = new FpsComponent(*this);
		mFpsComponent->Initialize();
		mRenderStateHelper = new RenderStateHelper(*this);
		
		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");



		Game::Initialize();

        mCamera->SetPosition(30.0f, 1.0f, 30.0f);

    }

    void RenderingGame::Shutdown()
    {
		
		DeleteObject(mDemo);
        DeleteObject(mCamera);
		
		
		DeleteObject(mKeyboard);
		DeleteObject(mMouse);
		ReleaseObject(mDirectInput);
		
		DeleteObject(mModel1);
		DeleteObject(mModel2);
		DeleteObject(mModel3);
		DeleteObject(mModel4);
		DeleteObject(mModel5);
		DeleteObject(mModel6);
		DeleteObject(mModel7);
		DeleteObject(mModel11);


		DeleteObject(mSpriteFont);
		DeleteObject(mSpriteBatch);
		 

		DeleteObject(mFpsComponent);
		DeleteObject(mRenderStateHelper);

		DeleteObject(mObjectDiffuseLight);
		

        Game::Shutdown();
    }

    void RenderingGame::Update(const GameTime &gameTime)
    {

		mFpsComponent->Update(gameTime);
		Game::Update(gameTime);
		

		//Add "ESC" to exit the application
		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}


		//bounding box , we need to see if we need to do the picking test
		if (Game::toPick)
		{
			
			if (mModel1->Visible())
			Pick(Game::screenX, Game::screenY, mModel1);
			
			if (mModel2->Visible())
			Pick(Game::screenX, Game::screenY, mModel2);
			
			if (mModel3->Visible())
			Pick(Game::screenX, Game::screenY, mModel3);

			if (mModel4->Visible())
			Pick(Game::screenX, Game::screenY, mModel4);

			if (mModel5->Visible())
			Pick(Game::screenX, Game::screenY, mModel5);

			if (mModel6->Visible())
			Pick(Game::screenX, Game::screenY, mModel6);

			if (mModel7->Visible())
			Pick(Game::screenX, Game::screenY, mModel7);

			if (mModel11->Visible())
			{
				Pick(Game::screenX, Game::screenY, mModel11);
				HasKey = true;
			}

			else if (mModel12->Visible()&& HasKey == true)
			{
				Door(Game::screenX, Game::screenY, mModel12);
			}
		
			Game::toPick = false;

		}
	}


	// do the picking here

	void RenderingGame::Pick(int sx, int sy, ModelFromFile* model)
	{
		//XMMATRIX P = mCam.Proj(); 
		XMFLOAT4X4 P;
		XMStoreFloat4x4(&P, mCamera->ProjectionMatrix());
		//Compute picking ray in view space.
		float vx = (+2.0f * sx / Game::DefaultScreenWidth - 1.0f) / P(0, 0);
		float vy = (-2.0f * sy / Game::DefaultScreenHeight + 1.0f) / P(1, 1);
		// Ray definition in view space.
		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVectorSet(vx, vy, -1.0f, 0.0f);
		// Tranform ray to local space of Mesh via the inverse of both of view and world transform
		XMMATRIX V = mCamera->ViewMatrix();
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);
		XMMATRIX W = XMLoadFloat4x4(model->WorldMatrix());
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);
		XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);
		rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);

		// Make the ray direction unit length for the intersection tests.
		rayDir = XMVector3Normalize(rayDir);
		float tmin = 0.0;
		if (model->mBoundingBox.Intersects(rayOrigin, rayDir, tmin))
		{
			std::wostringstream pickupString;
			pickupString << L"You have found: " << (model->GetModelDes()).c_str() << '\n' << '\t' << model->ModelValue() << L" points";
			int result = MessageBox(0, pickupString.str().c_str(), L"Object Found", MB_ICONASTERISK | MB_YESNO);

			//To make the object invisible after being picked, in the Pick function, add the following code:
			if (result == IDYES)
			{
				//hide the object
				model->SetVisible(false);

				//update the score
				mScore += model->ModelValue();

			}
		}

	}



	void RenderingGame::Door(int sx, int sy, ModelFromFile* model)
	{
		//XMMATRIX P = mCam.Proj(); 
		XMFLOAT4X4 P;
		XMStoreFloat4x4(&P, mCamera->ProjectionMatrix());
		//Compute picking ray in view space.
		float vx = (+2.0f * sx / Game::DefaultScreenWidth - 1.0f) / P(0, 0);
		float vy = (-2.0f * sy / Game::DefaultScreenHeight + 1.0f) / P(1, 1);
		// Ray definition in view space.
		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVectorSet(vx, vy, -1.0f, 0.0f);
		// Tranform ray to local space of Mesh via the inverse of both of view and world transform
		XMMATRIX V = mCamera->ViewMatrix();
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);
		XMMATRIX W = XMLoadFloat4x4(model->WorldMatrix());
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);
		XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);
		rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);

		// Make the ray direction unit length for the intersection tests.
		rayDir = XMVector3Normalize(rayDir);
		float tmin = 0.0;
		if (model->mBoundingBox.Intersects(rayOrigin, rayDir, tmin))
		{
			std::wostringstream pickupString;
			pickupString << L"You found: " << (model->GetModelDes()).c_str() << '\n' << '\t' << model->ModelValue();
			int result = MessageBox(0, pickupString.str().c_str(), L"Object Found", MB_ICONASTERISK | MB_YESNO);

			//To make the object invisible after being picked, in the Pick function, add the following code:
			if (result == IDYES)
			{
				//hide the object
				model->SetVisible(false);

				//End the game
				mScore += model->ModelValue();
				Exit();

			}
		}
	}

    void RenderingGame::Draw(const GameTime &gameTime)
    {
        mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
        mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        Game::Draw(gameTime);
		mRenderStateHelper->SaveAll();
		mFpsComponent->Draw(gameTime);
		
		     mSpriteBatch->Begin();

			 // draw the key score
			 std::wostringstream keyLabel;
			 keyLabel << L"Key: " << HasKey << "\n";
			 mSpriteFont->DrawString(mSpriteBatch, keyLabel.str().c_str(), XMFLOAT2(0.0f, 40.0f), Colors::DarkRed);

		//draw the score
		std::wostringstream scoreLabel;
		scoreLabel << L"Your current score: " << mScore<< "\n";
		mSpriteFont->DrawString(mSpriteBatch, scoreLabel.str().c_str(),  XMFLOAT2(0.0f, 10.0f), Colors::DarkRed);
	    mSpriteBatch->End();

		
		

		
		mRenderStateHelper->RestoreAll();

       
        HRESULT hr = mSwapChain->Present(0, 0);
        if (FAILED(hr))
        {
            throw GameException("IDXGISwapChain::Present() failed.", hr);
        }

    }
}