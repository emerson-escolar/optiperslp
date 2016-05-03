#!/usr/bin/env python

import sys
import os

import numpy as np

def usage():
    bname = os.path.basename(sys.argv[0])
    sys.stderr.write("{} - Determines whether points are in a given polygon.\n".format( bname ) )
    sys.stderr.write("Usage: {} <filename>\n".format(bname))
    
    sys.stderr.write("\tQuery polygon is specified by filename.\n")    
    sys.stderr.write("\tPoints is accepted via standard input.\n" )
        
    sys.stderr.flush()
    sys.exit(2)

def point_in_poly(x,y, poly):
    inside = 0
    for p1,p2 in lines(poly):
        if (( min(p1[1],p2[1]) < y <= max(p1[1],p2[1]) ) and x <= max(p1[0],p2[0])):
            if p1[1] != p2[1]:
                xints = (y - p1[1])*(p2[0]-p1[0])*1.0/(p2[1]-p1[1]) + p1[0]
                if p1[0] == p2[0] or x <= xints:
                    inside = 1 - inside
            
    return inside

def lines(poly):
    p0 = poly[-1]
    for p1 in poly:
        if ~np.array_equal(p0,p1):
            yield p0, p1
            p0 = p1


if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()

    fname = sys.argv[1]
    rfile = open(fname, 'r')    

    # parse fname to get polygon
    data = rfile.readlines()
    rfile.close()
    
    n = int(data[0])
    poly = np.zeros(shape=(n,2)) 

    i = 0
    for a_str in data[1:]:        
        x,y = map(float, a_str.split())
        poly[i] = [x,y]
        i = i + 1              

        
    while True:
        data = sys.stdin.readline()        
        if not data:
            break   

        # parse data to get point:
        x,y = map(float, data.split()) 
        #sys.stdout.write( "%f %f" % (x,y))
        
        # report results:        
        tval = point_in_poly(x,y,poly)        
        sys.stdout.write("%d \n" % tval)

    sys.stdout.flush()
