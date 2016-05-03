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
#include <iostream>

namespace Core{

// is just a map from int to T.
template<typename T>
class MapChain{
 public:
  typedef int IndexType;
  typedef T value_type;
  typedef typename std::map<int, T>::const_iterator const_iterator;
		
  std::set<int> get_nonzeros()const;
  int get_pivot()const;

  typename std::map<IndexType, T>::size_type size() const {return data.size();}  
  
  void set_entry(int entry, value_type coeff);
  value_type get_entry(int entry)const;
		
  void clear_zeros();
		
  void apply_map(const std::map<int,int>& transform);

  void add_multiple(const MapChain<T>&, const T&);
		
  MapChain<T>& operator+=(const MapChain<T>&);
  MapChain<T>& operator-=(const MapChain<T>&);
		
  MapChain<T>& operator*=(const value_type&);

  bool operator==(const MapChain<T>&)const;
  bool operator!=(const MapChain<T>&)const;

  
  friend std::ostream& operator<<(std::ostream& os,
                                  const MapChain<T>& that) {
    os << "{";
    for (auto const& a_pair : that.data){
      os << "(" << a_pair.first << ", "
         << a_pair.second << ") ";
    }
    os << "}";
    return os;
  }

  const_iterator begin()const;
  const_iterator end()const;

  void clear(){data.clear();}

 private:
  std::map<IndexType, T> data;
		
};

template<typename T>
MapChain<T> operator+(MapChain<T>, const MapChain<T>&);

template<typename T>
MapChain<T> operator-(MapChain<T>, const MapChain<T>&);

template<typename T>
MapChain<T> operator*(MapChain<T>, const typename MapChain<T>::value_type&);

	
}
