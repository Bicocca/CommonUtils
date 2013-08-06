source /afs/cern.ch/sw/lcg/contrib/gcc/4.3/x86_64-slc5/setup.sh

export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc43-opt/root/
export PATH=${ROOTSYS}/bin:${PATH}
export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}

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
