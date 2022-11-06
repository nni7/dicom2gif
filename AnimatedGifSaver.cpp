#include <gif_lib.h>
#include <stdio.h>

#include <vector>
#include <iostream>

using namespace std;

typedef unsigned char Byte;

typedef vector<GifByteType> Frame;

#include "AnimatedGifSaver.h"

// global vairables

static std::vector<Frame> frames;
static std::vector<int> delay;

static ColorMapObject* outputPalette;

static int gifsx, gifsy;


AnimatedGifSaver::AnimatedGifSaver(int sx, int sy){
    gifsx=sx;
    gifsy=sy;

    frames.clear();
    delay.clear();

    int paletteSize=256;

    outputPalette = GifMakeMapObject(paletteSize, NULL);
    GifColorType *c = outputPalette->Colors;
    if (!outputPalette) { 
        cerr << "output palette initialize error" << endl;
        exit(1);
    }
    for (int k = 0; k < outputPalette->ColorCount; k++) {
        c[k].Red   = k;
        c[k].Green = k;
        c[k].Blue  = k;
    }
}

AnimatedGifSaver::~AnimatedGifSaver(){
    GifFreeMapObject(outputPalette);
    frames.clear();
    delay.clear();
}


bool AnimatedGifSaver::myAddFrame(vector <Sint16> data, float dt) {
    
    unsigned int npix=gifsx*gifsy;
    int min, max;
    min = *min_element(data.begin(), data.end());
    max = *max_element(data.begin(), data.end());
    max = max + (min * -1);

    for (int i=0; i<npix; i++) {
        data[i] = data[i] + (min * -1);
    }

    Frame output(npix);


    // maunal assignment of color indices
    for (int i=0; i<npix; i++){
      output[i] = ( (float)data[i] / max) * 256;
    }

    frames.push_back(output);

    delay.push_back(int(dt*100.0));

    return true;
}


static bool AddLoop(GifFileType *gf){
  
   int loop_count;
   loop_count=0;
   {
     char nsle[12] = "NETSCAPE2.0";
     char subblock[3];
     if (EGifPutExtension(gf, APPLICATION_EXT_FUNC_CODE, 11, nsle) == GIF_ERROR) {
       return false;
     }
     subblock[0] = 1;
     subblock[2] = loop_count % 256;
     subblock[1] = loop_count / 256;
     if (EGifPutExtension(gf, APPLICATION_EXT_FUNC_CODE, 3, subblock) == GIF_ERROR) {
       return false;
     }
 
    }
    return true;
}


bool AnimatedGifSaver::Save(const char* filename){
  if (frames.size()==0) return false;
  
  GifFileType *GifFile = EGifOpenFileName(filename, 0, NULL);
  
  if (!GifFile) return false;

  if (EGifPutScreenDesc(
        GifFile,
        gifsx, gifsy, 8, 0,
        outputPalette
      ) == GIF_ERROR) return false;

  if (!AddLoop(GifFile)) return false;

  for (int ni=0; ni<frames.size(); ni++) {      

    static unsigned char
    ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };
 
    
    ExtStr[0] = (false) ? 0x06 : 0x04;
    ExtStr[1] = delay[ni] % 256;
    ExtStr[2] = delay[ni] / 256;
 
    /* Dump graphics control block. */
    EGifPutExtension(GifFile, GRAPHICS_EXT_FUNC_CODE, 4, ExtStr);
            
    
    if (EGifPutImageDesc(
       GifFile,
       0, 0, gifsx, gifsy, 0, NULL
      ) == GIF_ERROR)  return false;
       
       
    for (int y = 0, j=(gifsy-1)*gifsx; y < gifsy; y++, j-=gifsx) {
        if (EGifPutLine(GifFile, &(frames[ni][j]), gifsx) == GIF_ERROR) return false;
    }
  }

  if (EGifCloseFile(GifFile, NULL) == GIF_ERROR) return false;

  return true;       
}

  
