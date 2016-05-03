#!/usr/bin/env python

import sys
import os

import numpy as np

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Converts an optimal cycles file to an optimal cycles vertex format.\n".format( bname ) )
    sys.stderr.write("Usage: {} <filename>\n".format(bname))
    
    sys.stderr.write("\tInput file is filename.\n")    
    sys.stderr.write("\tOutput is sent to standard output.\n" )
        
    sys.stderr.flush()
    sys.exit(2)



if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()

    fname = sys.argv[1]
    rfile = open(fname, 'r')

    lnum = 1;
    collected_line = ""

    verts_list = []
        
    while True:        
        data = rfile.readline()

        if (not data or (data[0] == ";") and lnum > 1):
            # dump current contents
            verts_set = set(verts_list)
            collected_line += ( " " + str(len(verts_set)) ) 
            
            for a_vert in sorted(verts_set):
                collected_line += " " 
                collected_line += str(a_vert)
            collected_line += " \n"
            sys.stdout.write(collected_line)

            # clean running records
            collected_line = ""
            verts_list = []

        if not data:
            break
            
        if (data[0] == ";"):
            # bd pair header. Just append line, remove newline first.
            collected_line = collected_line + data.strip()
        
        if (data[0] != ";"):        
            # term in chain
            data = data.strip()
            chain_term = data.split(",")
            # Python black magic: filter out empty strings:
            chain_term = filter(None, chain_term)

            coeff = float(chain_term[0])
            if ( abs(coeff) > 1e-10 ):
                for v_index in chain_term[1:]:
                    verts_list.append(int(v_index))           


        lnum = lnum + 1



        
            
