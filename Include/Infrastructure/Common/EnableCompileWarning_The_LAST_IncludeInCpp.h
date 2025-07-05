#pragma once

#ifdef _WINDOWS
# pragma warning(default: 4244) /* 'conversion' conversion from 'type1' to 'type2', possible loss of data */ 
# pragma warning(default: 4275) /* non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier' */ 
# pragma warning(default: 4514) /* unreferenced inline/local function has been removed */ 
# pragma warning(default: 4800) /* 'type' : forcing value to bool 'true' or 'false' (performance warning) */ 
# pragma warning(default: 4097) /* typedef-name 'identifier1' used as synonym for class-name 'identifier2' */ 
# pragma warning(default: 4706) /* assignment within conditional expression */ 
# pragma warning(default: 4786) /* truncating debug info after 255 characters */ 
# pragma warning(default: 4660) /* template-class specialization 'identifier' is already instantiated */ 
# pragma warning(default: 4355) /* 'this' : used in base member initializer list */ 
# pragma warning(default: 4231) /* nonstandard extension used : 'extern' before template explicit instantiation */ 
# pragma warning(default: 4710) /* function not inlined */ 
# pragma warning(default: 4530) /* C++ exception handler used, but unwind semantics are not enabled. Specify -GX */
# pragma warning(default: 4548) /* expression before comma has no effect; expected expression with side-effect */
# pragma warning(default: 4127) /* conditional expression is constant */

#else
#endif