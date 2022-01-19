#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
using namespace sf;
using namespace std;

enum types {
	FIGHTER,
	SPITFIRE,
	FW190
};
enum directions {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};
enum engine_state {
	STARTUP,
	WORKING
};

typedef Vector2f v2f;
struct Engine {
	Engine() = default;
	Engine(short type);
	float throttle = 0.0f;
	float temperature = 0.1f;
	bool working = false;
	float power = 0.0f;
	float max_power;
	Vector2f thrust = { 0.0f, 0.0f };
	float rotation = 0.0f;
	void process(float rotation);
};
struct Bullet {
	float rotation;
	v2f mov;
	RectangleShape body;
};

class Cannon {
public:
	float damage;
	v2f pos;
	Bullet create_bullet(float rotation, v2f pmov);
};

struct Wing : public Drawable {
	Wing(Vector2f pos);
	Wing() = default;
	RectangleShape s;
	float rot = 0;
	void draw(RenderTarget& target, RenderStates state) const override;
};


class Plane : public Drawable
{
public:
	Engine engine;
	Wing w;
	Cannon c;
	Sprite p;
	Texture t;
	Vector2f forces;
	Vector2f mov;
	Sound engine_sound;
	//primitive types
	bool isCrashed;
	float rotation,mass;
	short actual_engine_state;
	short type;
	//functions
	Plane(float mass, Vector2f pos, short type);
	Plane() = default;
	Vector2f getVelocity();
	int getPos(unsigned short direction);
	void process();
	void set_scale(Vector2f s);
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
};
