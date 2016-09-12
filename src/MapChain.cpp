#include "MapChain.h"

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


namespace Core{

template<typename T>
std::set<int> MapChain<T>::get_nonzeros()const{
  std::set<int> ans;
  for (auto const & a_pair : data){
    ans.emplace(a_pair.first);
  }
  return ans;	
}

template<typename T>
int MapChain<T>::get_pivot()const{
  for (auto last_it = data.rbegin();
       last_it != data.rend(); ++last_it){
    if (last_it->second != 0){
      return last_it->first;
    }
  }
  return -1;
}

template<typename T>
void MapChain<T>::set_entry(int entry, value_type coeff){
  data[entry] = coeff;
}

template<typename T>
typename MapChain<T>::value_type
MapChain<T>::get_entry(int entry)const{
  if (data.count(entry)==1){
    return data.at(entry);
  }else{
    return T(0);
  }
}

template<typename T>
void MapChain<T>::apply_map(const std::map<int,int>& transform){
  return;
}
	
template<typename T>
void MapChain<T>::clear_zeros(){
  for (auto it = data.begin(); it != data.end(); ){
    if (it->second == 0 ){
      data.erase(it++);
    }else{
      ++it;
    }
  }
}

template<typename T>
void MapChain<T>::add_multiple(const MapChain<T>& other,
                               const T& coeff)
{
  for (auto it = other.data.begin();
       it != other.data.end(); ++it){
    data[it->first] += coeff * (it->second);
    if ( data[it->first] == 0 ){
      data.erase( it->first );
    } 
  }
  return; 
}

template<typename T>
MapChain<T>& MapChain<T>::operator+=(const MapChain<T>& other){
  for (auto it = other.data.begin();
       it != other.data.end(); ++it){
    data[it->first] += it->second;
    if ( data[it->first] == 0 ){
      data.erase( it->first );
    } 
  }
  return *this; 
}

template<typename T>
MapChain<T>& MapChain<T>::operator-=(const MapChain<T>& other){
  std::map<int, T> temp_data = data;
  for (auto it = other.data.begin();
       it != other.data.end(); ++it){
    temp_data[it->first] -= it->second;
    if ( temp_data[it->first] == 0 ){
      temp_data.erase( it->first );
    } 
  }
  std::swap(data, temp_data);
  return *this; 
}

template<typename T>
MapChain<T>& MapChain<T>::operator*=(const value_type& scalar){
  if (scalar == T(0)){
    data.clear();
  }else{
    for (auto it = data.begin();
         it != data.end(); ++it) {
      it->second *= scalar;
    }
  }
  return *this; 
}

template<typename T>
bool MapChain<T>::operator==(const MapChain<T>& other)const{
  return ( data == other.data );
}

template<typename T>
bool MapChain<T>::operator!=(const MapChain<T>& other)const{
  return not (this->operator==(other));
}

template<typename T>
MapChain<T> operator+(MapChain<T> lhs, const MapChain<T>& rhs){
  lhs += rhs;
  return lhs;
}

template<typename T>
MapChain<T> operator-(MapChain<T> lhs, const MapChain<T>& rhs){
  lhs -= rhs;
  return lhs;
}

template<typename T>
MapChain<T> operator*(MapChain<T> lhs, const typename MapChain<T>::value_type& rhs){
  lhs *= rhs;
  return lhs;
}


template<typename T>
typename MapChain<T>::const_iterator MapChain<T>::begin()const{
  return data.begin();
}

template<typename T>
typename MapChain<T>::const_iterator MapChain<T>::end()const{
  return data.end();
}

// template specializations
template class MapChain<int>;
template MapChain<int> operator+(MapChain<int>, const MapChain<int>&);
template MapChain<int> operator-(MapChain<int>, const MapChain<int>&);
template MapChain<int> operator*(MapChain<int>, const MapChain<int>::value_type&);

template class MapChain<double>;
template MapChain<double> operator+(MapChain<double>, const MapChain<double>&);
template MapChain<double> operator-(MapChain<double>, const MapChain<double>&);
template MapChain<double> operator*(MapChain<double>, const MapChain<double>::value_type&);


}
