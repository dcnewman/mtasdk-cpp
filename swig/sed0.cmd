#
#  Let's attach thread's as daemon threads to the Java VM.  That way,
#  we don't hang Java if we don't get explicitly detached later.
#
s|\([ \t]*\)swig_jvm->AttachCurrentThread(\(.*\)|\1// ** Code commented out by post_swig_java.sh (sed0)\
\1// swig_jvm->AttachCurrentThead(\2\
\1// ** End code commenting by post_swig_java.sh (sed0)\
\
\1// ** Code inserted by post_swig_java.sh (sed0)\
\1// Attach threads to the Java VM as "daemon" threads so that the Java VM\
\1// is not left hanging should one of the native code worker threads so attached\
\1// not be subsequently detached from the Java VM.  processDone() attempts to\
\1// do the detachment....\
\1swig_jvm->AttachCurrentThreadAsDaemon(\2\
\1// ** End code insertion by post_swig_java.sh (sed0)|
