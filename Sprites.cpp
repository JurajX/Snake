//
//  Sprites.cpp
//  Snake
//
//  Created by X on 16.05.18.
//

#include "Sprites.hpp"
#include <cstdlib>
#include <cmath>


//-------------------- Helper functions

int RandInt(int a, int b) {
    // Returns random integer in the interval [a, b).
    int n_buckets = b-a;
    int random_int = ( std::rand()/(RAND_MAX+1.0) )*n_buckets;
    return a + random_int;
}

bool operator==(const SDL_Point &pt1, const SDL_Point &pt2) {
    if (pt1.x == pt2.x and pt1.y == pt2.y) {return true;}
    else {return false;}
}


int Part::FrontToInt() { return static_cast<int>(front); }


int Part::BackToInt() {
    // Return the integer representation of opposite (reversed) direction,
    // i.e. front == Direction::UP(=1) the functiont returns 2(=Direction::DOWN).
    switch (front) {
        case Direction::UP:
            return static_cast<int>(Direction::DOWN);
        case Direction::DOWN:
            return static_cast<int>(Direction::UP);
        case Direction::RIGHT:
            return static_cast<int>(Direction::LEFT);
        case Direction::LEFT:
            return static_cast<int>(Direction::RIGHT);
    }
}



//-------------------- Snake class

Snake::Snake(SDL_Point playground_dimensions, int tile_size, Uint32 pixel_format)
:mHeadSurfaces()
,mBodySurfaces()
,mBodyParts()
,mDirection()
,mPlaygroundDims(playground_dimensions) // must hold true: playground_size % tile_size == 0
,mTileSize(tile_size) {
    Part head = {{0, 2*mTileSize}, Direction::DOWN};
    Part body = {{0, mTileSize}, Direction::DOWN};
    Part tail = {{0, 0}, Direction::DOWN};
    mBodyParts = {tail, body, head};
    mDirection = {Direction::DOWN};
    
    this->LoadImages();
}


void Snake::LoadImages() {
    // Load head images and store them in the mHeadSurfaces map,
    // keys are the int representation of Direction.
    SDL_Surface *head_u = IMG_Load("Images/head_up.png");
    SDL_Surface *head_d = IMG_Load("Images/head_down.png");
    SDL_Surface *head_r = IMG_Load("Images/head_right.png");
    SDL_Surface *head_l = IMG_Load("Images/head_left.png");
    if (head_u == nullptr or head_d == nullptr or head_r == nullptr or head_l == nullptr) {
        SDL_Log("Failed to load one of the head images: %s", IMG_GetError());}
    
    mHeadSurfaces[static_cast<int>(Direction::UP)] = head_u;
    mHeadSurfaces[static_cast<int>(Direction::DOWN)] = head_d;
    mHeadSurfaces[static_cast<int>(Direction::RIGHT)] = head_r;
    mHeadSurfaces[static_cast<int>(Direction::LEFT)] = head_l;

    // load tail images
    SDL_Surface *tail_u = IMG_Load("Images/tail_up.png");
    SDL_Surface *tail_d = IMG_Load("Images/tail_down.png");
    SDL_Surface *tail_r = IMG_Load("Images/tail_right.png");
    SDL_Surface *tail_l = IMG_Load("Images/tail_left.png");
    if (tail_u == nullptr or tail_d == nullptr or tail_r == nullptr or tail_l == nullptr) {
        SDL_Log("Failed to load one of the tail images: %s", IMG_GetError());}
    
    // load body images
    SDL_Surface *body_ud = IMG_Load("Images/body_up_down.png");
    SDL_Surface *body_rl = IMG_Load("Images/body_right_left.png");
    SDL_Surface *body_ur = IMG_Load("Images/body_up_right.png");
    SDL_Surface *body_ul = IMG_Load("Images/body_up_left.png");
    SDL_Surface *body_dr = IMG_Load("Images/body_down_right.png");
    SDL_Surface *body_dl = IMG_Load("Images/body_down_left.png");
    if (body_ud == nullptr or body_rl == nullptr or body_ur == nullptr or
        body_ul == nullptr or body_dr == nullptr or body_dl == nullptr) {
        SDL_Log("Failed to load one of the body images: %s", IMG_GetError());}
    
    // Here comes handy the fact that snake's body parts only connect either two different directions,
    // i.e. UP and RIGHT; or, in case of tail, connect to only one direction. Hence, I can sum
    // (int representations of) these directions to lable the body parts and store them in a map.
    // Moreover, the fact that addition is commutative (UP+RIGHT==RIGHT+UP) makes life easier.
    mBodySurfaces[static_cast<int>(Direction::UP)] = tail_u;
    mBodySurfaces[static_cast<int>(Direction::DOWN)] = tail_d;
    mBodySurfaces[static_cast<int>(Direction::RIGHT)] = tail_r;
    mBodySurfaces[static_cast<int>(Direction::LEFT)] = tail_l;
    
    mBodySurfaces[static_cast<int>(Direction::UP)+static_cast<int>(Direction::DOWN)] = body_ud;
    mBodySurfaces[static_cast<int>(Direction::RIGHT)+static_cast<int>(Direction::LEFT)] = body_rl;
    mBodySurfaces[static_cast<int>(Direction::UP)+static_cast<int>(Direction::RIGHT)] = body_ur;
    mBodySurfaces[static_cast<int>(Direction::UP)+static_cast<int>(Direction::LEFT)] = body_ul;
    mBodySurfaces[static_cast<int>(Direction::DOWN)+static_cast<int>(Direction::RIGHT)] = body_dr;
    mBodySurfaces[static_cast<int>(Direction::DOWN)+static_cast<int>(Direction::LEFT)] = body_dl;
}


