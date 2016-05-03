check_PROGRAMS = test_all_optiperslib

test_all_optiperslib_SOURCES = test_all_optiperslib.cpp \
test_weighted_alpha.cpp \
test_definitions.cpp \
test_mapchain.cpp \
test_optipersprob.cpp \
test_full.cpp

test_all_optiperslib_CPPFLAGS = -I$(srcdir)/../ -I$(srcdir)/../../catch/
test_all_optiperslib_LDADD = ../liboptiperslp.la -lCGAL -lgmp -lglpk -lmpfr 

TESTS = $(check_PROGRAMS)
