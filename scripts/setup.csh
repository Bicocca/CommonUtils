if (!(-d bin/)) then
mkdir bin
else
echo "bin/ already exists"
endif

if (!(-d lib/)) then
mkdir lib
else
echo "lib/ already exists"
endif

if (!(-d obj/)) then
mkdir obj
else
echo "obj/ already exists"
endif

if (${?COMMONUTILS}) then
echo "already set"
else
setenv THISDIR `pwd`

setenv LD_LIBRARY_PATH ${THISDIR}/lib:${LD_LIBRARY_PATH}
#setenv DYLD_LIBRARY_PATH ${THISDIR}/lib:${DYLD_LIBRARY_PATH}
setenv PATH ${THISDIR}/bin:${PATH}

setenv COMMONUTILS ${THISDIR}/
setenv COMMONUTILSINCLUDE ${THISDIR}/interface
setenv COMMONUTILSLIB ${THISDIR}/lib
endif
