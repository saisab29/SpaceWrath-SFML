#pragma once

#include"SFML/Graphics.hpp"
#include"SFML/Window.hpp"


using namespace sf;




class Enemy {
public:

	Sprite shape;


	int HP;
	int HPMax;

	Enemy(Texture* texture, Vector2u windowSize) {
		//this->HPMax = rand() % 3 + 1;
		this->HPMax = rand() % 51 + 50;
		this->HP = this->HPMax;

		this->shape.setScale(0.4f, 0.4f);
		this->shape.setTexture(*texture);
		this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, rand() % (int)(windowSize.y - this->shape.getGlobalBounds().height));
	}
	~Enemy() {}

};