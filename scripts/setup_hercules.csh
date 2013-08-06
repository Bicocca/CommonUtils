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
