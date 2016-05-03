#ifndef DO_OPTIPERS_H
#define DO_OPTIPERS_H

#include "optipers_problem.h"
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


Persistence::Persistence<double,double> do_optipers(Bdd_Type& bdd_matrix,
                                                    const AlphaMap_Type& alpha_map,
                                                    int target_dim,
                                                    bool opt_immediately);
#endif
