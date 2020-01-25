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
ERROR_NONE                                  =   0
ERROR_DEVICE_FILE_NOT_PROVIDED              =   100
ERROR_DEVICE_FILE_NOT_FOUND                 =   101
ERROR_DEVICE_FILE_BAD                       =   102
ERROR_DEVICE_ATTRIBUTES_NOT_FOUND           =   103
ERROR_PROCESS_DEVATTRIB_NOT_DEFINED         =   104
ERROR_DEVICE_PARAM_BAD                      =   105
ERROR_BUILD_PATH_NOT_PROVIDED               =   106
ERROR_BUILD_PATH_NOT_FOUND                  =   107
ERROR_PARAM_FILE_NOT_PROVIDED               =   108
ERROR_PARAM_FILE_NOT_FOUND                  =   109
ERROR_PARAM_FILE_BAD                        =   110
ERROR_BIN_PATH_NOT_PROVIDED                 =   111
ERROR_BIN_PATH_NOT_FOUND                    =   112
ERROR_SYMBOL_GEN_FAILED                     =   113

#
# Error description
#

error_description[ERROR_DEVICE_FILE_NOT_PROVIDED] = 'Device File path not provided. \
Please provide file path as 2nd argument'

error_description[ERROR_DEVICE_FILE_NOT_FOUND] = 'Device File not found in path provided. \
Please provide correct file path'

error_description[ERROR_DEVICE_FILE_BAD] = 'Invalid Device File provided. \
Please make sure that valid Device File is provided. See Documentation SD-1000'

error_description[ERROR_DEVICE_ATTRIBUTES_NOT_FOUND] = 'Device attributes not present in device file. \
Please provide device attriubutes in device file. See Documentation SD-1000'

error_description[ERROR_PROCESS_DEVATTRIB_NOT_DEFINED] = 'Process devattrib function not defined. \
Please define process devattrib. See Documentation SD-1000'

error_description[ERROR_DEVICE_PARAM_BAD] = 'Bad device parameter. \
Please define proper value for device parameters. See Documentation SD-1000'

error_description[ERROR_BUILD_PATH_NOT_PROVIDED] = 'Build path not provided. \
Please provide Build path as 4th argument'

error_description[ERROR_BUILD_PATH_NOT_FOUND] = 'Build path not found in path provided. \
Please provide correct Build path'

error_description[ERROR_PARAM_FILE_NOT_PROVIDED] = 'Param File path not provided. \
Please provide Param File path as 3rd argument'

error_description[ERROR_PARAM_FILE_NOT_FOUND] = 'Param File path not found in path provided. \
Please provide correct Param File path'

error_description[ERROR_PARAM_FILE_BAD] = 'Bad param file. \
Please define proper value for skibios parameters. See Documentation SD-1000'

error_description[ERROR_BIN_PATH_NOT_PROVIDED] = 'Bin tool path not provided. \
Please provide Binary tool path as 4th argument'

error_description[ERROR_BIN_PATH_NOT_FOUND] = 'Bin tool path not found in path provided. \
Please provide correct Bin tool path'

error_description[ERROR_SYMBOL_GEN_FAILED] = 'Symbol header generation failed. \
See Documentation SD-1001'