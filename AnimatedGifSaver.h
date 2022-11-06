/*
  AnimatedGifSaver by Marco Tarini 
  edited by NN

  A simple class to define simple looped animations
  and save them as gif.

  Usage: 
  Just include this file in your code and use the class. 
  Does not need to include giflib.
  Include gifSave.cpp in your project and link with -lgif

  This is a quick and dirty class. 
  At any given time, at most one instance of AnimatedGifSaver must exist!
 
*/

#include <vector>

#include "dcmtk/dcmdata/dctk.h"

typedef unsigned char Byte;

class AnimatedGifSaver{
public:
  
  // Constructor. Use a new instance for each new animation you want to save.
  // All the frames will be sized [sx]X[sy]
  // Constructor defines the palette
  AnimatedGifSaver(int sx, int sy);

  // Descrutcor
  ~AnimatedGifSaver();
  
  // Adds a frame that is to last [dt] seconds
  // "data" is the DCM image PixelData, bottom-to-top
  bool myAddFrame(std::vector <Sint16> data,  float dt);
  
  // Saves the results in a gif file
  bool Save(const char* filename);
  
};
