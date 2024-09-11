#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>


const int screenWidth = 3000;
const int screenHeight = 2200;

const sf::Time period = sf::milliseconds(1000.f / 60.f);  //60 fps => 1/60 seconds per frame = 1000/60
//const sf::Time period = sf::milliseconds(1000.f / 1.f);  //60 fps => 1/60 seconds per frame


/*
Takes a normal array, screenwidth, and screenheight as input
Outputs the vertex array to render, (still need to create the textures, for now will make it just plain white)
TODO: finish this. For now, will just create like 20 diff rectangles and make them just drawn individually.
*/
static sf::VertexArray generateVertices(std::vector<int> vec, int screenWidth, int screenHeight, sf::Color color);


/*Returns the maximum VALUE in an array (not the index)*/
static int getMaxVal(std::vector<int> vec);




int sort_main() {
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "CarsonTestingSFML", sf::Style::Default);
    sf::Texture texture;
    int width = 200;
    int height = 200;

    if (!texture.create(width, height)) {
        std::cout << "Error Creating Texture" << std::endl;
        return 1;
    }

    //sf::RectangleShape rect1(sf::Vector2f(50, 500));
    //rect1.setOrigin(50.f, 0.f);

    sf::Clock clock;
    sf::Time currTime = clock.restart();

    std::vector<int> nums;
    std::srand(2);

    bool sorted = false;


    for (int i = 0; i < 1000; i++) {  //initialize to random values between 1 and 100 for now
        nums.push_back((std::rand() % 100) + 1);
    }

    sf::VertexArray vertices;

    //event loop (will try to make run at 60fps for now, sleep rest of time)
    while (window.isOpen()) {

        currTime = clock.restart(); //get initial time at start of event loop
        window.clear();


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        //    rect1.move(0.f, -5.f);
        //}
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        //    rect1.move(-5.f, 0.f);
        //}
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        //    rect1.move(0.f, 5.f);
        //}
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        //    rect1.move(5.f, 0.f);
        //}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            bool swapped = false;
        //loop through and make one sorting pass
            for (std::size_t i = 0; i < nums.size()-1; i++) {
                if (nums[i] > nums[i + 1]) {
                    std::swap(nums[i], nums[i + 1]);
                    swapped = true;
                }
            }
            if (swapped == false) {
                sorted = true;
            }
        }
        vertices = generateVertices(nums, screenWidth, screenHeight, sorted ? sf::Color::Green : sf::Color::White);
        window.clear();
        window.draw(vertices);
        window.display();

        //loop through and redraw after one pass

        //float maxHeight = screenHeight * 0.75;
        //float barWidth = screenWidth / (nums.size() + 2.f);
        //float maxVal = static_cast<float>(getMaxVal(nums));

        //drawing one rect at a time, pushing to display en masse
        //for (std::size_t i = 0; i < nums.size(); i++) {
        //    float barHeight = maxHeight * static_cast<float>(nums[i]) / maxVal;
        //    sf::RectangleShape rect(sf::Vector2f(barWidth, barHeight));

        //    rect.setOrigin(sf::Vector2f(0.f, barHeight));
        //    rect.setPosition(sf::Vector2f(barWidth*(i+1), screenHeight*0.875));
        //    rect.setFillColor(sorted ? sf::Color::Green : sf::Color::White);
        //    window.draw(rect);
        //}
        

        //window.draw(rect1);
        //window.display();

        //we want to sleep for whatever is left of our 1/60th of a second
        sf::Time sleepTime = period - clock.restart();
        //std::cout << sleepTime.asMilliseconds() << std::endl;
        sf::sleep(sleepTime);
    }


    //std::vector<sf::VideoMode> x = sf::VideoMode::getFullscreenModes();
    //sf::VideoMode y = sf::VideoMode::getDesktopMode();
    //std::cout << y.width << " " << y.height << std::endl;
    //std::cout << "Start of next  ";
    //for (sf::VideoMode mode : x) {
    //    std::cout << mode.width << " " << mode.height << std::endl;
    //}

    //std::cout << "Testing";

    return 0;
}

static sf::VertexArray generateVertices(std::vector<int> vec, int screenWidth, int screenHeight, sf::Color color = sf::Color::White) {
    //first step is to figure out the boundaries, which for my purposes the max height will be 3/4 of the screenHeight
    //width border will be the width of a single bar, and bars are evenly spaced maximally

    sf::VertexArray vertices;
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertices.resize(vec.size() * 6);

    float maxHeight = screenHeight * 0.75;
    float barWidth = screenWidth / (vec.size() + 2.f);
    float maxVal = static_cast<float>(getMaxVal(vec));
    float bottom = screenHeight * 0.875;

    //loop through the vector and get the height, then make the 6 points
    for (std::size_t i = 0; i < vec.size(); i++) {
        float val = static_cast<float>(vec[i]);

        float barTop = bottom - maxHeight * (val / maxVal);

        vertices[i * 6 + 0].position = sf::Vector2f(barWidth * (i + 1), barTop);
        vertices[i * 6 + 1].position = sf::Vector2f(barWidth * (i + 2), barTop);
        vertices[i * 6 + 2].position = sf::Vector2f(barWidth * (i + 1), bottom);
        vertices[i * 6 + 3].position = sf::Vector2f(barWidth * (i + 1), bottom);
        vertices[i * 6 + 4].position = sf::Vector2f(barWidth * (i + 2), barTop);
        vertices[i * 6 + 5].position = sf::Vector2f(barWidth * (i + 2), bottom);

        vertices[i * 6 + 0].color = color;
        vertices[i * 6 + 1].color = color;
        vertices[i * 6 + 2].color = color;
        vertices[i * 6 + 3].color = color;
        vertices[i * 6 + 4].color = color;
        vertices[i * 6 + 5].color = color;

    }

    return vertices;

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