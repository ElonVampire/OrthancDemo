
#include "../PrecompiledHeaders.h"
#include "FilesystemStorage.h"

// http://stackoverflow.com/questions/1576272/storing-large-number-of-files-in-file-system
// http://stackoverflow.com/questions/446358/storing-a-large-number-of-images
#include "../Logging.h"
#include "../OrthancException.h"
#include "../StringMemoryBuffer.h"
#include "../SystemToolbox.h"
#include "../Toolbox.h"