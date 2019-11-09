#include <SFML/Graphics.hpp>
#include "Sources\Graphics\Game.h"

#include <memory>

int main()
{
	constexpr float deltaTime = 1.0f / 60.0f;
	auto window				  = new sf::RenderWindow{ sf::VideoMode( 800, 600 ), "SFML works!" };

	SteeringBehaviors::Graphics::Game game;

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

		game.update( deltaTime );
		game.render( window );
	}

	return 0;
}
