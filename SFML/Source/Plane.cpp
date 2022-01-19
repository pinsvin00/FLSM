#include "Plane.h"
#define N 0.03f
#define RESISTANCE_FACTOR 4.5f
#define GROUND_MASS 4000.0f
#define LIFT_FACTOR 7.0f
#define PI 3.14f
#pragma region PLANE_UTILS
float fopS(int n) {
	if (n == 0)
		return 0;
	if (n > 0)
		return -1;
	return 1;
}

int Plane::getPos(unsigned short direction) {
	switch (direction) {
	case LEFT:
		return p.getPosition().x - t.getSize().x / 2 * p.getScale().x;
	case RIGHT:
		return p.getPosition().x + t.getSize().x / 2 * p.getScale().x;
	case TOP:
		return p.getPosition().y - t.getSize().y / 2 * p.getScale().y;
	case BOTTOM:
		return p.getPosition().y + t.getSize().y / 2 * p.getScale().y;
	default:
		return -1;
	}
}

void Plane::set_scale(Vector2f s)
{
	w.s.scale(s);
	p.scale(s);
}
#pragma endregion

#pragma region CONSTRUCTORS
Wing::Wing(Vector2f pos) {
	s.setPosition(pos);
	s.setSize({ 70, 40 });
	s.setOrigin(35/2,20);
	s.setFillColor(Color::Black);
}


Plane::Plane(float mass, Vector2f pos, short type)
{
	engine = Engine(type);
	engine_sound = Sound();
	this->isCrashed = false;
	this->mass = mass;
	this->rotation = 0;
	this->forces = { 0,0 };

	switch (type) {
	case FIGHTER:
		if (!t.loadFromFile("fighter.png"))
			cout << "bruh";
		break;
	case FW190:
		if(!t.loadFromFile("bf109.png"))
			cout << "bruh";
		break;
	case SPITFIRE:
		if (!t.loadFromFile("spitfire.png"))
			cout << "bruh";
		break;
	}

	p.setTexture(t);
	p.setPosition(pos);
	p.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
	c.pos = p.getPosition();
	c.damage = 20;
	this->type = type;
//	w = Wing({ p.getPosition().x - 45, p.getPosition().y + 10 });
}
#pragma endregion


#pragma region PHYSICS
Vector2f Plane::getVelocity(){
	return mov;
}

void Plane::process(){
	//gravity
	forces.y = (10 * N) * mass;
	//collision with ground
	if (getPos(BOTTOM) >= 1080) {
		if (mov.y > 15.0f)
			isCrashed = true;
		mov.y = -(mov.y * (mass - 1800)) / (mass + 1800) * fopS(mov.y);
		if (mov.y == 0)
			forces.y = 0;
	}
	//lift
	forces.y -= cos(p.getRotation() * (PI / 180)) * mov.x * LIFT_FACTOR;
	//air resistance
	forces.y -= 0.5f * float(t.getSize().x) * mov.y * RESISTANCE_FACTOR * N;
	forces.x -= 0.5f * float(t.getSize().y) * mov.x * RESISTANCE_FACTOR * N;
	//engine related
	engine.process(p.getRotation());
	forces.x += engine.thrust.x;
	forces.y += engine.thrust.y;
	//rest
	mov.y += forces.y / mass;
	mov.x += forces.x / mass;
	forces = { 0 , 0 };
	engine.thrust = { 0 , 0 };
	p.move(mov);
	p.setRotation(-w.rot*20);
	w.s.move(mov);
	c.pos.x += mov.x;
	c.pos.y += mov.y;
}

Engine::Engine(short type)
{
	if (type == FIGHTER)
		max_power = 270;
	else if (type == SPITFIRE)
		max_power = 180;
	else if (type == FW190)
		max_power = 150;
	else max_power = 2137;
}

void Engine::process(float rotation){
	if (!working)
		return;
	thrust.x = temperature*cos((rotation)*(PI/180));
	thrust.y = temperature*sin((rotation)*(PI/180));
	if (temperature <= max_power*throttle)
		temperature += throttle * temperature / 200;
	else
		temperature -= throttle * temperature / 200;
}

#pragma endregion

#pragma region DRAW_FUNCTIONS
//draw methods
void Plane::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(this->p, state);
	target.draw(this->w.s, state);
}

void Wing::draw(RenderTarget& target, RenderStates state) const
{
	target.draw(this->s);
}
#pragma endregion

Bullet Cannon::create_bullet(float rotation, v2f pmov)
{
	Bullet bullet;
	bullet.body.setFillColor(Color::Yellow);
	bullet.body.setSize({20, 10});
	bullet.body.setPosition(this->pos);
	bullet.body.setRotation(rotation);
	bullet.mov = v2f(50, 0) + pmov;
	bullet.rotation = rotation;
	return bullet;
}
