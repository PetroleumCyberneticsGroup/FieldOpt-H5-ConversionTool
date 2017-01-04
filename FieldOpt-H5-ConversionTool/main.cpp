
#include <iostream>
//#include <fstream>

// BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <boost/property_tree/detail/json_parser_read.hpp>
//#include <boost/property_tree/detail/json_parser_write.hpp>
//#include <boost/property_tree/detail/json_parser_error.hpp>

#include <vector>
#include <H5Cpp.h>

// FIELDOPT: HDF5 SUMMARYREADER
#include <hdf5_summary_reader.h>

// ERT

// Using namespaces
using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {

    string filename;
    if (argc >= 2){
        filename = argv[1]; // char
    }else{
        cout << "usage: " << argv[0] << endl;
        cout << "examp: " << argv[0] << endl;
        exit(1);
    }

    std::stringstream input_str(argv[1]);

    input_str

    namespace pt = boost::property_tree;
    pt::ptree root;
    pt::read_json("../input/conv-params.json", root);

    Hdf5SummaryReader H5Data("TEST");

    return 0;
}
