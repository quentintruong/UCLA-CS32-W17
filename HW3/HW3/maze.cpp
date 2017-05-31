//
//  maze.cpp
//  HW3
//
//  Created by Quentin Truong on 2/12/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include <iostream>
#include <cassert>
using namespace std;


// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    if (maze[sr - 1][sc] == '.'){
        maze[sr - 1][sc] = '+';
        if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
            return true;
    }
    if (maze[sr][sc + 1] == '.'){
        maze[sr][sc + 1] = '+';
        if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    }
    if (maze[sr + 1][sc] == '.'){
        maze[sr + 1][sc] = '+';
        if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
            return true;
    }
    if (maze[sr][sc - 1] == '.'){
        maze[sr][sc - 1] = '+';
        if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    }
    return false;
}
/*
int main()
{
    string maze1[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    string maze2[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    string maze3[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    string maze4[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    assert(pathExists(maze1, 10,10, 6,4, 1,8) == true);
    assert(pathExists(maze2, 10,10, 6,4, 8,1) == false);
    assert(pathExists(maze3, 10,10, 8,1, 1,8) == false);
    assert(pathExists(maze4, 10,10, 8,8, 1,1) == true);
    
    std::cout<< "Pass" << std::endl;
}*/



