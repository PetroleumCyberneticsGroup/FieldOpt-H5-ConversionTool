
#include <iostream>
//#include <fstream>

// BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <boost/property_tree/detail/json_parser_read.hpp>
//#include <boost/property_tree/detail/json_parser_write.hpp>
//#include <boost/property_tree/detail/json_parser_error.hpp>

// OPM: PARSER
#include <opm/parser/eclipse/Parser/ParseContext.hpp>
#include <opm/parser/eclipse/Parser/Parser.hpp>
#include <opm/parser/eclipse/Utility/Functional.hpp>

// OPM: OUTPUT
#include <opm/output/eclipse/EclipseWriter.hpp>
//#include <opm/output/eclipse/EclipseReader.hpp>

#include "src/H5Conv.h"

#include <vector>
#include <H5Cpp.h>

// FIELDOPT: HDF5 SUMMARYREADER
#include <hdf5_summary_reader.h>

// ERT

// NAMESPACES
//using boost::property_tree::ptree;
using std::string;
using data::TargetType;
using measure = UnitSystem::measure;

int main(int argc, char *argv[]) {

    // Setup Eclipse data structures
    H5Conv Conversion(argc,*argv,(string)PROJECT_SOURCE_DIR);

    // Read H5 data
    Hdf5SummaryReader H5Data(Conversion.ADGPRS_H5_FILE);
    auto pressure = H5Data.reservoir_pressure();
    auto sgas = H5Data.sgas();
    auto soil = H5Data.soil();
    auto swat = H5Data.swat();

    int first_tstep = 0;
    int last_tstep = (int)H5Data.times_steps().size();

    // Fill H5 data into Eclipse structures, then print Eclipse output files
    for( int i = first_tstep; i < last_tstep; ++i ) {

        time_t time_step = (time_t)(H5Data.times_steps()[i]*24*60*60); // convert tstep to secs

        data::Solution sol;
        sol.insert( "PRESSURE", measure::pressure , pressure[i], TargetType::RESTART_SOLUTION );
        sol.insert( "SGAS",     measure::identity , sgas[i], TargetType::RESTART_SOLUTION );
        sol.insert( "SOIL",     measure::identity , soil[i], TargetType::RESTART_SOLUTION );
        sol.insert( "SWAT",     measure::identity , swat[i], TargetType::RESTART_SOLUTION );
        // sol.insert( "RS",       measure::identity , pressure[i], TargetType::RESTART_SOLUTION );
        // sol.insert( "RV",       measure::identity , pressure[i], TargetType::RESTART_SOLUTION );

        // writeTimeStep() function accumulates given time steps
        Conversion.eclWriter->writeTimeStep(i, false, time_step, sol, Conversion.wells);
    }

    return 0;
}
