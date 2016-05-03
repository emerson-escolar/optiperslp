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

#include "MapChain.h"
#include <vector>
#include <map>
#include <tuple>

typedef std::vector<Core::MapChain<double>> Bdd_Type; 
typedef int Index;

typedef std::tuple<int, double, std::string> DimAlpha_Type;
typedef std::vector<DimAlpha_Type> AlphaMap_Type;
typedef std::map<int, std::tuple<double,double,double,double> > I2P_Type; 
