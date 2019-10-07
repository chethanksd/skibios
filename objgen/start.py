#
# SKIBIOS objgen entry scipt
#
# author : chetandev.ksd@gmail.com
#
#

import sys, os
import subprocess
import atexit
import time

import ecode
import diagnostics
import svar


#
# Script exit handler definition
#
#

def exit_handler():

    if(error != ecode.ERROR_NONE):
        print('objgen error: ', error)
        print('ERROR!', ecode.error_description[error])
        diagnostics.run_diagnostics()

atexit.register(exit_handler)

#
# START STAGE 1: Get repository path      
#
#
diagnostics.start_stage = 1
svar.repo_path = os.path.dirname(sys.argv[0])  
svar.repo_path = os.path.abspath(svar.repo_path)  
svar.repo_path = os.path.relpath(svar.repo_path, 'skibios') 

print('repopath :', svar.repo_path)

#
# START STAGE 2: Get device file argument
#
#
diagnostics.start_stage = 2

try:
    svar.dfile_path = sys.argv[1]
except IndexError:
    error = ecode.ERROR_DEVICE_FILE_NOT_PROVIDED
    exit(1)


dfile_exists = os.path.isfile(svar.dfile_path)
if not dfile_exists:
    error = ecode.ERROR_DEVICE_FILE_NOT_FOUND
    exit(1)

#
# START STAGE 3: Check if build path exists
#
#
diagnostics.start_stage = 3

try:
    svar.build_path = sys.argv[2]
except IndexError:
    error = ecode.ERROR_BUILD_PATH_NOT_PROVIDED
    exit(1)


bpath_exists = os.path.isfile(svar.build_path)
if not bpath_exists:
    error = ecode.ERROR_BUILD_PATH_NOT_FOUND
    exit(1)


print('Done!')