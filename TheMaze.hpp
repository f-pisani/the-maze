#ifndef THEMAZE_HPP_INCLUDED
#define THEMAZE_HPP_INCLUDED

#include <QDebug>
#include <cmath>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "CMaze.hpp"

class TheMaze : public sf::RenderWindow
{
    public:
        TheMaze();
        ~TheMaze();
        void update();

    private:
        static const int cell_size = 64;
        static const int minimap_scale = 8;
        sf::View m_gameView;
        sf::RenderTexture m_minimap;

        CMaze m_maze;
        std::vector<SCoordinate> m_visitedCells;
        SCoordinate m_playerCoordinate;

        void BreathingView();
        void DrawMazeVertex();
        void DrawPlayer();
        void UpdateMiniMap();
};

#endif // THEMAZE_HPP_INCLUDED
