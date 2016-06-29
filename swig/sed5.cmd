#
#  Modify the C++ director code for processInit() to make it an optional
#  member of the class.
#
/[ \t]*SWIG_JavaThrowException(jenv, SWIG_JavaDirectorPureVirtual,[ \t]*/{
N
}
/.*\n[ \t]*"Attempted to invoke pure virtual method .*::processInit.");[  \t]*/{
N
}
s|\([ \t]*\)\(.*\)\n\([ \t]*\)\(.*\)\n\([ \t]*\)return result;|\1// ** Code commented out by post_swig_java.sh (sed5)\
\1// \2\
\3// \4\
\5// return result;\
\5// ** End code commenting by post_swig_java.sh (sed5)\
\
\5// ** Code inserted by post_swig_java.sh (sed5)\
\5// Allow processInit() to be an optional class member\
\5return (jobject)0;\
\5// ** End code insertion by post_swig_java.sh (sed5)|
