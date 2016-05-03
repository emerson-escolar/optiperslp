#ifndef WEIGHTED_ALPHA_3_OVER_R_H
#define WEIGHTED_ALPHA_3_OVER_R_H

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

/* Acknowledgements:
   Adopted from PHAT v1.4 <https://code.google.com/p/phat/> 
   addon alpha_3.cpp, but modified to handle weighted alpha shapes.
*/

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_euclidean_traits_3.h>
#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Alpha_shape_3.h>
#include <CGAL/Object.h>

#include <fstream>
#include <list>
#include <cassert>
#include <sstream>

#include <cmath>
#include <algorithm>

#include <map>
#include <tuple>
#include <string>

#include "common_definitions.h"
#include "weighted_alpha_shape_3_definitions.h"

#include "MapChain.h"

std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type>
get_weighted_alpha_3_over_r(std::string fname);

std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type>
get_weighted_alpha_3_over_r(std::istream& is);


#endif





