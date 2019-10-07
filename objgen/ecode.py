#
# OBJGEN Script error codes and description
#
# author : chetandev.ksd@gmail.com
#
#

# Error description dictionary object
error_description = dict()

#
# Error codes
#
ERROR_NONE                          =   0
ERROR_DEVICE_FILE_NOT_PROVIDED      =   100
ERROR_DEVICE_FILE_NOT_FOUND         =   101
ERROR_DEVICE_FILE_BAD               =   102
ERROR_BUILD_PATH_NOT_PROVIDED       =   103
ERROR_BUILD_PATH_NOT_FOUND          =   104

#
# Error description
#

error_description[ERROR_DEVICE_FILE_NOT_PROVIDED] = 'Device File path not provided. \
Please provide file path as 2nd argument'

error_description[ERROR_DEVICE_FILE_NOT_FOUND] = 'Device File not found in path provided. \
Please provide correct file path'

error_description[ERROR_DEVICE_FILE_BAD] = 'Invalid Device File provided. \
Please make sure that valid Device File is provided. See Documentation SD-1000'

error_description[ERROR_BUILD_PATH_NOT_PROVIDED] = 'Build path not provided. \
Please provide Build path as 3rd argument'

error_description[ERROR_BUILD_PATH_NOT_FOUND] = 'Build path not found in path provided. \
Please provide correct Build path'