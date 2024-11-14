#include "stdafx.h"
#include "AniPlayer.h"

AniPlayer::AniPlayer(const std::string& name)
	: GameObject(name)
{
}

void AniPlayer::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void AniPlayer::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void AniPlayer::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void AniPlayer::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		Utils::SetOrigin(body, originPreset);
	}
}

void AniPlayer::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void AniPlayer::Init()
{
	animator.SetTarget(&body);
	
	animator.AddEvent( "Idle", 0,
		[]()
		{
			std::cout << "Idle.." << std::endl;
		}
	);
	animator.AddEvent( "Idle", 0,
		[]()
		{
			std::cout << "??" << std::endl;
		}
		);
	animator.AddEvent( "Idle", 0,
		[]()
		{
			std::cout << "!!" << std::endl;
		}
		);
	//animator.AddEvent({ "Run", 1,
	//	[]()
	//	{
	//		std::cout << "Run Start" << std::endl;
	//	}
	//	});
	//animator.AddEvent({ "Run", 8,
	//[]()
	//{
	//	std::cout << "Run End" << std::endl;
	//}
	//	});
	//animator.AddEvent({ "Jump", 1,
	//	[]()
	//	{
	//		std::cout << "Jump Start!" << std::endl;
	//	}
	//	});
	//animator.AddEvent({ "Jump", 5,
	//	[]()
	//	{
	//		std::cout << "Jump End!" << std::endl;
	//	}
	//	});
}

void AniPlayer::Release()
{
}

void AniPlayer::Reset()
{
	speed = 500.f;
	animator.Play("animations/Idle.csv");
	SetOrigin(Origins::BC);
}

void AniPlayer::Update(float dt)
{
	animator.Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::B))
	{
		animator.PlayQueue("animations/Jump.csv");
		animator.PlayQueue("animations/Idle.csv");
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		animator.PlayQueue("animations/Idle.csv");
		
	}

	return;

	float h = 0.f;
	if (isGrounded)
	{
		h = InputMgr::GetAxis(Axis::Horizontal);
		velocity.x = h * speed;
	}
	if (isGrounded && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		velocity.y = -500.f;
		animator.Play("animations/Jump.csv");
	}
	if (!isGrounded)
	{
		velocity += gravity * dt;
	}
	position += velocity * dt;
	if (position.y > 0.f)
	{
		velocity.y = 0.f;
		position.y = 0.f;
		isGrounded = true;
	}
	SetPosition(position);
	if (h != 0.f)
	{
		SetScale(h > 0.f ? sf::Vector2f(1.0f, 1.0) : sf::Vector2f(-1.f, 1.0f));
	}

	// Ani
	if (animator.GetCurrentClipId() == "Idle")
	{
		if (h != 0.f)
		{
			animator.Play("animations/Run.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "Run")
	{
		if (h == 0.f)
		{
			animator.Play("animations/Idle.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "Jump" && isGrounded)
	{
		if (h == 0.f)
		{
			animator.Play("animations/Idle.csv");
		}
		else
		{
			animator.Play("animations/Run.csv");
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		animator.SetSpeed(2);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		animator.SetSpeed(0.5);
	}
}

void AniPlayer::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
