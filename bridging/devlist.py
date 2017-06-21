#!/usr/bin/env python

# script to return the USB ttyACM devices available as a json list:
#   [ "ttyACM0", "ttyACM1" ]
#

import glob
import json

print json.dumps(glob.glob("/dev/Andy*"))
