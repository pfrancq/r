#--------------------- R LIB -----------------------------
#VARIABLES USE :
#	$R_LIB environnement variable used for developpment
#	$R_INC_PATH = the path specified in the --with-r-includes
#	$R_LIB_PATH = the path specified in the --with-r-libs
#	$R_INC_DIR = the directory where the includes were found
#	$R_LIB_DIR = the directory where the libraries were found
#       $LIB_R = the link to the library needed by the application

#---------------------------------------------------------

# R_INC_CHK(package_Name , file to check (*.h))
# ----------------------------------------------
# def function to check for a specific R includes on the system

AC_DEFUN(R_INC_CHK,[
	R_INC_DIR=""
	for i in $R_INC_PATH ; do
		if test -f $i/$1/$2 ; then   # source project
			R_INC_DIR="-I$i/$1"
			add="$i/$1"
			break
		fi
		if test -f $i/r/$2 ; then
			R_INC_DIR="-I$i/r"
			add="$i/r"
			break
		fi
		if test -f $i/$2 ; then
			R_INC_DIR="-I$i"
			add="$i"
			break
		fi
	done
	if test -z "$R_INC_DIR"; then
		if test "x$R_INC_PATH" != "x"; then
			AC_MSG_ERROR($3)
		else
			AC_MSG_ERROR($3 Use --with-r-includes= to specify non-default path.)
		fi
	fi
	if test "x$add" != "x$PAST_R_INC_DIR"; then
		CPPFLAGS="$R_INC_DIR $CPPFLAGS"
	fi
	PAST_R_INC_DIR="$add"
])
# --- end def function -----


# R_LIB_CHK(package_Name,lib to to check)
# ------------------------------------------------------------
#
# def function to check for a specific R library on the system
AC_DEFUN(R_LIB_CHK,[
	R_LIB_DIR=""
	for i in $R_LIB_PATH ; do
		for t in $i/$1 $i; do
			str="$t/$2.*"

			for j in `echo $str`; do
				if test -r $j; then
					R_LIB_DIR=$j
					break 3
				fi
			done
		done
	done
	if test -z "$R_LIB_DIR"; then
		AC_MSG_ERROR(Cannot find R libraries.)
	fi
	PAST_R_LIB_DIR="$R_LIB_DIR"
	LIB_R="$R_LIB_DIR $LIB_R"
])
AC_DEFUN(RKDE_LIB_CHK,[
	RKDE_LIB_DIR=""
	for i in $R_LIB_PATH ; do
		for t in $i/$1/frontend/kde $i/$1 $i ; do
			str=`echo "$t/$2"kde.*`

			for j in `echo $str`; do
				if test -r $j; then
					RKDE_LIB_DIR=$j
					break 3
				fi
			done
		done
	done
	PAST_RKDE_LIB_DIR="$RKDE_LIB_DIR"
	LIBKDE_R="$RKDE_LIB_DIR $LIBKDE_R"
])
# --- end def function -----


# R_ALL_INC_CHK(list-of-frontend)
# ------------------------------------------------------------
#
# def function to check All R Includes on the system
AC_DEFUN(R_ALL_INC_CHK,[

	# Find Path pour R (R_LIB before standard path overwritten by option of configure)
	if test "x$R_LIB" != "x" ; then
		R_INC_PATH=$R_LIB
	else
		R_INC_PATH="/usr/include /usr/local /usr/local/include ${prefix}/include"
	fi
	AC_ARG_WITH(r-includes,
	[  --with-r-include=DIR         where the R includes are, like [/usr/include/r].],
		[
		R_INC_PATH="$withval"
	])

	# ---- check the presence of includes
	R_INC_CHK(rcore/std,rstd.h,Cannot find R headers.)
	R_INC_CHK(rcore/tools,rstring.h,Cannot find R headers.)
	R_INC_CHK(rcore/prg,rprg.h,Cannot find R headers.)
	R_INC_CHK(rcore/db,rmysql.h,Cannot find R headers.)
	R_INC_CHK(rmath/geometry,rgeometry.h,Cannot find R headers.)
	R_INC_CHK(rmath/graph,rgraph.h,Cannot find R headers.)
	R_INC_CHK(rmath/matrix,rmatrix.h,Cannot find R headers.)
	R_INC_CHK(roptimization/ga,rga.h,Cannot find R headers.)
	R_INC_CHK(roptimization/gga,rgga.h,Cannot find R headers.)
	R_INC_CHK(roptimization/ga2d,rga2d.h,Cannot find R headers.)
	R_INC_CHK(roptimization/promethee,rpromethee.h,Cannot find R headers.)
	for l in $1 ; do
		if test "$l" = "kde"; then
			R_INC_CHK(rcore/frontend/kde,rqt.h,Cannot find R headers for KDE frontend.)
			R_INC_CHK(roptimization/frontend/kde,qgamonitor.h,Cannot find R headers for KDE frontend.)
		fi
	done
	MSG="$MSG, headers $i"
	AC_MSG_RESULT($MSG)
])
# --- end def function -----


# R_ALL_LIB_CHK(list-of-frontend)
# ------------------------------------------------------------
#
# def function to check for all R libraries on the system
AC_DEFUN(R_ALL_LIB_CHK,[

	# Find Path pour R (R_LIB before standard path overwritten by option of configure)
	if test "x$R_LIB" != "x" ; then
		R_LIB_PATH=$R_LIB
	else
		R_LIB_PATH="/usr/lib /usr/lib/r /usr/local/lib /usr/local/lib/r ${prefix}/lib/r"
	fi
	AC_ARG_WITH(r-libs,
		AC_HELP_STRING(
			[--with-r-libs=DIR],
			[where the R libraries are installed, default: [/usr/lib].]),
		R_LIB_PATH="$withval")

	# ---- check the presence of all R libraries
	for l in `echo $1` ; do
		R_LIB_CHK($l,lib$l)
		for k in $2 ; do
			if test "$k" = "kde"; then
				RKDE_LIB_CHK($l,lib$l)
#				RKDE_LIB_CHK(roptimization/frontend/kde,qgamonitor.h,Cannot find R headers for KDE frontend.)
			fi
		done
	done
	MSG="libraries $i"

])
# --- end def function -----


# R_CHK(list-of-frontend)
# ------------------------------------------------------------
#
# def function to check for all includes and library for R on the system
# @param "list-of-frontend" the list of all frontend that must be compiled
AC_DEFUN(R_CHK,[
	AC_MSG_CHECKING(for R)
	R_ALL_LIB_CHK($1,$2)
	R_ALL_INC_CHK($2)
	AC_SUBST(LIB_R)
	AC_SUBST(LIBKDE_R)
])
# --- end def function -----
