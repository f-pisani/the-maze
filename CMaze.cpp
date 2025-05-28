#include "CMaze.hpp"

CMaze::CMaze() :
    m_width(0), m_height(0), m_cell()
{

}


CMaze::CMaze(int w, int h) :
    m_width(w), m_height(h), m_cell()
{
    if(w < 2 && h < 2)
        qFatal("Maze size should be equal or higher than 2x2.");

    for(int y=0; y<h; ++y)
    {
        for(int x=0; x<w; ++x)
        {
            SMazeCell cell;
            m_cell[CellID(x, y)] = cell;
        }
    }
}


CMaze::~CMaze()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PUBLIC METHODS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CMaze::CellID(int x, int y)
{
    if(IsInit() && IsValidCoordinate(x, y))
        return (y*m_width+x)+1;

    return 0;
}


SCoordinate CMaze::CellCoordinate(int id)
{
    SCoordinate coordinate;

    if(IsInit() && id >= 1)
    {
        coordinate.x = (id-1)%m_width;
        coordinate.y = (id-1)/m_width;
    }

    return coordinate;
}


bool CMaze::IsInit()
{
    if(m_width >= 2 && m_height >= 2)
        return true;

    return false;
}


bool CMaze::IsValidCoordinate(int x, int y)
{
    if((x >= 0 && y >= 0) && (x <= m_width && y <= m_height))
        return true;

    return false;
}


bool CMaze::CarveLeftWall(int x, int y)
{
    if(IsValidCoordinate(x, y) && x >= 1)
    {
        m_cell[CellID(x, y)].walls.reset(CMaze::WallLeft);
        m_cell[CellID(x-1, y)].walls.reset(CMaze::WallRight);

        return true;
    }

    return false;
}


bool CMaze::CarveRightWall(int x, int y)
{
    if(IsValidCoordinate(x, y) && x < m_width)
    {
        m_cell[CellID(x, y)].walls.reset(CMaze::WallRight);
        m_cell[CellID(x+1, y)].walls.reset(CMaze::WallLeft);

        return true;
    }

    return false;
}


bool CMaze::CarveTopWall(int x, int y)
{
    if(IsValidCoordinate(x, y) && y >= 1)
    {
        m_cell[CellID(x, y)].walls.reset(CMaze::WallTop);
        m_cell[CellID(x, y-1)].walls.reset(CMaze::WallBottom);

        return true;
    }

    return false;
}


bool CMaze::CarveBottomWall(int x, int y)
{
    if(IsValidCoordinate(x, y) && y < m_height)
    {
        m_cell[CellID(x, y)].walls.reset(CMaze::WallBottom);
        m_cell[CellID(x, y+1)].walls.reset(CMaze::WallTop);

        return true;
    }

    return false;
}


SMazeCell CMaze::GetWalls(int x, int y)
{
    SMazeCell defaultCell;
    if(IsInit() && IsValidCoordinate(x, y))
        return m_cell[CellID(x, y)];

    return defaultCell;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// STATIC PUBLIC METHODS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMaze::Export(std::string filename)
{
    std::fstream file(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if(file.is_open())
    {
        // Write Maze Size
        file.write(reinterpret_cast<char*>(&m_width), sizeof(int));
        file.write(reinterpret_cast<char*>(&m_height), sizeof(int));

        // Write Maze Cell
        for(int y=0; y<m_width; ++y)
        {
            for(int x=0; x<m_height; ++x)
            {
                unsigned long walls(GetWalls(x, y).walls.to_ulong());
                file.write(reinterpret_cast<char*>(&walls), sizeof(unsigned long));
            }
        }

        file.close();
        return true;
    }

    return false;
}


bool CMaze::Load(std::string filename)
{
    std::fstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        file.read(reinterpret_cast<char*>(&m_width), sizeof(int));
        file.read(reinterpret_cast<char*>(&m_height), sizeof(int));

        // Read Maze Cell
        m_cell.clear();
        for(int y=0; y<m_height; ++y)
        {
            for(int x=0; x<m_width; ++x)
            {
                unsigned long walls;
                file.read(reinterpret_cast<char*>(&walls), sizeof(unsigned long));
                m_cell[CellID(x,y)].walls = std::bitset<4>(walls);
            }
        }

        file.close();
        return true;
    }

    return false;
}