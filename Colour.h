#pragma once
typedef unsigned char BYTE;


class Colour
{
public:
	//Overloaded intitaliser for different colours
	Colour();
	Colour(unsigned int val);
	Colour(BYTE r, BYTE g, BYTE b);
	Colour(BYTE r, BYTE g, BYTE b, BYTE a);
	Colour(const Colour& src);

	constexpr BYTE GetR()const {return rgba[0];};
	void SetR(BYTE r) { rgba[0] = r; };

	constexpr BYTE GetG()const { return rgba[1]; };
	void SetG(BYTE g) { rgba[1] = g; };

	constexpr BYTE GetB()const { return rgba[2]; };
	void SetB(BYTE b) { rgba[2] = b; };

	constexpr BYTE GetA()const { return rgba[3]; };
	void SetA(BYTE a) { rgba[3] = a; };

private:
	union {
		//Store colour
		BYTE rgba[4];
		unsigned int colour;
	};
};

//Used as a default colour for objects that do not have a texture or material.
namespace Colours
{
	const Colour UnloadedTextureColour(0, 20, 147);
	const Colour UnhandledTextureColour(250, 0, 0);
}

