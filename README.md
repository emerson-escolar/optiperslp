# OptiPersLP - optimal cycles in persistence via linear programming.



## Build and compilation

#### Prerequisites
* Tools needed
	* A working C++ compiler that supports C++17.
	* GNU build system (autotools)
	* libtool
	* Python
* Dependencies
	* CGAL: 6.0.1 or newer.
    If you want to use an older version of CGAL, try an older version from the [download page](https://bitbucket.org/remere/optiperslp/downloads/). 
        * Version 1.1 (around CGAL 4.9.1)
        * Version 1.2.1 (up to CGAL 4.14)
		* Version 1.3 (around CGAL 5.0.1)
	* GLPK
	* BOOST
	* .. and their respective dependencies

#### Build
* If you downloaded the distribution package from the [download page](https://bitbucket.org/remere/optiperslp/downloads/):  
    Navigate to directory and do the usual
	
		./configure
		make
		make check


* If you cloned the project:

		autoreconf --install
		./configure
		make
		make check
	

* For advanced configuration, see: ```./configure --help```

* (Optional) install: ```make install```
	
## Usage
The main "program" is programs/optiperslp. This program computes
the optimal cycles in persistence of the weighted alpha shape of
the input point cloud.

#### Input
Input file is in the format of:
```
x1 y1 z1 r1
x2 y2 z2 r2
...
```
corresponding to 3d coordinates (x,y,z) and weights (r) of the weighted point cloud.
Weights are to be given in linear units (NOT squared radius!).

*Please ensure that your point cloud is not degenerate, and is actually 3d.*

See programs/sample.txt for an example. This is the point cloud of the
8 vertices of a cube. centered at the origin with side length 4.

#### Output Files
1. Generators  
   Each persistence generator is encoded by the following format:
   
		; b d
		c1, v0, v1, ... vd
		c2, w0, w1, ... wd
		...
   

	where b and d are the endpoints of the persistence interval of this generator, and the following lines contain information about the computed optimal cycle corresponding to it, and are to be read in the following way.

	A cycle is a linear combination of simplices. Each line represents a term in this linear combination. The first term is the coefficient, and the remaining (d+1) numbers are the (d+1) vertices of the d-simplex, where d is the chosen dimension to compute in.

	Vertices are given by "index numbers", which are to be interpreted in terms of the index-to-point output (defined below). **WARNING: index numbers DO NOT necessarily correspond to the order of the points in the input file.**

	Note that each generator starts with a semicolon (;).

2. Index-to-point (i2p)  
   This is a list of quintuples:  
   ```
   i x y z r
   ```  
   where (x,y,z,r) is an input weighted point, and i is its corresponding index number.

## License and other things
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

##### Bundled Software

OptiPersLP comes with the single-header version of the [Catch](https://github.com/catchorg/Catch2) testing framework, which is under the Boost software license. 

