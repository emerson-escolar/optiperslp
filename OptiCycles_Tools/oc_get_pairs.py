#!/usr/bin/env python

import sys, getopt
import os

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Extracts birth-death pairs from an optimal cycles file.\n".format( bname ) )
    sys.stderr.write("Usage: {} <filename>\n".format(bname))
    sys.stderr.write("Options: -v: File is in vertex-only format.")
    
    sys.stderr.write("\tInput optimal cycles file is specified by filename.\n")        
        
    sys.stderr.flush()
    sys.exit(2)


if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "v")
    except getopt.GetoptError as err:
        # print help information and exit:
        print(str(err)) # will print something like "option -a not recognized"
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

    if v_format:
        while True:
            data = rfile.readline()
            if not data:
                break
            if not (data[0] == ";"):
                continue

            a_pair = data.split()
            a_pair = a_pair[1:3]
            x,y = map(float, a_pair)

            sys.stdout.write("%f %f\n" % (x,y))

    else:
        while True:
            data = rfile.readline()
            if not data:
                break
            if not (data[0] == ";"):
                continue

            a_pair = data.split()
            a_pair = a_pair[1:]
            x,y = map(float, a_pair)

            sys.stdout.write("%f %f\n" % (x,y))

    rfile.close()
    sys.stdout.flush()
        
        
