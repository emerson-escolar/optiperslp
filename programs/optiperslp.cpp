/*  
    By: Emerson Escolar
        
    OptiPersLP is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License 
    as published by the Free Software Foundation, 
    either version 3 of the License, or
    (at your option) any later version.

    OptiPersLP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the 
    GNU Lesser General Public License along with OptiPersLP.
    If not, see <http://www.gnu.org/licenses/>. 
*/

/* Info: 
   Adopted from both the vespers project and optipers project.   
*/

#include <fstream>
#include <list>
#include <cassert>
#include <sstream>
#include <unistd.h>

#include <cmath>

#include "optiperslp.h"

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem;

void show_help(){
  std::cerr << "OptiPersLP: a persistent homology optimal cycles utility program.\n";
	
  std::cerr << "\nUsage: <program name> [options] <input file name>\n";
  std::cerr << "Options:\n";	
  std::cerr << "\t -h: Show this help message.\n";
  std::cerr << "\t -d: Choose the target dimension of cycles to optimize. Defaults to d=1\n";
  std::cerr << "\t -c: Ask to confirm output files before doing anything.\n";
  std::cerr << "\t -n <name>: (Optional) Specify output name.\n\t\tDefaults to determining name from basename of input file.\n\t\tOutput files become gen_<name>_d.txt and gen_<name>_i2p.txt,\n\t\twhere d is the chosen dimension.\n";
  std::cerr << "\t -p <path>: Specify directory in which to place outputs.\n";
  
    

  std::cerr << "Advanced options (You can probably ignore these):\n";
  std::cerr << "\t -i: Optimize cycles immediately.\n\t\t(i.e. do not wait for alpha value to change.)\n";
  std::cerr << "\t -l: Skip doing MIP if non-integral solution is found.\n";
  

  std::cerr << "Output:\n If succesful, this outputs persistence diagrams of the chosen dimension\n of the alpha shape filtration of the input data.\n Output files are placed in the directory where this is run.";
  
  std::cerr << "\nAcknowledgments:\n OptiPersLP is built on top of and uses the following libraries:\n";
  std::cerr << "\tThe GNU Linear Programming Kit (GLPK) (https://www.gnu.org/software/glpk/)\n";
  std::cerr << "\tThe Computational Geometry Algorithms Library (CGAL) (http://www.cgal.org)\n";
  std::cerr << "\tBoost (http://www.boost.org)\n";
  
  std::cerr << "\t... and their respective dependencies.\n\n";
		
  std::cerr << "OptiPersLP Copyright (C) 2016\nThis program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it\nunder certain conditions; see the COPYING file for details.\n";
	
  return;
}


int main(int argc, char** argv) {
  int c;
  bool name_provided = false;
  bool path_provided = false;
  bool debug_mode = false;

  bool opt_immediately = false; 
  bool do_mip = true;

  bool check_output_paths = false;
  
  std::string name;
  std::string output_path_str;
  
  if (argc == 1) {
    show_help();
    return 0;
  }

  int target_dim = 1;
  
  while ((c = getopt(argc,argv,"hvcd:n:p:il")) != -1) {
    switch(c) {
      case 'h':
        show_help();
        break;
      case 'c':
        check_output_paths = true;
        break;
      case 'n': 
        name_provided = true;
        name = optarg;
        break;
      case 'p': 
        path_provided = true;
        output_path_str = optarg;
        break;
      case 'd': 
        target_dim = std::stoi(std::string(optarg));
        break;
      case 'i':
        opt_immediately = true;
        break;
      case 'l':
        do_mip = false;
        break;      
      case 'v':
        debug_mode = true;
        break;
    }
  }
  if (argc-optind == 0) {
    std::cerr << "Need input file" << std::endl;
    std::exit(1); 
  }

  // operate on input file.
  bf::path input_p(argv[optind]);
  if (!bf::exists(input_p)) {
    std::cerr << "Input file not found!" << std::endl;
    exit(1);
  }
  if (!bf::is_regular_file(input_p)) {
    std::cerr << "Input file not a regular file!" << std::endl;
    exit(1);
  }  

  bf::path output_path;
  if (path_provided){
    // operate on output path.
    output_path = output_path_str;    
  } else {
    output_path = std::string("./");
  }
  if (!bf::exists(output_path) or !bf::is_directory(output_path)) {
    std::cerr << "Output directory:\n" <<
        output_path << "\nnot found or is not a directory!" << std::endl;
    exit(1);
  }    

  // output names  
  if (!name_provided) {
    // infer names from input...
    name = input_p.stem().string(); 
  }
  
  // name transformations.
  bf::path i2p_p = output_path;
  i2p_p /= (std::string("gen_") + name + "_i2p.txt");
  bf::path gen_file_p = output_path;
  gen_file_p /= (std::string("gen_") + name + "_" + std::to_string(target_dim) + ".txt");

  if ( check_output_paths ) {
    std::cout << "Please confirm output files:\n";
    std::cout << "Index to point file: " << i2p_p.string() << std::endl;
    std::cout << "Generators file: " << gen_file_p.string() << std::endl;
    std::string yn;
    std::cout << "Are these OK? (y/n)...";
    std::cin >> yn;
    if (yn != std::string("y")){
      std::cout << "Terminated by user.\n";
      exit(1);
    }
  }

  // Before doing computations check that file is writable...
  // (does not guarantee that file is writable when computation
  // ends, but at least we can avoid computing if we can
  // reasonably assume that the output file is misspecified...)  
  std::ofstream dummyfile(gen_file_p.string());
  if (dummyfile.is_open()) {
    dummyfile.close();
  } else {
    std::cerr << "Failed to open output file:\n" << gen_file_p.string() << "\n";
    std::cerr << "Please check that you have the correct permissions!\n";
    exit(1);
  } 
  

  std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type> ans =
      get_weighted_alpha_3_over_r(input_p.string());
  
  Bdd_Type& bdd_matrix = std::get<0>(ans);
  AlphaMap_Type& alpha_map = std::get<1>(ans);
  I2P_Type& i2p = std::get<2>(ans); 

  // Write i2p:
  std::ofstream i2p_file(i2p_p.string());
  if (i2p_file.is_open()) {
    for (auto a_pair : i2p) {
      i2p_file << a_pair.first << " "; 
      
      i2p_file << std::get<0>(a_pair.second) << " ";
      i2p_file << std::get<1>(a_pair.second) << " ";
      i2p_file << std::get<2>(a_pair.second) << " "; 
      i2p_file << std::get<3>(a_pair.second) << std::endl;
    }
    i2p_file.close();
  } else {
    std::cerr << "Failed to open output file:\n" << i2p_p.string() << "\n";
    std::cerr << "Please check that you have the correct permissions!\n";
    exit(1);
  }

  Persistence::Persistence<double,double> pers = do_optipers(bdd_matrix, alpha_map, target_dim, opt_immediately);
  bool success = pers.write_file_dim(gen_file_p.string(), target_dim);

  if (!success) {
    std::cerr << "Failed to open output file:\n" << gen_file_p.string() << "\n";
    std::cerr << "Please check that you have the correct permissions!\n";
    exit(1);
  }

  return 0;


}
