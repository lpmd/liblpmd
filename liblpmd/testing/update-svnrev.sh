#!/bin/sh

export LANG="C"
SED=`which sed`
SVNREV=`svn info | grep Revision | awk '{print $2}'`
SVNBRANCH=`svn info | grep '^URL' | awk '{print $2}'`

echo "#define SVNREVISION $SVNREV" > version.h
echo "#define SVNBRANCH \"$SVNBRANCH\"" >> version.h

