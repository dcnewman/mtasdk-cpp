#
#  Make the private member variable swig_jvm protected so that processDone()
#  can access it for purposes of doing a thread detach from the Java VM.
#
/[ \t]*class Director /{
N
}
/.*\n[ \t]*private:/{
N
}
/.*\n.*\n[ \t]*\/\* pointer to Java virtual machine \*\//{
N
}
s|\(.*\)\n\([ \t]*\)private:\([ \t]*\)\n\(.*\)\n\([ \t]*JavaVM[ \t]*\*swig_jvm;\)|\1\
\2// ** Code commented out by post_swig_java.sh (sed4)\
\2// private:\
\2// ** End code commenting by post_swig_java.sh (sed4)\
\
\2// ** Code inserted by post_swig_java.sh (sed4)\
\2// Make swig_jvm merely protected rather than private so that\
\2// processDone() can access it to detach threads from the Java VM\
\2protected:\3\
\2// ** End code insertion by post_swig_java.sh (sed4)\
\4\
\5|
