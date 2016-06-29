#ifndef __MTATHROWS_HPP__

#define __MTATHROWS_HPP__

// SWIGs wrapper code doesn't handle "throw" very well:
//   - In the wrapper C++ code, it catches the declared exception
//       and converts it to a generic exception, and
//   - It doesn't declare any exception in the methods for the proxy class.
//
// Consequently, we disable "throw" statements for SWIG pre-processing

#define EXCEPTION_SPECIFICATION

#ifdef SWIG
#  define EXCEPTION_SPECIFICATION
//#ifdef EXCEPTION_SPECIFICATION
//#undef EXCEPTION_SPECIFICATION
//#endif
#endif

#ifdef EXCEPTION_SPECIFICATION

#ifndef SWIG

#define THROWS(e) throw(e)
#define THROWS1(e1) throw(e1)
#define THROWS2(e1,e2) throw(e1,e2)
#define THROWS3(e1,e2,e3) throw(e1,e2,e3)
#define THROWS4(e1,e2,e3,e4) throw(e1,e2,e3,e4)
#define THROWS5(e1,e2,e3,e4,e5) throw(e1,e2,e3,e4,e5)
#define THROWS6(e1,e2,e3,e4,e5,e6) throw(e1,e2,e3,e4,e5,e6)
#define THROWS7(e1,e2,e3,e4,e5,e6,e7) throw(e1,e2,e3,e4,e5,e6,e7)

#else

#define THROWS(e) throw(mtaException)
#define THROWS1(e1) throw(mtaException)
#define THROWS2(e1,e2) throw(mtaException)
#define THROWS3(e1,e2,e3) throw(mtaException)
#define THROWS4(e1,e2,e3,e4) throw(mtaException)
#define THROWS5(e1,e2,e3,e4,e5) throw(mtaException)
#define THROWS6(e1,e2,e3,e4,e5,e6) throw(mtaException)
#define THROWS7(e1,e2,e3,e4,e5,e6,e7) throw(mtaException)

#endif

#else

#define THROWS(e)
#define THROWS1(e1)
#define THROWS2(e1,e2)
#define THROWS3(e1,e2,e3)
#define THROWS4(e1,e2,e3,e4)
#define THROWS5(e1,e2,e3,e4,e5)
#define THROWS6(e1,e2,e3,e4,e5,e6)
#define THROWS7(e1,e2,e3,e4,e5,e6,e7)

#endif

#endif // !__MTATHROWS_HPP__
