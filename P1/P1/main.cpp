
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>

#include "Rat.h"
#include "Player.h"
#include "Arena.h"
#include "Game.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(3, 5, 2);
    
    // Play the game
    g.play();
}
