#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sBall = SDL_Rect{ 34,0,16,16 };
	sBumper = SDL_Rect{ 74,158,56,56 };
	sKicker = SDL_Rect{ 0,0,32,145 };
	circle = box = rick = NULL;
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

	/*circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");*/

	background = App->textures->Load("pinball/Background.png");
	spritesheet = App->textures->Load("pinball/Spritesheet2.png");
	spriteball = App->textures->Load("pinball/Spritesheet.png");

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	ball = App->physics->CreateCircle(200, 474, 8);

	hearts = 3;

	//----------------------BUMPERS---------------------



	//Bumper 1
	b2BodyDef bumper1;
	bumper1.type = b2_staticBody; //this will be a dynamic body
	bumper1.position.Set(PIXEL_TO_METERS(232), PIXEL_TO_METERS(88)); //a little to the left

	bumper_1 = App->physics->world->CreateBody(&bumper1);
	b2CircleShape shape_bumper1;
	shape_bumper1.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //position, relative to body position
	shape_bumper1.m_radius = PIXEL_TO_METERS(28); //radius
	b2FixtureDef f_bumper1;
	f_bumper1.shape = &shape_bumper1; //this is a pointer to the shape above
	f_bumper1.restitution = 1, 1;
	bumper_1->CreateFixture(&f_bumper1); //add a fixture to the body

	//Bumper 2
	b2BodyDef bumper2;
	bumper2.type = b2_staticBody; //this will be a dynamic body
	bumper2.position.Set(PIXEL_TO_METERS(168), PIXEL_TO_METERS(167)); //a little to the left

	bumper_2 = App->physics->world->CreateBody(&bumper2);
	b2CircleShape shape_bumper2;
	shape_bumper2.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //position, relative to body position
	shape_bumper2.m_radius = PIXEL_TO_METERS(28); //radius
	b2FixtureDef f_bumper2;
	f_bumper2.shape = &shape_bumper2; //this is a pointer to the shape above
	f_bumper2.restitution = 1, 1;
	bumper_2->CreateFixture(&f_bumper2); //add a fixture to the body

	//Bumper 3
	b2BodyDef bumper3;
	bumper3.type = b2_staticBody; //this will be a dynamic body
	bumper3.position.Set(PIXEL_TO_METERS(296), PIXEL_TO_METERS(167)); //a little to the left

	bumper_3 = App->physics->world->CreateBody(&bumper3);

	b2CircleShape shape_bumper3;
	shape_bumper3.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //position, relative to body position
	shape_bumper3.m_radius = PIXEL_TO_METERS(28); //radius

	b2FixtureDef f_bumper3;
	f_bumper3.shape = &shape_bumper3;
	f_bumper3.restitution = 1, 1;//this is a pointer to the shape above
	bumper_3->CreateFixture(&f_bumper3); //add a fixture to the body



	//----------------------SLINGSHOTS---------------------



	//Slingshot 1
	b2BodyDef slingshot1;
	slingshot1.type = b2_staticBody; //this will be a dynamic body
	//slingshot1.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //a little to the left

	slingshot_1 = App->physics->world->CreateBody(&slingshot1);

	b2Vec2 vertices1[5];
	vertices1[0].Set(PIXEL_TO_METERS(62*2), PIXEL_TO_METERS(266*2));
	vertices1[1].Set(PIXEL_TO_METERS(77*2), PIXEL_TO_METERS(274*2));
	vertices1[2].Set(PIXEL_TO_METERS(80*2), PIXEL_TO_METERS(270*2));
	vertices1[3].Set(PIXEL_TO_METERS(66*2), PIXEL_TO_METERS(247*2));
	vertices1[4].Set(PIXEL_TO_METERS(62*2), PIXEL_TO_METERS(249*2));

	b2PolygonShape shape_slingshot1;
	shape_slingshot1.Set(vertices1, 5); //pass array to the shape

	b2FixtureDef fixture_slingshot1;
	fixture_slingshot1.shape = &shape_slingshot1; //change the shape of the fixture
	//slingshot1.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //in the middle

	b2Body* dynamicBody2_l = App->physics->world->CreateBody(&slingshot1);
	fixture_slingshot1.restitution = 2;
	dynamicBody2_l->CreateFixture(&fixture_slingshot1); //add a fixture to the body
	
	//Slingshot 2
	b2BodyDef slingshot2;
	slingshot2.type = b2_staticBody; //this will be a dynamic body
	//slingshot2.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //a little to the left

	slingshot_2 = App->physics->world->CreateBody(&slingshot2);

	b2Vec2 vertices2[5];
	vertices2[0].Set(PIXEL_TO_METERS(162*2), PIXEL_TO_METERS(266*2));
	vertices2[1].Set(PIXEL_TO_METERS(147*2), PIXEL_TO_METERS(274*2));
	vertices2[2].Set(PIXEL_TO_METERS(144*2), PIXEL_TO_METERS(270*2));
	vertices2[3].Set(PIXEL_TO_METERS(157*2), PIXEL_TO_METERS(247*2));
	vertices2[4].Set(PIXEL_TO_METERS(162*2), PIXEL_TO_METERS(249*2));

	b2PolygonShape shape_slingshot2;
	shape_slingshot2.Set(vertices2, 5); //pass array to the shape

	b2FixtureDef fixture_slingshot2;
	fixture_slingshot2.shape = &shape_slingshot2; //change the shape of the fixture
	//slingshot1.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //in the middle

	b2Body* dynamicBody2_2 = App->physics->world->CreateBody(&slingshot2);
	fixture_slingshot2.restitution = 2;
	dynamicBody2_2->CreateFixture(&fixture_slingshot2); //add a fixture to the body


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->physics->leftFlipper->body->ApplyForceToCenter({0, -100 }, true);
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
		76, 150,
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

	int Background3[64] = {
		76, 206,
		82, 205,
		90, 213,
		97, 235,
		103, 254,
		114, 265,
		132, 273,
		151, 280,
		157, 285,
		144, 291,
		141, 297,
		147, 307,
		158, 310,
		159, 316,
		146, 323,
		111, 356,
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
		73, 208
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
		176, 349,
		183, 353,
		183, 398,
		176, 404,
		168, 397,
		168, 355,
	};

	int Background7[12] = {
		224, 347,
		231, 353,
		231, 396,
		224, 404,
		216, 397,
		216, 353,
	};

	int Background8[12] = {
		264, 355,
		264, 396,
		272, 403,
		279, 396,
		280, 355,
		272, 349,
	};

	sceneElements.add(App->physics->CreateChain(0, 0, Background, 102));
	sceneElements.add(App->physics->CreateChain(0, 0, Background1, 26));
	sceneElements.add(App->physics->CreateChain(0, 0, Background2, 38));
	sceneElements.add(App->physics->CreateChain(0, 0, Background3, 64));
	sceneElements.add(App->physics->CreateChain(0, 0, Background4, 16));
	sceneElements.add(App->physics->CreateChain(0, 0, Background5, 16));
	sceneElements.add(App->physics->CreateChain(0, 0, Background6, 12));
	sceneElements.add(App->physics->CreateChain(0, 0, Background7, 12));
	sceneElements.add(App->physics->CreateChain(0, 0, Background8, 12));

	//print ball
	App->renderer->Blit(spriteball, ball->body->GetPosition().x * 50 - 8, ball->body->GetPosition().y * 50 - 8, &sBall);

	//print blumpers
	App->renderer->Blit(spritesheet, 232 - 28, 88 - 28, &sBumper);
	App->renderer->Blit(spritesheet, 168 - 28, 167 - 28, &sBumper);
	App->renderer->Blit(spritesheet, 296 - 28, 167 - 28, &sBumper);

	//lose ball
	if (ball->body->GetPosition().y * 50 > 645)
	{
		ball->body->GetWorld()->DestroyBody(ball->body);
		ball = App->physics->CreateCircle(200, 50, 8);//x=465 y=474
		hearts--;
	}

	if (hearts==0)
	{
		nohearts = true;
	}
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}


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
	int x, y;

	App->audio->PlayFx(bonus_fx);
	
}
