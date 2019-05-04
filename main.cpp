#include <iostream>
#include <H5Cpp.h>


using namespace H5;

const H5std_string  FILE_NAME( "SDS.h5" );
const H5std_string  DATASET_NAME( "CompoundArray" );
const int   NX = 6;                    // dataset dimensions
const int   NY = 5;
const int   RANK = 2;
const int   NITER = 40000;



struct ComplexData{
    double t, x, y, z;
    int ID;

    void init(double var, int ID){
        t=var;
        x=var+1.0;
        y=var+2.0;
        z=var-1.0;
        this->ID=ID;
    }

    CompType type_h5(){
        CompType result{sizeof(ComplexData)};
        result.insertMember("t", HOFFSET(ComplexData, t), PredType::NATIVE_DOUBLE);
        result.insertMember("x", HOFFSET(ComplexData, x), PredType::NATIVE_DOUBLE);
        result.insertMember("y", HOFFSET(ComplexData, y), PredType::NATIVE_DOUBLE);
        result.insertMember("z", HOFFSET(ComplexData, z), PredType::NATIVE_DOUBLE);
        result.insertMember("ID", HOFFSET(ComplexData, ID), PredType::NATIVE_INT);
        return result;
    }
//    static void dump(int const n_row, int const n_col, CompType const (&data)[n_row][n_col], std::string const& fileName){
//
//    }
};


template <class T>
class DataPrinter{
    H5std_string file_name;
    H5std_string dataset_name;
    const int n_row;
    const int n_col;
    H5File h5File;
    T fill_val = T();
    CompType type_t = fill_val.type_h5();
    hsize_t offset[2] = {0, 0}; //
    DataSet dataSet;
    DataSpace dataSpace;
    DataSpace writing_space;
    hsize_t size[2];
    hsize_t buffer_dim[2];

public:
    DataPrinter(std::string fileName, const std::string &datasetName, const int nRow, const int nCol)
            : file_name(std::move(fileName)), dataset_name(datasetName), n_row(nRow), n_col(nCol) {
        h5File = std::move(H5File(file_name, H5F_ACC_TRUNC)); // Open file
        buffer_dim[0] = static_cast<unsigned long long>(n_row);
        buffer_dim[1] = static_cast<unsigned long long>(n_col);
        hsize_t maxdims[2] = {H5S_UNLIMITED, static_cast<hsize_t>(n_col)};

        DSetCreatPropList creatParams;
        creatParams.setChunk(RANK, buffer_dim);

//        T fill_val = T();
//        CompType type_t = fill_val.type_h5();
        creatParams.setFillValue(type_t, &fill_val);
        dataSpace = std::move(DataSpace{RANK,buffer_dim, maxdims});
        dataSet = h5File.createDataSet(datasetName, type_t, dataSpace, creatParams);

        size[0] = n_row;
        size[1] = n_col;

        writing_space = std::move(DataSpace{RANK, size});
    }

    void append(const int nRow, const int nCol, void* buffer){
        try {
            dataSet.extend(size);
            std::cout << size[0] << ", " << size[1] << "\n";
            // select a hyperslab
            DataSpace memory_space = dataSet.getSpace();
            memory_space.selectHyperslab(H5S_SELECT_SET, buffer_dim, offset);
            // write buffer
            dataSet.write(buffer, type_t, writing_space, memory_space);
            // update offset
            offset[0] = offset[0] + n_row;
            // update size
            size[0] += n_row;
        }
        catch(...) {
            std::cout << "minchia\n";
            throw;
        }
    }
};

int main() {

    /*
 * Data initialization.
 */
    int i, j;
    ComplexData data[NX][NY];
    for (j = 0; j < NX; j++)
    {
        for (i = 0; i < NY; i++) {
            data[j][i].init(static_cast<double>(i + j),i + NX * j);
        }
    }
    DataPrinter<ComplexData> printer{FILE_NAME,DATASET_NAME,NX,NY};


    try {
        Exception::dontPrint();
        printer.append(NX, NY, data);
        printer.append(NX, NY, data);
        printer.append(NX, NY, data);
        printer.append(NX, NY, data);
//        H5File thisFile{FILE_NAME, H5F_ACC_TRUNC}; // Open file
//
//        hsize_t dimsf[2];
//        hsize_t maxdims[2] = {H5S_UNLIMITED, NY};
//        dimsf[0] = NX;
//        dimsf[1] = NY;
//
//        CompType myType = data[0][0].myType();
//
//        DataSpace dataSpace{RANK,dimsf, maxdims};
//
//        DSetCreatPropList creatParams;
//        hsize_t chunck_dims[2] = {NX, NY};
//        creatParams.setChunk(RANK, chunck_dims);
//
//        ComplexData fill_val = ComplexData();
//        creatParams.setFillValue(myType, &fill_val);
//
//
////        IntType datatype{PredType::NATIVE_INT};
////        datatype.setOrder(H5T_ORDER_LE);
//
//
//
//        DataSet dataSet = thisFile.createDataSet(DATASET_NAME, myType, dataSpace, creatParams);
//        /*
//         * Extend the dataset. This call assures that dataset is at least 6 x 5.
//         */
//        hsize_t size[2];
//        size[0] = NX;
//        size[1] = NY;
//        dataSet.extend(size);
//
//        /*
// * Select a hyperslab.
// */
//        DataSpace memSpace1 = dataSet.getSpace();
//        hsize_t offset[2];
//        offset[0] = 0;
//        offset[1] = 0;
//        hsize_t dims1[2] = {NX, NY};
//
//        memSpace1.selectHyperslab(H5S_SELECT_SET, dims1, offset);
//        /*
// * Write the data to the hyperslab.
// */
//
//        dataSet.write(data, myType, dataSpace, memSpace1);
//
//        if (true) {
//
//            /* ==================== NEW PART ============================*/
//
//            for (auto iter = 0; iter < NITER; ++iter) {
//                // extend the dataset: now it becomes 12 x 5
//                hsize_t dims2[2] = {NX, NY}; // dimensions of the added block
//                size[0] = size[0] + dims2[0];
////        size[1] = size[1]; // this will not change
//                dataSet.extend(size);
//                // select the second hyperslab (block)
//
//                DataSpace dataSpace2 = dataSet.getSpace();
//                offset[0] = size[0] - dims2[0];
//                offset[1] = 0;
//                dataSpace2.selectHyperslab(H5S_SELECT_SET, dims2, offset);
//
//                // define memory space
//                DataSpace memSpace2(RANK, dims2);
//                dataSet.write(data, myType, memSpace2, dataSpace2);
//            }
//        }

    }
    catch (std::exception const& e){
        std::cerr << e.what() << "\n";
    }
        // catch failure caused by the H5File operations
    catch( FileIException error )
    {
//        error.printError();
        return 1;
    }
        // catch failure caused by the DataSet operations
    catch( DataSetIException error )
    {
//        error.printError();
        return 2;
    }
        // catch failure caused by the DataSpace operations
    catch( DataSpaceIException error )
    {
//        error.printError();
        return 3;
    }
        // catch failure caused by the DataSpace operations
    catch( DataTypeIException error )
    {
//        error.printError();
        return 4;
    }
    catch (...){
        std::cerr << "cazzo!\n";
    }

    return 0;
}