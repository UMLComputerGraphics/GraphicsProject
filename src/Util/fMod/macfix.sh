#!/bin/bash
if [ `cat ~/.profile | grep "#ERIC" | grep $(pwd) | wc -l` -gt 0 ]; then exit 0; fi
mv ~/.profile ~/.profiletemp
cat ~/.profiletemp | grep -v "#ERIC" > ~/.profile
rm -f ~/.profiletemp
echo "export DYLD_LIBRARY_PATH=`pwd`/fMod/lib:\$DYLD_LIBRARY_PATH #ERIC IS THE GREATEST" >> ~/.profile
echo "MR. T SAYS TO SOURCE ~/.profile AND RUN MAKE AGAIN!"
exit 1
