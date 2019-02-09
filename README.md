# line_collision

A tiny demonstration of line collision. The program creates random lines, then it finds whether the lines are colliding or not, and the specific point where they are colliding, finally it draws the results on the screen and saves the data on a txt file.

Nothing fancy really.

I used g++ on ubuntu to compile like so:
```
g++ main.cpp -o line_collision -lsfml-graphics -lsfml-window -lsfml-system
```
And run it like so:
```
./line_collision
```

I don't think it should have any trouble compiling with other compilers, as long as you have SFML available. :)
