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

	b2Body* bumper_1;
	b2Body* bumper_2;
	b2Body* bumper_3;
	b2Body* bumper_4;
	b2Body* bumper_5;
	b2Body* slingshot_1;
	b2Body* slingshot_2;

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
