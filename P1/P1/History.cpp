//
//  History.cpp
//  P1
//
//  Created by Quentin Truong on 1/9/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include "History.h"

#include <iostream>
#include "globals.h"

History::History(int nRows, int nCols){
    m_rows = nRows;
    m_cols = nCols;
    
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_grid[r][c] = 0;
}

bool History::record(int r, int c){
    if (r < 1  ||  c < 1  ||  r > m_rows  ||  c > m_cols)
        return false;
    
    m_grid[r][c] += 1;
    
    return true;
}

void History::display() const{
    clearScreen();
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++){
            if (m_grid[r][c] == 0)
                std::cout << ".";
            else if (m_grid[r][c] <= 26)
                std::cout << char('A' + m_grid[r][c] - 1);
            else
                std::cout << "Z";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
