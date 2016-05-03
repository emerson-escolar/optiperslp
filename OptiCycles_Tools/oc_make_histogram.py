#!/usr/bin/env python

import sys, getopt
import os

import numpy as np

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Makes a histogram of lengths in an optimal cycles file.\n".format( bname ) )
    sys.stderr.write("Usage: {} [-v] <filename>\n".format(bname))
    sys.stderr.write("Options: -v: File is in vertex-only format.") 
    
    sys.stderr.write("\tInput file is filename.\n")    
    sys.stderr.write("\tOutput is sent to standard output.\n" )
        
    sys.stderr.flush()
    sys.exit(2)


if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "v")
    except getopt.GetoptError as err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)        

    if len(args) != 1:
        usage()

    v_format = False
    for o, a in opts:
        if o == "-v":
            v_format = True

    fname = args[0]
    rfile = open(fname, 'r')

    cnt = 0
    histo = {}

    lnum = 0
    if v_format:
        while True:
            data = rfile.readline()
            lnum = lnum + 1
            if not data:
                break

            cnt = int(data.split()[3])
            if histo.has_key(cnt):
                histo[cnt] = histo[cnt] + 1
            else:
                histo[cnt] = 1 
    else:
        # Ignore first line (b,d) pair of the first interval.
        data = rfile.readline()
        while True:
            data = rfile.readline()
            lnum = lnum + 1
            if (not data or (data[0] == ";")):
                if histo.has_key(cnt):
                    histo[cnt] = histo[cnt] + 1
                else:
                    histo[cnt] = 1                
                cnt = 0
            if (data and data[0] != ";"):        
                cnt = cnt + 1

            if not data:
                break

    rfile.close()
    
    
    for a_key in sorted(histo.keys()):
        sys.stdout.write("%d: %d\n" % (a_key, histo[a_key] ) ) 

    sys.stdout.flush()
                    
            
