#include "persistence.h"

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

namespace Persistence {

template<typename T, typename G_Coeff>
bool Interval<T,G_Coeff>::operator<(const Interval<T, G_Coeff>& other) const {
  return (pair < other.pair);
}

template<typename T, typename G_Coeff>
bool Interval<T,G_Coeff>::operator==(const Interval<T, G_Coeff>& other)const{
  return (pair == other.pair);
}

template<typename T, typename G_Coeff>
void Interval<T,G_Coeff>::write_pair(std::ostream& output)const{
  output << pair.first << ", " << pair.second << "\n";
}

template<typename T, typename G_Coeff>
void Interval<T,G_Coeff>::write_output(std::ostream& output)const{
  output << "; " << pair.first << " " << pair.second << "\n";
  for ( const auto & cell_coeff_pair : generator ) {      
    output << cell_coeff_pair.second << ", ";
    output << cell_coeff_pair.first; 				
    output << "\n";      
  }			
}



template<typename T, typename G_Coeff>    
bool Persistence<T,G_Coeff>::write_file_dim( std::string fname, int target_dim )const{  
  std::ofstream ofile(fname);
  bool file_was_opened = ofile.is_open();
  if (file_was_opened and intervals.count(target_dim) == 1) {
    ofile.precision(10);
    const auto & dim_intervals = intervals.at(target_dim);
    for ( const auto & an_interval : dim_intervals){
      an_interval.write_output( ofile );
    }
    ofile.close();    
  }
  return file_was_opened;  
}


template<typename T, typename G_Coeff>
int Persistence<T,G_Coeff>::get_betti_number(int dim)const{
  if (intervals.count(dim) == 0){
    return 0;
  }else{
    return intervals.at(dim).size();
  }

}


template struct Interval<double,double>;
template class Persistence<double,double>;





}
