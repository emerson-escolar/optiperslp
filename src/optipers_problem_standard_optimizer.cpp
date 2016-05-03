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

void OptiPers_Problem::standard_optimizer(const std::set<unsigned int>& cycles,
                                          std::vector<Core::MapChain<double>>& basis) {
  // cycles: set of indices on which to optimize. 

  glp_add_cols(lp, cycles.size());  
  auto cycles_it = cycles.begin();
  ++cycles_it;
  // skip first cycle, it will be the first to get optimized.
  // load all other cycles serve as "relative" cycles.  
  int rel_index = 0; 
  for ( ; cycles_it != cycles.end(); ++cycles_it) {
    glp_set_obj_coef(lp, 1 + cur_var_max + rel_index, 0);
    glp_set_col_bnds(lp, 1 + cur_var_max + rel_index, GLP_FR, 0,0);
    
    load_basis_chain_to_col(basis[*cycles_it], 1 + cur_var_max + rel_index);
    ++rel_index; 
  }

  // last one is locked for now:
  assert( 1 + cur_var_max + rel_index ==
          1 + cur_var_max + cycles.size() - 1);   
  glp_set_obj_coef(lp, 1 + cur_var_max + rel_index , 0);
  glp_set_col_bnds(lp, 1 + cur_var_max + rel_index, GLP_FX, 0, 0);  

  // Compute (relative) optimal cycles
  for (auto cur_index : cycles) {    
    int nonzeros = load_basis_chain_to_row_bounds(basis[cur_index]);
    
    // +++ SOLVE +++ 
    // glp_adv_basis(lp, NULL); 
    glp_std_basis(lp);     
    int res = glp_simplex(lp, &parm);
    ++cycles_optimized;    
        
    // +++ PROCESS SOLUTION +++    
    birth_of_cycle_to_var_index[cur_index] = cur_var_max;
    
    basis[cur_index].clear(); 
    for (int ii = 0; ii < sub_index; ++ii) {
      double sol_coeff = glp_get_col_prim(lp, 1+ii) -
          glp_get_col_prim(lp,1+ii+current_space_dimension); 
      if ( fabs(sol_coeff) > coeff_eps ) {        
        int order_index = prob_index_to_order.at(ii); 
        basis[cur_index].set_entry(order_index, sol_coeff); 
      } 
    }    
    
    if (do_mip) {
      // Check first:
      bool non_int = false; 
      for (auto a_pair : basis[cur_index]) {
        if (fabs(a_pair.second - std::round(a_pair.second)) > coeff_eps) {
          non_int = true;
          break;
        } 
      }

      if (non_int) {        
        glp_intopt(lp, &parm_i);       
        
        basis[cur_index].clear(); 
        for (int ii = 0; ii < sub_index; ++ii) {
          double sol_coeff = glp_mip_col_val(lp, 1+ii) -
              glp_mip_col_val(lp,1+ii+current_space_dimension); 
          if ( fabs(sol_coeff) > coeff_eps ) {
            int order_index = prob_index_to_order.at(ii); 
            basis[cur_index].set_entry(order_index, sol_coeff); 
          } 
        }
        
      } 
    }

    if (debug_mode) {
      std::cerr << "cur_index: " << cur_index << "\n";      
      print_stats(std::cerr);
      std::cerr << "Nonzeros in cycle that was optimized: " << nonzeros << "\n"; 
    }

    load_basis_chain_to_col(basis[cur_index], 1+cur_var_max);    
    ++cur_var_max; 
  } 
  assert ( glp_get_col_type(lp, 1 + cur_var_max-1) == GLP_FX );
  glp_set_col_bnds(lp, 1 +cur_var_max-1, GLP_FR, 0,0);
  
  
  return;
}
