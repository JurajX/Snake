//
//  Sprites.hpp
//  Snake
//
//  Created by X on 16.05.18.
//

#ifndef Sprites_hpp
#define Sprites_hpp

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <unordered_map>
#include <string>

enum class Direction :int { UP=1, DOWN=2, RIGHT=4, LEFT=7 };

struct Part {
    SDL_Point pt;
    Direction front;
    int FrontInt();
    int BackInt();
};

//-------------------- Snake class

class Snake {
public:
    Snake(SDL_Point playground_dimensions,
          int tile_size,
          Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    void FreeSurfaces();
    void SetNewDirection(Direction direction);
    int Update(SDL_Point pellet_top_left);
    void BlitOnPlayground(SDL_Surface *playground_surface);
    bool ColidesWithSnake(SDL_Point position);
    bool SelfColision();
private:
    //std::vector<SDL_Point> mTopLefts;
    std::unordered_map<int, SDL_Surface*> mHeadSurfaces;
    std::unordered_map<int, SDL_Surface*> mBodySurfaces;
    std::vector<Part> mBodyParts;
    Direction mDirection;
    SDL_Point mPlaygroundDims;
    int mTileSize;
};



//-------------------- Pellet class

class Pellet {
public:
    Pellet(SDL_Point playground_dimensions,
           int tile_size,
           Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    void FreeSurfaces();
    void MoveRandomly();
    SDL_Point GetTopLeft();
    void BlitOnPlayground(SDL_Surface *playground_surface);
private:
    SDL_Point mTopLeft;
    int mTileSize;
    SDL_Point mPlaygroundDims;
    std::vector<SDL_Surface*> mSurfaces;
    int mSurfaceIndex;
};

#endif /* Sprites_hpp */
