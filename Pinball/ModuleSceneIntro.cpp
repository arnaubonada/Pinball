#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sBall = SDL_Rect{ 34,0,16,16 };
	sBumper = SDL_Rect{ 74,158,56,56 };
	sBumperHeart = SDL_Rect{ 42,75,30,30 };
	sKicker = SDL_Rect{ 0,0,32,145 };
	sLeftFlipper = SDL_Rect{ 0,238,60,20 };
	sRightFlipper = SDL_Rect{ 70,238,60,20 };
	sHeart = SDL_Rect{ 274,277,24,24 };
	sBarrier = SDL_Rect{ 41,0,18,46 };
	sBluePoint = SDL_Rect{ 68,48,16,16 };
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/Background.png");
	spritesheet = App->textures->Load("pinball/Spritesheet.png");
	spriteball = App->textures->Load("pinball/Spriteball.png");
	scTexture = App->textures->Load("pinball/Score.png");
	scoreFont = App->fonts->Load("pinball/font.png", "0123456789", 1);
	playAgain = App->textures->Load("pinball/playAgain.png");
	
	flipper_fx = App->audio->LoadFx("pinball/Flipper.wav");
	kicker_fx = App->audio->LoadFx("pinball/Kicker.wav");
	bumper_fx = App->audio->LoadFx("pinball/Bumper.wav");
	bonus_fx = App->audio->LoadFx("pinball/Bonus.wav");

	ball = App->physics->CreateCircle(464, 400, 8);
	App->audio->PlayMusic("pinball/soundtrack.ogg");
	hearts = 3;
	bonusBumper = 0;

	sensorBluePoint1 = App->physics->CreateRectangleSensor(200, 376, 5, 5);
	sensorBluePoint1->listener = this;
	sensorBluePoint2 = App->physics->CreateRectangleSensor(248, 376, 5, 5);
	sensorBluePoint2->listener = this;

	//----------------------BUMPERS---------------------

	//Bumper 1
	bumper1.type = b2_staticBody;
	bumper1.position.Set(PIXEL_TO_METERS(232), PIXEL_TO_METERS(88));
	Bumper1 = App->physics->world->CreateBody(&bumper1);

	b2CircleShape BumperShape1;
	BumperShape1.m_radius = PIXEL_TO_METERS(28);

	bumperFixture1.shape = &BumperShape1;
	bumperFixture1.restitution = 1, 1;
	Bumper1->CreateFixture(&bumperFixture1);

	//Bumper 2
	bumper2.type = b2_staticBody;
	bumper2.position.Set(PIXEL_TO_METERS(168), PIXEL_TO_METERS(167));
	Bumper2 = App->physics->world->CreateBody(&bumper2);

	b2CircleShape BumperShape2;
	BumperShape2.m_radius = PIXEL_TO_METERS(28);

	bumperFixture2.shape = &BumperShape2;
	bumperFixture2.restitution = 1, 1;
	Bumper2->CreateFixture(&bumperFixture2);

	//Bumper 3
	bumper3.type = b2_staticBody;
	bumper3.position.Set(PIXEL_TO_METERS(296), PIXEL_TO_METERS(167));
	Bumper3 = App->physics->world->CreateBody(&bumper3);

	b2CircleShape BumperShape3;
	BumperShape3.m_radius = PIXEL_TO_METERS(28);

	bumperFixture3.shape = &BumperShape3;
	bumperFixture3.restitution = 1, 1;
	Bumper3->CreateFixture(&bumperFixture3);

	//Bumper 4 (right heart)

	RightHeartBumper.type = b2_staticBody;
	RightHeartBumper.position.Set(PIXEL_TO_METERS(392), PIXEL_TO_METERS(560));
	heartBumperRight = App->physics->world->CreateBody(&RightHeartBumper);

	b2CircleShape BumperShape4;
	BumperShape4.m_radius = PIXEL_TO_METERS(15);

	rightBumperFix.shape = &BumperShape4;
	rightBumperFix.restitution = 3;
	heartBumperRight->CreateFixture(&rightBumperFix);

	//Bumper 5 (left heart)

	LeftHeartBumper.type = b2_staticBody;
	LeftHeartBumper.position.Set(PIXEL_TO_METERS(56), PIXEL_TO_METERS(560));
	heartBumperLeft = App->physics->world->CreateBody(&LeftHeartBumper);

	b2CircleShape BumperShape5;
	BumperShape5.m_radius = PIXEL_TO_METERS(15);

	leftBumperFix.shape = &BumperShape5;
	leftBumperFix.restitution = 3;
	heartBumperLeft->CreateFixture(&leftBumperFix);

	//----------------------SLINGSHOTS---------------------
