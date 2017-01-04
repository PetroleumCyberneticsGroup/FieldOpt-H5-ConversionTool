
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

int main() {

    namespace pt = boost::property_tree;
    pt::ptree root;
    pt::read_json("conv-params.json", root);

    Hdf5SummaryReader H5Data("TEST");

    return 0;
}
