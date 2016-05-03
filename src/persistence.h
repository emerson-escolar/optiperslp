#pragma once

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

#include <set>
#include <map>
#include <fstream>

#include <boost/optional/optional_io.hpp>


namespace Persistence{		
template<typename T, typename G_Coeff>
struct Interval {
  typedef std::map< std::string, G_Coeff > GeneratorType;
		
  Interval(T b, boost::optional<T> d, GeneratorType g = {}):pair(b,d), generator(g){} 
				
  bool operator<(const Interval<T, G_Coeff>& other)const;
  bool operator==(const Interval<T, G_Coeff>& other)const;

  void write_pair(std::ostream& output)const;
  void write_output(std::ostream& output)const;

  std::pair<T, boost::optional<T> > pair;
  GeneratorType generator; 
};
	
template<typename T, typename G_Coeff>
class Persistence{
 public:
  typedef Interval<T, G_Coeff> IntervalType;
  typedef typename IntervalType::GeneratorType GeneratorType;
		
  Persistence()=default;
		
  void insert_interval(int dimension,T b, boost::optional<T> d,
                       GeneratorType g={}){
    intervals[dimension].emplace(b,d,g);
  }
  
  bool write_file_dim( std::string name, int target_dim )const;
  

  int get_betti_number(int dim)const;

 
  std::map<int, std::multiset< IntervalType > > intervals; 

}; 
}
