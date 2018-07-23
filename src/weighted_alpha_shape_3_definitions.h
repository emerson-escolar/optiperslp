#ifndef WEIGHTED_ALPHA_SHAPE_3_UTILS_H
#define WEIGHTED_ALPHA_SHAPE_3_UTILS_H

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

#include <CGAL/Alpha_shape_vertex_base_3.h>


typedef int Dim;
typedef int Index;

typedef std::vector<Dim> Dim_container;
typedef std::vector<std::vector<Index> > Matrix;

//**************************************** Defines*
typedef CGAL::Exact_predicates_inexact_constructions_kernel Ker;

typedef CGAL::Regular_triangulation_vertex_base_3<Ker> Rvb;
typedef CGAL::Regular_triangulation_cell_base_3<Ker> Rcb;

typedef CGAL::Alpha_shape_vertex_base_3<Ker, Rvb,
                                        CGAL::Tag_true,
                                        CGAL::Tag_true> Vb_3;
typedef CGAL::Alpha_shape_cell_base_3<Ker, Rcb,
                                      CGAL::Tag_true,
                                      CGAL::Tag_true> Cb_3;

typedef CGAL::Triangulation_data_structure_3<Vb_3, Cb_3>  Ts_3;
typedef CGAL::Regular_triangulation_3<Ker, Ts_3> Triangulation_3;
typedef CGAL::Alpha_shape_3<Triangulation_3, CGAL::Tag_true> Alpha_shape_3;

typedef Triangulation_3::Cell_circulator Cell_circulator;

typedef Alpha_shape_3::Cell_handle          Cell_handle;
typedef Alpha_shape_3::Vertex_handle        Vertex_handle;
typedef Alpha_shape_3::Facet                Facet;
typedef Alpha_shape_3::Edge                 Edge;
typedef Triangulation_3::Weighted_point                Weighted_point;
typedef Triangulation_3::Bare_point                    Bare_point;

typedef Triangulation_3::Finite_cells_iterator    Finite_cells_iterator;
typedef Triangulation_3::Finite_facets_iterator   Finite_facets_iterator;
typedef Triangulation_3::Finite_edges_iterator    Finite_edges_iterator;
typedef Triangulation_3::Finite_vertices_iterator Finite_vertices_iterator;

typedef Alpha_shape_3::FT FT;

typedef CGAL::Alpha_status<Alpha_shape_3::FT>   Alpha_status;
typedef CGAL::Compact_container<Alpha_status>   Alpha_status_container;
typedef Alpha_status_container::iterator        Alpha_status_iterator;
typedef std::pair<Vertex_handle, Vertex_handle> Vertex_handle_pair;


#endif
