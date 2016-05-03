#!/usr/bin/env sh

# Sample for usage.
# Does nothing.

echo "This file does nothing. Read its contents to get a sample of how to use the scripts here."
exit


# Here, opticycles is the output of optipers containing opticycles files.
# Usually, it is named "gen_foo_bar_1.txt" or something.


#########################################################################
#Part 0: Converting to vertex-only format file 

oc_convert_to_v_format.py opticycles.txt > v_opticycles.txt


#########################################################################
#PART 1: Restricting optimal cycles to those only within a defined region.

# Pseudocode:
oc_get_pairs.py opticycles.txt | oc_inpolygon.py polygon.txt | oc_apply_mask.py opticycles.txt 

# For vertex-only format file
oc_get_pairs.py -v v_opticycles.txt | oc_inpolygon.py polygon.txt | oc_apply_mask.py -v v_opticycles.txt

# Polygons are defined by giving its vertices in order. File format is:
#
# n
# x1 y1
# x2 y2
# ...
# xn xn
#
# Where n is the number of vertices of your polygon.


#########################################################################
#PART 2: Making histograms of cycle lengths

# Length in terms of number of simplices:
oc_make_histogram.py opticycles.txt

# Length in terms of number of vertices:
oc_make_histogram.py -v v_opticycles.txt

# Note that in higher dimensions, these two can give different answers.
# For dimension 1 this should always be equal.



#########################################################################
#PART 3: Recovering geometric information

#1. Apply "i2p" -- index-to-point
oc_v_apply_i2p.py i2p_file < v_opticycles.txt > p_opticycles.txt

#2. Get secondary structure:
# WORK IN PROGRESS!














