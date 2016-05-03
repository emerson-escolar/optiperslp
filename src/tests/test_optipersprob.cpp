#include "catch.hpp"
#include "optipers_problem.h"


bool problem_factory(int csd, int cfd) {
  bool state = true;
  try {
    OptiPers_Problem x(csd,cfd);
  } catch (const std::exception & e) {
    std::cerr << e.what() << std::endl;
    state = false;
  }
  return state;
}


template <> struct OptiPers_Problem::access_by<OptiPers_Problem*> {
  access_by(OptiPers_Problem* _p_prb): p_prb(_p_prb){};
  OptiPers_Problem* p_prb; 

  // ****************************************
  // not really necessary, already public:
  void increment_lp_prob(unsigned int outside_cell_number){
    p_prb->increment_lp_prob(outside_cell_number);   
  }

  void increment_boundary(const Core::MapChain<double> & col){
    p_prb->increment_boundary(col);    
  }
  void standard_optimizer(const std::set<unsigned int>& cycles,
                          std::vector<Core::MapChain<double>>& basis) {
    p_prb->standard_optimizer(cycles, basis);
  }
  // ****************************************

  // access to internals!
  int get_sub_index()const {
    return p_prb->sub_index;
  }

  int get_cur_var_max()const {
    return p_prb->cur_var_max;
  }

  // access to helpers!
  void load_basis_chain_to_row_bounds(const Core::MapChain<double> & col) {
    p_prb->load_basis_chain_to_row_bounds(col);
    return; 
  }

  void load_basis_chain_to_col(const Core::MapChain<double>& col, int col_num) {
    p_prb->load_basis_chain_to_col(col, col_num);
    return;
  }
};
using OptiPers_Tester = OptiPers_Problem::access_by<OptiPers_Problem*>;



TEST_CASE("OptiPers_Problem Constructions", "[optipersprob]"){
  // Does not handle empty input!
  // REQUIRE_THROW( OptiPers_Problem x(0,0) );

  CHECK_FALSE(problem_factory(0,0) );
  CHECK_FALSE(problem_factory(-1,0) );
  CHECK_FALSE(problem_factory(-1,-10) );

  CHECK(problem_factory(1,0) );

  // glpk should be able to handle at least 2*64000 columns.
  // (number arbitrarily chosen.)
  CHECK(problem_factory(64000,0));   
}

TEST_CASE("OptiPers_Problem Incrementing", "[optipersprob]"){
  OptiPers_Problem a_problem(1, 0);
  OptiPers_Tester tester(&a_problem);
  
  CHECK_NOTHROW(tester.increment_lp_prob(0));

  Core::MapChain<double> col;
  col.set_entry(0,1);  
  CHECK_NOTHROW(tester.load_basis_chain_to_row_bounds(col)); 
}


TEST_CASE("OptiPers_Problem Incrementing - 2", "[optipersprob]") {
  OptiPers_Problem a_problem(1, 0);
  a_problem.increment_lp_prob(0);

  OptiPers_Tester tester(&a_problem);
  
  Core::MapChain<double> col;
  col.set_entry(0,1);    

  SECTION("Incrementing beyond current space dimension" ){
    CHECK_THROWS(tester.increment_lp_prob(1));
  }
  
  SECTION("Setting incompatible column as row bounds" ){
    col.set_entry(1,1);
    CHECK_THROWS(tester.load_basis_chain_to_row_bounds(col));
  }
}

TEST_CASE("OptiPers_Problem Boundary Incrementing", "[optipersprob]"){
  OptiPers_Problem a_problem(1, 0);
  a_problem.increment_lp_prob(0);
  OptiPers_Tester tester(&a_problem);
    
  Core::MapChain<double> col;
  col.set_entry(0,1);
  
  CHECK_NOTHROW(tester.increment_boundary(col));
  CHECK(tester.get_cur_var_max() == 1 * 2 + 1);
  CHECK_NOTHROW(tester.increment_boundary(col));
  CHECK(tester.get_cur_var_max() == 1 * 2 + 2);

  col.set_entry(1,1);
  CHECK_THROWS(tester.increment_boundary(col));
}
