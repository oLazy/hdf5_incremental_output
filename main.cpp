#include <iostream>
#include <H5Cpp.h>

using namespace H5;

const H5std_string  FILE_NAME( "SDS.h5" );
const H5std_string  DATASET_NAME( "IntArray" );
const int   NX = 6;                    // dataset dimensions
const int   NY = 5;
const int   RANK = 2;

int main() {

    /*
 * Data initialization.
 */
    int i, j;
    int data[NX][NY];
    for (j = 0; j < NX; j++)
    {
        for (i = 0; i < NY; i++)
            data[j][i] = i + j;
    }

    try {
        Exception::dontPrint();
        H5File thisFile{FILE_NAME, H5F_ACC_TRUNC};

        hsize_t dimsf[2];
        hsize_t maxdims[2] = {H5S_UNLIMITED, NY};
        dimsf[0] = NX;
        dimsf[1] = NY;



        DataSpace dataSpace{RANK,dimsf, maxdims};

        DSetCreatPropList creatParams;
        hsize_t chunck_dims[2] = {NX, NY};
        creatParams.setChunk(RANK, chunck_dims);

        int fill_val = 0;
        creatParams.setFillValue(PredType::NATIVE_INT, &fill_val);


        IntType datatype{PredType::NATIVE_INT};
        datatype.setOrder(H5T_ORDER_LE);



        DataSet dataSet = thisFile.createDataSet(DATASET_NAME, datatype, dataSpace, creatParams);
        /*
         * Extend the dataset. This call assures that dataset is at least 6 x 5.
         */
        hsize_t size[2];
        size[0] = NX;
        size[1] = NY;
        dataSet.extend(size);

        /*
 * Select a hyperslab.
 */
        DataSpace memSpace1 = dataSet.getSpace();
        hsize_t offset[2];
        offset[0] = 0;
        offset[1] = 0;
        hsize_t dims1[2] = {NX, NY};

        memSpace1.selectHyperslab(H5S_SELECT_SET, dims1, offset);
        /*
 * Write the data to the hyperslab.
 */

        dataSet.write(data, PredType::NATIVE_INT, dataSpace, memSpace1);



        /* ==================== NEW PART ============================*/

        // extend the dataset: now it becomes 12 x 5
        hsize_t dims2[2] = {NX, NY};
        size[0] = size[0] + dims2[0];
        size[1] = size[1];
        dataSet.extend(size);
        // select the second hyperslab (block)

        DataSpace dataSpace2 = dataSet.getSpace();
        offset[0] = NX;
        offset[1] = 0;
        dataSpace2.selectHyperslab(H5S_SELECT_SET,dims2,offset);

        // define memory space
        DataSpace memSpace2(RANK, dims2);
        dataSet.write(data, PredType::NATIVE_INT, memSpace2, dataSpace2);
        

    }
    catch (std::exception const& e){
        std::cerr << e.what() << "\n";
    }
        // catch failure caused by the H5File operations
    catch( FileIException error )
    {
        error.printError();
        return 1;
    }
        // catch failure caused by the DataSet operations
    catch( DataSetIException error )
    {
        error.printError();
        return 2;
    }
        // catch failure caused by the DataSpace operations
    catch( DataSpaceIException error )
    {
        error.printError();
        return 3;
    }
        // catch failure caused by the DataSpace operations
    catch( DataTypeIException error )
    {
        error.printError();
        return 4;
    }
    catch (...){
        std::cerr << "cazzo!\n";
    }

    return 0;
}