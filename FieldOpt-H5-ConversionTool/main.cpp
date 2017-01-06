
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
using namespace std;
using std::string;
using data::TargetType;
using measure = UnitSystem::measure;

void outputEclipseDataFiles(Hdf5SummaryReader H5Data,
                            EclipseWriter &eclWriter,
                            data::Wells wells){

    auto pressure = H5Data.reservoir_pressure();
    auto sgas = H5Data.sgas();
    auto soil = H5Data.soil();
    auto swat = H5Data.swat();

    int last_tstep = (int)H5Data.times_steps().size();
    int first_tstep = 0;

    // to output folder defined in parameter file
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
        eclWriter.writeTimeStep(i, false, time_step, sol, wells);
    }

}

int main(int argc, const char *argv[]) {

    // Setup Eclipse data structures
    H5Conv Conversion(argc, argv,(string)PROJECT_SOURCE_DIR);

    // ------
    // todo
    // Note: This part should be inside H5Conv::setupEclipseDataStructures(),
    // but I'm having trouble initializing reference variables, or something...
    // help.
    //
    // Set up Eclipse state and grid objects
    EclipseState eclState = Parser::parse(Conversion.getDeck());
    eclState.getIOConfig().setBaseName(Conversion.BASE_NAME_ECL_OUTPUT);
    eclState.getIOConfig().setOutputDir(Conversion.OUTPUT_DIRECTORY);
    const EclipseGrid& eclGrid = eclState.getInputGrid(); // const EclipseGrid&

    EclipseWriter eclWriter(eclState, eclGrid);
    eclWriter.writeInitial();
    data::Wells wells;
    // ------

    // Read H5 data
    Hdf5SummaryReader H5Data(Conversion.ADGPRS_H5_FILE);

    // Fill H5 data into Eclipse structures, then print Eclipse result files
    outputEclipseDataFiles(H5Data, eclWriter, wells);

    cout << "----------------------------------------------------"
         << "----------------------------------------------------\n"
         << "Ignore the EGRID warning. Eclipse output files: "
         << " *.EGRID, *.INIT, *.RFT, *.SMSPEC, *.UNRST, *.UNSMRY\n"
         << "should have been created in output folder: "
         << Conversion.OUTPUT_DIRECTORY << "\n"
         << "You can now use ResInsight to analyze the results." << endl;

    return 0;
}

// ------
// todo: exception handling
// * For some yet unknown reason, the number of TSTEP calls in the Eclipse
// DATA file needs to be equal or larger than the number of TSTEP calls in
// the AD-GPRS simulation. Throw an exception call when this occurs and/or
// fix this somehow. Probably an allocation issue.
// Note: the STEP sizes need _not_ be the same, just the number of TSTEP calls.

// ------
// todo: tests
// * supply a faulty parameter file, i.e., test parameter read function

