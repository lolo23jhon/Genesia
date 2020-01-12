#include <iostream>
#include "Engine.h"




/*

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
TODO:
	- Finish the trait callback system <-
	- Finish the trait inhertiance system <-
	- Implement food generation
	- Implement energy system
	- Tweak inheritance system to create engaging narrative
	- Flesh out test cases
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/



int main() {

	Engine engine{ sf::Vector2u(800,600),"Test" };
	engine.run();

#ifdef _DEBUG
	std::cout << "> Exited at main" << std::endl;
#endif // _DEBUG

	return 0;
}