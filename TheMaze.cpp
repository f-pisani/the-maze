#include "TheMaze.hpp"

TheMaze::TheMaze() :
    sf::RenderWindow(sf::VideoMode::getDesktopMode(), "TheMaze", sf::Style::Fullscreen, sf::ContextSettings(4, 5, 16, 1, 1)),
    m_gameView(getDefaultView()), m_minimap(), m_maze(), m_playerCoordinate(0, 0), m_visitedCells()
{
    setFramerateLimit(60);
    m_gameView.zoom(0.2);
    m_minimap.create(getSize().x/TheMaze::minimap_scale,
                     getSize().y/TheMaze::minimap_scale);

    m_maze.Load("default.maze");
    m_visitedCells.push_back(m_playerCoordinate);
}


TheMaze::~TheMaze()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PUBLICS METHODS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMaze::update()
{
    // Events loop
    sf::Event event;
    while(pollEvent(event))
    {
        // Players movements
        if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
                // Goes up
                case sf::Keyboard::Up:
                    if(m_playerCoordinate.y-1 >= 0 &&
                       !m_maze.GetWalls(m_playerCoordinate.x, m_playerCoordinate.y).walls.test(CMaze::WallTop))
                    {
                        m_playerCoordinate.y -= 1;
                    }
                break;

                // Goes down
                case sf::Keyboard::Down:
                    if(m_playerCoordinate.y+1 <= m_maze.Height() &&
                       !m_maze.GetWalls(m_playerCoordinate.x, m_playerCoordinate.y).walls.test(CMaze::WallBottom))
                    {
                        m_playerCoordinate.y += 1;
                    }
                break;

                // Goes left
                case sf::Keyboard::Left:
                    if(m_playerCoordinate.x-1 >= 0 &&
                       !m_maze.GetWalls(m_playerCoordinate.x, m_playerCoordinate.y).walls.test(CMaze::WallLeft))
                    {
                        m_playerCoordinate.x -= 1;
                    }
                break;

                // Goes right
                case sf::Keyboard::Right:
                    if(m_playerCoordinate.x+1 <= m_maze.Width() &&
                       !m_maze.GetWalls(m_playerCoordinate.x, m_playerCoordinate.y).walls.test(CMaze::WallRight))
                    {
                        m_playerCoordinate.x += 1;
                    }
                break;
            }

            if(!std::count(m_visitedCells.begin(), m_visitedCells.end(), m_playerCoordinate))
                m_visitedCells.push_back(m_playerCoordinate);
        }

        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
            close();

        if(event.type == sf::Event::Closed)
            close();
    }

    // Physics

    // Sounds

    // Graphics
    clear(sf::Color::Black);
        DrawMazeVertex(); // Draw Maze (Debug)
        DrawPlayer(); // Draw Player (Debug)
        BreathingView(); // Camera lerp
        UpdateMiniMap();
    display();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PRIVATE METHODS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMaze::BreathingView()
{
    static float delta = 0.f;

    m_gameView.move(cos(delta)*1.2, sin(delta)*1.2);

    delta += 0.016666666f;
    if(delta > 6.28)
        delta = -6.28;
}


void TheMaze::DrawMazeVertex()
{
    setView(m_gameView);
    for(int i=0; i<m_visitedCells.size(); ++i)
    {
        int x(m_visitedCells[i].x), y(m_visitedCells[i].y);

        SMazeCell cell(m_maze.GetWalls(x, y));
        sf::VertexArray vertex(sf::PrimitiveType::Lines, 2);
        vertex[0].color = sf::Color::Yellow;
        vertex[1].color = sf::Color::Yellow;

        if(cell.walls.test(CMaze::WallLeft))
        {
            vertex[0].position = sf::Vector2f(x*TheMaze::cell_size,
                                              y*TheMaze::cell_size);
            vertex[1].position = sf::Vector2f(x*TheMaze::cell_size,
                                              y*TheMaze::cell_size+TheMaze::cell_size);
            draw(vertex);
        }

        if(cell.walls.test(CMaze::WallRight))
        {
            vertex[0].position = sf::Vector2f(x*TheMaze::cell_size+TheMaze::cell_size,
                                              y*TheMaze::cell_size);
            vertex[1].position = sf::Vector2f(x*TheMaze::cell_size+TheMaze::cell_size,
                                              y*TheMaze::cell_size+TheMaze::cell_size);
            draw(vertex);
        }

        if(cell.walls.test(CMaze::WallTop))
        {
            vertex[0].position = sf::Vector2f(x*TheMaze::cell_size,
                                              y*TheMaze::cell_size);
            vertex[1].position = sf::Vector2f(x*TheMaze::cell_size+TheMaze::cell_size,
                                              y*TheMaze::cell_size);
            draw(vertex);
        }

        if(cell.walls.test(CMaze::WallBottom))
        {
            vertex[0].position = sf::Vector2f(x*TheMaze::cell_size,
                                              y*TheMaze::cell_size+TheMaze::cell_size);
            vertex[1].position = sf::Vector2f(x*TheMaze::cell_size+TheMaze::cell_size,
                                              y*TheMaze::cell_size+TheMaze::cell_size);
            draw(vertex);
        }
    }
}