void Snake::FreeSurfaces() {
    for (auto surface: mHeadSurfaces) {
        SDL_FreeSurface(surface.second);
        surface.second = nullptr;}
    for (auto surface: mBodySurfaces) {
        SDL_FreeSurface(surface.second);
        surface.second = nullptr;}
}


void Snake::SetNewDirection(Direction direction) {
    // Turns by 180 degrees are not allowed.
    if (mDirection == Direction::UP and direction != Direction::DOWN) {
        mDirection = direction;}
    else if (mDirection == Direction::DOWN and direction != Direction::UP) {
        mDirection = direction;}
    else if (mDirection == Direction::RIGHT and direction != Direction::LEFT) {
        mDirection = direction;}
    else if (mDirection == Direction::LEFT and direction != Direction::RIGHT) {
        mDirection = direction;}
}


int Snake::Update(SDL_Point pellet_top_left) {
    // Move the snake in the newly set direction, and check for collisions with pellet.
    // Return the score gained; i.e. 1 if the snake ate a pellet, 0 otherwise.
    Part nextHead = mBodyParts.back();    
    if (mDirection == Direction::UP) {
        if (mBodyParts.back().pt.y == 0) {nextHead.pt.y = mPlaygroundDims.y-mTileSize;}
        else {nextHead.pt.y -= mTileSize;}
        nextHead.front = Direction::UP;
    } else if (mDirection == Direction::DOWN) {
        if (mBodyParts.back().pt.y == mPlaygroundDims.y-mTileSize) {nextHead.pt.y = 0;}
        else {nextHead.pt.y += mTileSize;}
        nextHead.front = Direction::DOWN;
    } else if (mDirection == Direction::RIGHT) {
        if (mBodyParts.back().pt.x == mPlaygroundDims.x-mTileSize) {nextHead.pt.x = 0;}
        else {nextHead.pt.x += mTileSize;}
        nextHead.front = Direction::RIGHT;
    } else if (mDirection == Direction::LEFT) {
        if (mBodyParts.back().pt.x == 0) {nextHead.pt.x = mPlaygroundDims.x-mTileSize;}
        else {nextHead.pt.x -= mTileSize;}
        nextHead.front = Direction::LEFT;
    }
    
    bool peletEaten = (pellet_top_left == nextHead.pt) ? true : false;
    if (!peletEaten) {
        for (int i=0; i < mBodyParts.size()-1; i++) {
            mBodyParts[i] = mBodyParts[i+1];
        }
        mBodyParts.back() = nextHead;
        mBodyParts[mBodyParts.size()-2].front = nextHead.front;
        return 0;
    } else {
        mBodyParts.push_back(nextHead);
        mBodyParts[mBodyParts.size()-2].front = nextHead.front;
        return 1;
    }
}


