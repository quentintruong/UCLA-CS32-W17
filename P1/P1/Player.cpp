//
//  Player.cpp
//  P1
//
//  Created by Quentin Truong on 1/9/17.
//  Copyright © 2017 QT. All rights reserved.
//

#include "Player.h"

#include <iostream>
#include "Arena.h"
#include "globals.h"

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        std::cout << "***** The player must be created in some Arena!" << std::endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        std::cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << std::endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

std::string Player::dropPoisonPellet()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poison pellet at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poison pellet has been dropped.";
}

std::string Player::move(int dir)
{
    if (attemptMove(*m_arena, dir, m_row, m_col))
    {
        if (m_arena->numberOfRatsAt(m_row, m_col) > 0)
        {
            setDead();
            return "Player walked into a rat and died.";
        }
        std::string msg = "Player moved ";
        switch (dir)
        {
            case NORTH: msg += "north"; break;
            case EAST:  msg += "east";  break;
            case SOUTH: msg += "south"; break;
            case WEST:  msg += "west";  break;
        }
        msg += ".";
        return msg;
    }
    else
        return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}
