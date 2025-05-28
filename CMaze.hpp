#ifndef CMAZE_HPP_INCLUDED
#define CMAZE_HPP_INCLUDED

#include <QDebug>

#include <iostream>
#include <fstream>
#include <bitset>
#include <unordered_map>

struct SMazeCell
{
    SMazeCell() : walls()
    {
        walls.set();
    }

    std::bitset<4> walls;
};

struct SCoordinate
{
    SCoordinate() : x(-1), y(-1){}
    SCoordinate(int X, int Y) : x(X), y(Y){}
    bool operator==(const SCoordinate& c)
    {
        if(c.x == x && c.y == y)
            return true;
        else
            return false;
    }

    int distance(const SCoordinate& c)
    {
        return std::abs((x-c.x)+(y-c.y));
    }

    int x, y;
};

class CMaze
{
public:
    CMaze();
    CMaze(int w, int h);
    ~CMaze();

    int CellID(int x, int y);
    SCoordinate CellCoordinate(int id);
    bool IsInit();
    bool IsValidCoordinate(int x, int y);
    bool CarveLeftWall(int x, int y);
    bool CarveRightWall(int x, int y);
    bool CarveTopWall(int x, int y);
    bool CarveBottomWall(int x, int y);

    SMazeCell GetWalls(int x, int y);

    inline int Width(){ return m_width; }
    inline int Height(){ return m_height; }

    bool Export(std::string filename);
    bool Load(std::string filename);
    static const int WallLeft = 0;
    static const int WallRight = 1;
    static const int WallTop = 2;
    static const int WallBottom = 3;

private:
    int m_width, m_height;
    std::unordered_map<int, SMazeCell> m_cell;
};

#endif // CMAZE_HPP_INCLUDED