AC_PREREQ(2.59)

AC_INIT([taille], [0.0.1],
        [Olivier DELHOMME <olivier.delhomme@free.fr>],
        [taille])
        
AM_INIT_AUTOMAKE([$PACKAGE_TARNAME], [$PACKAGE_VERSION])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_INTLTOOL([0.23])

dnl directory requirement
TOP_DIR=`pwd`
SRC_DIR="${TOP_DIR}/src"
AC_SUBST(TOP_DIR)
AC_SUBST(SRC_DIR)

dnl Libraries requirements
PKG_PROG_PKG_CONFIG([0.10])
GLIB2_VERSION=2.10.0

AC_SUBST(GLIB2_VERSION)

dnl ***********************************************************************
dnl * Put the preprocessor macros in a header file                        *
dnl * instead of command line                                             *
dnl ***********************************************************************
AC_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([src/taille.c])

dnl Checks for headers
dnl AC_HEADER_STDC
dnl AC_HEADER_DIRENT
dnl AC_CHECK_HEADERS([getopt.h])

dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_C_CONST
dnl AC_TYPE_PID_T
dnl AC_TYPE_SIZE_T
dnl AC_TYPE_SIGNAL
dnl AC_STRUCT_TM

dnl Checks for library functions.
dnl AC_FUNC_STAT
dnl AC_FUNC_STRFTIME
dnl AC_FUNC_VPRINTF
dnl AC_CHECK_FUNCS(gettimeofday select socket strdup strerror strstr)

dnl Checks for programs
dnl AM_PROG_LIBTOOL

dnl ***********************************************************************
dnl * Build time sanity check                                             *
dnl ***********************************************************************
AM_SANITY_CHECK


dnl ***********************************************************************
dnl * Initialize maintainer mode                                          *
dnl ***********************************************************************
AM_MAINTAINER_MODE


dnl ***********************************************************************
dnl * Checks for headers                                                  *
dnl ***********************************************************************
AC_HEADER_STDC

AC_CHECK_HEADERS( \
 getopt.h         
)

dnl ***********************************************************************
dnl * Checks dynamic libraries capabilities                               *
dnl ***********************************************************************
AC_DISABLE_STATIC
AC_PROG_LIBTOOL


if test "x$prefix" = "xNONE"; then
	prefix="/usr/local"
fi

if test "x$localstatedir" = "x\${prefix}/var"; then
	localstatedir="$prefix/var"
fi

if test "x$sysconfdir" = "x\${prefix}/etc"; then
    sysconfdir="$prefix/etc"
fi

AC_SUBST(prefix)
AC_SUBST(localstatedir)
AC_SUBST(sysconfdir)

dnl ********************************************************
dnl languages
dnl ********************************************************
ALL_LINGUAS="fr_FR"


dnl ********************************************************
dnl triggers gettext
dnl ********************************************************
GETTEXT_PACKAGE=taille
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, ["$GETTEXT_PACKAGE"], [Gettext Package used])
AM_GLIB_GNU_GETTEXT

dnl ***********************************************************************
dnl * directories definitions                                             *
dnl ***********************************************************************
LOCALE_DIR="$prefix/share/locale"
AC_DEFINE_UNQUOTED(LOCALE_DIR, "$prefix/share/locale", [Locale Directory])


dnl *******************************
dnl checking for glib
dnl *******************************
PKG_CHECK_MODULES(GLIB2, [glib-2.0 >= $GLIB2_VERSION])

AC_PROG_INSTALL

CFLAGS="$CFLAGS -Wall -Wstrict-prototypes -Wmissing-declarations \
-Wbad-function-cast -Wcast-qual -Wcast-align -Wnested-externs -Wunused \
-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE"

case $host in
    *linux*)
	    AC_DEFINE_UNQUOTED(SYS_LINUX, 1, [This is a Linux system])
	;;
    *mingw*)
	    AC_DEFINE_UNQUOTED(SYS_MINGW, 1, [This is a MinGW system])
     	AC_DEFINE_UNQUOTED(WINDOWS, 1, [This is a Windows system])
     	AC_DEFINE_UNQUOTED(_WIN32, 1, [This is a Windows system])
        CFLAGS="$CFLAGS -mms-bitfields -mwindows -mno-cygwin"
	    LDFLAGS="$LDFLAGS"
    ;;
esac


dnl *******************************
dnl debug options
dnl *******************************
AC_ARG_ENABLE([debug],
     [  --enable-debug    Turn on debugging],
     [case "${enableval}" in
       yes) debug=true ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
     esac], [debug=false])
if test x$debug = xtrue
then
 AC_DEFINE_UNQUOTED(DEBUG, TRUE, [Debug mode On])
else
 AC_DEFINE_UNQUOTED(DEBUG, FALSE, [Debug mode Off])
fi


dnl *******************************
dnl gcov option
dnl *******************************
AC_ARG_ENABLE([gcov],
     [  --enable-gcov     Enables gcov coverage tests],
     [case "${enableval}" in
       yes) gcov=true ;;
       no)  gcov=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-gcov]) ;;
     esac], [gcov=false])
if test x$gcov = xtrue
then
 CFLAGS="-fprofile-arcs -ftest-coverage $CFLAGS "
else
 CFLAGS="$CFLAGS"
fi


dnl *******************************
dnl gprof option
dnl *******************************
AC_ARG_ENABLE([gprof],
     [  --enable-gprof    Enables gprof profiling],
     [case "${enableval}" in
       yes) gprof=true ;;
       no)  gprof=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-gprof]) ;;
     esac], [gprof=false])
if test x$gprof = xtrue
then
 CFLAGS="-pg $CFLAGS"
else
 CFLAGS="$CFLAGS"
fi


dnl *******************************
dnl -m64 option
dnl *******************************
AC_ARG_ENABLE([m64],
     [  --enable-m64    Enables m64 compiling option],
     [case "${enableval}" in
       yes) m64=true ;;
       no)  m64=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-m64]) ;;
     esac], [m64=false])
if test x$m64 = xtrue
then
 CFLAGS="-m64 $CFLAGS"
else
 CFLAGS="$CFLAGS"
fi


dnl *******************************
dnl -m128bit-long-double option
dnl *******************************
AC_ARG_ENABLE([-m128bit-long-double],
     [  --enable-m128bit-long-double   Enables -m128bit-long-double compiling option],
     [case "${enableval}" in
       yes) m128=true ;;
       no)  m128=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-m128bit-long-double]) ;;
     esac], [m128=false])
if test x$m128 = xtrue
then
 CFLAGS="-m128bit-long-double $CFLAGS"
else
 CFLAGS="$CFLAGS"
fi


AC_SUBST(LDFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(GLIB2_CFLAGS)
AC_SUBST(GLIB2_LIBS)
AC_CONFIG_FILES([Makefile src/Makefile po/Makefile.in])
AC_OUTPUT


AC_MSG_NOTICE([

******** Dumping configuration ********
 
     - Host                   : $host
     - Build For OS           : $build_os
     - Compiler               : ${CC}
     - Prefix                 : $prefix
     - Exec prefix            : ${exec_prefix}
     - locale dir             : ${LOCALE_DIR}

You can now run 'make' to compile taille

])
