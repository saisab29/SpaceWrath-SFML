#include"SFML/Graphics.hpp"
#include"SFML/Window.hpp"
#include"SFML/System.hpp"
#include"SFML/Audio.hpp"
#include<iostream>
#include<cstdlib>
#include<math.h>
#include<vector>
#include<fstream>


using namespace sf;

//Vector Math
//Length of vector 
//[V] = sqrt(pow(2, Vx) + pow(2, Vy));
//Normalize
//U = V/[V] = (Vx, Vy)/ sqrt(pow(2, Vx) + pow(2,Vy));

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
class Enemy {
public:

	Sprite shape;


	int HP;
	int HPMax;

	Enemy(Texture* texture, Vector2u windowSize) {
		//this->HPMax = rand() % 3 + 1;
		this->HPMax = rand() % 101 + 50;
		this->HP = this->HPMax;

		this->shape.setScale(0.4f, 0.4f);
		this->shape.setTexture(*texture);
		this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, rand() % (int)(windowSize.y - this->shape.getGlobalBounds().height));
	}
	~Enemy() {}

};


int main() {

	




	RenderWindow window(VideoMode(1920, 1080), "Space Wrath");
	window.setFramerateLimit(60);

	
	//Icon
	sf::Image icon;
	if (!icon.loadFromFile("space wrath logo.png")) {
		// handle error
	}

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	Clock clock;
	float dt = 0.f;
	float dtMultiplier = 62.5f;

	//Init text

	Font font;
	font.loadFromFile("Fonts/eight-bit-dragon.otf");


	//Init texture

	Texture playerTex;
	playerTex.loadFromFile("Textures/player1.png");
	Sprite sprite;

	Texture playerShoot;
	playerShoot.loadFromFile("Textures/playershoot2.png");


	Texture backgroundTex;
	backgroundTex.loadFromFile("Textures/bg1.png");
	Sprite background;
	background.setTexture(backgroundTex);






	sprite.setScale(1.5f, 1.5f);
	sprite.setPosition(10.f, 10.f);
	// Set up animation variables
	int frameWidth = 48;
	int frameHeight = 48;
	int numFrames = 4; // assuming there are 8 frames in the sprite sheet
	int currentFrame = 0;
	float frameTime = 0.1f; // time in seconds to display each frame
	float elapsedTime = 0.0f;

	Texture enemyTex;
	enemyTex.loadFromFile("Textures/enemy1.png");
	Texture enemyBlast;
	enemyBlast.loadFromFile("Textures/blast.png");


	Texture bulletTex;
	bulletTex.loadFromFile("Textures/laserr.png");

	//UI INIT
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10.f, 10.f);

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(50);
	gameOverText.setFillColor(Color::White);
	gameOverText.setString("          GAME OVER   \nPRESS SPACE TO RESTART");
	gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 - gameOverText.getGlobalBounds().height);


	//Player init

	Player player(&playerTex);
	player.shape.setTextureRect(sf::IntRect(0, 0, 192, 48)); // set the initial frame to display
	int score = 0;
	int highScore = 0;
	float shootTimer = 20;
	Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(12);
	hpText.setFillColor(Color::White);



	//Enemy init
	float enemySpawnTimer = 0;
	std::vector<Enemy> enemies;

	Text ehpText;
	ehpText.setFont(font);
	ehpText.setCharacterSize(20);
	ehpText.setFillColor(Color::White);


	//HIGH SCORE
	//Load high score from file
	std::ifstream inputFile("highscore.txt");
	if (inputFile.is_open()) {
		inputFile >> highScore;
		inputFile.close();
	}


	/*Bullet bullet(&bulletTex, player.shape.getPosition());*/

	RectangleShape hpBar;
	RectangleShape ehpBar;


	bool isRunning = false;

	Texture homeTex;
	homeTex.loadFromFile("Textures/homescreen.png");
	Sprite homescreen;
	homescreen.setTexture(homeTex);



	Music music;

	if (!music.openFromFile("Sound/music.ogg")) {
		std::cout << "Error" << "\n";
	}
	SoundBuffer shootBuffer;
	if (!shootBuffer.loadFromFile("Sound/shoot.wav")) {
		std::cout << "Error" << "\n";
	}

	sf::Sound shoot;
	shoot.setBuffer(shootBuffer);

	SoundBuffer blastBuffer;
	if (!blastBuffer.loadFromFile("Sound/blast.wav")) {
		std::cout << "Error" << "\n";
	}

	shoot.setVolume(5.f);

	sf::Sound blast;
	blast.setBuffer(blastBuffer);

	music.play();

	while (window.isOpen()) {
		Event event;




		window.draw(homescreen);


		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
				isRunning = true;
			if ((event.type == Event::KeyPressed && event.key.code == Keyboard::Space)) {
				player.HP = player.HPMax;
				score = 0;
				enemies.clear();
				player.bullets.clear();
				isRunning = true;
			}

			//Music mute and unmute
			if (event.type == Event::KeyPressed && music.getStatus() == Music::Playing && event.key.code == Keyboard::M)
				music.pause();
			else if (event.type == Event::KeyPressed && music.getStatus() == Music::Paused && event.key.code == Keyboard::M)
				music.play();

		}




		//dt = 0.016 at 60fps
		//dt = clock.restart().asSeconds();
		// Update animation
		elapsedTime += clock.restart().asSeconds();
		dt = elapsedTime;


		if (elapsedTime >= frameTime)
		{
			elapsedTime -= frameTime;
			currentFrame = (currentFrame + 1) % numFrames;
			player.shape.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
		}


		//UPDATE==========================================================>



		if (isRunning == true) {
			if (player.HP >= 0) {


				//update player
				if (Keyboard::isKeyPressed(Keyboard::W))
					player.shape.move(0.f, -2.f * dt * dtMultiplier);
				if (Keyboard::isKeyPressed(Keyboard::A))
					player.shape.move(-2.f * dt * dtMultiplier, 0.f);

				if (Keyboard::isKeyPressed(Keyboard::S))
					player.shape.move(0.f, 2.f * dt * dtMultiplier);

				if (Keyboard::isKeyPressed(Keyboard::D))
					player.shape.move(2.f * dt * dtMultiplier, 0.f);



				hpText.setPosition(player.shape.getPosition().x, player.shape.getPosition().y - hpText.getGlobalBounds().height);
				hpText.setString(std::to_string(player.HP) + "/" + std::to_string(player.HPMax));


				hpBar.setSize(Vector2f(player.HP * 10.f, 10.f));
				hpBar.setFillColor(Color::Green);
				hpBar.setPosition(Vector2f(player.shape.getPosition().x, player.shape.getPosition().y));


				//Colision with window
				if (player.shape.getPosition().x <= 0) {
					player.shape.setPosition(0, player.shape.getPosition().y);
				}
				if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) {
					player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);
				}
				if (player.shape.getPosition().y <= 0) {
					player.shape.setPosition(player.shape.getPosition().x, 0);
				}
				if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) {
					player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);
				}

				//update controls
				if (shootTimer < 1) {
					shootTimer += 1;
				}
				if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 1) { //Shoot

					player.shape.setTexture(playerShoot);

					player.bullets.push_back(Bullet(&bulletTex, Vector2f(player.shape.getPosition().x, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2.f - 70.f)));
					shootTimer = 0;
				}
				if (!Mouse::isButtonPressed(Mouse::Left)) {
					player.shape.setTexture(playerTex);
				}


				//Bullets



				for (size_t i = 0; i < player.bullets.size(); i++) {

					shoot.play();
					player.bullets[i].shape.move(20.f * dt * dtMultiplier, 0);

					//Out of window Bounds
					if (player.bullets[i].shape.getPosition().x > window.getSize().x) {

						player.bullets.erase(player.bullets.begin() + i);
						break;
					}
					//Enemy Collision
					for (size_t k = 0; k < enemies.size(); k++) {


						if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds())) {

							if (enemies[k].HP <= 1) {
								score += enemies[k].HPMax;
								blast.play();
								enemies.erase(enemies.begin() + k);
								break;
							}
							else {


								enemies[k].HP--; //Enemy take damage
							}

							player.bullets.erase(player.bullets.begin() + i);
							break;
						}
					}

				}


				//Enemy
				if (enemySpawnTimer <= 40)
					enemySpawnTimer += 1;

				//enemy spawn
				if (enemySpawnTimer >= 40) {
					enemies.push_back(Enemy(&enemyTex, window.getSize()));
					enemySpawnTimer = 0; //reset timer
				}

				for (size_t i = 0; i < enemies.size(); i++) {




					enemies[i].shape.move(-2.f * dt * dtMultiplier, 0);



					if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width) {
						enemies.erase(enemies.begin() + i);
						score -= 20;

					}



					if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())) {
						enemies.erase(enemies.begin() + i);
						player.HP--;  //Player take damage
					}
				}

				if (score > highScore) {
					highScore = score;
				}


				//UI UPDATE
				scoreText.setString("Score: " + std::to_string(score) + "\nHigh Score: " + std::to_string(highScore));



			}


			//------------------------------------DRAW----------------------------------//
			//draw
			window.clear();



			window.draw(background);
			//player
			window.draw(player.shape);

			//bullets
			for (size_t i = 0; i < player.bullets.size(); i++) {
				window.draw(player.bullets[i].shape);

			}

			//enemy
			for (size_t i = 0; i < enemies.size(); i++) {
				ehpText.setString(std::to_string(enemies[i].HP) + "/" + std::to_string(enemies[i].HPMax));
				ehpText.setPosition(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y - ehpText.getGlobalBounds().height);
				window.draw(ehpText);

				ehpBar.setSize(Vector2f(enemies[i].HP * 1.f, 15.f));
				ehpBar.setFillColor(Color::Red);
				ehpBar.setPosition(Vector2f(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y));
				window.draw(ehpBar);
				window.draw(enemies[i].shape);


				window.draw(enemies[i].shape);
			}

			//UI 
			window.draw(scoreText);


			window.draw(hpText);
			window.draw(hpBar);

			if (player.HP <= 0) {

				window.draw(gameOverText);
				//Save high score from file




			}

		}
		window.display();

	}
	std::ofstream outputFile("highscore.txt");
	if (outputFile.is_open()) {
		outputFile << highScore;
		outputFile.close();
	}

	return 0;

}