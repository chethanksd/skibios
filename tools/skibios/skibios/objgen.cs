using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;

namespace skibios
{
    class objgen
    {

        // public variables
        public static string binpath = "";
        public static string outpath = "";
        public static string devfile = "";
        public static bool gen = false;
        public static bool btd = false;

        //private variables : Process Input
        private static int NUM_OF_INTERRUPTS;
        private static int KERNEL_REGION_SIZE;
        private static int UPPER_REGION_SIZE;
        private static int GHMB_REGION_SIZE;
        private static int PROCESS_STACK_SIZE;

        private static int PROCESS_PER_SEC;
        private static int ENABLE_SKIBIOS;
        private static int ENABLE_SAFE_LOCK;
        private static int DISABLE_BUFFER;

        //private varaibles : Process Output
        private static int MAX_PROCESS_COUNT;

        private static string partno;
        private static string gccid;
        private static long flash;
        private static long sram;

        private static int kdat_size = 0;
        private static int kbss_size = 0;

        //Kernel and Application base address for linker
        private static int kbase_addr;
        private static int abase_addr;

        //Pstack and Mstack address ofr linker
        private static int pstack_addr;
        private static int mstack_addr;

        private static int ghmb_addr;
        private static int srdbit;

        const int UNIT_PROCESS_SIZE = 35;

#if DEBUG
        static string srcpath = "../../../";
#else
        static string srcpath = AppDomain.CurrentDomain.BaseDirectory;
#endif

        const string PARAM = @"-DPROCESS_ID_START=4000";

        const string CFLAGS = @"-mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs=""nosys.specs"" -MD -std=c99 -c";

        public static void objgeninit()
        {
            KERNEL_REGION_SIZE = 8;
            UPPER_REGION_SIZE = 3;
            GHMB_REGION_SIZE = 1;
            PROCESS_STACK_SIZE = 512;

            MAX_PROCESS_COUNT = 1;
            ENABLE_SKIBIOS = 1;
            PROCESS_PER_SEC = 100;
            ENABLE_SAFE_LOCK = 1;
            DISABLE_BUFFER = 1;
        }

        public static ecode process()
        {

            ecode error = ecode.ERROR_NONE;

#if DEBUG
            Console.WriteLine("Starting Object Generator...");
#endif

            //Stage 0 : Check if Generator is invoked
            if(gen == false)
            {
#if DEBUG
                Console.WriteLine("Abort! Start option not specified");
#endif
                return ecode.ERROR_NONE;
            }

#if DEBUG
            Console.WriteLine("KERNEL_STACK_SIZE = " + objgen.KERNEL_REGION_SIZE);
            Console.WriteLine("UPPER_REGION_SIZE = " + objgen.UPPER_REGION_SIZE);
            Console.WriteLine("GHMB_REGION_SIZE = " + objgen.GHMB_REGION_SIZE);
            Console.WriteLine("PROCESS_STACK_SIZE = " + objgen.PROCESS_STACK_SIZE);
            Console.WriteLine();
#endif

            //Stage 1 : Read Device file
            error = ReadDeviceFile();

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            NUM_OF_INTERRUPTS = NUM_OF_INTERRUPTS + 16;


            //Stage 2: Check Paramaters
            error = ParameterCheck();

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            //Stage 3: Upper Region Size Validation
            error = UpperKernelValidation();

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            //Stage 4: Calculate Linker and Skibios variables
            kbase_addr = 0x20000000 + (NUM_OF_INTERRUPTS * 4);
            abase_addr = 0x20000000 + (KERNEL_REGION_SIZE * 1024);
            pstack_addr = 0x20000000 + (UPPER_REGION_SIZE * 1024);
            mstack_addr = pstack_addr - 4;
            ghmb_addr = pstack_addr + (MAX_PROCESS_COUNT * PROCESS_STACK_SIZE);

            //Stage 5: Calculate MPU SRD bit
            int y = NextPowerOf2(KERNEL_REGION_SIZE);
            int c = y / 8;

            int srdshift = (y - KERNEL_REGION_SIZE) / c;

            srdbit = 255 & (255 << (8 - srdshift));

            //Stage 6: Genreate mtable.c file
            string file = "";
            file += "#include <stdint.h>" + Environment.NewLine;
            file += "#include <mpu.h>" + Environment.NewLine;
            file += Environment.NewLine;

            file += "const uint32_t mpu_table[" + (2 * MAX_PROCESS_COUNT).ToString() + "] = {" + Environment.NewLine;
                
            for(int i=0; i < MAX_PROCESS_COUNT; i++)
            {
                file += "// Process " + i.ToString() + Environment.NewLine;
                int addr = pstack_addr + (i * PROCESS_STACK_SIZE) + 23;
                
                file += "0x" + addr.ToString("X4") + "," + Environment.NewLine;
                file += "(" + PStackSizeToString(PROCESS_STACK_SIZE) + " << 1) | MPU_KERNEL_DEFAULT | (MPU_AP_PRW_URW << 24) | MPU_REGION_ENABLE";

                if(i != (MAX_PROCESS_COUNT - 1))
                {
                    file += "," + Environment.NewLine;
                } else
                {
                    file += Environment.NewLine;
                }

                file += Environment.NewLine;
            }

            file += "};" + Environment.NewLine;

            StreamWriter fw;

            if (!File.Exists(outpath + "/mtable.c"))
            {
                fw = new StreamWriter(File.Create(outpath + "/mtable.c"));
            }
            else
            {
                fw = new StreamWriter(outpath + "/mtable.c");
            }
            
            fw.WriteLine(file);
            fw.Close();

            error = CompileSource("mtable", true);

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }
/*
#if DEBUG
#else
            File.Delete(outpath + "/mtable.c");
#endif*/

