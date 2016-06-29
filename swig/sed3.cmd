#
# Add cleanup code to processDone(): delete any global reference generated
# by processInit(), and detach this thread from the JavaVM.
#
/[ \t]*jenv->CallStaticVoidMethod/{
N
}
/.*\n[ \t]*if (jenv->ExceptionOccurred()) return[ \t]*;/{
N
}
s|\(.*\)\n\([ \t]*\)\(.*\)\n\([ \t]*\)}|\1\
\2// ** Code commented out by post_swig_java.sh (sed3)\
\2// \3\
\2// ** End code commenting by post_swig_java.sh (sed3)\
\
\2// ** Code inserted by post_swig_java.sh (sed3)\
done:\
\2if (jctx2) jenv->DeleteGlobalRef(jctx2);\
\2swig_jvm->DetachCurrentThread();\
\2// ** End code insertion by post_swig_java.sh (sed3)\
\}|
