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
