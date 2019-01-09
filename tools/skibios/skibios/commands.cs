using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;

namespace skibios
{
    static class commands
    {

        public static ecode version()
        {
            string str;

            Assembly assembly = Assembly.GetExecutingAssembly();
            FileVersionInfo fileVersionInfo = FileVersionInfo.GetVersionInfo(assembly.Location);

            str = fileVersionInfo.FileDescription;
            Console.WriteLine("SKIBIOS Object Generator Program");

            str = fileVersionInfo.ProductVersion;
            Console.WriteLine("Version : " + str);

            str = fileVersionInfo.LegalCopyright;
            Console.WriteLine(str);

            Console.WriteLine();

            Console.WriteLine("This is free software; see the source for copying conditions." + Environment.NewLine + "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A" + Environment.NewLine + "PARTICULAR PURPOSE.");

            Console.WriteLine();

            return ecode.ERROR_NONE;
        }

        public static ecode help()
        {

            Console.WriteLine("Sorry! Work in progress");
            Console.WriteLine("Report bugs to (skibios@gmail.com)");
            Console.WriteLine();
            return ecode.ERROR_NONE;

        }


        public static ecode binfolder(string arg)
        {

            objgen.binpath = arg.Substring(2);

            if (!Directory.Exists(objgen.binpath))
            {
                return ecode.ERROR_PATH_UNKNOWN;
            }

            return ecode.ERROR_NONE;

        }

        public static ecode outfolder(string arg)
        {

            objgen.outpath= arg.Substring(2);

            if (!Directory.Exists(objgen.outpath))
            {
                return ecode.ERROR_PATH_UNKNOWN;
            }

            return ecode.ERROR_NONE;

        }

        public static ecode startgen()
        {

            objgen.gen = true;

            return ecode.ERROR_NONE;

        }

        public static ecode devicefile(string arg)
        {

            objgen.devfile = arg.Substring(2);

            if (!File.Exists(objgen.devfile))
            {
                return ecode.ERROR_FILE_NOT_FOUND;
            }

            return ecode.ERROR_NONE;

        }

        public static ecode defineparamter(string arg)
        {
            ecode error = ecode.ERROR_NONE;

            arg = arg.Substring(2);

            string[] list = arg.Split(',');

            foreach(string item in list)
            {   

                if(item.Contains("=") == false)
                {
                    return ecode.ERROR_INVALID_DEFINE;
                }

                string[] elements = item.Split('=');

                elements[0] = elements[0].Trim();
                elements[1] = elements[1].Trim();

                if(elements[0] == "")
                {
                    return ecode.ERROR_INVALID_DEFINE;
                }

                if(elements[1] == "")
                {
                    return ecode.ERROR_INVALID_DEFINE;
                }

                if(elements[1].All(char.IsDigit) == false)
                {
                    return ecode.ERROR_INVALID_DEFINE;
                }

                error = objgen.UpdateParameter(elements[0], int.Parse(elements[1]));
                if(error != ecode.ERROR_NONE)
                {
                    return error;
                }

            }

            return ecode.ERROR_NONE;

        }

        public static ecode basetaskdefined(string arg)
        {
            objgen.btd = true;

            return ecode.ERROR_NONE;

        }

    }
}
