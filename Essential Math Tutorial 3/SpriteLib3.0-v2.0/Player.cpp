#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//initialize UVs
	m_animController->InitUVs(fileName);

	//loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	/*for (int i = 0, i < animationNames.size(), i++)
	{
		m_animController->AddAnimation(animations[animationNames[i]].get<Animation>());
	}*/

	//IDLE ANIMATIONS\\
	
	//IdleLeft
	m_animController->AddAnimation(animations["IdleLeft"].get<Animation>());
	//IdleRight
	m_animController->AddAnimation(animations["IdleRight"].get<Animation>());
#ifdef TOPDOWN
	//IdleUp
	m_animController->AddAnimation(animations["IdleUp"].get<Animation>());
#endif
	//IdleDown
	m_animController->AddAnimation(animations["IdleDown"].get<Animation>());

	//WALK ANIMATIONS\\

	//WalkLeft
	m_animController->AddAnimation(animations["WalkLeft"].get<Animation>());
	//WalkRight
	m_animController->AddAnimation(animations["WalkRight"].get<Animation>());
#ifdef TOPDOWN
	//WalkUp
	m_animController->AddAnimation(animations["WalkUp"].get<Animation>());
	//WalkDown
	m_animController->AddAnimation(animations["WalkDown"].get<Animation>());
#endif

	//ATTACK ANIMATIONS\\

	//AttackLeft
	m_animController->AddAnimation(animations["AttackLeft"].get<Animation>());
	//AttackRight
	m_animController->AddAnimation(animations["AttackRight"].get<Animation>());
#ifdef TOPDOWN
	//AttackUp
	m_animController->AddAnimation(animations["AttackUp"].get<Animation>());
	//AttackDown
	m_animController->AddAnimation(animations["AttackDown"].get<Animation>());
#endif

	//set default animation
	m_animController->SetActiveAnim(IDLELEFT);
}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void Player::MovementUpdate()
{
	m_moving = false;

	if (m_hasPhysics)
	{
		/*float speed = 10.f;
		//vec3 vel = vec3(0.f, 0.f, 0.f);
		b2Vec2 vel = b2Vec2(0.f, 0.f);

		if (Input::GetKey(Key::Shift))
		{
			speed *= 3.f;
		}*/
		
//#ifdef TOPDOWN
		/*if (Input::GetKeyDown(Key::W) || Input::GetKeyDown(Key::Space))
		{
			//vel.y += 15.f;
			//vel = vel + vec3(0.f, 1.f, 0.f);
			//m_facing = UP;
			m_moving = true;
		}*/
		if (Input::GetKey(Key::S))
		{
			//vel.y += -1.f;
			//vel = vel + vec3(0.f, -1.f, 0.f);
			m_facing = DOWN;
			//m_moving = true;
		}
//#endif

		if (Input::GetKey(Key::A))
		{
			//vel.x += -1.f;
			//vel = vel + vec3(-1.f, 0.f, 0.f);
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			//vel.x += 1.f;
			//vel = vel + vec3(1.f, 0.f, 0.f);
			m_facing = RIGHT;
			m_moving = true;
		}

		//GetBody()->SetLinearVelocity(speed * vel + b2Vec2(player.GetBody()->GetLinearVelocity().x * 0.9f, player.GetBody()->GetLinearVelocity().y));
		//m_physBody->SetVelocity(vel * speed);
	}/*else {
		//regular movement
		float speed = 15.f;

//#ifdef TOPDOWN
		if (Input::GetKey(Key::W))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() + (speed * Timer::deltaTime));
			//m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() - (speed * Timer::deltaTime));
			m_facing = DOWN;
			m_moving = true;
		}
//#endif

		if (Input::GetKey(Key::A))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() - (speed * Timer::deltaTime));
			m_facing = LEFT;
			m_moving = true;
		}

		if (Input::GetKey(Key::D))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() + (speed * Timer::deltaTime));
			m_facing = RIGHT;
			m_moving = true;
		}

	
	}*/

	if (Input::GetKeyDown(Key::S))
	{
		m_moving = false;
		//SetActiveAnimation(IDLEDOWN);
	}

	if (Input::GetKeyDown(Key::E))
	{
		m_moving = false;

		if (m_facing == DOWN) {
			m_attacking = false;
		}
		else {
			m_attacking = true;
			m_locked = true;
		}
		/*if (m_hasPhysics)
		{
			//m_physBody->SetVelocity(vec3());
		}*/
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//puts it into WALK category
		activeAnimation = WALK;
	}
	else if (m_attacking) {
		activeAnimation = ATTACK;
		m_sprite->SetWidth(25);
		m_sprite->SetHeight(32);
		//check if attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			m_sprite->SetWidth(20);
			m_sprite->SetHeight(30);
			//will auto set to IDLE
			m_locked = false;
			m_attacking = false;
			//resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	else
	{
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
