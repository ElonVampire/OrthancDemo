
#include "PrecompiledHeaders.h"
#include "SystemToolbox.h"

#if defined(WIN32)
#   include <winsock2.h>    // For GetMacAddresses(), must be included before "windows.h"
#   include <windows.h>

#   include <iphlpapi.h>    // For GetMacAddresses()
#   include <process.h>     // For "_spawnvp()" and "_getpid()"
#   include <stdlib.h>      // For "environ"
#else
#   include <net/if.h>      // For GetMacAddresses()
#  include <netinet/in.h>    // For GetMacAddresses()
#  include <sys/ioctl.h>     // For GetMacAddresses()
#  include <sys/wait.h>      // For "waitpid()"
#  include <unistd.h>        // For "execvp()"
#endif

#if defined(__APPLE__) && defined(__MACH__)
#   include <limits.h>      // PATH_MAX
#   include <mach-o/dyld.h> // _NSGetExecutablePath
#endif

#if (defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD_kernel__) || defined(__FreeBSD__)
#  include <net/if_dl.h>     // For GetMacAddresses()
#  include <net/if_types.h>  // For GetMacAddresses()
#  include <sys/sysctl.h>    // For GetMacAddresses()
#endif


#if defined(__linux__) || defined(__FreeBSD_kernel__) || defined(__FreeBSD__)
#  include <limits.h>        // PATH_MAX
#  include <signal.h>
#  include <unistd.h>
#endif


#if defined(__OpenBSD__)
#  include <sys/sysctl.h>    // For "sysctl", "CTL_KERN" and "KERN_PROC_ARGS"
#endif

#include "Logging.h"
#include "OrthancException.h"
#include "Toolbox.h"

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include <cassert>
#include <string.h>

/*=========================================================================
  The section below comes from the Boost 1.68.0 project:
  https://github.com/boostorg/program_options/blob/boost-1.68.0/src/parsers.cpp
  
  Copyright Vladimir Prus 2002-2004.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt
  or copy at http://www.boost.org/LICENSE_1_0.txt)
  =========================================================================*/

// The 'environ' should be declared in some cases. E.g. Linux man page says:
// (This variable must be declared in the user program, but is declared in 
// the header file unistd.h in case the header files came from libc4 or libc5, 
// and in case they came from glibc and _GNU_SOURCE was defined.) 
// To be safe, declare it here.

// It appears that on Mac OS X the 'environ' variable is not
// available to dynamically linked libraries.
// See: http://article.gmane.org/gmane.comp.lib.boost.devel/103843
// See: http://lists.gnu.org/archive/html/bug-guile/2004-01/msg00013.html
#if defined(__APPLE__) && defined(__DYNAMIC__)
// The proper include for this is crt_externs.h, however it's not
// available on iOS. The right replacement is not known. See
// https://svn.boost.org/trac/boost/ticket/5053
extern "C"
{
  extern char ***_NSGetEnviron(void);
}
#  define environ (*_NSGetEnviron()) 
#else
#  if defined(__MWERKS__)
#    include <crtl.h>
#  else
#    if !defined(_WIN32) || defined(__COMO_VERSION__)
extern char** environ;
#    endif
#  endif
#endif

/*=========================================================================
  End of section from the Boost 1.68.0 project
  =========================================================================*/

namespace Orthanc
{
    static bool finish_;
    static ServerBarrierEvent barrierEvent_;

#if defined(WIN32)
    static BOOL WINAPI ConsoleControlHandler(DWORD dwCtrlType)
    {
        // http://msdn.microsoft.com/en-us/library/ms683242(v=vs.85).aspx
        finish_ = true;
        return true;
    }
#else

#endif

}