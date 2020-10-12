#include "GoosePlayground.h"
#include "Utilities.h"

GoosePlayground::GoosePlayground(std::string name)
	: Scene(name)
{
	//Gravity (side scroller)
	m_gravity = b2Vec2(0.f, -300.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void GoosePlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		//Create Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attach camera to horizontal and vertical scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup Goose
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Set up components
		std::string fileName = "goosespritesheet.png";
		std::string animations = "goose.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 30, 30, &ECS::GetComponent<Sprite>(entity),
									&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 40.f, 2.f));
		
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 14.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);
	}

	//Static Platform
	{
		//Scene::CreateSprite(m_sceneReg, "boxSprite.jpg", 2000, 20, 1.f, vec3(30.f, -40.f, 2.f), b2_staticBody, 0.f, 0.f, false, false);
		
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2000, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -50.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
	}


	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

}

void GoosePlayground::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	Scene::AdjustScrollOffset();
	player.Update();
}
b2Vec2 prevVel;
void GoosePlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	float speed = 3.5f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);
	bool right = false;
	bool rightCont = false;
	float speedCount = 1;

	if (Input::GetKey(Key::Shift))
	{
		speed *= 2.f;
	}

	if (Input::GetKey(Key::Space))
	{

		vel += b2Vec2(0.f, 2.f);
		
	}

	if (Input::GetKey(Key::S))
	{
		printf("%f", prevVel + vel, "% ");
	}

	if (Input::GetKey(Key::A))
	{
		right = false;
		rightCont = false;
		vel += b2Vec2(-1.f, 0.f);
	}

	if (Input::GetKey(Key::D))
	{
		right = true;
		if (right && rightCont == false)
		{
			speedCount = 1;
			rightCont = true;
		}
		else if (right && rightCont) {
			speedCount += 0.2;
			if (speedCount > 3.4) {
				speedCount = 3.4;
			}
			printf("%f", speedCount);
		}
		
		vel += b2Vec2(speedCount, 0.f);
	}

	player.GetBody()->SetLinearVelocity(speed * vel); 
	
}

void GoosePlayground::KeyboardDown()
{
	if (Input::GetKeyDown(Key::L))
	{
		printf("Hello");
	}
}

void GoosePlayground::KeyboardUp()
{

}