#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Tilemap.cpp"


/*
	A recreation of a project that I made in Java, Python, and possibly C? (Can't remember if I figured out how to draw or not, but probably not) that was
	unfortunately lost to the sands of time (i.e. my computer's hard drive was wiped while the touchpad was being repaired.
	Now it's in C++, so hopefully it's able to run faster / larger grids than the java and python counterparts

*/

static int processGame(std::vector<int>*& base, std::vector<int>*& next, int size);
static bool willSurvive(std::vector<int>* vec, int size, int index);
static int getNeighborCount(std::vector<int>* vec, int size, int index);
static int isInbounds(std::vector<int>* vec, int size, int startIndex, int neighborIndex);

const int size = 200; //The width and height of the grid. Total number of elements is size*size
const sf::Time period = sf::milliseconds(1000.f / 60.f);  //60 fps => 1/60 seconds per frame = 1000/60 ms
//const sf::Time period = sf::milliseconds(1000.f / 5.f);  //5 fps => 1/5 seconds per frame = 1000/5


int main() {

	std::time_t seed = std::time(0);  // seed is an integer type
	std::srand(seed);


	//Make two vectors of appropriate size, one to represent current grid layout and one to represent future grid layout.
	//Also make two pointers to swap between the two
	std::vector<int> vec;
	std::vector<int> vec2(size * size, 0);

	std::vector<int>* mainArr = &vec;
	std::vector<int>* backArr = &vec2;
	//will use pointer swapping to make it basically double-buffered. One array will determine the next state of the other array which is then rendered next cycle

	for (int i = 0; i < size * size; i++) {

		if (std::rand() % 10 < 2) {
			vec.push_back(1);
		}
		else {
			vec.push_back(0);
		}
	}


	//The map is going to be reloaded every frame so keeping the texture loaded constantly
	sf::Texture mapText;
	if (!mapText.loadFromFile("tilesets/colors.png")) {
		std::cout << "Unable to load texture from file" << std::endl;
		return -1;
	}

	TileMap map;
	if (!map.load(&mapText, sf::Vector2u(32, 32), vec, size, size)) {
		return -1;
	}


	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	int screenWidth = desktop.width;
	int screenHeight = desktop.height;
	sf::RenderWindow window(desktop, "CarsonTestingSFML", sf::Style::Default);


	sf::View view = window.getDefaultView();
	view.setCenter(sf::Vector2f(size * 16, size * 16));
	view.zoom(size / 50);
	window.setView(view);


	sf::Clock clock;


	//Start event loop
	while (window.isOpen()) {

		//Just a fun little thing to allow stepping through the program
		//while (true) {
		//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		//		break;
		//}



		clock.restart();

		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) continue; //this lets us "pause" the simulation, will probably change to a toggle if SFML has its own button debouncing 
		//note: that probably means implementing the above as an event which is fine, gotta read the docs later


		//Enter the game logic:
		//std::cout << "value of base and next before function call: " << mainArr << "\t" << backArr << std::endl;

		processGame(mainArr, backArr, size);

		map.load(&mapText, sf::Vector2u(32, 32), *mainArr, size, size);

		//std::cout << "value of base and next after function call: " << mainArr << "\t" << backArr << std::endl;



		window.clear();
		window.draw(map);
		window.display();

		sf::Time sleepTime = period - clock.restart();
		sf::sleep(sleepTime);

	}


	return 0;



}

//This method will take references to two vector pointers as input, the current and the next vector, and then loop through base vec, checking if each cell should live, and copying the new val to nextVec
//Then, it will swap the values of the pointers, which hopefully means that the change will propogate back through to main
static int processGame(std::vector<int>*& base, std::vector<int>*& next, int size) {

	for (int i = 0; i < size * size; i++) {
		if (willSurvive(base, size, i)) {
			(*next)[i] = 1;
			//std::cout << "index " << i << " will survive!" << std::endl;
		}
		else {
			(*next)[i] = 0;
			//std::cout << "index " << i << " will die" << std::endl;
		}
	}

	//std::cout << "value of base and next before swap: " << base << "\t" << next << std::endl;
	std::vector<int>* temp = base;	//unfortunately it doesn't let me XOR reference types ;(
	base = next;
	next = temp;
	//std::cout << "value of base and next after swap: " << base << "\t" << next << std::endl;

	return 0;
}

//Checks if a given population will survive to the next cycle
static bool willSurvive(std::vector<int>* vec, int size, int index) {
	int numNeighbors = getNeighborCount(vec, size, index);
	//std::cout << "index " << index << " has " << numNeighbors << " neighbors!" << std::endl;
	if (numNeighbors == 3) {
		return true;
	}
	if (numNeighbors == 2 && (*vec)[index] == 1) {
		return true;
	}
	return false;
}

//Make method to get the total number of alive neighbors
static int getNeighborCount(std::vector<int>* vec, int size, int index) {
	//For a general array representing a 2d array, the 8 neighbors (four orthogonal and four diagonal) of a point at index i will be given by: i-1,i+1, i-size, i-size-1, i-size+1, i+size,i+size-1, i+size+1
	//Basically, instead of using an enum or something like that I'm just writing a function that'll go through each of those
	//Bounds checking will be handled by isInBounds
	int ret = 0;
	//Prepare yourself for some of the ugliest code I've ever written
	ret += isInbounds(vec, size, index, index - 1);
	ret += isInbounds(vec, size, index, index + 1);
	ret += isInbounds(vec, size, index, index - size);
	ret += isInbounds(vec, size, index, index - size - 1);
	ret += isInbounds(vec, size, index, index - size + 1);
	ret += isInbounds(vec, size, index, index + size);
	ret += isInbounds(vec, size, index, index + size - 1);
	ret += isInbounds(vec, size, index, index + size + 1);
	return ret;

}

//this is to check for issues where the point of interest is on an edge and a neighbor is out of bounds
//returns the value of neighbor (i.e. the alive status) if the neighbor is in the bounds, 0 if not
//originally, I circumvented this issue by just adding a border of 0's to the array (so a 10x10 grid would really be 12x12, with the outer border constantly being dead),
// but this time I'm choosing to do it this way
static int isInbounds(std::vector<int>* vec, int size, int startIndex, int neighborIndex) {


	//so lets say we have a 4x4:				
	/*
					_	_	_	_
					_	_	_	_
					_	_	_	_
					_	_	_	_

		Anything in spots 0, 1, 2, 3 has to be checked for if the neighbor index is <0
		Anything in spots 12,13,14,15 needs to be checked if index>11
		Anything in 0,4,8,12 needs to check if neighbor < currDex (wrapped around to the right side)
		Anything in 3,7,11,15 needs to check if neighborDex > curDex (wrapped forward to the left side)
		The above is redundant for 0 and 15 but that doesn't matter

	*/

	//this handles the top and bottom rows nicely
	if (neighborIndex < 0)
		return 0;
	if (neighborIndex >= size * size)
		return 0;



	//checking if to the left of first column
	if (startIndex % size == 0) {
		if (neighborIndex < startIndex)
			return 0;
	}

	//checking if to the right of last column
	if (startIndex % size == size - 1) {
		if (neighborIndex > startIndex)
			return 0;
	}

	return (*vec)[neighborIndex];

}