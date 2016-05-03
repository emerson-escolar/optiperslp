#include "catch.hpp"

#include "optiperslp.h"


std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type> cube_factory() {
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
  return get_weighted_alpha_3_over_r(test_data);
}

TEST_CASE( "The whole enchilada", "[starttoend]" ) {  
  auto ans = cube_factory();
  
  Bdd_Type& bdd_matrix = std::get<0>(ans);
  AlphaMap_Type& alpha_map = std::get<1>(ans);   

  auto pers = do_optipers(bdd_matrix, alpha_map, 1, false);

  CHECK(pers.get_betti_number(0) == 8);
  CHECK(pers.get_betti_number(1) == 5);  
  CHECK(pers.get_betti_number(2) == 1);

  auto one_dim_ints = pers.intervals.at(1);
  for (const auto& an_interval : one_dim_ints) {
    CHECK(an_interval.generator.size() == 4);
  }
}




TEST_CASE( "The whole enchilada - nonfull filtration", "[starttoend]" ) {
  auto ans = cube_factory();
  
  // 8 vertices then 12 edges
  Bdd_Type bdd_matrix = Bdd_Type(std::get<0>(ans).begin(),
                                 std::get<0>(ans).begin() + 20);
  AlphaMap_Type alpha_map = AlphaMap_Type(std::get<1>(ans).begin(),
                                          std::get<1>(ans).begin() + 20);
  auto pers = do_optipers(bdd_matrix, alpha_map, 1, false);
  CHECK(pers.get_betti_number(0) == 8);
  CHECK(pers.get_betti_number(1) == 5);

  auto one_dim_ints = pers.intervals.at(1);
  for (const auto& an_interval : one_dim_ints) {
    CHECK(an_interval.generator.size() == 4);
  }
  
}