            //Stage6 : Compile all Source files with original parameters
            error = CompileSource("kernel");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("process");

            if(error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("share");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("interrupt");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("heap");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("startup");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("kvar");

            if(error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("svctable");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("umpu");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("permissions");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("service/svr_process", false, "svr_process");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("service/svr_umpu", false, "svr_umpu");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("service/svr_permissions", false, "svr_permissions");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("service/svr_interrupts", false, "svr_interrupts");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("service/svr_heap", false, "svr_heap");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("arch/arm-m3m4/svc_handler", false, "svc_handler", "S");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("arch/arm-m3m4/pendsv_handler", false, "pendsv_handler", "S");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("arch/arm-m3m4/mutex", false, "mutex", "S");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }


            if (btd == false)
            {

                error = CompileSource("baset");

                if(error != ecode.ERROR_NONE)
                {
                    return error;
                }

            }

            //Stage7 : Generate param.lds

            ParamLDSGen();
            LogGenerate();


            ParamHeaderGen();

#if DEBUG
            Console.WriteLine("Done Opertion!");
#endif

            return ecode.ERROR_NONE;

        }


        private static ecode ReadDeviceFile()
        {
            bool fpartno = false;
            bool fgccid = false;
            bool fintcnt = false;
            bool fflash = false;
            bool fsram = false;

            if (devfile == "")
            {
                return ecode.ERROR_NO_DEVICE_FILE;
            }

            XDocument myfile = new XDocument();

            try
            {
                myfile = XDocument.Load(devfile);
            }
            catch
            {
                return ecode.ERROR_BAD_DEVICE_FILE;
            }


            foreach (XElement node in myfile.Nodes())
            {   
                if(node.Name != "device")
                {
                    continue;
                }

                foreach (XElement subnode in node.Nodes())
                {

                    if (subnode.Name == "intstr")
                    {
                        continue;
                    }

#if DEBUG
                    Console.WriteLine(subnode.Name + " = " + subnode.Value);
#endif

                    switch (subnode.Name.ToString())
                    {
                        case "partno":
                            fpartno = true;
                            partno = subnode.Value;
                            break;

                        case "gccid":
                            fgccid = true;
                            gccid = subnode.Value;
                            break;

                        case "intcnt":
                            fintcnt = true;
                            NUM_OF_INTERRUPTS = int.Parse(subnode.Value);
                            break;

                        case "flash":
                            fflash = true;
                            flash = long.Parse(subnode.Value);
                            break;

                        case "sram":
                            fsram = true;
                            sram = long.Parse(subnode.Value);
                            break;

                        default:
                            return ecode.ERROR_BAD_DEVICE_FILE;
                            
                    }

                }


            }

            if (fflash == false || fsram == false || fpartno == false || fgccid == false || fintcnt == false)
            {
                return ecode.ERROR_BAD_DEVICE_FILE;
            }

#if DEBUG
            Console.WriteLine();
#endif

            return ecode.ERROR_NONE;

        }

