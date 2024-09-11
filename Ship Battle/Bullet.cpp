#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Bullet : public sf::Drawable, public sf::Transformable {

	static const int BASE_X = 320;
	static const int BASE_Y = 0;


public:


	Bullet(sf::Vector2f pos, sf::Vector2f dir, sf::Texture* ptileset, sf::Time lifespan = sf::milliseconds(3000)) {
		m_position = pos;
		m_direction = dir;
		m_ptileset = ptileset;//probably don't need this
		m_sprite.setTexture(*ptileset);
		m_sprite.setTextureRect(sf::IntRect(BASE_X, BASE_Y, 32, 32));
		m_sprite.setOrigin(16, 16);
		m_lifespan = lifespan;
	}

	~Bullet() {
		std::cout << "Destructor called for bullet!" << std::endl;
		delete this;
	}

	sf::Vector2f getDirection() {
		return m_direction;
	}

	sf::Time getLifespan() {
		return m_lifespan;
	}

	void setLifespan(sf::Time life) {
		m_lifespan = life;
	}

	void setSpeed(float speed) {
		float x = m_direction.x;
		float y = m_direction.y;
		float curSpeed = sqrt(x * x + y * y);
		m_direction.x = x * speed / curSpeed;
		m_direction.y = y * speed / curSpeed;
	}


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = m_ptileset;

		// draw the vertex array
		target.draw(m_sprite, states);
	}


private:
	sf::Vector2f m_position;
	sf::Vector2f m_direction;
	int speed = 5;
	sf::Texture* m_ptileset; //pointer to the passed tileset
	sf::Sprite m_sprite;
	sf::Time m_lifespan;

};