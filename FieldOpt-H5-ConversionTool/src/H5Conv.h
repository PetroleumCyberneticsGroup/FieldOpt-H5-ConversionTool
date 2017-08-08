//
// Created by bellout on 1/5/17.
//

#ifndef H5_CONVERSIONTOOL_H5CONV_H
#define H5_CONVERSIONTOOL_H5CONV_H

// OPM: PARSER
#include <opm/parser/eclipse/Parser/ParseContext.hpp>
#include <opm/parser/eclipse/Parser/Parser.hpp>

// OPM: OUTPUT
#include <opm/output/eclipse/EclipseWriter.hpp>
//#include <opm/output/eclipse/EclipseReader.hpp>

// BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

// STANDARD
#include <string>
#include <iostream>

// NAMESPACES
using namespace std;
using namespace Opm;

class H5Conv {

public:

    /*!
     * Convert AD-GPRS HDF5 summary file to Eclipse unformated ouput.
     *
     * \note
     * @param
     * @return
     */
    H5Conv(int argc, const char *argv[], const string PROJECT_SOURCE_DIR_);

    // Simple variables
    string file_path_;
    string ECL_DATA_FILE_PATH, BASE_NAME_ECL_OUTPUT,
           OUTPUT_DIRECTORY, ADGPRS_H5_FILE;

    Deck deck_;

    // const EclipseGrid& eclGrid;
    // [Keep for reference]
    // EclipseGrid ignores ACTNUM in Deck, and therefore needs ACTNUM
    // explicitly.  If a null pointer is passed, every cell is active.
    // EclipseGrid(const Deck& deck, const int * actnum = nullptr);

    // EclipseState eclState;

    /*!
     * Get json parameter file name
     */
    void getParamFileName(int argc, const char *argv[], const string PROJECT_SOURCE_DIR_);

    /*!
     * Read json parameter file and load conversion variables
     */
    void readParamFile(const string file_path);

    /*!
    * Establishes OPM Eclipse state, grid and EclipseWrite objects
    */
    void setupEclipseDataStructures();

    /*!
    * Return Deck object created in setupEclipseDataStructures()
    */
    Deck getDeck(){ return deck_; };

private:
};

#endif //H5_CONVERSIONTOOL_H5CONV_H
