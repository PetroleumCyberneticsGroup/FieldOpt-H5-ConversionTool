
#include <iostream>
//#include <fstream>

// BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/detail/json_parser_read.hpp>
#include <boost/property_tree/detail/json_parser_write.hpp>
#include <boost/property_tree/detail/json_parser_error.hpp>

#include <vector>
#include <H5Cpp.h>

// FIELDOPT: HDF5 SUMMARYREADER
#include <hdf5_summary_reader.h>

// ERT

// Using namespaces
using std::cout;
using std::endl;
using std::string;
using boost::property_tree::ptree;


void getParamFileName(){}
void readParamFile(){}

//void print(ptree const& pt)
//{
//    ptree::const_iterator end = pt.end();
//
//    for (ptree::const_iterator it = pt.begin(); it != end; ++it)
//    {
//        std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
//        print(it->second);
//    }
//}

const string
        ECL_DATA_FILE_PATH, BASE_NAME_ECL_OUTPUT,
        OUTPUT_DIRECTORY, ADGPRS_H5_FILE;

int main(int argc, char *argv[]) {

    // Get parameter file name getParamFileName()
    string filename;
    if (argc >= 2){
        filename = argv[1]; // char
    }else{
        cout << "usage: " << argv[0] << "" << endl;
        cout << "example: " << argv[0] << "" << endl;

        filename = (string)PROJECT_SOURCE_DIR + "/input/conv-params.json";
        cout << "Using input paramenters from default file:\n"
             << filename << endl;
        // exit(1);
    }

    // Read json parameter file readParamFile()
    ptree pt;
    read_json(filename, pt);

    // Load conversion variables
      const string ECL_DATA_FILE_PATH = pt.get<string>("ECL_DATA_FILE_PATH");
    const string BASE_NAME_ECL_OUTPUT = pt.get<string>("BASE_NAME_ECL_OUTPUT");
        const string OUTPUT_DIRECTORY = pt.get<string>("OUTPUT_DIRECTORY");
          const string ADGPRS_H5_FILE = pt.get<string>("ADGPRS_H5_FILE");

    getParamFileName();
    readParamFile();





//    Hdf5SummaryReader H5Data("TEST");

    return 0;
}
