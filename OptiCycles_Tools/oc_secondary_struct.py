#!/usr/bin/env python

import sys, getopt
import os

import numpy

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Computes secondary structure.\n".format( bname ) )
    sys.stderr.write("Usage: {} [-s fname] [-p n] [-r m] <i2p file>\n".format(bname)) 
    sys.stderr.write("Input optimal cycles by standard input\n")
    sys.stderr.write("Outputs new point cloud to standard output.\n")
    sys.stderr.write("Option [-s fname] enables outputting of intervals involved, to the specified file.\n")
    sys.stderr.write("Option [-p n] specifies that we only are looking at entries with n vertices.\n")
    sys.stderr.write("Option [-r m] specifies method used to compute radii.\n")
    sys.stderr.write("\t\tPossible values for m (For each optimal cycle):\n")
    sys.stderr.write("\t\t0: Always radii 0.\n")
    sys.stderr.write("\t\t1: Use standard deviation of coordinates, then take minimum.\n")
    sys.stderr.write("\t\t2: Use standard deviation of coordinates, then take maximum.\n")
    sys.stderr.write("Only works with vertex-only format input.\n")
    
        
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

def compute_stats(points):
    p_x = 0
    p_y = 0
    p_z = 0
    for p in points:
        p_x = p_x + float(dx)
        p_y = p_y + float(dy)
        p_z = p_z + float(dz)

    return average
    
    
    
if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "p:r:s:") 
    except getopt.GetoptError as err:
        # print help information and exit:
        print(str(err)) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)        
        
    if len(args) != 1:
        usage()

    points_targeted = None
    r_choice = 0;
    intervals_fname = os.devnull
        
    for o, a in opts:
        if o == "-p":
            points_targeted = int(a)
        if o == "-r":
            r_choice = int(a)
        if o == "-s":
            intervals_fname = a

    wfile = open(intervals_fname, 'w') 
            
    fname = args[0] 
    i2p = read_i2p(fname)

    sep = None
    # detect i2p format:
    tester = i2p[0].split(",",3)
    if len(tester) > 2:
        sep = ","
    
    while True:
        data = sys.stdin.readline()        
        if not data.strip():
            break 

        sdata = data.split()
        
        
        # number of vertices:
        n = int(sdata[3])
        
        if points_targeted == None or points_targeted == n:
            # sys.stdout.write("%s %s %s %s\n" % (sdata[0],sdata[1],sdata[2],sdata[3]))
            wfile.write("%s %s\n" % (sdata[1], sdata[2]))
            
            points = numpy.array([ (map(float, i2p[int(a_vert)].split(sep, 3)[0:3])) for a_vert in sdata[4:] ])
            
            cx,cy,cz = numpy.mean(points, axis=0)

            rr = 0
            if (r_choice == 1):
                rr = min(numpy.std(points, axis=0))
            elif (r_choice == 2):
                rr = max(numpy.std(points, axis=0))
                
            sys.stdout.write("%f %f %f %f\n" % (
                cx, cy, cz, rr ))
            
        

    

    sys.stdout.flush()

