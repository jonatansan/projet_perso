#include "dungeonGenerator.h"
#include "tileMap.h"

#include <SFML/Graphics/View.hpp>

#define TILE_SIZE 16

#define WINDOW_WIDTH ( TILE_SIZE * 52 )
#define WINDOW_HEIGHT ( TILE_SIZE * 32 )

#define WORLD_WIDTH 80
#define WORLD_HEIGHT 80
#define NB_FEATURE 100

#define ZOOM .5f

 
int main()
{
	std::cout<<"Start"<<std::endl;
	// Génération du donjon
	DungeonGenerator generator(WORLD_WIDTH, WORLD_HEIGHT, NB_FEATURE) ;
 
	auto mapASCII = generator.Generate();
 
	//mapASCII.Print();

    // on crée la fenêtre
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tilemap");

    // on crée la tilemap avec le niveau précédemment défini
    TileMap map;
	if (!map.load("Images/tileset.png", sf::Vector2u(TILE_SIZE, TILE_SIZE), mapASCII, WORLD_WIDTH, WORLD_HEIGHT))
        return -1;

	//on crée la vue
	sf::View view(sf::Vector2f( (WORLD_WIDTH * TILE_SIZE)/2.f, (WORLD_HEIGHT * TILE_SIZE)/2.f), sf::Vector2f(WINDOW_WIDTH,WINDOW_HEIGHT));
	view.zoom(ZOOM);
	window.setView(view);

    // on fait tourner la boucle principale
    while (window.isOpen())
    {
        // on gère les évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && view.getCenter().x >  WINDOW_WIDTH/2 * ZOOM)
				view.move(-TILE_SIZE,0);
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && view.getCenter().x < TILE_SIZE * WORLD_WIDTH - WINDOW_WIDTH/2 * ZOOM)
				view.move(TILE_SIZE,0);
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && view.getCenter().y > WINDOW_HEIGHT/2 * ZOOM )
				view.move(0,-TILE_SIZE);
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && view.getCenter().y < TILE_SIZE * WORLD_HEIGHT - WINDOW_HEIGHT/2 * ZOOM)
				view.move(0,TILE_SIZE);
        }

        // on dessine le niveau
        window.clear();
        window.draw(map);
        window.display();

		window.setView(view);
    }

    return 0;
}

