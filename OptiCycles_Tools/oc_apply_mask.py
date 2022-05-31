#!/usr/bin/env python

import sys, getopt
import os

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Applies mask.\n".format( bname ) )
    sys.stderr.write("Usage: {} [-v] <filename>\n".format(bname))
    sys.stderr.write("Options: -v: File is in vertex-only format.") 
        
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

            mask_val = sys.stdin.readline()
            if mask_val[0] == "1":
                sys.stdout.write(data) 
    else:
        stack = "" 
        while True:
            data = rfile.readline() 
            if (not data or (data[0] == ";")) and len(stack) != 0:
                mask_val = sys.stdin.readline()
                if mask_val[0] == "1":
                    sys.stdout.write(stack)
                stack=""        
            stack = stack + data

            if not data:
                break

    

    rfile.close()    
    sys.stdout.flush()
        
        
