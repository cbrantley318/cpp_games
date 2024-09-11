#include <iostream>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "TileMap.cpp"
#include "Ship.cpp"
#include "Bullet.cpp"

#include <direct.h>   




const sf::Time period = sf::milliseconds(1000.f / 60.f);  //60 fps => 1/60 seconds per frame = 1000/60
const sf::Time lifespan = sf::milliseconds(3000);
const int numEnemies = 4;


static sf::VertexArray generateVertices(std::vector<int> vec, int screenWidth, int screenHeight, sf::Color color);
static int getMaxVal(std::vector<int> vec);
static int fireBullet(std::list<Bullet*>& bulList, Ship* ship, sf::Texture* text);
static int tile8(sf::RenderTarget& target, TileMap* obj, int width, int height);
static int tile8(sf::RenderTarget& target, Bullet* obj, int width, int height);
static int tile8(sf::RenderTarget& target, Ship* obj, int width, int height);
static int wrapAround(sf::Transformable* obj, sf::FloatRect screenRect);
static int moveTowards(Ship& me, Ship& target, int speed, sf::FloatRect screenRect);
static float calcDistSquared(sf::Vector2f pos1, sf::Vector2f pos2);


static bool checkShipHealth(const Ship* ship) { return (!ship->isAlive()); }


