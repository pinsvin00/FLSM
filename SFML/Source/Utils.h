#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Plane.h"
#include <vector>
#include <string>
#include <iostream>
using namespace sf;
using namespace std;
template<class T1, class T2>
float distanceBetween(T1 a, T2 b) {
	return sqrt(pow((a.getPosition().x - b.getPosition().x), 2) + pow((a.getPosition().y - b.getPosition().y), 2));
}


class Logger : public Drawable{
public:
	Logger(Plane p, string f_loc);
	void update(Plane p, Vector2f m, Vector2f tar);
	Font f;
	vector<Text> t;
private:
	void draw(RenderTarget& t, RenderStates s) const override;
};

struct ResourceManager {
	ResourceManager() = default;
	vector<shared_ptr<Texture>> textures;
	vector<string> texture_names;
	vector<shared_ptr<SoundBuffer>> audio;
	vector<string> audio_names;
	void add_texture(string location);
	void add_audio(string location);
	shared_ptr<Texture> getTexture(string name);
	shared_ptr<SoundBuffer> getSound(string name);
};

struct Indicator {
	Indicator(shared_ptr<Texture> arrow);
	Indicator() = default;
	CircleShape t;
	shared_ptr<Texture> tex;
};