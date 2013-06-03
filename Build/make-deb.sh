#!/bin/bash

debfilename=$(cat debian/files | awk '{print $1;}');


fakeroot dpkg-buildpackage -b &> /dev/null;
if [ -s ../$debfilename ]; then
    echo Success;
    mv ../$debfilename ./;
    #sudo dpkg -i curmudgeon_1.0-1_i386.deb
else
    echo Failure;
fi