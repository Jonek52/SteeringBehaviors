#include <SFML\Graphics.hpp>
#include "Sources\Graphics\Game.h"

#include <memory>

float deltaTime{ 1.f / 60.f };

int main()
{
	auto window = new sf::RenderWindow{ sf::VideoMode( 800, 600 ), "SFML works!" };
	window->setVerticalSyncEnabled( true );

	auto start		  = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();

	SteeringBehaviors::Graphics::Game game{ window };

	while( window->isOpen() )
	{
		start = current_time;
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
		game.update( deltaTime );
		game.render( window );

		current_time								  = std::chrono::high_resolution_clock::now();
		std::chrono::duration< float, std::milli > elapsed_ms = current_time - start;
		deltaTime									  = elapsed_ms.count() / 1000.f;
	}

	return 0;
}
