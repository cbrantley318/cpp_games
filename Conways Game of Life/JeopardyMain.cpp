//#include <iostream>
//#include <vector>
//
//#include <SFML/Graphics.hpp>
//
//#include "Tilemap.cpp"
//
//
///*
//
//	This is NOT the true main file, this is just a consequence of laziness and a recreation of the game show Jeopardy!
//	For the reimplimentation of Conway's Game of Life, please refer to and uncomment CGOLmain.cpp
//
//*/
//
//static int processGame(std::vector<int>*& base, std::vector<int>*& next, int size);
//static bool willSurvive(std::vector<int>* vec, int size, int index);
//static int getNeighborCount(std::vector<int>* vec, int size, int index);
//static int isInbounds(std::vector<int>* vec, int size, int startIndex, int neighborIndex);
//static std::vector<std::string> getAnswers();
//static std::vector<std::string> getQuestions();
//static std::vector<std::string> getMoney();
//
//const int size = 200; //The width and height of the grid. Total number of elements is size*size
//const sf::Time period = sf::milliseconds(1000.f / 60.f);  //60 fps => 1/60 seconds per frame = 1000/60
////const sf::Time period = sf::milliseconds(1000.f / 5.f);  //60 fps => 1/60 seconds per frame = 1000/60
//
//const int tileWidth = 32; //width of each tile in tilemap. Default is 32
//const int tileHeight = 32; //self explanatory
//
//
//int main() {
//
//	std::time_t seed = std::time(0);  // seed is an integer type
//	std::srand(seed);
//
//	int clickTimer = 0; //counter to limit click registers / prevent debouncing
//	int navCounter = 0; //mod3 counter where 0 is main window, 1 is zoomed in on a question, 2 is swap questin w/ answer
//	int targetBox = 0; //if navCounter is not 0, then this is the window we're zoomed in on
//
//
//
//	//Make two vectors of appropriate size, one to represent current grid layout and one to represent future grid layout.
//	//Also make two pointers to swap between the two
//	std::vector<int> vec;
//	std::vector<int> vec2(size * size, 0);
//
//	std::vector<int>* mainArr = &vec;
//	std::vector<int>* backArr = &vec2;
//	//will use pointer swapping to make it basically double-buffered. One array will determine the next state of the other array which is then rendered next cycle
//
//
//	std::vector<std::string> answers = getAnswers(); //both of these should be exactly 25 long, ordered by category first
//	std::vector<std::string> questions = getQuestions();
//	std::vector<std::string> moneys = getMoney();
//
//	std::vector<std::string> categories; //this one just has the category names
//	categories.push_back("Geography");
//	categories.push_back("Holidays");
//	categories.push_back("Food");
//	categories.push_back("Pop Culture");
//	categories.push_back("Misc");
//
//	std::vector<sf::IntRect> rectArr;	//an array of bounding boxes for each tile (approx)
//	for (int i = 0; i < 5; i++) {		//Not a huge fan of modular arithmetic so double nesting it is
//		for (int j = 1; j < 6; j++) {
//			rectArr.push_back(sf::IntRect(1285 * i, 1050 * j, 1285, 1050));
//		}
//	}
//
//
//
//	for (int i = 0; i < size * size; i++) {
//
//		if (i % (size / 5) == 0 || (i/size)%(size/6) == 0 || i%size == size-1 || (i / size) % (size) == size-1) {
//			vec.push_back(1);
//		}
//		else {
//			vec.push_back(0);
//		}
//
//	}
//
//	sf::Font font;
//	if (!font.loadFromFile("fonts/arial.ttf"))
//	{
//		std::cout << "Unable to load font from file" << std::endl;
//		return -1;
//	}
//
//	sf::Text text;
//	text.setFont(font); // font is a sf::Font
//	text.setString("Hello world");
//	text.setCharacterSize(200); // in pixels, not points!
//	text.setFillColor(sf::Color::Yellow);
//	text.setStyle(sf::Text::Bold);
//
//
//
//	//The map is going to be reloaded every frame so keeping the texture loaded constantly
//	sf::Texture mapText;
//	if (!mapText.loadFromFile("tilesets/colors.png")) {
//		std::cout << "Unable to load texture from file" << std::endl;
//		return -1;
//	}
//
//	TileMap map;
//	if (!map.load(&mapText, sf::Vector2u(tileWidth, tileHeight), vec, size, size)) {
//		return -1;
//	}
//
//
//	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
//	//sf::VideoMode desktop = sf::VideoMode(800,600);
//	int screenWidth = desktop.width;
//	int screenHeight = desktop.height;
//	sf::RenderWindow window(desktop, "CarsonTestingSFML", sf::Style::Default);
//
//
//	sf::View view = window.getDefaultView();
//	//view.setCenter(sf::Vector2f(size * 16, size * 16));
//	view.setCenter(sf::Vector2f(size * tileWidth/2, size * tileHeight/2));
//	view.zoom(size/60.0);
//	window.setView(view);
//
//	sf::View smallView = sf::View(sf::FloatRect(0,0,1200,1000));
//
//
//	sf::Clock clock;
//
//
//	//Start event loop
//	while (window.isOpen()) {
//
//
//		clock.restart();
//		clickTimer++;
//
//		sf::Event event;
//
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				window.close();
//			}
//		}
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && clickTimer > 10) {
//			clickTimer = 0;
//			navCounter = 0;
//		}
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && clickTimer >10) { //if they get it right / move to answer display
//			clickTimer = 0;
//			if (navCounter % 3 == 1) {
//				navCounter++;
//			}
//			else if (navCounter % 3 == 2) {
//				moneys[targetBox] = " ";
//			}
//
//		}
//		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickTimer > 10) //going to check if Worldpos is contained in any of the bounding rectangles
//		{
//			clickTimer = 0;
//			//std::cout << "Mouse was clicked at: " << sf::Mouse::getPosition().x << "," << sf::Mouse::getPosition().y << std::endl;
//			sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
//			sf::Vector2f wp1 = window.mapPixelToCoords(pixelPos);
//			sf::Vector2i worldPos = sf::Vector2i(wp1.x, wp1.y);
//			//std::cout << "PixelPos: " << pixelPos.x << " " << pixelPos.y << std::endl;
//			std::cout << "WorldPos: " << worldPos.x << " " << worldPos.y << std::endl;
//			for (int i = 0; i < 25; i++) {
//				if (rectArr[i].contains(worldPos)) {
//					//std::cout << "Located in Rect " << i << std::endl;
//					if (navCounter % 3 == 0) {
//						targetBox = i;
//						navCounter++;
//						smallView.setCenter(sf::Vector2f(rectArr[i].left + rectArr[i].width / 2, rectArr[i].top + rectArr[i].height / 2 + 75));
//					}
//				}
//			}
//			//std::cout << std::endl;
//
//		}
//
//		//sf::Vector2i position = sf::Mouse::getPosition();
//
//			
//
//
//
//
//
//		//processGame(mainArr, backArr, size);
//		map.load(&mapText, sf::Vector2u(tileWidth, tileHeight), *mainArr, size, size);
//
//
//		std::vector<std::string>* printList = &moneys;
//		switch (navCounter % 3) {
//		case 0:
//			printList = &moneys;
//			break;
//		case 1:
//			printList = &questions;
//			break;
//		case 2:
//			printList = &answers;
//			break;
//		default:
//			break;
//		}
//
//
//		if (navCounter % 3 == 0) { //displaying main screen
//			window.setView(view);
//		}
//		else {
//			window.setView(smallView);
//		}
//		window.clear();
//		window.draw(map);
//
//		text.setCharacterSize(200);
//		for (int i = 0; i < 5; i++) { //drawing category titles
//			text.setPosition(sf::Vector2f(1285*i+150, 350));
//			text.setString(categories[i]);
//			window.draw(text);
//		}
//
//		if (navCounter % 3 != 0) {
//		text.setCharacterSize(70);
//		}
//		for (int i = 0; i < 25; i++) { //drawing either money, question, or answer
//			text.setPosition(sf::Vector2f(1285*(i/5)+150, 1050*(i%5)+1400));
//			text.setString((*printList)[i]);
//			window.draw(text);
//		}
//		window.display();
//
//		//if we clicked on a specific box, then create a custom view that's zoomed in soley on that box, ensure that everything else is out I guess
//		//then, if the spacebar is pressed it will change the text to display the question (answer)
//		//from either step, pressing escape will reset the counter to 0
//		//Im thinking a mod3 counter should be fine let's go with that
//		//if instead of esc, you press spacebar on that question, then change the backing vector of text to display from a dollar amount to empty space
//
//
//		sf::Time sleepTime = period - clock.restart();
//		sf::sleep(sleepTime);
//
//	}
//
//
//	return 0;
//
//
//
//}
//
////This method will take references to two vector pointers as input, the current and the next vector, and then loop through base vec, checking if each cell should live, and copying the new val to nextVec
////Then, it will swap the values of the pointers, which hopefully means that the change will propogate back through to main
//static int processGame(std::vector<int>*& base, std::vector<int>*& next, int size) {
//
//	for (int i = 0; i < size * size; i++) {
//		if (willSurvive(base, size, i)) {
//			(*next)[i] = 1;
//			//std::cout << "index " << i << " will survive!" << std::endl;
//		}
//		else {
//			(*next)[i] = 0;
//			//std::cout << "index " << i << " will die" << std::endl;
//		}
//	}
//
//	//std::cout << "value of base and next before swap: " << base << "\t" << next << std::endl;
//	std::vector<int>* temp = base;	//unfortunately it doesn't let me XOR reference types ;(
//	base = next;
//	next = temp;
//	//std::cout << "value of base and next after swap: " << base << "\t" << next << std::endl;
//
//	return 0;
//}
//
////Checks if a given population will survive to the next cycle
//static bool willSurvive(std::vector<int>* vec, int size, int index) {
//	int numNeighbors = getNeighborCount(vec, size, index);
//	//std::cout << "index " << index << " has " << numNeighbors << " neighbors!" << std::endl;
//	if (numNeighbors == 3) {
//		return true;
//	}
//	if (numNeighbors == 2 && (*vec)[index] == 1) {
//		return true;
//	}
//	return false;
//}
//
////Make method to get the total number of alive neighbors
//static int getNeighborCount(std::vector<int>* vec, int size, int index) {
//	//For a general array representing a 2d array, the 8 neighbors (four orthogonal and four diagonal) of a point at index i will be given by: i-1,i+1, i-size, i-size-1, i-size+1, i+size,i+size-1, i+size+1
//	//Basically, instead of using an enum or something like that I'm just writing a function that'll go through each of those
//	//Bounds checking will be handled by isInBounds
//	int ret = 0;
//	//Prepare yourself for some of the ugliest code I've ever written
//	ret += isInbounds(vec, size, index, index - 1);
//	ret += isInbounds(vec, size, index, index + 1);
//	ret += isInbounds(vec, size, index, index - size);
//	ret += isInbounds(vec, size, index, index - size - 1);
//	ret += isInbounds(vec, size, index, index - size + 1);
//	ret += isInbounds(vec, size, index, index + size);
//	ret += isInbounds(vec, size, index, index + size - 1);
//	ret += isInbounds(vec, size, index, index + size + 1);
//	return ret;
//
//}
//
////this is to check for issues where the point of interest is on an edge and a neighbor is out of bounds
////returns the value of neighbor (i.e. the alive status) if the neighbor is in the bounds, 0 if not
////originally, I circumvented this issue by just adding a border of 0's to the array (so a 10x10 grid would really be 12x12, with the outer border constantly being dead),
//// but this time I'm choosing to do it this way
//static int isInbounds(std::vector<int>* vec, int size, int startIndex, int neighborIndex) { 
//
//
//	//so lets say we have a 4x4:				
//	/*
//					_	_	_	_
//					_	_	_	_
//					_	_	_	_
//					_	_	_	_
//
//		Anything in spots 0, 1, 2, 3 has to be checked for if the neighbor index is <0
//		Anything in spots 12,13,14,15 needs to be checked if index>11
//		Anything in 0,4,8,12 needs to check if neighbor < currDex (wrapped around to the right side)
//		Anything in 3,7,11,15 needs to check if neighborDex > curDex (wrapped forward to the left side)
//		The above is redundant for 0 and 15 but that doesn't matter
//
//	*/
//
//	//this handles the top and bottom rows nicely
//	if (neighborIndex < 0)
//		return 0;
//	if (neighborIndex >= size*size)
//		return 0;
//
//
//
//	//checking if to the left of first column
//	if (startIndex % size == 0) {
//		if (neighborIndex < startIndex)
//			return 0;
//	}
//
//	//checking if to the right of last column
//	if (startIndex % size == size - 1) {
//		if (neighborIndex > startIndex)
//			return 0;
//	}
//
//	return (*vec)[neighborIndex];
//
//}
//
//
//static std::vector<std::string> getAnswers() {
//	std::vector<std::string> ret;
//	ret.push_back("What is 50?");
//	ret.push_back("What is California?");
//	ret.push_back("What is Alaska?");
//	ret.push_back("What is 271?");
//	ret.push_back("What is the \nMissouri River?"); //last geography one
//	ret.push_back("Who is Santa?");
//	ret.push_back("What is Halloween?");
//	ret.push_back("What is Thanksgiving?");
//	ret.push_back("What is Independence Day\n(Fourth of July)?");
//	ret.push_back("What is Martin\nLuther King Jr. Day?"); // last holiday
//	ret.push_back("What is Basketball?");
//	ret.push_back("What is Football?");
//	ret.push_back("What are Hot Dogs?");
//	ret.push_back("What is Hockey?");
//	ret.push_back("What is Swimming?"); // last food
//	ret.push_back("Who is Taylor Swift?");
//	ret.push_back("What is Spongebob Squarepants?");
//	ret.push_back("What is Disney?");
//	ret.push_back("Who was Michael Jackson?"); // last pop cult
//	ret.push_back("What are Barbie\n and Oppenheimer?");
//	ret.push_back("Who is Joseph Biden?");
//	ret.push_back("What is McDonald's?");
//	ret.push_back("What is Hollywood?");
//	ret.push_back("What is there is\nno official language?");
//	ret.push_back("Who is \nColonel Sanders?"); // last misc
//	return ret;
//}
//static std::vector<std::string> getQuestions() {
//	std::vector<std::string> ret;
//	ret.push_back("This is the number of\nstates in the US");
//	ret.push_back("This is the state with the \nhighest population");
//	ret.push_back("This is the \nLargest US State");
//	ret.push_back("This many Taiwans can fit\ninside the USA");
//	ret.push_back("This is the longest river\nin the US"); //last geography one
//	ret.push_back("This person gives gifts from \nthe North Pole, saying \n\"Ho Ho Ho\" as he\nrides his sleigh\npulled by reindeer");
//	ret.push_back("During this US holiday,\nchildren dress up in costumes\nand ask adults for candy");
//	ret.push_back("This US holiday is known as\na time for appreciating your\nfamily members and \neating turkey");
//	ret.push_back("This summer holiday is to\ncelebrate the birthday of the\nUnited States, where people\noften eat BBQ\nand launch fireworks");
//	ret.push_back("This holiday is to celebrate the\nleader of the\nUS Civil Rights movement"); // last holiday
//	ret.push_back("Steph Curry plays this sport");
//	ret.push_back("The most popular US sport");
//	ret.push_back("The most popular food to\neat at a baseball game");
//	ret.push_back("This icy sport is played\nwith curved sticks and a \"puck\"");
//	ret.push_back("Michael Phelps and\nKatie Ledecky are\nconsidered the two best\nathletes in this\nOlympic sport"); // last food
//	ret.push_back("This is the most\npopular US singer");
//	ret.push_back("This cartoon takes place\nin Bikini Bottom and\nfeatures characters such as\nPatrick Star, Mr. Krabs,\nSquidward, and Plankton"); 
//	ret.push_back("This company operates six\nresorts around the world\nand is home to famous characters\nsuch as Mickey Mouse,\nTinker Bell, and Simba");
//	ret.push_back("This singer was widely known\nas the\"King of Pop\"\nafter releasing\nalbums including \"Thriller\"\nand \"Off the Wall\""); // last pop cult
//	ret.push_back("These two movies were released\non the same day last summer:\none about a plastic doll\nand the other about\nthe Manhattan Project");
//	ret.push_back("The current President of\nthe United States of America");
//	ret.push_back("This company is America's\nlargest and most well-known\nfast-food chain");
//	ret.push_back("This Los Angeles neighborhood\nis known for\nmovies and filmmaking\nand its name is written\non a famous sign");
//	ret.push_back("This is the official\nlanguage of the USA");
//	ret.push_back("This is the founder and mascot\nof KFC (Kentucky Fried Chicken)"); // last misc
//	return ret;
//}
//static std::vector<std::string> getMoney() { //I wish...
//	std::vector<std::string> ret;
//	ret.push_back("100");
//	ret.push_back("200");
//	ret.push_back("300");
//	ret.push_back("400");
//	ret.push_back("500");	
//	ret.push_back("100");
//	ret.push_back("200");
//	ret.push_back("300");
//	ret.push_back("400");
//	ret.push_back("500");	
//	ret.push_back("100");
//	ret.push_back("200");
//	ret.push_back("300");
//	ret.push_back("400");
//	ret.push_back("500");
//	ret.push_back("100");
//	ret.push_back("200");
//	ret.push_back("300");
//	ret.push_back("400");
//	ret.push_back("500");
//	ret.push_back("100");
//	ret.push_back("200");
//	ret.push_back("300");
//	ret.push_back("400");
//	ret.push_back("500");
//	return ret;
//}