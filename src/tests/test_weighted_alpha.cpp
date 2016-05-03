#include "catch.hpp"
#include <iostream>
#include <sstream>
#include <map>
#include "weighted_alpha_3_over_r.h"


std::map<int, unsigned int> count_cells_dimensionwise(const AlphaMap_Type& amap) {  
  std::map<int, unsigned int> dim_to_sizes;
  for (const DimAlpha_Type& dimalpha : amap) {
    int dim = std::get<0>(dimalpha);
    if (dim_to_sizes.count(dim)){
      ++dim_to_sizes[dim];
    } else {
      dim_to_sizes[dim] = 1;
    }
  }
  return dim_to_sizes;
}

TEST_CASE( "Weighted alpha shape on cube", "[alphashape]" ) {
  // Note: The cube is actually not in general position.
  // So I'm not sure what we're really testing here.
  // CGAL seems to give a correct answer, though.
  // From what I understand with degeneracy, the alpha complex
  // is not uniquely defined (??)
  // At least the multiplicities should be well-defined.
  
  // hardcode a cube
  std::stringstream test_data;
  for (int i = -1; i < 2; i+=2) {
    for (int j = -1; j < 2; j+=2) {
      for (int k = -1; k < 2; k+=2) {
        test_data << std::to_string(2*i) << " "
                  << std::to_string(2*j) << " "
                  << std::to_string(2*k) << " 0.0\n";
      }
    }
  }
  
  auto ans = get_weighted_alpha_3_over_r(test_data);  
  REQUIRE( std::get<1>(ans).size() == 51 );

  auto dim_to_sizes = count_cells_dimensionwise(std::get<1>(ans));

  // the correct answer for a triangulation on the cube.
  REQUIRE( dim_to_sizes.size() == 4 );
  REQUIRE( dim_to_sizes.at(0) == 8 );
  REQUIRE( dim_to_sizes.at(1) == 19 );
  REQUIRE( dim_to_sizes.at(2) == 18 );
  REQUIRE( dim_to_sizes.at(3) == 6 ); 
}

TEST_CASE( "Test supported delimiters", "[alphashape]" ) {
  std::vector<std::string> delims = {" ",  "	", "  ", " 	", ",", ", "};
  // delims here are: space, tab, spacespace, spacetab, comma, commaspace
  for (auto delim : delims) {
    // hardcode a cube    
    std::stringstream test_data;
    for (int i = -1; i < 2; i+=2) {
      for (int j = -1; j < 2; j+=2) {
        for (int k = -1; k < 2; k+=2) {
          test_data << std::to_string(2*i) << delim
                    << std::to_string(2*j) << delim
                    << std::to_string(2*k) << delim << "0.0\n";
        }
      }
    }  
    auto ans = get_weighted_alpha_3_over_r(test_data);  
    REQUIRE( std::get<1>(ans).size() == 51 );
  } 
}

TEST_CASE( "Weighted alpha shape on a weighted cube", "[alphashape]" ) {
  // hardcode a cube
  std::stringstream test_data;
  for (int i = -2; i < 5; i+=4) {
    for (int j = -2; j < 5; j+=4) {
      for (int k = -2; k < 5; k+=4) {
        test_data << std::to_string(2*i) << " "
                  << std::to_string(2*j) << " "
                  << std::to_string(2*k) << " 2.0\n";
      }
    }
  }
  
  auto ans = get_weighted_alpha_3_over_r(test_data);  
  REQUIRE( std::get<1>(ans).size() == 51 );

  auto dim_to_sizes = count_cells_dimensionwise(std::get<1>(ans));

  // the correct answer for a triangulation on the cube.
  REQUIRE( dim_to_sizes.size() == 4 );
  REQUIRE( dim_to_sizes.at(0) == 8 );
  REQUIRE( dim_to_sizes.at(1) == 19 );
  REQUIRE( dim_to_sizes.at(2) == 18 );
  REQUIRE( dim_to_sizes.at(3) == 6 ); 
}



