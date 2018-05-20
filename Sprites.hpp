//
//  Sprites.hpp
//  Snake
//
//  Created by X on 16.05.18.
//

#ifndef Sprites_hpp
#define Sprites_hpp

#include "SDL2/SDL.h"
#include <vector>

enum class Direction { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };


//-------------------- Snake class

class Snake {
public:
    Snake(SDL_Point playground_dimensions,
          int tile_size,
          Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    ~Snake();
    void SetNewDirection(Direction direction);
    int Update(SDL_Point pellet_top_left);
    void BlitOnPlayground(SDL_Surface *playground_surface);
    bool ColidesWithSnake(SDL_Point position);
    bool SelfColision();
private:
    std::vector<SDL_Point> mTopLefts;
    Direction mDirection;
    int mTileSize;
    SDL_Point mPlaygroundDims;
    SDL_Surface *mHeadSurface;
    SDL_Surface *mBodySurface;
    SDL_Surface *mTailSurface;
};



//-------------------- Pellet class

class Pellet {
public:
    Pellet(SDL_Point playground_dimensions,
           int tile_size,
           Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    ~Pellet();
    void MoveRandomly();
    SDL_Point GetTopLeft();
    void BlitOnPlayground(SDL_Surface *playground_surface);
private:
    SDL_Point mTopLeft;
    int mTileSize;
    SDL_Point mPlaygroundDims;
    SDL_Surface *mSurface;
};

#endif /* Sprites_hpp */
