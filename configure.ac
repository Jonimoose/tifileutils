
AC_INIT([TIUtils],[0.1],[jonimoose@gmail.com])

AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

AC_PREFIX_DEFAULT(/usr/local)
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
PKG_CHECK_MODULES([TIFILES], [tifiles2 >= 1.1.0])

PKG_CHECK_MODULES([TICONV], [ticonv >= 1.1.0])

PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.6.0])

# Checks for header files.
AC_CHECK_HEADERS([locale.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
