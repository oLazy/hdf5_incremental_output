#include <iostream>
#include <H5Cpp.h>

using namespace H5;

const H5std_string  FILE_NAME( "SDS.h5" );
const H5std_string  DATASET_NAME( "IntArray" );
const int   NX = 5;                    // dataset dimensions
const int   NY = 6;
const int   RANK = 2;

int main() {

    /*
 * Data initialization.
 */
    int i, j;
    int data[NX][NY];          // buffer for data to write
    for (j = 0; j < NX; j++)
    {
        for (i = 0; i < NY; i++)
            data[j][i] = i + j;
    }

    try {
        Exception::dontPrint();
        H5File thisFile{FILE_NAME, H5F_ACC_TRUNC};

        hsize_t dimsf[2];
        dimsf[0] = NX;
        dimsf[1] = NY;

        DataSpace dataSpace{RANK,dimsf};



    }
    catch (...){
        std::cerr << "cazzo!\n";
    }

    return 0;
}