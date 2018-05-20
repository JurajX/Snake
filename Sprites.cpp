//
//  Sprites.cpp
//  Snake
//
//  Created by X on 16.05.18.
//

#include "Sprites.hpp"
#include <cstdlib>
#include <cmath>
#include <ctime>

//-------------------- Helper functions

int RandInt(int a, int b) {
    // returns random integer in the interval [a, b)
    int n_buckets = b-a;
    int random_int = ( std::rand()/(RAND_MAX+1.0) )*n_buckets;
    return a + random_int;
}

bool SDLpointsEqual(SDL_Point pt1, SDL_Point pt2) {
    if (pt1.x == pt2.x and pt1.y == pt2.y) {
        return true;
    } else {
        return false;
    }
}



//-------------------- Snake class

Snake::Snake(SDL_Point playground_dimensions, int tile_size, Uint32 pixel_format)
:mTileSize(tile_size)
,mPlaygroundDims(playground_dimensions) // must hold true: playground_size % tile_size == 0
,mHeadSurface(nullptr)
,mBodySurface(nullptr)
,mTailSurface(nullptr) {
    SDL_Point head = {0, 2*mTileSize};
    SDL_Point body = {0, mTileSize};
    SDL_Point tail = {0, 0};
    mTopLefts = {tail, body, head};
    mDirection = {Direction::DOWN};
    
    mHeadSurface = SDL_CreateRGBSurfaceWithFormat(0, mTileSize, mTileSize, 32, pixel_format);
    mBodySurface = SDL_CreateRGBSurfaceWithFormat(0, mTileSize, mTileSize, 32, pixel_format);
    mTailSurface = SDL_CreateRGBSurfaceWithFormat(0, mTileSize, mTileSize, 32, pixel_format);
    if (mHeadSurface == nullptr or mTailSurface == nullptr or mBodySurface == nullptr) {
        SDL_Log("Failed to create head, body or tail surfaces: %s", SDL_GetError());}
    
    SDL_FillRect(mHeadSurface, nullptr, SDL_MapRGBA(mHeadSurface->format, 0, 200, 0, 255));
    SDL_FillRect(mBodySurface, nullptr, SDL_MapRGBA(mBodySurface->format, 0, 200, 0, 255));
    SDL_FillRect(mTailSurface, nullptr, SDL_MapRGBA(mTailSurface->format, 0, 200, 0, 255));
}

Snake::~Snake() {
    SDL_FreeSurface(mHeadSurface);
    mHeadSurface = nullptr;
    SDL_FreeSurface(mBodySurface);
    mBodySurface = nullptr;
    SDL_FreeSurface(mTailSurface);
    mTailSurface = nullptr;
}

void Snake::SetNewDirection(Direction direction) {
    if (mDirection == Direction::UP and direction != Direction::DOWN) {
        mDirection = direction;
    } else if (mDirection == Direction::DOWN and direction != Direction::UP) {
        mDirection = direction;
    } else if (mDirection == Direction::RIGHT and direction != Direction::LEFT) {
        mDirection = direction;
    } else if (mDirection == Direction::LEFT and direction != Direction::RIGHT) {
        mDirection = direction;
    }
}

int Snake::Update(SDL_Point pellet_top_left) {
    SDL_Point headsNextPosition = {mTopLefts.back().x, mTopLefts.back().y};
    if (mDirection == Direction::UP) {
        if (mTopLefts.back().y == 0) {headsNextPosition.y = mPlaygroundDims.y-mTileSize;}
        else { headsNextPosition.y -= mTileSize; }
    } else if (mDirection == Direction::DOWN) {
        if (mTopLefts.back().y == mPlaygroundDims.y-mTileSize) {headsNextPosition.y = 0;}
        else {headsNextPosition.y += mTileSize;}
    } else if (mDirection == Direction::RIGHT) {
        if (mTopLefts.back().x == mPlaygroundDims.x-mTileSize) {headsNextPosition.x = 0;}
        else {headsNextPosition.x += mTileSize;}
    } else if (mDirection == Direction::LEFT) {
        if (mTopLefts.back().x == 0) {headsNextPosition.x = mPlaygroundDims.x-mTileSize;}
        else {headsNextPosition.x -= mTileSize;}
    }
    
    bool peletEaten = SDLpointsEqual(pellet_top_left, headsNextPosition) ? true : false;
    if (!peletEaten) {
        for (int i=0; i < mTopLefts.size()-1; i++) {
            mTopLefts[i] = mTopLefts[i+1];
        }
        mTopLefts.back().x = headsNextPosition.x;
        mTopLefts.back().y = headsNextPosition.y;
        return 0;
    } else {
        SDL_Point newHead = {headsNextPosition.x, headsNextPosition.y};
        mTopLefts.push_back(newHead);
        return 1;
    }
}

void Snake::BlitOnPlayground(SDL_Surface *playground_surface) {
    for (int i = 0; i < mTopLefts.size(); i++) {
        SDL_Rect dstRect = {mTopLefts[i].x, mTopLefts[i].y, mTileSize, mTileSize};
        if (i == 0) {
            SDL_BlitSurface(mTailSurface, nullptr, playground_surface, &dstRect);
        } else if (i == mTopLefts.size()-1) {
            SDL_BlitSurface(mHeadSurface, nullptr, playground_surface, &dstRect);
        } else {
            SDL_BlitSurface(mBodySurface, nullptr, playground_surface, &dstRect);
        }
    }
}

bool Snake::ColidesWithSnake(SDL_Point position) {
    for (SDL_Point pt: mTopLefts) {
        if (SDLpointsEqual(pt, position)) {return true;}
    }
    return false;
}

bool Snake::SelfColision() {
    SDL_Point headsTopLeft = {mTopLefts.back()};
    for (int i=0; i < mTopLefts.size()-1; i++) {
        if (SDLpointsEqual(headsTopLeft, mTopLefts[i])) {return true;}
    }
    return false;
}



//-------------------- Pellet class

Pellet::Pellet(SDL_Point playground_dimensions, int tile_size, Uint32 pixel_format)
:mTopLeft({0,0})
,mTileSize(tile_size)
,mPlaygroundDims(playground_dimensions) // must hold true: playground_size % tile_size == 0
,mSurface(nullptr) {
    this->MoveRandomly();
    
    mSurface = SDL_CreateRGBSurfaceWithFormat(0, mTileSize, mTileSize, 32, pixel_format);
    if (mSurface == nullptr) { SDL_Log("Failed to create pellet surface: %s", SDL_GetError());}
    SDL_FillRect(mSurface, nullptr, SDL_MapRGBA(mSurface->format, 255, 255, 255, 255));
}

Pellet::~Pellet() {
    SDL_FreeSurface(mSurface);
    mSurface = nullptr;
}

void Pellet::MoveRandomly() {
    int x = RandInt(0, mPlaygroundDims.x/mTileSize)*mTileSize;
    int y = RandInt(0, mPlaygroundDims.y/mTileSize)*mTileSize;
    SDL_Point pt = {x, y};
    mTopLeft = pt;
}

SDL_Point Pellet::GetTopLeft() {
    return mTopLeft;
}

void Pellet::BlitOnPlayground(SDL_Surface *playground_surface) {
    SDL_Rect dstRect = {mTopLeft.x, mTopLeft.y, mTileSize, mTileSize};
    SDL_BlitSurface(mSurface, nullptr, playground_surface, &dstRect);
}
