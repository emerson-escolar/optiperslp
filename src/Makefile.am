
noinst_LTLIBRARIES = liboptipers_problem.la libweighted_alpha.la libmapchain.la libpersistence.la libdo_optipers.la

liboptipers_problem_la_SOURCES = optipers_problem.cpp optipers_problem_standard_optimizer.cpp
libweighted_alpha_la_SOURCES = weighted_alpha_3_over_r.cpp
libmapchain_la_SOURCES = MapChain.cpp
libpersistence_la_SOURCES = persistence.cpp
libdo_optipers_la_SOURCES = do_optipers.cpp

lib_LTLIBRARIES = liboptiperslp.la
liboptiperslp_la_SOURCES =
liboptiperslp_la_LIBADD = liboptipers_problem.la libweighted_alpha.la libmapchain.la libpersistence.la libdo_optipers.la

liboptiperslp_la_LDFLAGS=


pkginclude_HEADERS = common_definitions.h \
optipers_problem.h \
weighted_alpha_3_over_r.h \
MapChain.h \
weighted_alpha_shape_3_definitions.h \
persistence.h \
do_optipers.h \
optiperslp.h


SUBDIRS = tests
