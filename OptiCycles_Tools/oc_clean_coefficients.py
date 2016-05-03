#!/usr/bin/env python

import sys
import os

import numpy as np

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Cleans up near-zero coefficients in an optimal cycles file.\n".format( bname ) )
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

    data = rfile.readlines()
    sys.stderr.write("%d" % len(data))
    
    rfile.close()

    
    for line in data:
        do_write = True
        if (line[0] != ";"):
            y = float(line.split(",")[0])
            if ( abs(y) <  1e-10 ):
                do_write = False

        if do_write:
            sys.stdout.write(line)
    sys.stdout.flush()
                    
            
