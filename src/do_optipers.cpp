#include "do_optipers.h"
#include <numeric>
#include <algorithm>

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

std::pair<int,int> count_dimension(const AlphaMap_Type& alpha_map,
                                   int target_dim){  
  int current_space_dimension = 0;
  int coface_space_dimension = 0;  
  for (unsigned int j = 0; j < alpha_map.size(); ++j) {    
    int cur_dim = std::get<0>(alpha_map.at(j));    
    if (cur_dim == target_dim) {
      ++(current_space_dimension);
    } else if (cur_dim  == target_dim + 1) {
      ++(coface_space_dimension);
    } 
  }  
  return std::make_pair(current_space_dimension, coface_space_dimension);
}


std::map<std::string, double> basis_to_generator(const Core::MapChain<double>& basis_chain,
                                                 const AlphaMap_Type& alpha_map) {
  std::map<std::string, double> g; 
  for (const auto & ind_coeff_pair : basis_chain) {
    double val = ind_coeff_pair.second; 
    if (val != 0) {
      std::string cell_code=std::get<2>(alpha_map.at(ind_coeff_pair.first));              
      g[cell_code] = val;			
    }				    
  }
  return g;
}


Persistence::Persistence<double,double> do_optipers(Bdd_Type& bdd_matrix,
                                                    const AlphaMap_Type& alpha_map,
                                                    int target_dim,
                                                    bool opt_immediately) {
  int filtration_size = bdd_matrix.size();
  //TODO: upgrade to exception.
  assert(alpha_map.size() == filtration_size);
  assert(filtration_size > 0); 

  // **************************************************
  // For optimizations  
  // dimension counting
  auto dim_pair = count_dimension(alpha_map, target_dim); 
  OptiPers_Problem op_prob(dim_pair.first, dim_pair.second);

  // initalize a basis
  std::vector<Core::MapChain<double>> basis(filtration_size); 
  for (unsigned int j = 0; j < filtration_size; ++j) {
    basis[j].set_entry(j,1); 
  }  
  
  // cycles queue yet to be optimized:
  std::set<unsigned int> cycles;

  // We *may* set up a set of (postponed) optimal cycles computation.
  // When the birth alpha increases,
  // we flush all of these values, computing their
  // respective optimal cycles. 
  double current_birth = std::get<1>(alpha_map.at(0));
  
  
  // **************************************************
  // For persistence:
  // Keeps track of which (known) index number has a pivot at the current row number n.  
  std::vector<int> lookup_table(filtration_size, -1);        
  Persistence::Persistence<double, double> pers;
  std::vector<unsigned int> paired_indices;

  // **************************************************  
  std::cerr << "Computing Persistence...\n";  
  for (unsigned int j = 0; j < filtration_size; ++j) {
    #ifndef DEBUG
    std::cerr << "\r" << j << "/" << filtration_size;
    std::cerr.flush();
    #endif
    
    if (cycles.size() != 0 and (opt_immediately or std::get<1>(alpha_map.at(j)) != current_birth)) {
      #ifdef DEBUG
      std::cerr << "Optimizing cycle immediately!\n";
      #endif
      op_prob.standard_optimizer(cycles, basis);
      cycles.clear();     
    }       
    current_birth = std::get<1>(alpha_map.at(j));    

    // Persistence reduction.	    
    int pivot = bdd_matrix[j].get_pivot();
    int l_value = -1;
    if ( pivot != -1) {
      l_value = lookup_table[ pivot ];
    }    
    while ( pivot != -1 && l_value != -1 ) {
      double entry_value = bdd_matrix[j].get_entry(pivot);
      double pivot_value = bdd_matrix[l_value].get_entry(pivot); 
      double q = (-1) * (entry_value / pivot_value); 
     
      bdd_matrix[j].add_multiple(bdd_matrix[l_value], q); 
      basis[j].add_multiple(basis[l_value], q); 
      
      pivot = bdd_matrix[j].get_pivot(); 
      if ( pivot != -1) { l_value = lookup_table[pivot];} 
    }
    // End of reduction loop
    // ****************************************

    // **************************************************
    // Increment LP problem
    int current_dim = std::get<0>(alpha_map.at(j));
    pivot = bdd_matrix[j].get_pivot();    
    if (current_dim == target_dim) {
      op_prob.increment_lp_prob(j); 
      if (pivot == -1) {cycles.insert(j);} 
    }

    // ***************
    if ( pivot != -1 ) {
      // Nonzero column
      int i = pivot;
      lookup_table[i] = j;
      paired_indices.push_back(i);
      paired_indices.push_back(j);

      int dim = std::get<0>(alpha_map.at(i));      
      double b = std::get<1>(alpha_map.at(i));
      double d = std::get<1>(alpha_map.at(j));

      if (b != d) {                
        pers.insert_interval(dim,b,d, basis_to_generator(basis[i],alpha_map));        
      } 

      cycles.erase(i);
      if ( std::get<0>(alpha_map.at(j)) == target_dim + 1) {       
        if ( op_prob.query_has_var_index_cell(i) == false) {
          // record the (reduced) boundary into the op prob.
          op_prob.increment_boundary(bdd_matrix[j]); 
        }
      }
      
    }
  }
  // handle still unoptimized stuff, if any
  if (cycles.size() != 0){
    op_prob.standard_optimizer(cycles, basis);
  }
  
  // Handle infinite intervals
  std::vector<unsigned int> all_indices(filtration_size);
  std::iota(std::begin(all_indices),std::end(all_indices),0);  
  std::sort(paired_indices.begin(), paired_indices.end());
  std::vector<unsigned int> unpaired_indices(filtration_size);
  auto it = std::set_difference(all_indices.begin(),
                                all_indices.end(),
                                paired_indices.begin(),
                                paired_indices.end(),
                                unpaired_indices.begin());
  unpaired_indices.resize(it - unpaired_indices.begin());
  
  for (auto i : unpaired_indices) {
    int dim = std::get<0>(alpha_map.at(i));
    double b = std::get<1>(alpha_map.at(i)); 
    pers.insert_interval(dim, b, boost::none, basis_to_generator(basis[i],alpha_map));
  } 

  
  std::cerr << "\nOptimization complete!\n"; 
	
  return pers;
}
