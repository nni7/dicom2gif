/*
  Dicom2Gif by NN
*/

#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <getopt.h>

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

#include "AnimatedGifSaver.h"


typedef unsigned char Byte;

class Dicom2Gif {
public:

  // Constructor
  Dicom2Gif( std::string dir, std::string out_file  );

  // Descrutcor
  //~Dicom2Gif();

  void get_files( std::string dir, std::list<std::string> &files );

  std::vector <Sint16> getPixelData( DicomImage *image );
};