int main() {

    std::time_t seed = std::time(0);  // seed is an integer type
    std::srand(seed);

    int mapWidth = 22;
    int mapHeight = 16;

    std::vector<int> level =
    {
        0, 1, 5, 3, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 5, 3, 0, 0, 0, 0, 0,
        0, 1, 3, 0, 3, 5, 1, 3, 3, 3, 1, 1, 1, 0, 0, 0, 1, 3, 1, 3, 1, 3,
        0, 1, 1, 0, 3, 1, 1, 1, 1, 0, 1, 1, 1, 5, 0, 0, 1, 3, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 0, 3, 5, 0, 3, 1, 1, 1, 1, 3, 0, 5, 3, 3, 5, 3, 5,
        3, 0, 1, 0, 3, 0, 3, 1, 5, 0, 1, 5, 1, 1, 1, 1, 1, 3, 3, 1, 3, 1,
        0, 0, 1, 0, 3, 5, 3, 1, 0, 0, 3, 0, 1, 3, 1, 1, 3, 3, 3, 1, 3, 1,
        3, 0, 1, 0, 3, 0, 3, 1, 5, 0, 1, 1, 1, 3, 1, 1, 1, 3, 3, 1, 3, 1,
        0, 1, 3, 0, 3, 5, 1, 3, 3, 3, 1, 1, 1, 0, 0, 0, 1, 3, 1, 3, 1, 3,
        0, 1, 1, 0, 3, 1, 1, 1, 1, 0, 1, 1, 1, 5, 0, 0, 1, 3, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 5, 3, 1, 0, 0, 0, 0, 1, 1, 1, 1, 5, 3, 3, 1, 3, 1,
        3, 0, 1, 0, 3, 0, 3, 1, 5, 0, 1, 1, 1, 5, 1, 1, 1, 3, 3, 1, 3, 1,
        0, 5, 1, 0, 3, 5, 3, 1, 0, 0, 5, 0, 1, 1, 1, 1, 1, 3, 3, 1, 3, 1,
        0, 0, 1, 0, 3, 5, 3, 1, 0, 0, 0, 3, 1, 1, 1, 1, 5, 3, 3, 1, 3, 1,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 5, 3, 0, 0, 0, 0, 0,
        3, 0, 1, 0, 3, 0, 3, 1, 5, 0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 3, 1,
        0, 1, 3, 0, 3, 5, 1, 3, 3, 3, 1, 1, 1, 0, 0, 0, 1, 3, 1, 3, 1, 3,
    };
    
    //std::cout << "Address of mapWidth (stack): " << &mapWidth << std::endl;
    //std::cout << "Address of mapHeight (stack): " << &mapHeight << std::endl;
    //std::cout << "Address of level (stack): " << &level << std::endl;
    //std::cout << "Address of level[0] (heap I hope): " << &(level[0]) << std::endl;



    for (int i = 0; i < mapWidth * mapHeight; i++) {
        if (level[i] == 0) {
            level[i]++;
        }
        level[i] = level[i] + 48;
    }


    TileMap map;
    if (!map.load("tilesets/World_A1.png", sf::Vector2u(32, 32), level, mapWidth, mapHeight)) {
        return -1;
    }

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("tilesets/Objects.png")) {
        std::cout << "Could not load bullet texture!" << std::endl;
        return -1;
    }

    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("tilesets/Vehicle.png")) {
        std::cout << "Could not load ship texutre!" << std::endl;
        return -1;
    }

    sf::SoundBuffer shotBuffer;
    if (!shotBuffer.loadFromFile("audios/Shot3.ogg")) {
        std::cout << "Could not load gunshot audio!" << std::endl;
        return -1;
    }
    sf::Sound shotSound;
    shotSound.setBuffer(shotBuffer);


    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int screenWidth = desktop.width;
    int screenHeight = desktop.height;
    sf::RenderWindow window(desktop, "CarsonTestingSFML", sf::Style::Fullscreen);


    sf::View view = window.getDefaultView();
    view.setCenter(sf::Vector2f(mapWidth*16, mapHeight*16));
    view.zoom(0.165);
    window.setView(view);


    sf::FloatRect mapBounds = map.getBounds();
    sf::FloatRect screenRect(mapWidth*16 - mapBounds.width/2,
                            mapHeight*16 - mapBounds.height/2, 
                            mapWidth*16 + mapBounds.width / 2, 
                            mapHeight*16 + mapBounds.height / 2);


    //Creating sprites
    Ship* ship = new Ship(sf::Vector2f(256.f, 128.f), &shipTexture);
    ship->setPosition(view.getCenter());

    std::list<Ship*> enemies;
    for (int i = 0; i < numEnemies; i++) {
        Ship* enemy = new Ship(sf::Vector2f(0.f, 0.f), &shipTexture);
        int xDir = ((std::rand() % 2) << 1) - 1;
        int yDir = ((std::rand() % 2) << 1) - 1;
        enemy->setPosition(sf::Vector2f(ship->getPosition().x + xDir * (800 + std::rand() % 400), ship->getPosition().y + yDir*(600 + std::rand() % 400)));
        enemies.push_back(enemy);
    }



    std::list<Bullet*> enemyBullets;
    std::list<Bullet*> myBullets;

    sf::Clock bulletClock;
    sf::Time fireRate = sf::seconds(1.f);
    sf::Time bulletTime = bulletClock.restart();

    sf::Clock clock;
    sf::Time currTime = clock.restart();
    sf::VertexArray vertices;


    while (window.isOpen()) {
        currTime = clock.restart(); //get initial time at start of event loop
        window.clear();

        //Start event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            ship->move(0.f, -5.f);
            ship->setHeading(sf::Vector2f(0.f, -1.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            ship->move(-5.f, 0.f);
            ship->setHeading(sf::Vector2f(-1.f, 0.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            ship->move(0.f, 5.f);
            ship->setHeading(sf::Vector2f(0.f, 1.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            ship->move(5.f, 0.f);
            ship->setHeading(sf::Vector2f(1.f, 0.f));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (bulletClock.getElapsedTime() > fireRate) {
                bulletClock.restart();
                fireBullet(myBullets, ship, &bulletTexture);
                shotSound.play();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) window.close();
        if (!ship->isAlive()) window.close();
        //End event handling



        //bullet movement + lifetime handling
        for (Bullet* b : myBullets) {
            b->setLifespan(b->getLifespan() - period);
            b->move(b->getDirection());
        }
        //annoying but this is easier than trying to pop from the front during the iterator traversal
        if (myBullets.size() > 0 && myBullets.front()->getLifespan() <= sf::seconds(0)) {
            myBullets.pop_front();
        }

        for (Bullet* b : enemyBullets) {
            b->setLifespan(b->getLifespan() - period);
            b->move(b->getDirection());
        }

        if (enemyBullets.size() > 0 && enemyBullets.front()->getLifespan() <= sf::seconds(0)) {
            enemyBullets.pop_front();
        }
        ////


        //moving enemy ships
        for (Ship* enemy : enemies) {
            moveTowards(*enemy, *ship, 4, screenRect);
            wrapAround(enemy, screenRect);
        }

        //checking the in-game bounds
        wrapAround(ship, screenRect);
        //wrapAround(testShot, screenRect);
        for (Bullet* b : myBullets) {
            wrapAround(b, screenRect);
        }
        for (Bullet* b : enemyBullets) {
            wrapAround(b, screenRect);
        }
        ////


        //collision detection and handling
        for (Ship* en : enemies) {
            if (calcDistSquared(ship->getPosition(), en->getPosition()) < 400) {
                //std::cout << "You died! Ship collision" << std::endl;
                ship->kill();
            }

            if (std::rand() % 401 == 3) {
                fireBullet(enemyBullets, en, &bulletTexture);
            }

            for (Bullet* b : myBullets) {
                if (calcDistSquared(en->getPosition(), b->getPosition()) < 400) {
                    //std::cout << "Ship killed!" << std::endl;
                    en->kill();
                }
            }
        }

        for (Bullet* b : enemyBullets) {
            if (calcDistSquared(b->getPosition(), ship->getPosition()) < 400) {
                //std::cout << "You died! Ship collision" << std::endl;
                ship->kill();
            }
        }

        enemies.remove_if(checkShipHealth);

        ////



        //Now the rendering bits

        view.setCenter(ship->getPosition());
        window.setView(view);
        window.clear();
        window.draw(map);

        //haven't yet found a better way to do this, although at this point it would be more efficient to try to get the view itself to wrap into four quadrants or something
        tile8(window, &map, screenRect.width, screenRect.height);
        //tile8(window, testShot, screenRect.width, screenRect.height);
        for (Ship* enemy : enemies) {
        tile8(window, enemy, screenRect.width, screenRect.height);
        }
        for (Bullet* b : myBullets) {
            tile8(window, b, screenRect.width, screenRect.height);
            window.draw(*b);
        }
        for (Bullet* b : enemyBullets) {
            tile8(window, b, screenRect.width, screenRect.height);
            window.draw(*b);
        }

        //this is unnecessary since the tile8 function does this all
        window.draw(*ship);
        for (Ship* enemy : enemies) {
        if (enemy->isAlive()) window.draw(*enemy);
        }
        //window.draw(*testShot);
        window.display();


        sf::Time sleepTime = period - clock.restart();
        sf::sleep(sleepTime);
    }
    return 0;
}


static int getMaxVal(std::vector<int> vec) {
    int maxVal = INT_MIN;
    for (int num : vec) {
        if (num > maxVal) {
            maxVal = num;
        }
    }
    return maxVal;
}


static int tile8(sf::RenderTarget& target, TileMap* obj, int width, int height) {
    sf::Vector2f pos = obj->getPosition();
    obj->move(width, 0);
    target.draw(*obj);
    obj->move(0, height);
    target.draw(*obj);
    obj->move(-width, 0);
    target.draw(*obj);
    obj->move(-width, 0);
    target.draw(*obj);
    obj->move(0, -height);
    target.draw(*obj);
    obj->move(0, -height);
    target.draw(*obj);
    obj->move(width, 0);
    target.draw(*obj);
    obj->move(width, 0);
    target.draw(*obj);
    obj->setPosition(pos);
    return 0;
}

static int tile8(sf::RenderTarget& target, Ship* obj, int width, int height) {
    sf::Vector2f pos = obj->getPosition();
    obj->move(width, 0);
    target.draw(*obj);
    obj->move(0, height);
    target.draw(*obj);
    obj->move(-width, 0);
    target.draw(*obj);
    obj->move(-width, 0);
    target.draw(*obj);
    obj->move(0, -height);
    target.draw(*obj);
    obj->move(0, -height);
    target.draw(*obj);
    obj->move(width, 0);
    target.draw(*obj);
    obj->move(width, 0);
    target.draw(*obj);
    obj->setPosition(pos);
    return 0;
}

static int tile8(sf::RenderTarget& target, Bullet* obj, int width, int height) {
    sf::Vector2f pos = obj->getPosition();
    obj->move(width, 0);
    target.draw(*obj);
    obj->move(0, height);
    target.draw(*obj);
    obj->move(-width, 0);
    target.draw(*obj);
    obj->move(-width, 0);
    target.draw(*obj);
    obj->move(0, -height);
    target.draw(*obj);
    obj->move(0, -height);
    target.draw(*obj);
    obj->move(width, 0);
    target.draw(*obj);
    obj->move(width, 0);
    target.draw(*obj);
    obj->setPosition(pos);
    return 0;
}


static int wrapAround(sf::Transformable* obj, sf::FloatRect screenRect) {
    sf::Vector2f pos = (*obj).getPosition();
    if (!screenRect.contains(pos)) { //if we need to wrap the ship around to the other side
        if (pos.x <= screenRect.left) {
            (*obj).move(screenRect.width, 0);
        }
        if (pos.x >= screenRect.left + screenRect.width) {
            (*obj).move(-screenRect.width, 0);
        }
        if (pos.y <= screenRect.top) {
            (*obj).move(0, screenRect.height);
        }
        if (pos.y >= screenRect.top + screenRect.height) {
            (*obj).move(0, -screenRect.height);
        }
    }
    return 0;
}

static int moveTowards(Ship& me, Ship& target, int speed, sf::FloatRect screenRect) {
    sf::Vector2f myPos = me.getPosition();
    sf::Vector2f targetPos = target.getPosition();

    float xMax, xMin, yMax, yMin;

    if (myPos.x > targetPos.x) {
        xMax = myPos.x;
        xMin = targetPos.x;
    }
    else {
        xMax = targetPos.x;
        xMin = myPos.x;
    }

    if (myPos.y > targetPos.y) {
        yMax = myPos.y;
        yMin = targetPos.y;
    }
    else {
        yMax = targetPos.y;
        yMin = myPos.y;
    }

    float xDis1 = xMax - xMin;
    float xDis2 = screenRect.width - xMax + xMin;
    float yDis1 = yMax - yMin;
    float yDis2 = screenRect.height - yMax + yMin;

    sf::Vector2f dir = targetPos - myPos; //wanna change this bit
    if (dir.x == 0 && dir.y == 0) return 0;

    if (xDis1 > xDis2) {
        dir.x = -1 * dir.x;
    }

    if (yDis1 > yDis2) {
        dir.y = -1 * dir.y;
    }


    float vel = sqrt(dir.x * dir.x + dir.y * dir.y); //want to normalize and then scale by speed


    dir.x = dir.x / vel * speed;
    dir.y = dir.y / vel * speed;

    if (dir.x*dir.x > dir.y*dir.y) {
        me.setHeading(sf::Vector2f(dir.x, 0));
    }
    else {
        me.setHeading(sf::Vector2f(0, dir.y));
    }
    me.move(dir);
    return 0;
}

static int fireBullet(std::list<Bullet*>& bulList, Ship* ship, sf::Texture* tex) {
        //std::cout << "We got here!" << std::endl;
        Bullet* b = new Bullet(ship->getPosition(), ship->getHeading(), tex, lifespan);
        b->setPosition(ship->getPosition());
        b->setSpeed(7);
        //std::cout << "Address of bullet: " << &b << std::endl;
        //std::cout << "Address of *bullet: " << &*b << std::endl;
        //std::cout << " Value of Bullet: " << b << std::endl;
        //std::cout << "And here!" << std::endl;
        bulList.push_back(b);
        //for (Bullet* b : bulList) {
        //    std::cout << b << "\t";
        //}
        return 0;
}

static float calcDistSquared(sf::Vector2f pos1, sf::Vector2f pos2) {
    return (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y);
}
