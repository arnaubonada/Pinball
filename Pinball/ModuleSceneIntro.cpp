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
	sBumper = SDL_Rect{ 37,79,28,28 };
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
	spritesheet = App->textures->Load("pinball/Spritesheet.png");

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	ball = App->physics->CreateCircle(50, 50, 16);



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
	vertices1[0].Set(PIXEL_TO_METERS(62), PIXEL_TO_METERS(266));
	vertices1[1].Set(PIXEL_TO_METERS(77), PIXEL_TO_METERS(274));
	vertices1[2].Set(PIXEL_TO_METERS(80), PIXEL_TO_METERS(270));
	vertices1[3].Set(PIXEL_TO_METERS(66), PIXEL_TO_METERS(247));
	vertices1[4].Set(PIXEL_TO_METERS(62), PIXEL_TO_METERS(249));

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
	vertices2[0].Set(PIXEL_TO_METERS(162), PIXEL_TO_METERS(266));
	vertices2[1].Set(PIXEL_TO_METERS(147), PIXEL_TO_METERS(274));
	vertices2[2].Set(PIXEL_TO_METERS(144), PIXEL_TO_METERS(270));
	vertices2[3].Set(PIXEL_TO_METERS(157), PIXEL_TO_METERS(247));
	vertices2[4].Set(PIXEL_TO_METERS(162), PIXEL_TO_METERS(249));

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
	p2List_item<PhysBody*>* chain = ricks.getFirst();
	while (chain != nullptr) {
		b2Body* body = chain->data->body;
		chain->data->body->GetWorld()->DestroyBody(body);
		chain = chain->next;
	}
	ricks.clear();

	App->renderer->Blit(background, 0, 0);

	// Pivot 0, 0
	int Background[98] = {
		239, 239,
		238, 49,
		237, 29,
		232, 17,
		225, 9,
		215, 4,
		56, 3,
		44, 7,
		36, 12,
		29, 21,
		24, 30,
		20, 40,
		18, 50,
		16, 63,
		16, 75,
		14, 84,
		9, 96,
		8, 103,
		10, 167,
		14, 177,
		19, 182,
		27, 194,
		26, 221,
		17, 230,
		16, 236,
		17, 301,
		45, 320,
		178, 320,
		205, 302,
		206, 239,
		204, 234,
		197, 229,
		197, 214,
		204, 208,
		210, 197,
		213, 187,
		214, 47,
		210, 37,
		204, 34,
		196, 36,
		181, 48,
		178, 43,
		194, 27,
		201, 24,
		209, 26,
		218, 31,
		224, 44,
		224, 239,
		238, 239
	};

	ricks.add(App->physics->CreateChain(0, 0, Background, 98));

	//print ball
	App->renderer->Blit(spritesheet, ball->body->GetPosition().x * 25 - 8, ball->body->GetPosition().y * 25 - 8, &sBall);

	//print blumpers
	App->renderer->Blit(spritesheet, 232/2 - 14, 88/2 - 14, &sBumper);
	App->renderer->Blit(spritesheet, 168 / 2 - 14, 167 / 2 - 14, &sBumper);
	App->renderer->Blit(spritesheet, 296 / 2 - 14, 167 / 2 - 14, &sBumper);


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