        public static ecode UpdateParameter(string param, int value)
        {

            switch (param)
            {
                case "KERNEL_REGION_SIZE":
                    KERNEL_REGION_SIZE = value;
                    break;

                case "UPPER_REGION_SIZE":
                    UPPER_REGION_SIZE = value;
                    break;

                case "GHMB_REGION_SIZE":
                    GHMB_REGION_SIZE = value;
                    break;

                case "PROCESS_STACK_SIZE":
                    PROCESS_STACK_SIZE = value;
                    break;

                case "PROCESS_PER_SEC":
                    PROCESS_PER_SEC = value;
                    break;

                case "ENABLE_SKIBIOS":
                    ENABLE_SKIBIOS = value;
                    break;

                case "ENABLE_SAFE_LOCK":
                    ENABLE_SAFE_LOCK = value;
                    break;

                case "DISABLE_BUFFER":
                    DISABLE_BUFFER = value;
                    break;

                default:
                    return ecode.ERROR_INVALID_DEFINE;
            }

            return ecode.ERROR_NONE;
        }

        public static ecode ParameterCheck()
        {
            int temp;

            //Check Kernel Region Size
            switch (KERNEL_REGION_SIZE)
            {
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    break;
                case 8:
                    break;
                case 10:
                    break;
                case 12:
                    break;
                case 14:
                    break;
                case 16:
                    break;
                case 20:
                    break;
                case 24:
                    break;
                case 28:
                    break;
                case 32:
                    break;
                case 40:
                    break;
                case 48:
                    break;
                case 56:
                    break;
                case 64:
                    break;
                default:
                    return ecode.ERROR_BAD_MPU_REGION;
            }

            //check Upper Kernel Size
            switch (UPPER_REGION_SIZE)
            {
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                default:
                    return ecode.ERROR_BAD_UKERNEL_REGION;
            }

            //check General Heap Marker Bookeeping Size
            switch (GHMB_REGION_SIZE)
            {
                case 1:
                    break;
                case 2:
                    break;
                default:
                    return ecode.ERROR_BAD_GHMB_SIZE;
            }

            //check Process Stack Size

            switch (PROCESS_STACK_SIZE)
            {
                case 512:
                    break;
                case 1024:
                    break;
                default:
                    return ecode.ERROR_BAD_PROCESS_STACK;
            }

            //check SRAM v/s Kernel Region Size
            if((KERNEL_REGION_SIZE * 1024) >= sram)
            {
                return ecode.ERROR_INSUFFICENT_SRAM;
            }

            temp = (int)sram / 1024;
            temp = NextPowerOf2(temp)/2;

            if(KERNEL_REGION_SIZE > temp)
            {
                return ecode.ERROR_KREGION_HALF_OVERFLOW;
            }

            return ecode.ERROR_NONE;

        }

        public static int NextPowerOf2(int number)
        {
            int pos = 0;
            int temp;

            temp = number;

            while (number > 0)
            {
                pos++;
                number = number >> 1;
            }

            number = (int)Math.Pow(2, pos);
            
            if ((temp * 2) == number)
            {
                number = temp;
            }

            return number;
        }

