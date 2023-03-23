
#include"SFML/Graphics.hpp"
using namespace sf;
class Bullet {
public:
	Sprite shape;

	Bullet(Texture* texture, Vector2f pos) {
		this->shape.setTexture(*texture);
		this->shape.setScale(1.f, 1.f);
		this->shape.setPosition(pos);

	}
	~Bullet() {}

};