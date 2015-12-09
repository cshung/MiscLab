namespace ManagedApplication
{
    using System;
    using System.Runtime.InteropServices;
    
    internal static class NativeMethods
    {
        [DllImport("NativeLibrary.dll")]
        internal static extern IntPtr GetNativeComponent();
    }

    internal static class Program
    {
        public static void Main(string[] args)
        {
            IntPtr unused = NativeMethods.GetNativeComponent();
            Console.WriteLine("Hello World");
        }
    }
}