void TheMaze::DrawPlayer()
{
    setView(m_gameView);
    sf::CircleShape player(cell_size/4, 128);
    player.setOrigin(sf::Vector2f(player.getLocalBounds().width/2, player.getGlobalBounds().height/2));
    player.setPosition(sf::Vector2f(m_playerCoordinate.x*TheMaze::cell_size+(TheMaze::cell_size/2),
                                    m_playerCoordinate.y*TheMaze::cell_size+(TheMaze::cell_size/2)));
    player.setFillColor(sf::Color::Yellow);

    draw(player);

    m_gameView.setCenter(player.getPosition().x, player.getPosition().y);
}


void TheMaze::UpdateMiniMap()
{
    setView(getDefaultView());
    // Draw minimap
    m_minimap.clear(sf::Color::Black);
    sf::CircleShape player(cell_size/8/4, 128);
    player.setOrigin(sf::Vector2f(player.getLocalBounds().width/2, player.getGlobalBounds().height/2));
    player.setPosition(sf::Vector2f(m_playerCoordinate.x*(TheMaze::cell_size/8)+((TheMaze::cell_size/8)/2),
                                    m_playerCoordinate.y*(TheMaze::cell_size/8)+((TheMaze::cell_size/8)/2)));
    player.setFillColor(sf::Color::Yellow);
    sf::View view(m_minimap.getDefaultView());
    view.setCenter(player.getPosition().x, player.getPosition().y);
    m_minimap.setView(view);
    m_minimap.draw(player);
    for(int i=0; i<m_visitedCells.size(); ++i)
    {
        int x(m_visitedCells[i].x), y(m_visitedCells[i].y);

        SMazeCell cell(m_maze.GetWalls(x, y));
        sf::VertexArray vertex(sf::PrimitiveType::Lines, 2);
        vertex[0].color = sf::Color::Yellow;
        vertex[1].color = sf::Color::Yellow;

        if(cell.walls.test(CMaze::WallLeft))
        {
            vertex[0].position = sf::Vector2f(x*(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8));
            vertex[1].position = sf::Vector2f(x*(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8)+(TheMaze::cell_size/8));
            m_minimap.draw(vertex);
        }

        if(cell.walls.test(CMaze::WallRight))
        {
            vertex[0].position = sf::Vector2f(x*(TheMaze::cell_size/8)+(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8));
            vertex[1].position = sf::Vector2f(x*(TheMaze::cell_size/8)+(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8)+(TheMaze::cell_size/8));
            m_minimap.draw(vertex);
        }

        if(cell.walls.test(CMaze::WallTop))
        {
            vertex[0].position = sf::Vector2f(x*(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8));
            vertex[1].position = sf::Vector2f(x*(TheMaze::cell_size/8)+(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8));
            m_minimap.draw(vertex);
        }

        if(cell.walls.test(CMaze::WallBottom))
        {
            vertex[0].position = sf::Vector2f(x*(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8)+(TheMaze::cell_size/8));
            vertex[1].position = sf::Vector2f(x*(TheMaze::cell_size/8)+(TheMaze::cell_size/8),
                                              y*(TheMaze::cell_size/8)+(TheMaze::cell_size/8));
            m_minimap.draw(vertex);
        }
    }

    // Show minimap
    sf::RectangleShape minimapBorder(sf::Vector2f(m_minimap.getSize().x, m_minimap.getSize().y));
    minimapBorder.setOutlineThickness(2.f);
    minimapBorder.setOutlineColor(sf::Color::White);
    minimapBorder.setOrigin(minimapBorder.getSize().x, 0);
    minimapBorder.setPosition(getSize().x-16, 16);
    draw(minimapBorder);

    m_minimap.display();
    sf::Sprite minimap(m_minimap.getTexture());
    minimap.setOrigin(minimap.getGlobalBounds().width, 0);
    minimap.setPosition(minimapBorder.getPosition().x, minimapBorder.getPosition().y);
    draw(minimap);

}
