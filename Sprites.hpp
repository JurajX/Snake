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
#include <map>
#include <string>


//-------------------- Helper structures


// Note that (in the integer representation) neither direction nor sum of any two directions
// are equal. Keep this in mind, it will become useful in Snake::LoadImages().
enum class Direction :int { UP=1, DOWN=2, RIGHT=4, LEFT=7 };

    
struct Part {
    SDL_Point pt;
    Direction front;
    int FrontToInt() const;
    int BackToInt() const;
};



//-------------------- Snake class

class Snake {
public:
    Snake(SDL_Point playground_dimensions,
          int tile_size,
          Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    Snake(const Snake&) = delete;
    Snake& operator=(const Snake&) = delete;
    Snake(Snake&&) = delete;
    Snake& operator=(Snake&&) = delete;
    ~Snake();
    
    void LoadImages();
    void SetNewDirection(Direction direction);
    int Update(SDL_Point pellet_top_left);
    void BlitOnPlayground(SDL_Surface *playground_surface);
    bool ColidesWithPoint(SDL_Point position) const;
    bool SelfColision() const;
private:
    std::map<int, SDL_Surface*> mHeadSurfaces;
    std::map<int, SDL_Surface*> mBodySurfaces;
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
    Pellet(const Pellet&) = delete;
    Pellet& operator=(const Pellet&) = delete;
    Pellet(Pellet&&) = delete;
    Pellet& operator=(Pellet&&) = delete;
    ~Pellet();
    
    void LoadImages();
    void MoveRandomly();
    SDL_Point GetTopLeft() const;
    void BlitOnPlayground(SDL_Surface *playground_surface) const;
private:
    SDL_Point mTopLeft;
    int mTileSize;
    SDL_Point mPlaygroundDims;
    std::vector<SDL_Surface*> mSurfaces;
    int mSurfaceIndex;
};


#endif /* Sprites_hpp */
