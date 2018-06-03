//
//  main.cpp
//  Snake
//
//  Created by X on 15.05.18.
//

#include <cstdlib>
#include <ctime>
#include "Game.hpp"


int main(int argc, const char * argv[]) {
    std::srand(std::time(nullptr));

    //-------------------- Dimensions of window parts;
    SDL_Point PLAYGROUND_TOP_LEFT = {10, 60};
    SDL_Point PLAYGROUND_DIMS = {600, 400};
    SDL_Point SCORE_TOP_LEFT = {0, 0};
    SDL_Point SCORE_DIMS = {620, 50};
    int TILE_SIZE = 20;

    Game game(PLAYGROUND_TOP_LEFT, PLAYGROUND_DIMS, SCORE_TOP_LEFT, SCORE_DIMS, TILE_SIZE);
    bool initialised = game.Initialise();
    if (initialised) {game.RunLoop();}
    game.Shoutdown();

    return 0;
}
