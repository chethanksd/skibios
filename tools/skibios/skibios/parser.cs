using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using skibios;
using System.IO;

namespace parser
{
    static class parse
    {

        static public string[] estring =
        {
            "",
            "Invalid Option",
            "Unknown Option",
            "Path Specified Cannot Be Found",
            "Duplicate arguments. Don't know which one to choose",
            "File Not Found",
            "Bad Device file",
            "Device File Required",
            "Kernel Region Size Not Supported",
            "Invalid Define Syntax",
            "Invalid Upper Kernel Region Size",
            "Invalid GHMB Region Size",
            "Invalid Process Stack Size",
            "Insufficient SRAM to suport for given KERNEL_REGION_SIZE",
            "Kernel Region size cannot be greater than 50% of SRAM size",
            "Insufficient process stack region",
            "Complier Not Found in the Path Specified",
            "Compilation Error has occured. Hmm, what have you done?",
            "Not enough space for Main stack"
        };

        //Hit flag for arugments
        private static  bool binf = false;
        private static  bool outf = false;
        private static  bool devf = false;

        public static ecode read(string arg)
        {

            ecode error = ecode.ERROR_NONE;

            arg = arg.Trim();

            if(error != 0)
            {
                return error;
            }

            switch (arg[1])
            {

                case 'v':
                    error = commands.version();
                    break;

                case 'h':
                    error = commands.help();
                    break;

                case 'b':
                    error = commands.binfolder(arg);
                    break;

                case 'o':
                    error = commands.outfolder(arg);
                    break;

                case 'g':
                    error = commands.startgen();
                    break;

                case 'd':
                    error = commands.devicefile(arg);
                    break;

                case 'D':
                    error = commands.defineparamter(arg);
                    break;

                case 'B':
                    error = commands.basetaskdefined(arg);
                    break;

            }

            return error;
        }
        
        // function to check arguments grammer
        public static ecode grammer(string arg)
        {
            ecode error = ecode.ERROR_NONE;

            arg = arg.Trim();

            if (arg[0] != '-')
            {
                return ecode.ERROR_INVALID_OPTION;
            }

            switch (arg[1])
            {
                case 'v':
                    error = arg.Length != 2 ? ecode.ERROR_INVALID_OPTION : ecode.ERROR_NONE;
                    break;

                case 'h':
                    error = arg.Length != 2 ? ecode.ERROR_INVALID_OPTION : ecode.ERROR_NONE;
                    break;

                case 'b':
                    
                    if(parse.binf == true)
                    {
                        error = ecode.ERROR_DUPLICATE_ARGS;
                        break;
                    } else
                    {
                        binf = true;
                    }

                    error = ecode.ERROR_NONE;

                    break;

                case 'o':

                    if (parse.outf == true)
                    {
                        error = ecode.ERROR_DUPLICATE_ARGS;
                        break;
                    } else
                    {
                        outf = true;
                    }

                    error = ecode.ERROR_NONE;

                    break;

                case 'g':
                    error = arg.Length != 2 ? ecode.ERROR_INVALID_OPTION : ecode.ERROR_NONE;
                    break;

                case 'd':

                    if (parse.devf == true)
                    {
                        error = ecode.ERROR_DUPLICATE_ARGS;
                        break;
                    }
                    else
                    {
                        devf = true;
                    }

                    error = ecode.ERROR_NONE;

                    break;

                case 'D':
                    error = ecode.ERROR_NONE;
                    break;

                case 'B':
                    error = arg.Length != 2 ? ecode.ERROR_INVALID_OPTION : ecode.ERROR_NONE;
                    break;

                default:
                    error = ecode.ERROR_UNKNOWN_OPTION;
                    break;

            }

            return error;

        }

    }
}
