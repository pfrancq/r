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
#
# def function to check for a specific R includes on the system

AC_DEFUN(R_INC_CHK,[
	R_INC_DIR=""
	str="$1/$2"
	for i in $R_INC_PATH ; do
		test -f $i/$str                    && R_INC_DIR=$i/$1 && break
		#test -f $i/r/$str              && R_INC_DIR=$i/r/$1 && break
	done
	AC_MSG_CHECKING(for $1/$2)
	if test -z "$R_INC_DIR"; then
		if test "x$R_INC_PATH" != "x"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR(Cannot find $1 header files under $R_INC_PATH)
		else
			AC_MSG_RESULT(no)
			AC_MSG_ERROR(Cannot find $2 headers.  Use --with-r-includes= to specify non-default path.)
		fi
	fi
	AC_MSG_RESULT(yes in $R_INC_DIR)

	if test "x$i" != "x$PAST_R_INC_DIR"; then
		CPPFLAGS="-I$i $CPPFLAGS"
	fi
	PAST_R_INC_DIR="$i"

])
# --- end def function -----


# R_LIB_CHK(package_Name,lib to to check)
# ------------------------------------------------------------
#
# def function to check for a specific R library on the system
AC_DEFUN(R_LIB_CHK,[
	str="$1/$2.*"
	R_LIB_DIR=""
	for j in `echo $str`; do
		if test -r $j; then
		R_LIB_DIR=$1
		break 2
	fi
	done
])
# --- end def function -----


# R_ALL_INC_CHK(list-of-frontend)
# ------------------------------------------------------------
#
# def function to check All R Includes on the system
AC_DEFUN(R_ALL_INC_CHK,[

	# ---- add configure option : --with-r-includes=DIR ----
	if test "x$R_LIB" != "x" ; then
		R_INC_PATH=$R_LIB
	else
		R_INC_PATH="/usr/include/r /usr/include /usr/include/r /usr/local /usr/local/include /usr/local/include/r ${prefix}/include/r"
	fi

	AC_ARG_WITH(r-includes,
	[  --with-r-include=DIR         where the R includes are, like [/usr/include/r].],
		[
		R_INC_PATH="$withval"
	])
	# ---- end arg for include path  ----


	# ---- check the presence of the includes for all the R libraries
	R_INC_CHK(rdb,rmysql.h)
	R_INC_CHK(rga,rga.h)
	R_INC_CHK(rmath,rgraph.h)
	R_INC_CHK(rprg,rprg.h)
	R_INC_CHK(rpromethee,rpromethee.h)
	R_INC_CHK(rstd,rcontainer.h)

	for l in $1 ; do
		if test "$l" != "kde"; then
			# ---- check the presence of the includes for all the R frontend libraries ----
			R_INC_CHK(frontend/kde,rqt.h)
		fi
	done

])
# --- end def function -----

# R_ALL_LIB_CHK(list-of-frontend)
# ------------------------------------------------------------
#
# def function to check for all R libraries on the system
AC_DEFUN(R_ALL_LIB_CHK,[

	# ---- add configure option : --with-r-libs=DIR ----
	if test "x$R_LIB" != "x" ; then
		R_LIB_PATH=$R_LIB
	else
		R_LIB_PATH="/usr/lib /usr/lib/r /usr/local/lib /usr/local/lib/r ${prefix}/lib/r"
	fi
	AC_ARG_WITH(r-libs,
	[--with-r-libs=DIR               where the R libraries are installed, like [/usr/lib].],
		[
		R_LIB_PATH="$withval"
	])
	# ---- end arg for configure ----

	# ---- check the presence of all R libraries
	for l in rdb rga rmath rprg rpromethee rstd; do

		#AC_MSG_CHECKING(for lib$l)
		for i in $R_LIB_PATH ; do
			R_LIB_CHK($i/$l,lib$l)  #check for local presence
			R_LIB_CHK($i,lib$l)
			R_LIB_CHK($i/r,lib$l)
		done
		AC_MSG_CHECKING(for lib$l)
		if test -z "$R_LIB_DIR"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR(Cannot find lib$l)
		else
			AC_MSG_RESULT(yes in $R_LIB_DIR)
		fi
		if test "x$R_LIB_DIR" != "x$PAST_R_LIB_DIR"; then
			LDFLAGS="-L$R_LIB_DIR $LDFLAGS"
		fi
		PAST_R_LIB_DIR="$R_LIB_DIR"
		LIB_R="$R_LIB_DIR/lib$l.la $LIB_R"

	done

	# ---- check the presence of all R frontend libraries ----
	# --------------------------------------------------------------
	for l in $1 ; do

		if test "$l" = "kde"; then
			for i in $R_LIB_PATH ; do
				R_LIB_CHK($i/frontend/kde,libkde_r)
				R_LIB_CHK($i,libkde_r)
				R_LIB_CHK($i/r,libkde_r)
			done

			AC_MSG_CHECKING(for libkde_r)
			if test -z "$R_LIB_DIR"; then
				AC_MSG_RESULT(no)
				AC_MSG_ERROR(Cannot find libkde_r.
No GUI will be available for r libraries.
Verify that R was installed
using the --enable-kde=yes option !!!
-------------------------------------)

			else
				AC_MSG_RESULT(yes in $R_LIB_DIR)

				if test "x$R_LIB_DIR" != "x$PAST_R_LIB_DIR"; then

					LDFLAGS="-L$R_LIB_DIR $LDFLAGS"
				fi
				LIB_R="$R_LIB_DIR/libkde_r.la $LIB_R"
			fi

			PAST_R_LIB_DIR="$R_LIB_DIR"


		fi
	done

	# Add link to CURL library
	LDFLAGS="-lcurl $LDFLAGS"

])
# --- end def function -----


# R_CHK(list-of-frontend)
# ------------------------------------------------------------
#
# def function to check for all includes and library for R on the system
# @param "list-of-frontend" the list of all frontend that must be compiled
AC_DEFUN(R_CHK,[
	R_ALL_INC_CHK($1)
	R_ALL_LIB_CHK($1)
	AC_SUBST(LIB_R)
])
# --- end def function -----
