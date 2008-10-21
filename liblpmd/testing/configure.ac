
AC_INIT

AC_CONFIG_HEADERS(config.h)

AM_INIT_AUTOMAKE(liblpmd, 1.0.3pre)

AC_PROG_CXX

AC_PROG_INSTALL

AC_PROG_LIBTOOL

AC_STDC_HEADERS

AC_ARG_ENABLE(omp,
 [  --enable-omp  Enable OpenMP parallelization of loops],
 [AC_SUBST(cpp_omp, "-fopenmp ") 
  AC_SUBST(ld_omp, "-lgomp")]
 )

AC_OUTPUT(Makefile lpunit/Makefile tests/Makefile liblpmd-1.0.pc)