void Snake::BlitOnPlayground(SDL_Surface *playground_surface) {
    // Draw the snake on the playground surface.
    for (int i = 0; i < mBodyParts.size(); i++) {
        SDL_Rect dstRect = {mBodyParts[i].pt.x, mBodyParts[i].pt.y, mTileSize, mTileSize};
        if (i == mBodyParts.size()-1) {
            SDL_BlitScaled(mHeadSurfaces[mBodyParts[i].FrontToInt()], nullptr, playground_surface, &dstRect);
        } else if (i == 0) {
            SDL_BlitScaled(mBodySurfaces[mBodyParts[i].FrontToInt()], nullptr, playground_surface, &dstRect);
        } else if (i > 0){
            SDL_BlitScaled(mBodySurfaces[mBodyParts[i].FrontToInt()+mBodyParts[i-1].BackToInt()], nullptr, playground_surface, &dstRect);
        }
    }
}


bool Snake::ColidesWithPoint(SDL_Point position) {
    for (auto part: mBodyParts) {
        if (part.pt == position) {return true;}
    }
    return false;
}


bool Snake::SelfColision() {
    SDL_Point headsTopLeft = {mBodyParts.back().pt};
    for (int i=0; i < mBodyParts.size()-1; i++) {
        if (headsTopLeft == mBodyParts[i].pt) {return true;}
    }
    return false;
}



//-------------------- Pellet class

Pellet::Pellet(SDL_Point playground_dimensions, int tile_size, Uint32 pixel_format)
:mTopLeft({0,0})
,mTileSize(tile_size)
,mPlaygroundDims(playground_dimensions) // must hold true: playground_size % tile_size == 0
,mSurfaces()
,mSurfaceIndex(0) {
    this->LoadImages();
    this->MoveRandomly();
}


void Pellet::LoadImages() {
    SDL_Surface *apple = IMG_Load("Images/apple.png");
    SDL_Surface *cherry = IMG_Load("Images/cherry.png");
    SDL_Surface *watermelon = IMG_Load("Images/watermelon.png");
    if (apple == nullptr or cherry == nullptr or watermelon == nullptr) {
        SDL_Log("Failed to load apple, cherry or watermelon: %s", IMG_GetError()); }
    mSurfaces = {apple, cherry, watermelon};
}


void Pellet::FreeSurfaces() {
    for (auto surface: mSurfaces) {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }
}


void Pellet::MoveRandomly() {
    int x = RandInt(0, mPlaygroundDims.x/mTileSize)*mTileSize;
    int y = RandInt(0, mPlaygroundDims.y/mTileSize)*mTileSize;
    SDL_Point pt = {x, y};
    mTopLeft = pt;
    mSurfaceIndex = RandInt(0, static_cast<int>(mSurfaces.size()));
}


SDL_Point Pellet::GetTopLeft() {
    return mTopLeft;
}


void Pellet::BlitOnPlayground(SDL_Surface *playground_surface) {
    // Draw the pellet on the playground surface.
    SDL_Rect dstRect = {mTopLeft.x, mTopLeft.y, mTileSize, mTileSize};
    SDL_BlitScaled(mSurfaces[mSurfaceIndex], nullptr, playground_surface, &dstRect);
}
