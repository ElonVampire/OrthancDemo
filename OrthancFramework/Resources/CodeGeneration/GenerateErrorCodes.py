
import json
import os
import re
import sys

START_PLUGINS = 1000000
BASE = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))


##
## Read all the available error codes and HTTP status
##