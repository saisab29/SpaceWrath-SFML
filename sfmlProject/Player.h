#pragma once

#include"SFML/Graphics.hpp"
#include"Bullet.h"

using namespace sf;

class Player {
public:
	Sprite shape;
	Texture* texture;
	int HP;
	int HPMax;


	std::vector<Bullet > bullets;

	Player(Texture* texture) {
		this->HPMax = 10;
		this->HP = this->HPMax;

		this->texture = texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(3.f, 3.f);
	}

	~Player() {}

};