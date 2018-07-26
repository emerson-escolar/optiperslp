#include "catch.hpp"
#include "common_definitions.h"

TEST_CASE( "Test DimAlpha typedef" , "[typedefs]" ) {
  REQUIRE_NOTHROW( DimAlpha_Type{-1, 0.0001, "foo"} );
    
  DimAlpha_Type an_alpha_entry(-1, 0.0001, "foo");
  REQUIRE_NOTHROW( std::get<0>(an_alpha_entry) );
  REQUIRE_NOTHROW( std::get<1>(an_alpha_entry) );
  REQUIRE_NOTHROW( std::get<2>(an_alpha_entry) ); 

  
  REQUIRE( std::get<0>(an_alpha_entry) == -1 );
  REQUIRE( std::get<1>(an_alpha_entry) == 0.0001 );


}

TEST_CASE( "Test Index typedef" , "[typedefs]" ) {
  Index n = -1;
  REQUIRE( n == -1 );
  n = 0;
  --n;
  REQUIRE( n == -1 );  
}