        public static ecode UpperKernelValidation()
        {

            ecode error = ecode.ERROR_NONE;

            //Stage1 : Compile all SKIBIOS source files
            error = CompileSource("kernel");

            if(error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("process");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("share");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("interrupt");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("heap");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("startup");

            if (error != ecode.ERROR_NONE)
            {
                return error;
            }

            error = CompileSource("kvar");

            if(error != ecode.ERROR_NONE)
            {
                return error;
            }

            //Stage 2: Generate Object dump
            string output;
            error = RunObjdump(out output);

            if(error != ecode.ERROR_NONE)
            {
                return error;
            }

            //Stage 3: Get Kernel Variables Unit Size
            string[] lines = output.Split('\n');

            foreach(string line in lines)
            {
                string[] columns = line.Split();

                if(columns.Length != 11)
                {
                    continue;
                }

                if(columns[7] == ".kdat")
                {
                    int x = int.Parse(columns[8]);
                    kdat_size += x;
                }

                if(columns[7] == ".kbss")
                {
                    int x = int.Parse(columns[8]);
                    kbss_size += x;
                }

            }

            kbss_size -= UNIT_PROCESS_SIZE;

            // generate kernel.o dissbemly
            error = GetDissembly("kernel", out output);

            if(error != ecode.ERROR_NONE)
            {
                return error;
            }

            StreamWriter fw;

            if (!File.Exists(outpath + "/kasm.S"))
            {
                fw = new StreamWriter(File.Create(outpath + "/kasm.S"));
            }
            else
            {
                fw = new StreamWriter(outpath + "/kasm.S");
            }

            fw.WriteLine(output);
            fw.Close();

#if DEBUG
            Console.WriteLine("kbss size = " + kbss_size.ToString());
            Console.WriteLine("kdat size = " + kdat_size.ToString());
#endif
            
            File.Delete(outpath + "/kernel.o");
            File.Delete(outpath + "/kernel.d");
            File.Delete(outpath + "/process.o");
            File.Delete(outpath + "/process.d");
            File.Delete(outpath + "/share.o");
            File.Delete(outpath + "/share.d");
            File.Delete(outpath + "/interrupt.o");
            File.Delete(outpath + "/interrupt.d");
            File.Delete(outpath + "/heap.o");
            File.Delete(outpath + "/heap.d");

            //stage 4: Compute MAX_PROCESS_COUNT and check if it is greater than 1
            int temp = 0;
            temp = UPPER_REGION_SIZE + GHMB_REGION_SIZE;
            temp = KERNEL_REGION_SIZE - temp;
            temp = temp * 1024;
            temp = temp / PROCESS_STACK_SIZE;

            MAX_PROCESS_COUNT = temp;

            if (MAX_PROCESS_COUNT <= 1)
            {
                return ecode.ERRRO_SHORT_PROESS_STACK;
            }

#if DEBUG
            Console.WriteLine("Calculated Max Process Count = " + MAX_PROCESS_COUNT.ToString());
            Console.WriteLine();
#endif

            //Stage 5: Upper Kernel Size Validation

            int size = MAX_PROCESS_COUNT * 16;
            size = size + kbss_size + kdat_size;
            size = size + (NUM_OF_INTERRUPTS * 4);

            size = (UPPER_REGION_SIZE * 1024) - size;

            if(size < 1024)
            {
                return ecode.ERROR_MSTACK_SIZE_VIOLATION;
            }

            return ecode.ERROR_NONE;
        }

        private static ecode CompileSource(string file, bool build_path = false, string outfile="", string extension = "c")
        {

            string param = PARAM;
            string include_path;

            Process process = new Process();
            process.StartInfo.FileName = binpath + "/arm-none-eabi-gcc.exe";

            include_path = "";
            include_path += " -I" + srcpath + "api ";
            include_path += " -I" + srcpath + "src/include ";
            include_path += " -I" + srcpath + "src/service";
            include_path += " -I" + srcpath + "src/arch/arm-m3m4/ ";
            include_path += " -I" + outpath + " ";

            param += " -DNUM_OF_INTERRUPTS=" + NUM_OF_INTERRUPTS;
            param += " -DMAX_PROCESS_COUNT=" + MAX_PROCESS_COUNT;
            param += " -DPROCESS_STACK_SIZE=" + (PROCESS_STACK_SIZE/4);
            param += " -DKERNEL_REGION_SIZE=" + KernelRegionToString(KERNEL_REGION_SIZE);
            param += " -DMPU_PSTACK_SIZE=" + PStackSizeToString(PROCESS_STACK_SIZE);
            param += " -DKERNEL_REGION_SRD=" + srdbit.ToString();
            param += " -DENABLE_SKIBIOS=" + (ENABLE_SKIBIOS >= 1 ? 1 : 0).ToString();
            param += " -DPROCESS_PER_SEC=" + PROCESS_PER_SEC;
            param += " -DHEAP_BOOKEEPING_SIZE=" + (GHMB_REGION_SIZE * 256).ToString();
            param += include_path;


            if (outfile == "")
            {
                outfile = file;
            }

            if (build_path == false)
            {
                process.StartInfo.Arguments = CFLAGS + " " + param + srcpath + "src/" + file + "." + extension + " -o" + outpath + "/" + outfile + ".o";
            }
            else
            {
                process.StartInfo.Arguments = CFLAGS + " " + param + outpath + "/" + file + "." + extension + " -o" + outpath + "/" + outfile + ".o";
            }

            process.StartInfo.UseShellExecute = false;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;

            try
            {
                process.Start();
            }
            catch
            {
                try
                {
                    process.WaitForExit();
                }
                catch
                {

                }

                return ecode.ERROR_COMPLIER_NOT_FOUND;

            }

#if DEBUG
            string output = process.StandardOutput.ReadToEnd();
            if (output != "")
            {
                Console.WriteLine(output);
            }
#endif


            process.WaitForExit();

            if(process.ExitCode != 0)
            {

#if DEBUG
                string err = process.StandardError.ReadToEnd();
                Console.WriteLine(err);
#endif

                return ecode.ERROR_COMPILATION_ERROR;
            }

            return ecode.ERROR_NONE;

        }

        private static ecode RunObjdump(out string output)
        {

            output = "";

            Process process = new Process();
            process.StartInfo.FileName = binpath + "/arm-none-eabi-objdump.exe";
            process.StartInfo.Arguments = "-t " + outpath + "/*.o";
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;

            try
            {
                process.Start();
            }
            catch
            {
                try
                {
                    process.WaitForExit();
                }
                catch
                {

                }

                return ecode.ERROR_COMPLIER_NOT_FOUND;

            }

            output = process.StandardOutput.ReadToEnd();

            process.WaitForExit();

            if (process.ExitCode != 0)
            {

#if DEBUG
                string err = process.StandardError.ReadToEnd();
                Console.WriteLine(err);
#endif

                return ecode.ERROR_COMPILATION_ERROR;
            }

            return ecode.ERROR_NONE;
        }

        private static ecode GetDissembly(string file, out string output)
        {

            ecode error = ecode.ERROR_NONE;

            output = "";

            Process process = new Process();
            process.StartInfo.FileName = binpath + "/arm-none-eabi-objdump.exe";
            process.StartInfo.Arguments = "-S -d  " + outpath + "/" + file + ".o";
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;

            try
            {
                process.Start();
            }
            catch
            {
                try
                {
                    process.WaitForExit();
                }
                catch
                {

                }

                return ecode.ERROR_COMPLIER_NOT_FOUND;

            }

            output = process.StandardOutput.ReadToEnd();

            process.WaitForExit();

            if (process.ExitCode != 0)
            {

#if DEBUG
                string err = process.StandardError.ReadToEnd();
                Console.WriteLine(err);
#endif

                return ecode.ERROR_COMPILATION_ERROR;
            }

            return error;

        }
      

        private static string PStackSizeToString(int size)
        {
            string str = "";

            switch (size)
            {

                case 512:
                    str = "MPU_REGION_SIZE_512B";
                    break;

                case 1024:
                    str = "MPU_REGION_SIZE_1KB";
                    break;

            }

            return str;

        }

        private static string KernelRegionToString(int size)
        {
            string str = "";

            int value = NextPowerOf2(size);

            switch (value)
            {
                case 8:
                    str = "MPU_REGION_SIZE_8KB";
                    break;

                case 16:
                    str = "MPU_REGION_SIZE_16KB";
                    break;

                case 32:
                    str = "MPU_REGION_SIZE_32KB";
                    break;

                case 64:
                    str = "MPU_REGION_SIZE_64KB";
                    break;

            }

            return str;

        }

        private static void ParamLDSGen()
        {

            StreamWriter fw;

            if (!File.Exists(outpath + "/param.lds"))
            {
                fw = new StreamWriter(File.Create(outpath + "/param.lds"));
            }
            else
            {
                fw = new StreamWriter(outpath + "/param.lds");
            }

            fw.WriteLine("_kernel_base_address = " + kbase_addr.ToString() + ";");
            fw.WriteLine("_app_base_address = " + abase_addr.ToString() + ";");
            fw.WriteLine("_kstack_end_addr = " + mstack_addr.ToString() + ";");
            fw.WriteLine("_proc_heap_addr = " + pstack_addr.ToString() + ";");
            fw.WriteLine("_ghmb_addr = " + ghmb_addr.ToString() + ";");

            fw.Close();

        }

        private static void ParamHeaderGen()
        {

            StreamWriter fw;

            if (!File.Exists(outpath + "/ccsparam.h"))
            {
                fw = new StreamWriter(File.Create(outpath + "/ccsparam.h"));
            }
            else
            {
                fw = new StreamWriter(outpath + "/ccsparam.h");
            }

            fw.WriteLine("#include <mpu.h>");
            fw.WriteLine();
            fw.WriteLine("#define MAX_PROCESS_COUNT " + MAX_PROCESS_COUNT);
            fw.WriteLine("#define PROCESS_STACK_SIZE " + PROCESS_STACK_SIZE/4);
            fw.WriteLine("#define KERNEL_REGION_SIZE " + KernelRegionToString(KERNEL_REGION_SIZE));
            fw.WriteLine("#define MPU_PSTACK_SIZE " + PStackSizeToString(PROCESS_STACK_SIZE));
            fw.WriteLine("#define KERNEL_REGION_SRD " + srdbit.ToString());
            fw.WriteLine("#define HEAP_BOOKEEPING_SIZE " + (GHMB_REGION_SIZE * 256).ToString());
            fw.WriteLine("#define NUM_OF_INTERRUPTS " + NUM_OF_INTERRUPTS.ToString());
            fw.WriteLine("#define PROCESS_PER_SEC " + PROCESS_PER_SEC);

            fw.Close();
        }

        private static void LogGenerate()
        {
            StreamWriter fw;
            int y, c;


            if (!File.Exists(outpath + "/skibios-log.txt"))
            {
                fw = new StreamWriter(File.Create(outpath + "/skibios-log.txt"));
            }
            else
            {
                fw = new StreamWriter(outpath + "/skibios-log.txt");
            }

            fw.WriteLine("SKIBIOS Object Generator Log");
            fw.WriteLine();

            fw.WriteLine(string.Format("{0:HH:mm:ss tt}", DateTime.Now));
            fw.WriteLine("--------------------------------------------------------");
            fw.WriteLine();

            if(ENABLE_SKIBIOS == 0)
            {
                fw.WriteLine("SKIBIOS is Disabled");
            }
            else
            {
                fw.WriteLine("SKIBIOS is Enabled");
            }

            fw.WriteLine();

            if(ENABLE_SAFE_LOCK == 0)
            {
                fw.WriteLine("Safe Mutex Locking is Disabled");
            }
            else
            {
                fw.WriteLine("Safe Mutex Locking is Enabled");
            }

            fw.WriteLine();

            if (DISABLE_BUFFER == 0)
            {
                fw.WriteLine("Buffering is Enabled");
            }
            else
            {
                fw.WriteLine("BUFFERING is Disabled");
            }

            fw.WriteLine();

            fw.WriteLine("User defined paramters");
            fw.WriteLine("--------------------------");
            fw.WriteLine("KERNEL_REGION_SIZE    :   " + KERNEL_REGION_SIZE);
            fw.WriteLine("UPPER_REGION_SIZE     :   " + UPPER_REGION_SIZE);
            fw.WriteLine("PROCESS_STACK_SIZE    :   " + PROCESS_STACK_SIZE);
            fw.WriteLine("GHMB_REGION_SIZE      :   " + GHMB_REGION_SIZE);
            fw.WriteLine();

            fw.WriteLine("PROCESS_PER_SECOND    :   " + PROCESS_PER_SEC);
            fw.WriteLine();

            fw.WriteLine("Calculation Log");
            fw.WriteLine("--------------------------");
            fw.WriteLine("Maximum Process count :   " + MAX_PROCESS_COUNT);
            fw.WriteLine("MPU SRD Bit           :   " + (srdbit << 8).ToString());

            y = kbase_addr + kbss_size + kdat_size + (MAX_PROCESS_COUNT * UNIT_PROCESS_SIZE);
            c = mstack_addr - y;
            fw.WriteLine("Main stack size       :   " + c + " Bytes");
            fw.WriteLine();

            fw.WriteLine("Target Device Info");
            fw.WriteLine("---------------------------");
            fw.WriteLine("Device Name           :   " + partno);
            fw.WriteLine("GCC Code              :   " + gccid);
            fw.WriteLine("Number of Interrupts  :   " + NUM_OF_INTERRUPTS);
            fw.WriteLine("Flash Size            :   " + (flash / 1024) + " KB");
            fw.WriteLine("SRAM Size             :   " + (sram / 1024) + " KB");

            fw.Close();
        }

    }

}
