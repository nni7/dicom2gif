#include <gif_lib.h>
#include <stdio.h>

#include <vector>
#include <iostream>
#include <list>

using namespace std;

#include "Dicom2Gif.h"


Dicom2Gif::Dicom2Gif( std::string dir, std::string out_file ){
    string directory_path = dir;

    list<string> files;
    get_files( dir, files );

    // get gif image size from first dcm image 
    DicomImage *image = new DicomImage( files.front().c_str() );
    if (image == NULL) {
      cout << "error read file" << endl;
      exit(0);
    }
    unsigned long height = image->getHeight();
    //cout << "height: " << height << endl;
    unsigned long width = image->getWidth();
    //cout << "width: " << width << endl;
    delete image;


    AnimatedGifSaver saver(width, height);
    for( string str : files ) {
      DicomImage *image = new DicomImage( str.c_str() );
      if (image != NULL) {
        vector <Sint16> pixelData = getPixelData( image );
        saver.myAddFrame(pixelData, 0.5); // first frame: three secs
      }
      else {
          cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
      }
      delete image;
    }
    int res = saver.Save( out_file.c_str() );
}

/*
Dicom2Gif::~Dicom2Gif(){
    cout << "desctructor Dicom2Gif" << endl;
}
*/

void Dicom2Gif::get_files( std::string dir, std::list<std::string> &files ) {
  DIR *dirp = opendir(dir.c_str());
  if(!dirp) {
    cout << "о�ибка о�к���и� ди�ек�о�ии" << endl;
    exit(1);
  }

  struct dirent *file;
  while( file = readdir(dirp) ) {
    string file_name = file->d_name;

    if( (file_name == ".") || (file_name == "..") ) {
      continue;
    }

    struct stat stbuf;
    string path = dir + "\\" + file_name;

    if (stat(path.c_str(), &stbuf) < 0) {
      cout << "о�ибка пол��ени� �ведени� о �айле" << endl;
    }

    if( S_ISDIR(stbuf.st_mode) ) {
      //cout << "��о ди�ек�о�и�!" << endl;
      get_files( path + "/", files );
    }
    else {
      // е�ли �айл не �оdcmе�жи� ".tif
      std::string::size_type n = file_name.find(".dcm");
      if( n == std::string::npos ) {
        //cout << "n: " << n << endl;
        continue;
      }

      files.push_back(path);
    }

  }

  closedir(dirp);

  files.sort();
}


vector <Sint16> Dicom2Gif::getPixelData( DicomImage *image ) {
  vector <Sint16> result = {};

  if (image->getStatus() == EIS_Normal)
  {
    const DiPixel * pix = image->getInterData();
    unsigned long count2 = pix->getCount();

    Sint16 * pixelData = (Sint16 *)pix->getData();
    for(int i=0; i<count2; i++) {
        result.push_back( pixelData[i] );
    }
  }
  return result;
}


