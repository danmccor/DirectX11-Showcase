#include "Colour.h"

//Set colour to 0
Colour::Colour() : colour(0)
{
}
//Set colour to the value
Colour::Colour(unsigned int val) : colour(val)
{
}
//Set colour to the input colours, and 255 for maxiumum alpha
Colour::Colour(BYTE r, BYTE g, BYTE b) : Colour(r,g,b,255)
{
}

//Inidiually set the RGBA
Colour::Colour(BYTE r, BYTE g, BYTE b, BYTE a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

//Copy colour
Colour::Colour(const Colour& src) : colour(src.colour)
{
}