#include "weighted_alpha_3_over_r.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

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

// ****************************************
// Helper functions

template<typename Index_>
struct Vertex_info_3 {
  typedef Index_ Index;
  Vertex_info_3() {
    index_=boost::none;
  }

  bool has_index() {
    return bool(index_);
  }

  Index index() {
    CGAL_assertion(has_index());
    return index_.get();
  }

  void set_index(Index I) {
    index_=I;
  }

 private: 
  boost::optional<Index> index_;
};


template<typename Index_>
struct Cell_info_3 {
  typedef Index_ Index;
    
  Cell_info_3() {
    for(std::size_t i=0; i<6; i++) {
      edge_index_[i] = boost::none;
    }
    for(std::size_t i=0; i<4; i++) {
      facet_index_[i] = boost::none;
    }
  }

  int edge_conv(int i, int j) {
    if(i>j) std::swap(i,j);
    if(i==0 && j==1) return 0;
    if(i==0 && j==2) return 1;
    if(i==0 && j==3) return 2;
    if(i==1 && j==2) return 3;
    if(i==1 && j==3) return 4;
    if(i==2 && j==3) return 5;
    //should'nt reach here!
    throw;
    return -1;
  }

  bool has_edge_index(int i, int j) {
    return bool(edge_index_[edge_conv(i,j)]);
  }

  Index edge_index(int i, int j) {
    CGAL_assertion(has_edge_index(i,j));
    int k = edge_conv(i,j);
    return edge_index_[k].get();
  }

  bool has_facet_index(int i) {
    CGAL_assertion(i>=0 && i<4);
    return bool(facet_index_[i]);
  }

  Index facet_index(int i) {
    CGAL_assertion(has_facet_index(i));
    return facet_index_[i].get();
  }

  void set_edge_index(int i, int j, Index I) {
    edge_index_[edge_conv(i,j)]=I;
  }

  void set_facet_index(int i, Index I) {
    facet_index_[i]=I;
  }
 private:
  boost::optional<Index> edge_index_[6];
	boost::optional<Index> facet_index_[4];
};



void set_index_of_edge(const Triangulation_3& T,
                       const Edge& e,
                       std::map<Cell_handle, Cell_info_3<Index> >& c_info,
                       Index I) {
	Vertex_handle v1 = e.first->vertex(e.second);
  Vertex_handle v2 = e.first->vertex(e.third);

  Cell_circulator ch=T.incident_cells(e);
  Cell_circulator ch_start=ch;
  int count=0;
  do {
    c_info[(Cell_handle)ch].set_edge_index(ch->index(v1),
                                           ch->index(v2),
                                           I);
    ch++;
    count++;
  } while(ch!=ch_start);
}

void set_index_of_facet(const Triangulation_3& T, const Facet& f,
                        std::map<Cell_handle, Cell_info_3<Index> >& c_info,
                        Index I) {
  c_info[f.first].set_facet_index(f.second, I);
  Facet mf = T.mirror_facet(f);    
  c_info[mf.first].set_facet_index(mf.second, I); 
}

template<typename Triple>
struct Sort_triples {
  bool operator() (const Triple& a, const Triple& b) {
    if(a.first < b.first) return true;
    if(a.first > b.first) return false;
    return a.second < b.second;
  }
};




std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type>
get_weighted_alpha_3_over_r(std::string fname) {
  std::list<Weighted_point> lp;
    
  //read input
  std::ifstream is(fname);
  std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type> ans;
  if (is.is_open()) {
    ans = get_weighted_alpha_3_over_r(is);
    is.close(); 
  }
  return ans; 
}



