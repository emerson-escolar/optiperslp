#include "catch.hpp"
#include "MapChain.h"
#include <typeinfo>

typedef Core::MapChain<double> MC; 

TEST_CASE( "MapChain Type" , "[mapchain]" ) {
  CHECK(typeid(Core::MapChain<double>::IndexType) == typeid(int));
}

TEST_CASE( "MapChain data" , "[mapchain]" ) {  
  MC mc1, mc_temp, zero; 

  mc1.set_entry(-1, 2);
  mc1.set_entry(0, 1.1);
  mc1.set_entry(1, 0);

  mc_temp = mc1;

  CHECK(mc1.get_entry(-1) == 2);
  CHECK(mc1.get_entry(0) == 1.1);
  CHECK(mc1.get_entry(1) == 0);
  // no entry named 10 in mc1, but
  // this is the expected behaviour.
  CHECK(mc1.get_entry(10) == 0);

  // mc1 is not modified by query to entries.
  CHECK(mc1 == mc_temp);
  
  mc1.clear_zeros();
  // This may be strange, but we are just comparing equality as std::maps!
  CHECK_FALSE(mc_temp == mc1);

  mc1.clear();
  CHECK(mc1 == zero);

}

TEST_CASE( "MapChain Arithmetic" , "[mapchain]" ) {  
  MC mc1, mc2, mc3, zero; 
  REQUIRE(mc1 == mc2);

  mc1.set_entry(-1, 2);
  mc1.set_entry(0, 1.1);
  mc2 = mc1;

  mc3.set_entry(0, -1.1);
  mc3.set_entry(1, 3);  

  SECTION( "Additive Inverses" ){    
    SECTION( "Additive Inverse 1" ) {
      mc2 -= mc1;
      CHECK(mc2 == zero);
    }
    SECTION( "Additive Inverse 2 - self subtraction" ) {      
      CHECK_NOTHROW(mc2 -= mc2);
      CHECK(mc2 == zero);
    }    
    SECTION( "Additive Inverse 3 - subtraction" ) {      
      MC x = mc2 - mc2;
      CHECK(x == zero);
    }

    SECTION( "Additive Inverse 4 - via addition" ) {      
      mc2 *= -1;
      mc1 += mc2;
      CHECK(mc1 == zero);
    }

    SECTION( "Additive Inverse 5 - Compound Operation" ) {            
      mc1 += (mc2* -1);
      CHECK(mc1 == zero);
    }
  }  

  SECTION( "Addition on MapChain" ){    
    SECTION( "Addition 1" ) {
      mc1 += mc3;

      mc2.clear();
      mc2.set_entry(-1,2);
      mc2.set_entry(1,3);

      CHECK(mc1 == mc2);
    }
    SECTION( "Addition 2" ) {
      mc2 = mc3 + mc3;
      CHECK_NOTHROW(mc3 += mc3);      
      CHECK(mc2 == mc3);            
    }
  }
}
