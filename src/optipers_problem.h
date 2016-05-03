#ifndef OPTIPERS_PROBLEM_H
#define OPTIPERS_PROBLEM_H

/*  
    By: Emerson Escolar
    
    This file part of OptiPersLP.

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

#include "common_definitions.h"
#include <glpk.h>

#include <exception>
#include <stdexcept>
#include <sstream>

class OptiPers_Problem {
 public:
  OptiPers_Problem(int _current_space_dimension,
                   int _coface_space_dimension);
  
  OptiPers_Problem(const OptiPers_Problem&) = delete;
  OptiPers_Problem& operator=(const OptiPers_Problem&) = delete;

  ~OptiPers_Problem() {
    glp_delete_prob(lp);
  }
  
  
  void increment_lp_prob(unsigned int outside_cell_number); 
  void increment_boundary(const Core::MapChain<double> & col);  
  void standard_optimizer(const std::set<unsigned int>& cycles,
                          std::vector<Core::MapChain<double>>& basis);  

  bool query_has_var_index_cell(unsigned int outside_cell_number) const {
    return (birth_of_cycle_to_var_index.count(outside_cell_number));
  }
  
  int query_var_index_of_cell(unsigned int outside_cell_number) const {    
    return birth_of_cycle_to_var_index.at(outside_cell_number); 
  }

 protected:
  glp_prob* lp;
  
  double coeff_eps; // ... 
  
  int current_space_dimension;
  int coface_space_dimension;

  
  int sub_index;
  int cur_var_max;
  int cycles_optimized;

  bool do_mip;  
  bool debug_mode;

  glp_smcp parm;
  glp_iocp parm_i;
  
  // LP problem does not involve all cells.
  // We have to map, for all cells with dim target_dim:
  //    - indices of those cells in vec_cells
  //    - indices of those cells, in order of appearance in filtration.
  std::map<unsigned int, int> order_to_prob_index;
  std::map<int, unsigned int> prob_index_to_order;

  // Record the position of a module cycle (which was minimized).
  // When the cycle dies, we can avoid inserting a boundary cycle,
  // since the boundary cycle can be written as a LC of the present cycles.
  std::map<int, int> birth_of_cycle_to_var_index;

  // helper functions
  int load_basis_chain_to_row_bounds(const Core::MapChain<double>& col);
  void load_basis_chain_to_col(const Core::MapChain<double>& col, int col_num);

  void print_stats(std::ostream& os)const ;

 public:
  // for testing of protected stuff via friends
  template <class T> struct access_by;
  template <class T> friend struct access_by;
};


class ProblemSizeError : public std::runtime_error {  
public:
  ProblemSizeError(int _csd, int _cfd)
    : runtime_error( "problem size error" ), csd( _csd ), cfd( _cfd )
    {}

  int getcsd() const{return csd;}
  int getcfd() const{return cfd;}

  virtual const char* what() const throw() {
    probsizeerr_msg.str("");
    probsizeerr_msg << runtime_error::what()
                    << ": in initialization of OptiPers_Problem object,"
                    << " csd = "
                    << getcsd()
                    << " and cfd = "
                    << getcfd();
    return probsizeerr_msg.str().c_str();
  }

 private:
  int csd;
  int cfd;
  static std::ostringstream probsizeerr_msg;
};

class IncrementLPError : public std::runtime_error {  
public:
  IncrementLPError()
    : runtime_error( "increment lp error" )
    {}
  
  virtual const char* what() const throw() {   
    return "increment lp error:  attempted to increment lp problem beyond current_space_dimension given."; 
  } 
  
};

  

#endif
