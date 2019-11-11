#include <SFML/Graphics.hpp>
#include "Sources\Graphics\Game.h"

#include <memory>

constexpr float deltaTime = 1.0f / 60.0f;

int main()
{
	auto window = new sf::RenderWindow{ sf::VideoMode( 1920, 1080 ), "SFML works!" };

	SteeringBehaviors::Graphics::Game game{ window };

	while( window->isOpen() )
	{
		sf::Event event;
		while( window->pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
				window->close();
			else
			{
				game.processEvents( event );
			}
		}

		game.processInput();
		game.update();
		game.step( deltaTime );
		game.render( window );
	}

	return 0;
}
