#!/usr/bin/env bash

set -e

# for msys we make a static ionary and libnormaliz by Makefile.classic
# since the autotools build does not produce anything useful
if [ "$OSTYPE" == "msys" ]; then
	cd source
	make -f Makefile.classic clean
	make -f Makefile.classic -j8
	make -f Makefile.classic install
	echo "Normaliz installation for MS Windows/MSYS2 complete"
	exit 0
fi

./bootstrap.sh

source $(dirname "$0")/install_scripts_opt/common.sh


CONFIGURE_FLAGS="--prefix=${PREFIX}"
if [ "$GMP_INSTALLDIR" != "" ]; then
    CONFIGURE_FLAGS="${CONFIGURE_FLAGS} --with-gmp=$GMP_INSTALLDIR"
fi

if [ "x$NO_OPENMP" != x ]; then
    CONFIGURE_FLAGS="${CONFIGURE_FLAGS} --disable-openmp"
fi

if [ "x$NMZ_EXTENDED_TESTS" != x ]; then
    CPPFLAGS="${CPPFLAGS} -DNMZ_EXTENDED_TESTS"
fi

# No static build possible with Linux clang since libomp.a is missing
if [[ $CXX == *clang* ]]; then
    if [[ $OSTYPE != darwin* ]]; then
        NMZ_SHARED="yes"
    fi
fi

# for the future we leave the autotools varaiant of
# an msysy build, but comment it out

# if [ "$OSTYPE" != "msys" ]; then
	echo "building shared"
#else
#	echo "MSYS build only static"
#	CONFIGURE_FLAGS="${CONFIGURE_FLAGS} --disable-shared --with-nauty=no --with-cocoa=no"
# fi

mkdir -p build
cd build

../configure ${CONFIGURE_FLAGS} CPPFLAGS="${CPPFLAGS}" LDFLAGS="${LDFLAGS} -Wl,-s" $EXTRA_FLAGS --srcdir=..

make clean
make -j8
make install
if [ "x$NMZ_SHARED" == x ]; then
#	if [ "$OSTYPE" != "msys" ]; then
#		rm source/normaliz.exe
#	else
		rm source/normaliz
#	fi
    make -j4 LDFLAGS="${LDFLAGS} -all-static"
    make install
    if [[ $OSTYPE != darwin* ]]; then
        strip --strip-unneeded --remove-section=.comment --remove-section=.note ${PREFIX}/lib/libnormaliz.a
    fi
fi

cd ..

cp -f ${PREFIX}/bin/* .
cp ${PREFIX}/lib/libnormaliz.a source/libnormaliz ## for compatibility with Makefile.classic

# 	to make version.h accessible to Makefile.classic
cp ${PREFIX}/incude/libnormaliz/version.h install_scripts_opt/header_files_for_Makefile.classic

echo "Normaliz installation complete"
