%module(directors="1") jmtasdk;

//  Director mechanism still broken in as much as specifying specific
//  class members still doesn't work as of SWIG 1.3.24.  So, we make
//  directors for all of the mtaDequeue class.

%feature("director") mtaDequeue;

//  Provide our own code for handling exceptions.  While we have a
//  a number of different C++ exceptions in the C++ code we are wrapping
//  we relabel all the exceptions as being of one specific type and
//  put the detail information into it.

%{

#include "mtasdk.hpp"

using namespace MTASDK_NAMESPACE;

static void
jmtasdk_exception(JNIEnv *jenv, const mtaException& e)
{
     // Bail now if we don't have an JVM/JRE environment
     if (!jenv)
	  return;

     MTASDK_NAMESPACE::mtaException *our_except = NULL;

     // Create our own copy of the mtaException so that we
     // don't have to worry about garbage collection of the
     // empheral one we just caught
     try {
	  our_except = new mtaException(e.getErrno(),
					e.getMessage(),
					e.getArgno());
	  if (!our_except)
	       goto simple_throw;

	  // Get a pointer to the Java mtaException class
	  jclass clazz = jenv->FindClass(SWIGOUTDIR "/mtaException");
	  if (!clazz)
	  {
	       clazz = jenv->FindClass("mtaException");
	       if (!clazz)
		    goto simple_throw;
	  }
    
	  // Now get the a pointer to the Java protected class member
	  // mtaException(long Cptr, boolean cMemoryOwn)
	  jmethodID mid = jenv->GetMethodID(clazz, "mtaException", "(JZ)V");
	  if (!mid)
	       goto simple_throw;
	  
	  // Build an exception by invoking the Java mtaException()
	  jthrowable throwObj = (jthrowable)jenv->NewObject(clazz, mid,
							    (jlong)our_except,
							    true);
	  if (!throwObj)
	       goto simple_throw;

	  // And throw it
	  jenv->ExceptionClear();
	  jenv->Throw(throwObj);
     }
     catch (...)
     {
	  // Some sort of problem.  Likely insufficient VM
	  // encountered when instantiating a C++ mtaException
	  // object or by constructing the error message within
	  // the C++ mtaException object.
	  goto simple_throw;
     }

     // All done
     return;

     // We failed to throw an mtaException() exception
     // Let us try something a little less ambitious
simple_throw:
     if (our_except)
     {
	  delete our_except;
	  our_except = NULL;
     }
     const char *msg = e.getMessage();
     SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException,
			     msg ? msg : "The jmtasdk library has thrown an "
			     "mtaException error; however, construction of a "
			     "mtaException throwable has failed");
}

%}


%include "exception.i"

%typemap(throws, throws="mtaException") mtaException {
	  jmtasdk_exception(jenv, _e);
	  return $null;
}

// For writeLine() and mtaAddress()
%typemap(in, numinputs=0) (size_t len)
	"$1 = ($1_type) 0;"

%typemap(default) (size_t dflags)
	"$1 = ($1_type) 0;"

%typemap(default) (size_t nflags)
	"$1 = ($1_type) NOTIFY_DEFAULT;"

%typemap(default) (size_t atype)
	"$1 = ($1_type) ADDR_DEFAULT;"

%typemap(default) (size_t mm_level)
        "$1 = ($1_type) 5;"

%typemap(default) (size_t debug_flags)
        "$1 = ($1_type) 0;"

%typemap(default) bool
	"$1 = ($1_type) true;"

%typemap(default) (size_t max_recipients)
	"$1 = ($1_type) (((size_t)-1) >> 1);"

%typemap(default) (size_t max_blocks)
	"$1 = ($1_type) (((size_t)-1) >> 1);"

%typemap(default) (size_t max_lines)
	"$1 = ($1_type) (((size_t)-1) >> 1);"

// mtaMessageDequeue::getLine() optimally returns a pointer and length
//   to a section of read-only memory.  However, the JNI library lacks
//   a call to convert a non-NUL terminated UTF8 string to a Java string.
//   So, we have to grab memory on the heap, copy the UTF8 string, NUL
//   terminate it, then copy it into JVM land, and finally free the UTF8
//   string.  All because the JNI weenies have NUL terminated string
//   disease.
%typemap(out) const char *getLine {
	if ($1) {
		$result = jenv->NewStringUTF($1);
		free($1);
	}
}

// mtaDequeue::processInit() drills down to Java and gets an Object which
//   is returned to it (hopefully) intact and which it then returns back
//   up the food chain as a jobject
%typemap(in) jobject ctx2
	     "$1 = (void *)($input);"
//%typemap(out) jobject
//	      "$result = $1; // BLAH"

%typemap(jni) void *ctx2 "jobject"
%typemap(jstype) void *ctx2 "Object"
%typemap(jtype) void *ctx2 "Object"
%typemap(javain) void *ctx2 "ctx2"
%typemap(directorin,descriptor="Ljava/lang/Object;") void *ctx2
	      "$input = (jobject)($1);"
%typemap(javadirectorin) void *ctx2 "$jniinput"

%rename(assign) *::operator=;

%include "mtaThrows.hpp"
%include "mtaException.hpp"
%javaconst(1);
%include "mtasdk.hpp"
%javaconst(0);
%include "mtaAddressProp.hpp"
%include "mtaAddress.hpp"
%include "mtaAddressDequeue.hpp"
%include "mtaAddressEnqueue.hpp"
%include "mtaDequeueProp.hpp"
%include "mtaDequeue.hpp"
%include "mtaEnqueueProp.hpp"
%include "mtaMessageDequeue.hpp"
%include "mtaMessageEnqueue.hpp"
