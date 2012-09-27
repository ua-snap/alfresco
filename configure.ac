AC_INIT([Fresco], [2.0.1], [apbennett@alaska.edu], [fresco], [http://www.snap.uaf.edu/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.11.1 no-define])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(src/)
AC_PROG_CXX
AC_OUTPUT

AC_ARG_ENABLE(aiem,
[  --enable-aiem    Enable building against aiem project],
[case "${enableval}" in
  yes) aiem=true ;;
  no)  aiem=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-aiem) ;;
esac],[aiem=false])
AM_CONDITIONAL(AIEM, test x$aiem = xtrue)

AC_PROG_LIBTOOL
AC_OUTPUT(Makefile lib/Makefile mpi/Makefile src/Makefile)
