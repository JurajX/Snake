# Snake

This is a fun 2D game of Snake written in C++ using SDL2 library.
The ultimate goal is to extend this game to a OpenAI gym-like environment for testing reinforcement learning algorithms which take only raw sensory input (pixels of the screen); and then, of course, test bunch of things on it.

The SDL2 library (only a version for apple's OSX) is included in the repository. The game was written in Xcode IDE. To run the game simply open the Xcode project and compile.

The actual snake is controlled with arrows (up, down, left, and right), escape key terminates the program. The game ends when the snake bites its tail.


This project was partly inspired by:
 - Sanhay Madhav's book Game Programming in C++ [Publisher's site](http://www.pearsoned.co.uk/bookshop/detail.asp?item=100000000644133)
 - Lazy Foo' [tutorials](http://lazyfoo.net/tutorials/SDL/index.php).
 
where I borrowed some ideas.
