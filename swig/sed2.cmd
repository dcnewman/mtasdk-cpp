#
# Modify the C++ director code for processDone() so as to make it an
# optional class member.
#
/[ \t]*SWIG_JavaThrowException(jenv, SWIG_JavaDirectorPureVirtual,[ \t]*/{
N
}
/.*\n[ \t]*"Attempted to invoke pure virtual method .*::processDone.");[  \t]*/{
N
}
s|\([ \t]*\)\(.*\)\n\([ \t]*\)\(.*\)\n\([ \t]*\)return;|\1// ** Code commented out by post_swig_java.sh (sed2)\
\1// \2\
\3// \4\
\1// ** End code commenting by post_swig_java.sh (sed2)\
\
\1// ** Code inserted by post_swig_java.sh (sed2)\
\1// Allow processDone() to be an optional member of the class\
\1goto done;\
\1// ** End code insertion by post_swig_java.sh (sed2)|
