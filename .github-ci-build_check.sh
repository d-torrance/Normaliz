#!/bin/bash

source VARS2

case $BUILDSYSTEM in

    *static*)
        if [[ $OSTYPE == darwin* ]]; then
            install -m 0644 /usr/local/opt/llvm/lib/libomp.dylib ${PREFIX}/bin
            otool -L ${PREFIX}/bin/normaliz
            install_name_tool -id "@loader_path/./libomp.dylib" ${PREFIX}/bin/libomp.dylib
            install_name_tool -change "/usr/local/opt/llvm/lib/libomp.dylib" "@loader_path/./libomp.dylib" ${PREFIX}/bin/normaliz
        fi

        if [[ $OSTYPE == darwin* ]]; then
            otool -L ${PREFIX}/bin/*
        else
            ldd ${PREFIX}/bin/*
        fi
        
        export NORMPARA=-x=1 ## more is not possible on GitHub Mac

        make check
        ;;

    *makedistcheck*)
        make -j2 distcheck
        ;;

    *)
        # make -j2 -k
        make -j2 -k check
        make install
        if [[ $OSTYPE == darwin* ]]; then
            otool -L ${PREFIX}/bin/*
        else
            ldd ${PREFIX}/bin/*
        fi
        make installcheck
        ;;
esac

export -p | sed 's/declare -x/export/g' > VARS3
