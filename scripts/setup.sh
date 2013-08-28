if [ ! -d bin/ ]; then
mkdir bin
else
echo "bin/ already exists"
fi

if [ ! -d lib/ ]; then
mkdir lib
else
echo "lib/ already exists"
fi

if [ ! -d obj/ ]; then
mkdir obj
else
echo "obj/ already exists"
fi

if [ -n "${COMMONUTILS}" ]; then
echo "already set"
else
export THISDIR=`pwd`

export LD_LIBRARY_PATH=${THISDIR}/lib:${LD_LIBRARY_PATH}
#export DYLD_LIBRARY_PATH=${THISDIR}/lib:${DYLD_LIBRARY_PATH}
export PATH=${THISDIR}/bin:${PATH}

export COMMONUTILS=${THISDIR}/
export COMMONUTILSINCLUDE=${THISDIR}/interface
export COMMONUTILSLIB=${THISDIR}/lib
fi
