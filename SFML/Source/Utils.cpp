#include "Utils.h"


Logger::Logger(Plane p, string f_loc){
	f.loadFromFile(f_loc);
	float x = 30;
	float y = 330;
	t.push_back(Text("pos.x" + to_string(p.p.getPosition().x), f, 20));
	t.push_back(Text("pos.y" + to_string(p.p.getPosition().y), f, 20));
	t.push_back(Text("vel.x" + to_string(p.mov.x), f, 20));
	t.push_back(Text("vel.y" + to_string(p.mov.y), f, 20));
	t.push_back(Text("throttle" + to_string(p.engine.throttle), f, 20));
	t.push_back(Text("thrust" + to_string(p.engine.thrust.x), f, 20));
	t.push_back(Text("thrust" + to_string(p.engine.thrust.x), f, 20));
	t.push_back(Text("thrust" + to_string(p.engine.thrust.x), f, 20));
	for (auto &element : t) {
		element.setPosition({ x, y });
		y += 30;
	}
}

void Logger::update(Plane p, Vector2f m, Vector2f tar) {
	t[0].setString("pos.x" + to_string(p.p.getPosition().x));
	t[1].setString("pos.y" + to_string(p.p.getPosition().y));
	t[2].setString("vel.x" + to_string(p.mov.x));
	t[3].setString("vel.y" + to_string(p.mov.y));
	t[4].setString("tar.x " + to_string(tar.x));
	t[5].setString("tar.y " + to_string(tar.y));
	t[6].setString("throttle" + to_string(p.engine.throttle));
	t[7].setString("thrust" + to_string(p.engine.thrust.x));
	for (auto& element : t)
		element.move(m);
}

void Logger::draw(RenderTarget& trg, RenderStates s) const{
	for (auto& element : t)
		trg.draw(element);
}

void ResourceManager::add_texture(string location){
	auto p1 = make_shared<Texture>();
	p1->loadFromFile(location);
	textures.push_back(p1);
	this->texture_names.push_back(location);
}


void ResourceManager::add_audio(string location)
{
	auto p1 = make_shared<SoundBuffer>();
	p1->loadFromFile(location);
	audio.push_back(p1);
	if(location.find("engine") != string::npos)
		this->audio_names.push_back("engine");
	else if (location.find("startup") != string::npos)
		this->audio_names.push_back("startup");
	else this->audio_names.push_back(location);
}

shared_ptr<Texture> ResourceManager::getTexture(string name) {
	shared_ptr<Texture> k;
	return k;
}

shared_ptr<SoundBuffer> ResourceManager::getSound(string name)
{
	auto i = 0;
	for (auto element : audio_names) {
		if (element == name)
			return audio[i];
		i++;
	}
}

Indicator::Indicator(shared_ptr<Texture> arrow){
	t = CircleShape(10, 4);
	t.setFillColor(Color::Black);
	t.setPosition(1280 / 2 + 100, 900);
	t.setOrigin(-200, 0);
}
