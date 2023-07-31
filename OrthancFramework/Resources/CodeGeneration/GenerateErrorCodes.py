
import json
import os
import re
import sys

START_PLUGINS = 1000000
BASE = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))


##
## Read all the available error codes and HTTP status
##

with open(os.path.join(BASE, 'OrthancFramework', 'Resources', 'CodeGeneration', 'ErrorCodes.json'), 'r') as f:
    ERRORS = json.loads(re.sub('/\*.*?\*/', '', f.read()))

for error in ERRORS:
    if error['Code'] >= START_PLUGINS:
        print('ERROR: Error code must be below %d, but "%s" is set to %d' % (START_PLUGINS, error['Name'], error['Code']))
        sys.exit(-1)
    # else :
    #     print(error)

with open(os.path.join(BASE, 'OrthancFramework', 'Sources', 'Enumerations.h'), 'r') as f:
    a = f.read()

HTTP = {}
for i in re.findall('(HttpStatus_([0-9]+)_\w+)', a):
    HTTP[int(i[1])] = i[0]
    # print(i)

##
## Generate the "ErrorCode" enumeration in "Enumerations.h"
##

path = os.path.join(BASE, 'OrthancFramework', 'Sources', 'Enumerations.h')
with open(path, 'r') as f:
    a = f.read()

