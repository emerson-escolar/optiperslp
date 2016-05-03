#!/usr/bin/env python

import sys, getopt
import os

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Applies i2p.\n".format( bname ) )
    sys.stderr.write("Usage: {} [-n] <i2p file>\n".format(bname))
    sys.stderr.write("Input optimal cycles by standard input\n")
    sys.stderr.write("Only works with vertex-only format input.\n")
    sys.stderr.write("Option -n declares that the input (via stdin) lacks birth, death information.\n") 
        
    sys.stderr.flush()
    sys.exit(2)
    
def read_i2p(fname):
    rfile = open(fname, 'r')

    i2p = {}
    
    while True:
        data = rfile.readline()

        if not data:
            break

        sdata = data.split(None, 1)
        index = int(sdata[0])

        if i2p.has_key(index):
            # THIS SHOULDNT HAPPEN
            raise ImportError("Index to point file has redundant entries!")
        else:
            i2p[index] = sdata[1]

    rfile.close()

    return i2p
    
    
if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "n")
    except getopt.GetoptError as err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)        
        
    if len(args) != 1:
        usage()

    no_b_d = False
    for o, a in opts:
        if o == "-n":
            no_b_d = True
            
    fname = args[0]

    i2p = read_i2p(fname)
    
    while True:
        data = sys.stdin.readline()        
        if not data.strip():
            break 

        sdata = data.split()

        if (no_b_d):
            sys.stdout.write("%s %s\n" % (sdata[0],sdata[1]) )
            for a_vert in sdata[2:]:
                sys.stdout.write( i2p[int(a_vert)] ) 
        else:
            sys.stdout.write("%s %s %s %s\n" % (sdata[0],sdata[1],sdata[2],sdata[3]) )
            for a_vert in sdata[4:]:
                sys.stdout.write( i2p[int(a_vert)] ) 
        

    

    sys.stdout.flush()
