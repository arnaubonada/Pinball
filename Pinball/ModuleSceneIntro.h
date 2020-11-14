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
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	int strength = 0;
	int framesToSec = 0;

	int scoreFont = 0;
	int score = 0;
	char scoreText[10];

	int hearts;
	bool nohearts = false;

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> sceneElements;
	PhysBody* ball;
	PhysBody* sensorBluePoint1;
	PhysBody* sensorBluePoint2;
	bool sensed;

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
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
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
