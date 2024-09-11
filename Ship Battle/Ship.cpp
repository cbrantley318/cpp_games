#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>



class Ship : public sf::Drawable, public sf::Transformable {

    static sf::Texture shipTexture;
    static const int BASE_X = 96;
    static const int BASE_Y = 0;




public:


    Ship(sf::Vector2f position, sf::Texture* ptileset = NULL) {
        m_heading = sf::Vector2f(1.f, 0.f);
        m_ptileset = ptileset;//probably don't need this
        m_position = position;
        m_sprite.setTexture(*ptileset);
        m_sprite.setTextureRect(sf::IntRect(BASE_X, BASE_Y, 32, 32));
        m_sprite.setOrigin(16, 16);
        m_alive = true;
        //x = 0;
    }


	~Ship() {
        std::cout << "Destructor called for ship!" << std::endl;
        delete this;
	}

    int setHeading(sf::Vector2f newHeading) { //sets the heading and updates the texture
        m_heading = newHeading;
        int offset = getOffset(m_heading);
        m_sprite.setTextureRect(sf::IntRect(BASE_X, BASE_Y + offset, 32, 32));
        return 0;

    }

    sf::Vector2f getHeading() {
        return m_heading;
    }

    bool isAlive() const {
        return m_alive;
    }

    void kill() {
        m_alive = false;
    }




    //need to change this so it draws the sprite
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
	sf::Vector2f m_heading;
	sf::Vector2f m_position;
    sf::Texture* m_ptileset; //pointer to the passed tileset
    sf::Sprite m_sprite;
    bool m_alive;

    //returns the pixel offset for the texture dependent on current heading
    //there are 4 possible headings, so hardcoding 4 possible offsets of 0, 32, 64, and 96
    int getOffset(sf::Vector2f heading) {
        if (heading.x == 0) {
            if (heading.y > 0) {
                return 0;
            }
            return 96;
        }

        if (heading.x > 0) {
            return 64;
        }
        return 32;
    }


};