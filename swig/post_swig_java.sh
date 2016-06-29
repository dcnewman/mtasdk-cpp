#/bin/sh

# Handle the .cxx file produced by SWIG

cat $1 | \
  sed -f sed0.cmd |
  sed -f sed1.cmd |
  sed -f sed2.cmd |
  sed -f sed3.cmd |
  sed -f sed4.cmd |
  sed -f sed5.cmd > $1.tmp
mv $1.tmp $1

# Handle some specific .java files produced by SWIG

cat $2 | sed -f sed6.cmd > $2.tmp
mv $2 $2.old
mv $2.tmp $2

cat $3 | sed -f sed7.cmd > $3.tmp
mv $3.tmp $3


