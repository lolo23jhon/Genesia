#include <iostream>
#include "Engine.h"

int main() {

	Engine engine{ sf::Vector2u(1080,1080),"Test" };
	engine.run();

#ifdef _DEBUG
	std::cout << "> Exited at main" << std::endl;
#endif // _DEBUG

	return 0;
}