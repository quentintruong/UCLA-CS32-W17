//
//  History.h
//  P1
//
//  Created by Quentin Truong on 1/9/17.
//  Copyright © 2017 QT. All rights reserved.
//

#ifndef History_h
#define History_h

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_grid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
};

#endif /* History_h */
