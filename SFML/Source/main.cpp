#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdint.h>
#include "Plane.h"
#include "World.h"
#include "Utils.h"
#define PI 3.14
//TODO
//
using namespace sf;
using namespace std;

//global variables
clock_t time_dif = clock();
int hsw = 1920 / 2;
int hsh = 1080 / 2;
shared_ptr<Sound> s;
bool compv(Vector2f a, Vector2f b) {
	if (a.x == b.x && a.y == b.y)
		return true;
	else return false;
}

void generateNewChunks(World& world, const Vector2f& mid, shared_ptr<ResourceManager>& txtm) 
{
	//Node of chunk that is closest to screen edge
	Vector2f k = { mid.x - hsw - (int(mid.x - hsw) % CHUNK_SIZE),
				   mid.y + hsh - (int(mid.y - hsh) % CHUNK_SIZE) };
	for (int i = k.x; i <= mid.x + hsw + CHUNK_SIZE; i += CHUNK_SIZE)
		for (int j = k.y; j >= mid.y - hsh - CHUNK_SIZE; j -= CHUNK_SIZE) {
			bool e = false;
			for (const auto& element : world.chunks) {
				if (compv(element.top_left, Vector2f(i, j))) {
					e = true;
					break;
				}
			}
			if (!e)
				world.chunks.push_back(Chunk(Vector2f(i, j), txtm, NEW_GENERATED));
		}
}

void deleteOldChunks(World& world, const Vector2f& mid) {
	for (auto& element : world.chunks) {
		if (element.top_left.x <= (mid.x - 1920 / 2))
			element.loaded = false;
	}
}

void process_kb(World& world, Plane& fight, vector<Bullet> & bullets) {
	if (Keyboard::isKeyPressed(Keyboard::Space) && clock() - time_dif >= 1000) {
		bullets.push_back(fight.c.create_bullet(fight.p.getRotation(), fight.mov));
		shared_ptr<SoundBuffer> sb = world.resources->getSound("cannon.wav");
		if (s->getStatus() != Sound::Playing) {
			s->setBuffer(*sb);
			s->play();
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::I) && clock() - time_dif >= 1000) {
		fight.engine.working = !fight.engine.working;
		time_dif = clock();
		fight.engine_sound.setBuffer(*world.resources->getSound("startup"));
		fight.actual_engine_state = STARTUP;
		fight.engine_sound.play();
	}
	else if (Keyboard::isKeyPressed(Keyboard::LControl)) {
		fight.engine.throttle = fight.engine.throttle > 0.99f ? fight.engine.throttle : fight.engine.throttle + 0.01f;
	}
	else if (Keyboard::isKeyPressed(Keyboard::LShift)) {
		fight.engine.throttle = fight.engine.throttle < 0.00f ? fight.engine.throttle : fight.engine.throttle - 0.01f;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Up)) {
		fight.w.rot += 0.04f;
		fight.w.s.rotate(1);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down)) {
		fight.w.rot -= 0.04f;
		fight.w.s.rotate(-1);
	}

}

void process_bullet(Bullet& b) {
	b.body.move({ float(cos(b.body.getRotation()*(PI/180))*b.mov.x), float(sin(b.body.getRotation() * (PI / 180))*b.mov.x)});
}

int main()
{
	short c;
	int mass;
	cout << "Welcome to FLSM 1!\n";
	cout << "Choose your aircraft: \n";
	cout << "1 : Hunter F1\n VELOCITY[===]\n";
	cout << "2 : Spitfire\n VELOCITY[== ]\n";
	cout << "3 : BF109\n VELOCITY[=  ]\n";
	while (1) {
		cin >> c;
		if (c == 1) {
			mass = 150; break;
		}
		else if (c == 2) {
			mass = 100; break;
		}
		else if (c == 3) {
			mass = 90; break;
		}
	}
	Plane fighter = Plane(mass, { 1280 / 2 , 900 }, c-1);
	system("cls");
	cout << "Type in amount of targets to destroy: ";
	cin >> c;
	shared_ptr<ResourceManager> resources = make_shared<ResourceManager>();
	World world(resources, c);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "FLSM 2137");
	Vector2f mid = { 1920 / 2, (1080 / 2) };
	srand(time(NULL));
	world.resources->add_texture("landing.png");
	world.resources->add_texture("grass.png");
	world.resources->add_texture("grass.png");
	world.resources->add_texture("cloud.png");
	world.resources->add_texture("arrow.png");
	if (fighter.type == FIGHTER) {
		world.resources->add_audio("jet_startup.wav");
		world.resources->add_audio("jet_engine.wav");
	}
	else if (fighter.type == SPITFIRE) {
		world.resources->add_audio("spitfire_startup.wav");
		world.resources->add_audio("spitfire_engine.wav");
	}
	else if (fighter.type == FW190) {
		world.resources->add_audio("fw190_startup.wav");
		world.resources->add_audio("fw190_engine.wav");
	}
	fighter.engine_sound.setBuffer(*world.resources->getSound("engine"));
	world.resources->add_audio("cannon.wav");
	world.create_runway();
	world.create_indicator();
	View view(sf::FloatRect(0, 100, 1920, 1080));
	Logger log(fighter, "arial.ttf");
	s = make_shared<Sound>();
	window.setView(view);
	window.setFramerateLimit(60);
	fighter.set_scale({ 0.15f, 0.15f});


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		//move
		view.move(fighter.mov);
		mid.x += fighter.mov.x;
		mid.y += fighter.mov.y;
		log.update(fighter, fighter.mov, world.target.getPosition());

		//drawing
		window.setView(view);
		for (const auto& element : world.bullets)
			window.draw(element.body);
		for (const auto& chunk : world.chunks)
			window.draw(chunk);
		window.draw(fighter);
		window.draw(log);
		window.draw(world.target);
		window.draw(world.ind.t);
		world.ind.t.setRotation(atan2((world.target.getPosition().y) - world.ind.t.getPosition().y,
			(world.target.getPosition().x - world.ind.t.getPosition().x)) * (180 / PI));
		world.ind.t.move(fighter.mov);
		//processes
		if (fighter.engine.working) {
			if (fighter.engine_sound.getStatus() != Sound::Playing) {
				if (fighter.actual_engine_state == STARTUP) {
					fighter.engine_sound.setBuffer(*world.resources->getSound("engine"));
					fighter.actual_engine_state = WORKING;
				}
				fighter.engine_sound.play();
			}
		}
		generateNewChunks(world, mid, world.resources);
		process_kb(world, fighter, world.bullets);
		world.isTargetHit();
		fighter.process();
		for (auto& element : world.bullets)
			process_bullet(element);
		//window
		window.display();
		window.clear(Color::Cyan);
		if (world.target.getRadius() == 1) {
			time_dif = clock();
			log.t.clear();
			log.t.push_back(Text("You've won!", log.f, 20));
			log.t[0].setPosition(mid);
			while (1) {
				window.setView(view);
				for (const auto& element : world.bullets)
					window.draw(element.body);
				for (const auto& chunk : world.chunks)
					window.draw(chunk);
				window.draw(fighter);
				window.draw(log);
				window.draw(world.target);
				window.draw(world.ind.t);
				window.pollEvent(event);
				if (event.type == Event::KeyPressed && clock() - time_dif >= 3000)
					break;
				window.display();
				window.clear(Color::Cyan);
			}
			break;
		}
	}
	world.chunks.clear();
	resources->textures.clear();
	resources->audio.clear();
	return 0;
}
