using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using parser;

namespace skibios
{


    class Program
    {

        static void Main(string[] args)
        {
            ecode error = ecode.ERROR_NONE;
            int count = 1;

             //Global Variables
            string selfpath = AppDomain.CurrentDomain.BaseDirectory;

            //Stage 0 : Zero argument check
            if(args.Length == 0)
            {
                Console.WriteLine("Nothing to do! Have a Nice Day");
#if DEBUG
                Console.ReadKey(true);
#endif
                Environment.Exit(0);
            }

            objgen.objgeninit();


            //Stage 1 : Remove Duplicate Arguments
            args = args.ToList().Distinct().ToArray();


            //Stage 2 : Check Grammer
            foreach(string str in args)
            {
                error = parse.grammer(str);

                if(error != ecode.ERROR_NONE)
                {
                    Console.WriteLine("ERROR! " + parse.estring[(int)error] + "  argument " + count + "  " + str);
#if DEBUG
                    Console.ReadKey(true);
#endif
                    Environment.Exit(0);
                }

                count++;

            }


            //Stage 3 : parse each arguments for Pre-processing Requirements
            count = 0;
            foreach (string str in args)
            {
                error = parse.read(str);

                if (error != ecode.ERROR_NONE)
                {
                    Console.WriteLine("ERROR! " + parse.estring[(int)error] + "  argument " + count + "  " + str);
#if DEBUG
                    Console.ReadKey(true);
#endif
                    Environment.Exit(0);
                }

                count++;

            }

            //Stage 4: Invoke Object Generator
            error = objgen.process();

            if (error != ecode.ERROR_NONE)
            {
                Console.WriteLine("ERROR! " + parse.estring[(int)error]);
            }

#if DEBUG
            Console.ReadKey(true);
#endif

        }
    }


}
