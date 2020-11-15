#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "Application.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	int strength = 0;
	int framesToSec = 0;

	int scoreFont = 0;

	int score = 0;
	char scoreText[10];
	int highScore = 0;
	char highScoreText[10];
	int prevScore = 0;
	char prevScoreText[10];

	int hearts;
	bool nohearts = false;

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> sceneElements;
	PhysBody* ball;
	PhysBody* sensorBluePoint1;
	PhysBody* sensorBluePoint2;

	bool sensed;
	bool bumperSensed = false;
	int bonusBumper;

	b2BodyDef bumper1;
	b2BodyDef bumper2;
	b2BodyDef bumper3;
	b2BodyDef RightHeartBumper;
	b2BodyDef LeftHeartBumper;

	b2FixtureDef bumperFixture1;
	b2FixtureDef bumperFixture2;
	b2FixtureDef bumperFixture3;
	b2FixtureDef rightBumperFix;
	b2FixtureDef leftBumperFix;

	b2BodyDef slingshot1;
	b2BodyDef slingshot2;
	b2BodyDef slingshot3;

	b2FixtureDef leftSlingshotFix;
	b2FixtureDef rightSlingshotFix;

	b2Body* Bumper1;
	b2Body* Bumper2;
	b2Body* Bumper3;
	b2Body* heartBumperRight;
	b2Body* heartBumperLeft;

	b2Body* Slingshot1;
	b2Body* Slingshot2;
	b2Body* slinghsotLeft;
	b2Body* slinghsotRight;

	SDL_Texture* background;
	SDL_Texture* playAgain;
	SDL_Texture* spritesheet;
	SDL_Texture* spriteball;
	SDL_Texture* scTexture;
	
	uint flipper_fx;
	uint kicker_fx;
	uint bumper_fx;
	uint bonus_fx;

	p2Point<int> ray;
	bool ray_on;

	SDL_Rect sBall;
	SDL_Rect sBumper;
	SDL_Rect sBumperHeart;
	SDL_Rect sKicker;
	SDL_Rect sLeftFlipper;
	SDL_Rect sRightFlipper;
	SDL_Rect sHeart;
	SDL_Rect sBarrier;
	SDL_Rect sBluePoint;
};
