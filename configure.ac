
AC_INIT([OptiPersLP],[1.3.1],[e.g.escolar@people.kobe-u.ac.jp])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])

AM_PROG_AR
LT_INIT

AC_PROG_CXX
AC_CONFIG_MACRO_DIRS([m4])

AX_CXX_COMPILE_STDCXX([17], ,[mandatory])

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/tests/Makefile])

AC_CONFIG_FILES([programs/Makefile])
AC_CONFIG_FILES([catch/Makefile])

AC_CONFIG_FILES([OptiCycles_Tools/Makefile])
AC_CONFIG_FILES([OptiCycles_Tools/tests/Makefile])

AC_OUTPUT
