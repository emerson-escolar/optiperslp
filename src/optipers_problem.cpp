#include "optipers_problem.h"
#include <cmath>
#include <cassert>

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

OptiPers_Problem::OptiPers_Problem(int _current_space_dimension,
                                   int _coface_space_dimension):
    current_space_dimension(_current_space_dimension),
    coface_space_dimension(_coface_space_dimension) {
  
  do_mip = true;

  #ifdef DEBUG
  debug_mode = true;
  #else
  debug_mode = false;
  #endif

  
  coeff_eps = 0.0001;
  
  sub_index = 0;
  cur_var_max = 0;  
  cycles_optimized = 0;  

  if ( current_space_dimension <= 0 or coface_space_dimension < 0 ){
    throw ProblemSizeError(current_space_dimension,
                           coface_space_dimension);
  }
  
  lp = glp_create_prob();
  
  glp_set_obj_dir(lp, GLP_MIN); 
  glp_add_cols(lp, 2 * current_space_dimension);
  
  for ( int ii = 0; ii < current_space_dimension; ++ii) {    	    
    glp_set_obj_coef(lp, 1 + ii, 0);
    glp_set_obj_coef(lp, 1 + ii + current_space_dimension, 0);

    glp_set_col_bnds(lp, 1 + ii, GLP_FX, 0, 0);
    glp_set_col_bnds(lp, 1 + ii + current_space_dimension, GLP_FX, 0, 0);

    glp_set_col_kind(lp, 1 + ii, GLP_IV);
    glp_set_col_kind(lp, 1 + ii + current_space_dimension, GLP_IV);
  } 
  cur_var_max = 2 * current_space_dimension;

  assert( glp_get_num_rows(lp) == sub_index );
  assert( glp_get_num_cols(lp) == cur_var_max );

  glp_init_smcp(&parm);
  //parm.msg_lev = GLP_MSG_ALL;
  parm.msg_lev = GLP_MSG_ERR;
  parm.meth = GLP_DUALP;

  
  glp_init_iocp(&parm_i);
  parm_i.msg_lev = GLP_MSG_ERR;  
}

void OptiPers_Problem::increment_lp_prob(unsigned int outside_cell_number) {
  assert( glp_get_num_rows(lp) == sub_index );
  
  if (sub_index >= current_space_dimension){throw IncrementLPError();}

  // int *ib = new int [2];
  // double *ab = new double [2];
  int ib[2] = {};
  double ab[2] = {};
  
  glp_add_rows(lp, 1); 
      
  glp_set_obj_coef(lp, 1 + sub_index, 1);
  glp_set_obj_coef(lp, 1 + sub_index + current_space_dimension, 1);

  // Identity matrix elements: 
  ib[1] = 1 + sub_index;
  ab[1] = 1;
  glp_set_mat_col(lp, 1 + sub_index, 1, ib, ab);

  ab[1] = -1;
  glp_set_mat_col(lp, 1 + sub_index + current_space_dimension,
                  1, ib, ab); 

  glp_set_col_bnds(lp, 1 + sub_index, GLP_LO, 0, 1);
  glp_set_col_bnds(lp, 1 + sub_index +
                   current_space_dimension, GLP_LO, 0, 1);
  // delete [] ib;
  // delete [] ab;

  order_to_prob_index[outside_cell_number] = sub_index;
  prob_index_to_order[sub_index] = outside_cell_number;

  ++sub_index;
  return;
}


int OptiPers_Problem::load_basis_chain_to_row_bounds(const Core::MapChain<double>& col){
  int nonzeros = 0;
  // first, cleanup
  for (int ii = 0; ii < sub_index; ++ii) {
    glp_set_row_bnds(lp, 1+ii, GLP_FX, 0, 0);
  }
    
  for (const auto & ind_coeff_pair : col) {
    double coeff = ind_coeff_pair.second; 
    if ( fabs(coeff) > coeff_eps ) {            
      int uu = order_to_prob_index.at( ind_coeff_pair.first ); 
      glp_set_row_bnds(lp, 1 + uu, GLP_FX, coeff, coeff); 
      ++nonzeros;            
    } 
  }
  return nonzeros;
}

void OptiPers_Problem::load_basis_chain_to_col(const Core::MapChain<double>& col, int col_num){  
  int *ib = new int [1 + col.size()];
  double *ab = new double [1 + col.size()]; 
  int b_mem_index = 1;

  for (const auto & ind_coeff_pair : col) {    
    double coeff = ind_coeff_pair.second; 
    if ( fabs(coeff) > coeff_eps ) {
      int uu = order_to_prob_index.at(ind_coeff_pair.first);
      assert(b_mem_index < 1+col.size());
      ib[b_mem_index] = 1 + uu;
      ab[b_mem_index] = coeff;
      ++b_mem_index; 
    } 
  }
  glp_set_mat_col(lp, col_num, b_mem_index-1, ib, ab);

  delete [] ib;
  delete [] ab;
  
  return;
}

void OptiPers_Problem::increment_boundary(const Core::MapChain<double> & col) {
  assert( glp_get_num_cols(lp) == cur_var_max );
  
  glp_add_cols(lp, 1);			            
  glp_set_obj_coef(lp, 1+cur_var_max,0);
  glp_set_col_bnds(lp, 1+cur_var_max, GLP_FR, 0, 0);    
  
  load_basis_chain_to_col(col, 1 + cur_var_max);
  ++cur_var_max;  
  return; 
}

void OptiPers_Problem::print_stats(std::ostream& os)const {
  os << "****************************************\n";   
  os << "Subindex: " << sub_index << "\n";
  os << "Numrows: " << glp_get_num_rows(lp) << "\n";
  os << "curvarmax: " << cur_var_max << "\n";
  os << "Numcols: " << glp_get_num_cols(lp) << "\n";
  os << "Cycles optimized: " << cycles_optimized
            << "\n";

  os << "Objective value: " <<  glp_get_obj_val(lp) << "\n";
  os << "MIP Objective value: " <<  glp_mip_obj_val(lp) << "\n";
  return; 
}



std::ostringstream ProblemSizeError::probsizeerr_msg;