std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type>
get_weighted_alpha_3_over_r(std::istream& is) {
  std::list<Weighted_point> lp;
    
  //read input 
  std::string next_line;
  while( getline( is, next_line ) ) {
    if( next_line != "" && next_line[ 0 ] != '#' ) {
      std::stringstream sstr(next_line);
      
      // double x,y,z,r;
      // sstr >> x >> y >> z >> r;                        
      // lp.emplace_back( Bare_point(x,y,z), r*r );
      
      std::vector<std::string> vstrings;
      boost::split(vstrings, next_line,
                   boost::is_any_of(", 	"),
                   boost::token_compress_on);
      
      std::vector<double> vdoubles(vstrings.size());
      std::transform(vstrings.begin(), vstrings.end(),
                     vdoubles.begin(),
                     [](const std::string& val)
                     {
                       return std::stod(val);
                     });

      if (vdoubles.size() == 4) {
        lp.emplace_back( Bare_point(vdoubles.at(0),vdoubles.at(1),vdoubles.at(2)), vdoubles.at(3)*vdoubles.at(3) );
      } else {
        return std::tuple<Bdd_Type, AlphaMap_Type, I2P_Type>();
      }           
    }
  }
    
  std::cerr << "Computing Alpha shape..." << std::endl;
  Alpha_shape_3 shape(lp.begin(),lp.end(), 0, Alpha_shape_3::GENERAL);

  std::cerr << "Computing births..." << std::endl;

  typedef CGAL::Triple<FT,int,CGAL::Object> Triple;
  std::vector<Triple> circumradii;
  FT alpha_value;    
    
  for (Finite_vertices_iterator vertex = shape.finite_vertices_begin();
       vertex != shape.finite_vertices_end(); vertex++) {
    Vertex_handle vh(vertex);

    Alpha_status* as = vh->get_alpha_status();
    if(as->is_Gabriel()){
      alpha_value = as->alpha_min();
    }else{
      alpha_value = as->alpha_mid();
    }				
    circumradii.push_back(CGAL::make_triple(
        alpha_value,
        0,
        CGAL::make_object(vh))); 
  }
    
  for (Finite_edges_iterator edge = shape.finite_edges_begin();
       edge != shape.finite_edges_end(); edge++) {
    Vertex_handle v1 = edge->first->vertex(edge->second);
    Vertex_handle v2 = edge->first->vertex(edge->third);

    Vertex_handle_pair vhp = (v1 < v2 ?
                              std::make_pair(v1,v2) :
                              std::make_pair(v2,v1)
                              );

    Alpha_status_iterator as =
        shape.get_edge_alpha_map()->find(vhp)->second;               
	        
    if(as->is_Gabriel()){
      alpha_value = as->alpha_min();
    }else{
      alpha_value = as->alpha_mid();
    }
        
    circumradii.push_back(CGAL::make_triple(
        alpha_value,
        1,
        CGAL::make_object(*edge)));
  }
    
  for (Finite_facets_iterator f = shape.finite_facets_begin();
       f != shape.finite_facets_end(); f++)  {
    Vertex_handle v1 = f->first->vertex((f->second+1)%4);
    Vertex_handle v2 = f->first->vertex((f->second+2)%4);
    Vertex_handle v3 = f->first->vertex((f->second+3)%4);

    Alpha_status_iterator as =
        f->first->get_facet_status(f->second); 
        
    if(as->is_Gabriel()){
      alpha_value = as->alpha_min();
    }else{
      alpha_value = as->alpha_mid();
    }        
        
    circumradii.push_back(CGAL::make_triple(
        alpha_value,
        2,
        CGAL::make_object(*f)));
  }
    
  for (Finite_cells_iterator cit = shape.finite_cells_begin(); 
       cit != shape.finite_cells_end(); cit++) {
    Vertex_handle v1 = cit->vertex(0);
    Vertex_handle v2 = cit->vertex(1);
    Vertex_handle v3 = cit->vertex(2);
    Vertex_handle v4 = cit->vertex(3);

    Cell_handle ch(cit);
    circumradii.push_back(CGAL::make_triple(
        cit->get_alpha(),
        3,
        CGAL::make_object(ch)));
  }
    
  std::cerr << "Sorting alpha_values..." << std::endl;

  std::sort(circumradii.begin(),circumradii.end(),
            Sort_triples<Triple>());

  std::cerr << "Filtration of size " << circumradii.size() << std::endl;

  std::size_t filtration_index = 0;
  std::size_t filtration_size = circumradii.size();
    
  Bdd_Type boundary_matrix(filtration_size); 
  AlphaMap_Type alpha_map(filtration_size);
  I2P_Type index_to_point;
    
  Vertex_handle v;
  Edge e;
  Facet f;
  Cell_handle c; 

  Index curr_index = 0;

  std::map<Vertex_handle, Vertex_info_3<Index> > v_info;
  std::map<Cell_handle, Cell_info_3<Index> > c_info;

  for(std::vector<Triple>::const_iterator
          it = circumradii.begin();
      it != circumradii.end(); it++) {
    // if(filtration_index % 100000 == 0) {
    //   std::cerr << filtration_index << " of "
    //             << filtration_size << std::endl;
    // }
    filtration_index++;

    const CGAL::Object& obj = it->third; 
    
    std::string cell_code;
    if(CGAL::assign(v,obj)) {
      v_info[v].set_index(curr_index); 
      cell_code = std::to_string(curr_index); 
      // empty boundary

      if (true) {
        // get point        
        index_to_point[curr_index]
            = std::make_tuple(v->point().x(), 
                              v->point().y(),
                              v->point().z(),
                              std::sqrt(v->point().weight())
                              ); 
      }
    }
    if(CGAL::assign(e,obj)) {
      Vertex_handle v1 = e.first->vertex(e.second);
      CGAL_assertion(v_info.at(v1).has_index());
      Vertex_handle v2 = e.first->vertex(e.third);
      CGAL_assertion(v_info.at(v2).has_index());
            
      Index i1 = v_info[v1].index();
      Index i2 = v_info[v2].index();
            
      if(i1>i2) {
        std::swap(v1,v2);
        std::swap(i1,i2);
      }
      cell_code = std::to_string(i1) + ","
          + std::to_string(i2); 
      boundary_matrix[curr_index].set_entry(i2, 1);
      boundary_matrix[curr_index].set_entry(i1, -1); 
            
      set_index_of_edge(shape, e,
                        c_info, curr_index);
    }
        
    if(CGAL::assign(f,obj)) {
      // First, determine ordering of vertex indices:
      Vertex_handle v1 = (f.first)->vertex( (f.second+1)%4 );
      Vertex_handle v2 = (f.first)->vertex( (f.second+2)%4 );
      Vertex_handle v3 = (f.first)->vertex( (f.second+3)%4 );

      Index vi1 = v_info[v1].index();
      Index vi2 = v_info[v2].index();
      Index vi3 = v_info[v3].index();

      Index nums[3] = {vi1,vi2,vi3};
      unsigned int num_size = sizeof(nums)/sizeof(nums[0]);
      std::sort(nums, nums + num_size);
      Index mid = nums[1];

      cell_code = std::to_string(nums[0]);
      for (int i = 1; i < 3; i++){
        cell_code += ",";
        cell_code += std::to_string(nums[i]);
      }

      double coe1=1, coe2=1, coe3=1;

      if (mid == vi1) {
        coe3 = -1;
      }
      else if (mid == vi2) {
        coe2 = -1;
      }
      else if (mid == vi3) {
        coe1 = -1;
      } 
	        
      Index i1= c_info.at(f.first).edge_index(
          (f.second+1)%4,
          (f.second+2)%4 );
      boundary_matrix[curr_index].set_entry(i1, coe1);      
            
      Index i2= c_info.at(f.first).edge_index(
          (f.second+1)%4,
          (f.second+3)%4 );      
      boundary_matrix[curr_index].set_entry(i2, coe2);
            
      Index i3= c_info.at(f.first).edge_index(
          (f.second+2)%4,
          (f.second+3)%4 );      
      boundary_matrix[curr_index].set_entry(i3, coe3); 
            
      set_index_of_facet(shape, f,
                         c_info, curr_index);
    }
        
    if(CGAL::assign(c,obj)) {
      std::vector<std::pair<Index, int>> nums;
      for (int j=0; j<4; ++j) {
        // Index vi1 = v_info[ c->vertex(0) ].index();
        nums.emplace_back(v_info[c->vertex(j)].index(), j);
      }
      std::sort(nums.begin(), nums.end());
      std::map<Index, double> to_coeff;
      cell_code = std::to_string(nums[0].first);	        
      for (int j=1; j<4;++j) {
        cell_code += ",";
        cell_code += std::to_string(nums[j].first);
      }
	        
      for (int j = 0; j < 4; ++j) {
        to_coeff[nums[j].second] = ( (j%2 == 0) ? 1 : -1 ); 
      }

      for (int j=0; j<4; ++j){
        Index i = c_info.at(c).facet_index(j); 
        boundary_matrix[curr_index].set_entry(i, to_coeff[j]);
      }      
    }
    alpha_map[curr_index] =
        std::make_tuple(it->second,
                        it->first.exact().to_double(),
                        cell_code);
        
    ++curr_index;
  }    
     	
  return std::make_tuple(boundary_matrix, alpha_map, index_to_point);
}
