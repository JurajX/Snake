# Snake

This is a fun 2D game of Snake written in C++ using SDL2 library.
The ultimate goal is to extend this game to a OpenAI gym-like environment for testing reinforcement learning algorithms which take only raw sensory input (pixels of the screen); and then, of course, test bunch of things on it.

To run the game one needs to compile it using cmake
```terminal
% cd path/this/repository/
% mkdir build
% cd build
% cmake ..
% make
% ./HomeWork
```

The prerequisites are *SDL2*, *SDL2_ttf*, and *SDL2_image*. Mac users can install them using *Homebrew* as follows
```terminal
% brew install sdl2 sdl2_ttf sdl2_image
```

The actual snake is controlled with arrows (up, down, left, and right), escape key terminates the program. The game ends when the snake bites its body.


This project was partly inspired by:
 - Sanhay Madhav's book Game Programming in C++ [Publisher's site](http://www.pearsoned.co.uk/bookshop/detail.asp?item=100000000644133);
 - Lazy Foo' [tutorials](http://lazyfoo.net/tutorials/SDL/index.php).
