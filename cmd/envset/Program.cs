using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;
using System.Reflection;

namespace envset
{

    public class OLEHelper
    {
        public static object createObject(string progID)
        {
            var t = Type.GetTypeFromProgID(progID);
            return Activator.CreateInstance(t);
        }

        public static void freeObject(object o)
        {
            Marshal.FinalReleaseComObject(o);
        }

        public static object getProperty(object o, string name)
        {
            return o.GetType().InvokeMember(name, BindingFlags.GetProperty, null, o, null);
        }

        public static object getProperty(object o, string name1, string name2)
        {
            return o.GetType().InvokeMember(name1, BindingFlags.GetProperty, null, o, new object[] { name2 });
        }

        public static void setPropery(object o, string name, object value)
        {
            o.GetType().InvokeMember(name, BindingFlags.SetProperty, null, o, new object[] { value });
        }

        public static void setPropery(object o, string name1, string name2, object value)
        {
            o.GetType().InvokeMember(name1, BindingFlags.SetProperty, null, o, new object[] { name2, value });
        }

        // paramsは可変長引数
        public static object call(object o, string name, params object[] args)
        {
            return o.GetType().InvokeMember(name, BindingFlags.InvokeMethod, null, o, args);
        }
    }

    class Program
    {
        static void Usage()
        {
            Console.WriteLine("Usage: envset set|append|prepend <VAR_NAME> <VALUE>");

        }
        static void Do_Set(string name, string value)
        {
            var wshell = OLEHelper.createObject("WScript.Shell");
            var env = OLEHelper.getProperty(wshell, "Environment");
            var user = OLEHelper.getProperty(wshell, "Environment", "User");
            OLEHelper.setPropery(user, "Item", name, value);
        }
        static void Do_Append(string name, string value)
        {
            var wshell = OLEHelper.createObject("WScript.Shell");
            var env = OLEHelper.getProperty(wshell, "Environment");
            var user = OLEHelper.getProperty(wshell, "Environment", "User");
            var prev = OLEHelper.getProperty(user, "Item", name).ToString();
            var prev_list = prev.Split(';');
            List<string> result = new List<string>();
            for (int i = 0; i < prev_list.Length; i++)
            {
                if (!string.Equals(prev_list[i], value, StringComparison.CurrentCultureIgnoreCase))
                {
                    result.Add(prev_list[i]);
                }
            }
            result.Add(value);
            OLEHelper.setPropery(user, "Item", name, string.Join(";", result));
        }
        static void Do_Prepend(string name, string value)
        {
            var wshell = OLEHelper.createObject("WScript.Shell");
            var env = OLEHelper.getProperty(wshell, "Environment");
            var user = OLEHelper.getProperty(wshell, "Environment", "User");
            var prev = OLEHelper.getProperty(user, "Item", name).ToString();
            var prev_list = prev.Split(';');
            List<string> result = new List<string>();
            result.Add(value);
            for (int i = 0; i < prev_list.Length; i++)
            {
                if (!string.Equals(prev_list[i], value, StringComparison.CurrentCultureIgnoreCase))
                {
                    result.Add(prev_list[i]);
                }
            }
            OLEHelper.setPropery(user, "Item", name, string.Join(";", result));
        }
        static void Main(string[] args)
        {
            /*
            Console.WriteLine("Main(1)");
            Console.WriteLine(args.Length);
            for(int i=0; i<args.Length; i++)
            {
                Console.WriteLine(args[i]);
            }
            */
            if(args.Length != 3)
            {
                Usage();
                Environment.Exit(1);
            }
            switch(args[0])
            {
                case "set":
                    Do_Set(args[1], args[2]);
                    break;
                case "append":
                    Do_Append(args[1], args[2]);
                    break;
                case "prepend":
                    Do_Prepend(args[1], args[2]);
                    break;
                default:
                    Usage();
                    Environment.Exit(1);
                    break;
            }
        }
    }
}
