
#pragma once

#if defined(_WIN32) && !defined(NOMINMAX)
#   define NOMINMAX
#endif

#if ORTHANC_USE_PRECOMPILED_HEADERS == 1

#include "OrthancFramework.h"  // Must be the first one

//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/locale.hpp>
//#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <json/value.h>

#if ORTHANC_ENABLE_PUGIXML == 1
#  include <pugixml.hpp>
#endif

#include "Compatibility.h"
#include "Enumerations.h"
#include "Logging.h"
#include "OrthancException.h"

#if ORTHANC_ENABLE_DCMTK == 1
// Headers from DCMTK used in Orthanc headers 
#  include <dcmtk/dcmdata/dcdatset.h>
#  include <dcmtk/dcmdata/dcfilefo.h>
#  include <dcmtk/dcmdata/dcmetinf.h>
#  include <dcmtk/dcmdata/dcpixseq.h>
#endif

#if ORTHANC_ENABLE_DCMTK_NETWORKING == 1
// Headers from DCMTK used in Orthanc headers 
#  include <dcmtk/dcmnet/dimse.h>
#endif

#endif