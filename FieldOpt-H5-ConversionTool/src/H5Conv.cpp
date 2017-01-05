//
// Created by bellout on 1/5/17.
//

#include "H5Conv.h"

using boost::property_tree::ptree;
using namespace Opm;

H5Conv::H5Conv(int argc, char *argv, const string PROJECT_SOURCE_DIR_){

    getParamFileName(argc, argv, PROJECT_SOURCE_DIR_);
    readParamFile(file_path_);
    setupEclipseDataStructures();

}

void H5Conv::setupEclipseDataStructures(){

    // Read Eclipse deck; PARSER using with specific error-handling settings;
    // alt: InputError::IGNORE
    auto deck = Parser().parseFile(ECL_DATA_FILE_PATH, Opm::ParseContext({
               { ParseContext::PARSE_RANDOM_TEXT , InputError::WARN },
               { ParseContext::PARSE_UNKNOWN_KEYWORD , InputError::WARN },
               { ParseContext::PARSE_RANDOM_SLASH , InputError::WARN },
               { ParseContext::SUMMARY_UNKNOWN_WELL , InputError::WARN} }));

    // Set up Eclipse state and grid objects
    EclipseState eclState = Parser::parse(deck);
    eclState.getIOConfig().setBaseName(BASE_NAME_ECL_OUTPUT);
    eclState.getIOConfig().setOutputDir(OUTPUT_DIRECTORY);
    auto& eclGrid = eclState.getInputGrid(); // EclipseGrid

    EclipseWriter eclWriter(eclState, eclGrid);
    eclWriter.writeInitial();
    data::Wells wells;
}

void H5Conv::getParamFileName(int argc, char *argv, const string PROJECT_SOURCE_DIR_){

    if (argc >= 2){
        file_path_ = argv[1]; // char
    }else{
        cout << "No parameter file passed as argument. "
                "Correct usage is: " << argv[0] << "/path/to/conv-params.json" << endl;

        file_path_ = PROJECT_SOURCE_DIR_ + "/input/conv-params.json";
        cout << "Using input parameters from default file:\n"
             << file_path_ << endl;
    }
}

void H5Conv::readParamFile(const string file_path){

    // Read json parameter file
    ptree pt;
    read_json(file_path, pt);

    // Load conversion variables
    ECL_DATA_FILE_PATH = pt.get<std::string>("ECL_DATA_FILE_PATH");
    BASE_NAME_ECL_OUTPUT = pt.get<std::string>("BASE_NAME_ECL_OUTPUT");
    OUTPUT_DIRECTORY = pt.get<std::string>("OUTPUT_DIRECTORY");
    ADGPRS_H5_FILE = pt.get<std::string>("ADGPRS_H5_FILE");

    // Echo variables
    cout << "The following conversion parameters have been provided:" << endl;
    cout << "ECL_DATA_FILE_PATH:" << ECL_DATA_FILE_PATH << endl;
    cout << "BASE_NAME_ECL_OUTPUT:" << BASE_NAME_ECL_OUTPUT << endl;
    cout << "OUTPUT_DIRECTORY:" << OUTPUT_DIRECTORY << endl;
    cout << "ADGPRS_H5_FILE:" << ADGPRS_H5_FILE << endl;
}