//Slingshot 1
	slingshot1.type = b2_staticBody;
	Slingshot1 = App->physics->world->CreateBody(&slingshot1);

	b2Vec2 vertices1[5];
	vertices1[0].Set(PIXEL_TO_METERS(124), PIXEL_TO_METERS(532));
	vertices1[1].Set(PIXEL_TO_METERS(154), PIXEL_TO_METERS(548));
	vertices1[2].Set(PIXEL_TO_METERS(160), PIXEL_TO_METERS(540));
	vertices1[3].Set(PIXEL_TO_METERS(132), PIXEL_TO_METERS(494));
	vertices1[4].Set(PIXEL_TO_METERS(124), PIXEL_TO_METERS(498));

	b2PolygonShape SlingshotShape1;
	SlingshotShape1.Set(vertices1, 5);

	leftSlingshotFix.shape = &SlingshotShape1;

	slinghsotLeft = App->physics->world->CreateBody(&slingshot1);
	leftSlingshotFix.restitution = 2;
	slinghsotLeft->CreateFixture(&leftSlingshotFix);

	//Slingshot 2
	slingshot2.type = b2_staticBody;
	Slingshot2 = App->physics->world->CreateBody(&slingshot2);

	b2Vec2 vertices2[5];
	vertices2[0].Set(PIXEL_TO_METERS(324), PIXEL_TO_METERS(532));
	vertices2[1].Set(PIXEL_TO_METERS(294), PIXEL_TO_METERS(548));
	vertices2[2].Set(PIXEL_TO_METERS(288), PIXEL_TO_METERS(540));
	vertices2[3].Set(PIXEL_TO_METERS(314), PIXEL_TO_METERS(494));
	vertices2[4].Set(PIXEL_TO_METERS(324), PIXEL_TO_METERS(496));

	b2PolygonShape SlingshotShape2;
	SlingshotShape2.Set(vertices2, 5);

	rightSlingshotFix.shape = &SlingshotShape2;

	slinghsotRight = App->physics->world->CreateBody(&slingshot2);
	rightSlingshotFix.restitution = 2;
	slinghsotRight->CreateFixture(&rightSlingshotFix);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background);
	App->textures->Unload(spritesheet);
	App->textures->Unload(spriteball);
	App->textures->Unload(scTexture);
	App->textures->Unload(playAgain);
	App->fonts->UnLoad(scoreFont);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	p2List_item<PhysBody*>* chain = sceneElements.getFirst();
	while (chain != nullptr) {
		b2Body* body = chain->data->body;
		chain->data->body->GetWorld()->DestroyBody(body);
		chain = chain->next;
	}
	sceneElements.clear();

	App->renderer->Blit(background, 0, 0);
	if (!nohearts)
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			framesToSec++;

			if (framesToSec == 1 || framesToSec == 30 || framesToSec == 60 || framesToSec == 90 || framesToSec == 120) {

				strength += 50;

				if (strength > 250) { strength = 250; }

			}

		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			App->audio->PlayFx(kicker_fx);
			App->physics->Kicker->body->ApplyForceToCenter(b2Vec2(0, -strength), true);
			framesToSec = 0;
			strength = 0;

		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{

			App->audio->PlayFx(flipper_fx);

		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			App->physics->leftFlipper->body->ApplyForceToCenter({ 0, -100 }, true);
			App->physics->RevoluteJointLeft.lowerAngle = -15 * DEGTORAD;

			App->physics->leftTopFlipper->body->ApplyForceToCenter({ 0, -100 }, true);
			App->physics->RevoluteJointTopLeft.lowerAngle = -15 * DEGTORAD;

		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			App->physics->rightFlipper->body->ApplyForceToCenter({ 0, -100 }, true);
			App->physics->RevoluteJointLeft.lowerAngle = -15 * DEGTORAD;

			App->physics->rightTopFlipper->body->ApplyForceToCenter({ 0, -100 }, true);
			App->physics->RevoluteJointTopLeft.lowerAngle = -15 * DEGTORAD;

		}

		//input to restart a ball
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			ball->body->GetWorld()->DestroyBody(ball->body);
			ball = App->physics->CreateCircle(464, 400, 8);
		}

		//input to lose a heart
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			ball->body->GetWorld()->DestroyBody(ball->body);
			ball = App->physics->CreateCircle(464, 400, 8);
			hearts=0;
		}

	}
	if (nohearts) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			hearts = 3;
			nohearts = false;
			if (score > highScore) highScore = score;
			prevScore = score;
			score = 0;
			bonusBumper = 0;
		}
	}

	int Background[102] = {
			480, 477,
			478, 101,
			477, 71,
			464, 35,
			450, 20,
			428, 7,
			114, 6,
			93, 13,
			79, 22,
			65, 37,
			55, 55,
			47, 77,
			40, 110,
			35, 140,
			34, 162,
			31, 179,
			25, 190,
			19, 200,
			19, 239,
			19, 298,
			22, 330,
			30, 352,
			52, 381,
			53, 447,
			35, 469,
			35, 598,
			90, 641,
			90, 676,
			361, 677,
			361, 639,
			411, 604,
			412, 478,
			397, 462,
			397, 430,
			412, 413,
			425, 391,
			428, 369,
			427, 94,
			424, 82,
			414, 74,
			400, 77,
			362, 95,
			353, 88,
			390, 49,
			413, 49,
			430, 58,
			439, 67,
			447, 81,
			449, 95,
			447, 480,
			480, 480
	}; 
	int BackgroundBarrier[88] = {
		113, 5,
		93, 10,
		79, 22,
		65, 37,
		55, 55,
		47, 77,
		40, 110,
		35, 140,
		34, 162,
		31, 179,
		25, 190,
		19, 200,
		19, 239,
		19, 298,
		22, 330,
		30, 352,
		52, 381,
		53, 447,
		35, 469,
		35, 598,
		90, 641,
		90, 676,
		361, 677,
		361, 639,
		411, 604,
		412, 478,
		397, 462,
		397, 430,
		412, 413,
		425, 391,
		428, 369,
		427, 94,
		424, 82,
		414, 74,
		400, 77,
		362, 95,
		353, 88,
		392, 50,
		402, 48,
		414, 49,
		422, 48,
		430, 48,
		430, 6,
		121, 6
	};
	int Background1[26] = {
		63, 113,
		71, 83,
		85, 59,
		93, 45,
		107, 38,
		121, 38,
		146, 60,
		148, 75,
		143, 83,
		74, 150,
		63, 144,
		60, 127,
		62, 113
	};
	int Background2[38] = {
		386, 140,
		376, 143,
		371, 149,
		371, 220,
		367, 238,
		353, 251,
		328, 266,
		293, 280,
		293, 287,
		303, 289,
		306, 300,
		296, 310,
		298, 315,
		312, 317,
		375, 360,
		386, 362,
		396, 352,
		396, 149,
		389, 140
	};

	int Background3[66] = {
		76, 206,
		82, 205,
		90, 213,
		97, 235,
		103, 254,
		114, 265,
		132, 273,
		151, 280,
		153, 285,
		144, 291,
		141, 297,
		147, 307,
		158, 310,
		159, 316,
		146, 323,
		111, 356,
		108, 360,
		101, 356,
		76, 331,
		73, 324,
		77, 317,
		68, 305,
		67, 295,
		76, 286,
		75, 279,
		67, 273,
		68, 263,
		77, 255,
		77, 247,
		66, 239,
		66, 230,
		73, 225,
		73, 212
	};

	int Background4[16] = {
		79, 520,
		84, 518,
		91, 522,
		91, 570,
		155, 602,
		142, 617,
		76, 581,
		75, 522
	};

	int Background5[16] = {
		292, 604,
		307, 615,
		369, 582,
		370, 525,
		363, 518,
		357, 522,
		355, 571,
		294, 603
	};

	int Background6[12] = {
		176, 351,
		183, 353,
		183, 398,
		176, 401,
		168, 397,
		168, 355,
	};

	int Background7[12] = {
		224, 349,
		231, 353,
		231, 396,
		224, 401,
		216, 397,
		216, 353,
	};

	int Background8[12] = {
		264, 355,
		264, 396,
		272, 401,
		279, 396,
		280, 355,
		272, 350,
	};
	
	if (METERS_TO_PIXELS(ball->body->GetPosition().x) > 428) sceneElements.add(App->physics->CreateChain(0, 0, Background, 102));
	else sceneElements.add(App->physics->CreateChain(0, 0, BackgroundBarrier, 88));
	sceneElements.add(App->physics->CreateChain(0, 0, Background1, 26));
	sceneElements.add(App->physics->CreateChain(0, 0, Background2, 38));
	sceneElements.add(App->physics->CreateChain(0, 0, Background3, 66));
	sceneElements.add(App->physics->CreateChain(0, 0, Background4, 16));
	sceneElements.add(App->physics->CreateChain(0, 0, Background5, 16));
	sceneElements.add(App->physics->CreateChain(0, 0, Background6, 12));
	sceneElements.add(App->physics->CreateChain(0, 0, Background7, 12));
	sceneElements.add(App->physics->CreateChain(0, 0, Background8, 12));

	//print blue points
	App->renderer->Blit(spritesheet, 192, 368, &sBluePoint);
	App->renderer->Blit(spritesheet, 240, 368, &sBluePoint);

	//print ball
	App->renderer->Blit(spriteball, METERS_TO_PIXELS(ball->body->GetPosition().x) - 8, METERS_TO_PIXELS(ball->body->GetPosition().y) - 8, &sBall);

	//print bumpers
	App->renderer->Blit(spritesheet, 204, 60, &sBumper);
	App->renderer->Blit(spritesheet, 140, 139, &sBumper);
	App->renderer->Blit(spritesheet, 268, 139, &sBumper);
	App->renderer->Blit(spritesheet, 376, 545, &sBumperHeart);
	App->renderer->Blit(spritesheet, 41, 545, &sBumperHeart);

	//print barrier
	App->renderer->Blit(spritesheet, 412, 8, &sBarrier);

	//print right flippers
	App->renderer->Blit(spritesheet, 238, 605, &sRightFlipper, 1.0f, App->physics->rightFlipper->GetRotation(), 50, 5);
	App->renderer->Blit(spritesheet, 238, 285, &sRightFlipper, 1.0f, App->physics->rightTopFlipper->GetRotation(), 50, 5);
	
	//print left flippers
	App->renderer->Blit(spritesheet, 150, 605, &sLeftFlipper, 1.0f, App->physics->leftFlipper->GetRotation(), 5, 5);
	App->renderer->Blit(spritesheet, 150, 285, &sLeftFlipper, 1.0f, App->physics->leftTopFlipper->GetRotation(), 5, 5);

	//print kicker
	App->renderer->Blit(spritesheet, METERS_TO_PIXELS(App->physics->Kicker->body->GetPosition().x) - 15, METERS_TO_PIXELS(App->physics->Kicker->body->GetPosition().y) - 2, &sKicker);

	//print hearts
	if (hearts == 3) 
	{
		App->renderer->Blit(spritesheet, 390, 615, &sHeart);
		App->renderer->Blit(spritesheet, 420, 615, &sHeart);
		App->renderer->Blit(spritesheet, 450, 615, &sHeart);
	}	
	if (hearts == 2) 
	{
		App->renderer->Blit(spritesheet, 420, 615, &sHeart);
		App->renderer->Blit(spritesheet, 450, 615, &sHeart);
	}	
	if (hearts == 1) App->renderer->Blit(spritesheet, 450, 615, &sHeart);

	//lose ball
	if (METERS_TO_PIXELS(ball->body->GetPosition().y) > 645)
	{
		ball->body->GetWorld()->DestroyBody(ball->body);
		ball = App->physics->CreateCircle(464, 400, 8);
		hearts--;
	}

	if (hearts==0)
	{
		App->renderer->Blit(playAgain, 40, 140);
		App->fonts->BlitText(170, 265, scoreFont, scoreText);
		nohearts = true;
	}

	//print score
	App->renderer->Blit(scTexture, 0, 640);

	sprintf_s(scoreText, 10, "%1d", score);
	if (hearts != 0) App->fonts->BlitText(-15, 5, scoreFont, scoreText);
	
	sprintf_s(highScoreText, 10, "%1d", highScore);
	App->fonts->BlitText(75, 649, scoreFont, highScoreText);
	
	sprintf_s(prevScoreText, 10, "%1d", prevScore);
	App->fonts->BlitText(325, 649, scoreFont, prevScoreText);

	if (bumperSensed == true) {
		bumperSensed = false;
		App->audio->PlayFx(bumper_fx);
		score += 100;
		bonusBumper++;
	}
	if (bonusBumper == 20)
	{
		App->audio->PlayFx(bonus_fx);
		score += 10000;
		bonusBumper = 0;
	}
	if (score > 999999) score = 999999;

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	//// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == sensorBluePoint1 || bodyA == sensorBluePoint2)
	{
		score += 20;
	}
}
