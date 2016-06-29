#
#  Modify the C++ director code for processInit() to convert the
#  Object returned by Java to a global reference.
#
/[ \t]*jresult[ \t]*=[ \t]*(jobject)[ \t]*jenv->CallStaticObjectMethod/{
N
}
/.*\n[ \t]*if (jenv->ExceptionOccurred()) return result;/{
N
}
/.*\n.*\n[ \t]*result = jresult;/{
N
}
s|\(.*\)\n\([ \t]*\)\(.*\)\n\([ \t]*\)\(.*\)\n\([ \t]*\)return result;|\1\
\2// ** Code commented out by post_swig_java.sh (sed1)\
\2// \3\
\4// \5\
\6// return result;\
\2// ** End code commenting by post_swig_java.sh (sed1)\
\
\6// ** Code inserted by post_swig_java.sh (sed1)\
\6// Convert any returned Java Object into a global reference\
\6// so that it will survive across calls to native code \& threads\
\6if (jenv->ExceptionOccurred()) return (jobject)0;\
\6return (jresult) ? jenv->NewGlobalRef(jresult) : (jobject)0;\
\6// ** End code insertion by post_swig_java.sh (sed1)|
