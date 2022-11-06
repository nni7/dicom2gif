#include <iostream>
#include <vector>
#include <list>
#include <dirent.h>
#include <sys/stat.h>
#include <getopt.h>

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

#include "Dicom2Gif.h"


using namespace std;


string dir = ".";
string out_file = "out.gif";


void usage(char *name);


void get_options(int argc, char *argv[]);


int main(int argc, char *argv[]) {

    get_options(argc, argv);

    Dicom2Gif d2g( dir, out_file );

    return 0;
}


void usage(char *name) {
    printf( "usage: %s [-h] [-d directory] [-o out_file]\n \
        -h - this message\n \
        -d directory - directory path, by default is current\n \
        -o out_file - output file name, by default is out.gif\n \
        \n",
        name );

    exit(0);
    return;
}


void get_options(int argc, char *argv[]) {
    struct option long_opt[] = {
                    {"help", 0, 0, 'h'},
                    {"dir", 1, 0, 'd'},
                    {"out", 1, 0, 'o'},
                    {0,0,0,0}
                  };
    int optIdx;

    // �и�аем а�г�мен�� командной ���оки
    char *opts = (char *)"hd:o:";
    int opt; // кажда� �лед���а� оп�и� попадае� ��да
    unsigned int channel = 0;

    while((opt = getopt_long(argc, argv, opts, long_opt, &optIdx)) != -1) { // в�з�ваем getopt пока она не ве�не� -1
        switch(opt) {
            case 'h':
                usage(argv[0]);
                break;
            case 'd':
                dir = optarg;
                break;
            case 'o':
                out_file = optarg;
                break;
            default:
                usage(argv[0]);
                break;
        }
    }
}
