//
// Created by bellout on 1/5/17.
//

#include "H5Conv.h"
#include "src/colors.hpp"

using boost::property_tree::ptree;
using namespace Opm;
using namespace std;

H5Conv::H5Conv(int argc,
               const char *argv[],
               const string PROJECT_SOURCE_DIR_){

    getParamFileName(argc, argv, PROJECT_SOURCE_DIR_);
    readParamFile(file_path_);
    setupEclipseDataStructures();

}

void H5Conv::setupEclipseDataStructures() {

    bool debug = true;

    // Read Eclipse deck; PARSER using with specific error-handling settings;
    // alt: InputError::IGNORE
    if ( !boost::filesystem::exists(ECL_DATA_FILE_PATH) ) {
        throw std::runtime_error("ECL_DATA_FILE_PATH not found!");
    } else {
        cout << "ECL_DATA_FILE_PATH exists." << endl
             << CYAN << "Parse deck..." << END << endl;
        try {
            deck_ = Parser().parseFile(
                ECL_DATA_FILE_PATH, Opm::ParseContext(
                    {{ParseContext::PARSE_MISSING_DIMS_KEYWORD, InputError::WARN},
                     {ParseContext::PARSE_RANDOM_TEXT, InputError::WARN},
                     {ParseContext::PARSE_UNKNOWN_KEYWORD, InputError::WARN},
                     {ParseContext::PARSE_RANDOM_SLASH, InputError::WARN},
                     {ParseContext::SUMMARY_UNKNOWN_WELL, InputError::WARN}}));

            if(debug) {
                cout << "-- Deck status --" << endl;
                cout << "Total # of keywords: " << deck_.size() << endl;
                cout << "Unit system: " << deck_.getActiveUnitSystem().getName() << endl;
                for(int ii=0; ii < deck_.size(); ++ii) {
                    if (deck_.getKeyword(ii).name() == "INIT" ||
                        deck_.getKeyword(ii).name() == "GRID")
                    cout << "Keyword[" << ii << "]:"
                         << deck_.getKeyword(ii).name() << " -- ";
                }
                cout << endl;
            }
        } catch(const exception &e) {
            cout << "Parser crashed with: " << e.what() << endl;
        }
    }
}

void H5Conv::getParamFileName(int argc,
                              const char *argv[],
                              const string PROJECT_SOURCE_DIR_){

    if (argc >= 2){
        file_path_ = argv[1]; // char
    }else{
        cout << "No parameter file passed as argument. "
                "Correct usage is: " << argv[0]
             << " /path/to/conv-params.json" << endl;

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
    cout << "----------------------------------------------------"
         << "----------------------------------------------------" << endl;
    cout << "The following conversion parameters have been provided:" << endl;
    cout << CYAN << "ECL_DATA_FILE_PATH: " << END << ECL_DATA_FILE_PATH << endl;
    cout << CYAN << "BASE_NAME_ECL_OUTPUT: " << END << BASE_NAME_ECL_OUTPUT << endl;
    cout << CYAN << "OUTPUT_DIRECTORY: " << END << OUTPUT_DIRECTORY << endl;
    cout << CYAN << "ADGPRS_H5_FILE: " << END << ADGPRS_H5_FILE << endl;
}