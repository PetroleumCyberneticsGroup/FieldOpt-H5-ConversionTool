
#include <sstream>
#include <string>
#include <iostream>

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

// H5CONV
#include "src/H5Conv.h"

#include <vector>
#include <H5Cpp.h>

// FIELDOPT: HDF5 SUMMARYREADER
#include <hdf5_summary_reader.h>
#include <stdlib.h>

// LAYOUT
#include "src/colors.hpp"

// NAMESPACES
//using boost::property_tree::ptree;
using namespace std;
using std::string;
using data::TargetType;
using measure = UnitSystem::measure;

void outputEclipseDataFiles(Hdf5SummaryReader H5Data,
                            EclipseWriter &eclWriter,
                            data::Wells wells,
                            EclipseGrid eclGrid){

  bool debug = true;
  int first_tstep = 0, last_tstep = 0;
  int actnum_h5, actnum_ecl;
  ostringstream oss;

  auto pressure = H5Data.reservoir_pressure();
  auto sgas = H5Data.sgas();
  auto soil = H5Data.soil();
  auto swat = H5Data.swat();

  if (debug) {
    cout << "size of pres vector: " << pressure.size() << endl;
    cout << "size of sgas vector: " << sgas.size() << endl;
    cout << "size of soil vector: " << soil.size() << endl;
    cout << "size of swat vector: " << swat.size() << endl;
  }

  try {
    last_tstep = (int)H5Data.times_steps().size();
  }
  catch (const runtime_error &e) {
    throw runtime_error("Time step # is zero.");
  }

  if (debug) {
    cout << "first_tstep: " << first_tstep << endl;
    cout << "last_tstep: " << last_tstep << endl;
  }

  if(pressure.size() < 1){
    throw runtime_error("Pressure vector has size zero.");
  }

  // Check if active cell number for h5 data is the same as ground Eclipse file
  try {
    actnum_h5 = (int)pressure[0].size();
    actnum_ecl = (int)eclGrid.getNumActive();
  }
  catch (const runtime_error &e) {
    throw runtime_error("Could not access h5 data.");
  }
  cout << "# of active cells from h5 data: " << YELLOW << actnum_h5 << END << endl;
  cout << "# of active cells from ecl data: " << YELLOW << actnum_ecl << END << endl;

  cout << "tsteps: " << endl;
  int nsteps = 25;

  if( actnum_h5 == actnum_ecl ) {
    // to output folder defined in parameter file
    for( int i = first_tstep; i < last_tstep; ++i ) {

      time_t time_step = (time_t)(H5Data.times_steps()[i]*24*60*60); // convert tstep to secs

      auto rem = div(i,nsteps);
      cout << " .. " << i;
      if ( rem.rem == nsteps-1 ) {
        cout << endl;
      }

      data::Solution sol = new data::Solution;

      sol.insert( "PRESSURESS", measure::pressure , pressure[i], TargetType::SUMMARY );
      sol.insert( "SGASS",     measure::identity , sgas[i], TargetType::SUMMARY );
      sol.insert( "SOILS",     measure::identity , soil[i], TargetType::SUMMARY);
      sol.insert( "SWATS",     measure::identity , swat[i], TargetType::SUMMARY);

      sol.insert( "PRESSURE", measure::pressure , pressure[i], TargetType::RESTART_SOLUTION );
      sol.insert( "SGAS",     measure::identity , sgas[i], TargetType::RESTART_SOLUTION );
      sol.insert( "SOIL",     measure::identity , soil[i], TargetType::RESTART_SOLUTION );
      sol.insert( "SWAT",     measure::identity , swat[i], TargetType::RESTART_SOLUTION );
      // sol.insert( "RS",       measure::identity , pressure[i], TargetType::RESTART_SOLUTION );
      // sol.insert( "RV",       measure::identity , pressure[i], TargetType::RESTART_SOLUTION );

      // writeTimeStep() function accumulates given time steps
      eclWriter.writeTimeStep(i, false, time_step, sol, wells);
    }
  } else {

    oss << "Error: actnum of h5 data (" << actnum_h5 << ") "
        << "is not equal to actnum of ecl data (" << actnum_ecl << ")" << endl
        << "No PRESSURE, SGAS, SOIL, SWAT vs TIME will be available" << endl;
    cout << RED << oss.str() << END;
//        throw runtime_error("No PRESSURE, SGAS, SOIL, SWAT vs TIME will be available.\n");
  }



  cout << endl;
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
  cout << CYAN << "Setup eclState object..." << END << endl;
  EclipseState eclState = Parser::parse(Conversion.getDeck());
  cout << CYAN << "SetBaseName..." << END << endl;
  eclState.getIOConfig().setBaseName(Conversion.BASE_NAME_ECL_OUTPUT);
  eclState.getIOConfig().setOutputDir(Conversion.OUTPUT_DIRECTORY);

  cout << CYAN << "Setup eclGrid object..." << END << endl;
  const EclipseGrid& eclGrid = eclState.getInputGrid(); // const EclipseGrid&

  cout << CYAN << "Setup eclWriter object..." << END << endl;
  EclipseWriter eclWriter(eclState, eclGrid);
  eclWriter.writeInitial();
  data::Wells wells;
  // ------

  // Read H5 data
  /*!
   * Read the HDF5 summary file written
   * by AD-GPRS at the specified path.
   *
   * \note The provided path is not checked by this method,
   * and should therefore be checked before invoking this.
   * @param file_path Path to a .H5 summary file.
   * @param get_cell_data Flag for whether to read cell data
   * or not (defaults to empty string)
   * @param debug Flag to print H5 related data during testing
   * (defaults to false)
   * @return A Hdf5SummaryReader object containing
   * information from the summary.
   *
   * Hdf5SummaryReader(const std::string file_path,
   * bool get_cell_data = false,
   * bool debug = false);
   */

  cout << "Calling Hdf5SummaryReader..." << endl;
  Hdf5SummaryReader H5Data(Conversion.ADGPRS_H5_FILE, true, false);

  // Fill H5 data into Eclipse structures, then print Eclipse result files
  outputEclipseDataFiles(H5Data, eclWriter, wells, eclGrid);

  cout << "----------------------------------------------------"
       << "----------------------------------------------------\n"
       << "Ignore the (IOConfig) warning about GRID files. Files:"
       << " *.EGRID, *.INIT, *.RFT, *.SMSPEC, *.UNRST, *.UNSMRY\n"
       << "should have been created in provided output folder: "
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

