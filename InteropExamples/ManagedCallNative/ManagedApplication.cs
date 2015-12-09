namespace ManagedApplication
{
    using System;
    using System.Runtime.InteropServices;
    
    internal static class NativeMethods
    {
        [DllImport("NativeLibrary.dll")]
        internal static extern NativeInterface.INativeComponent GetNativeComponent();
    }

    internal static class Program
    {
        public static void Main(string[] args)
        {
            NativeInterface.INativeComponent nativeComponent = NativeMethods.GetNativeComponent();
            int result;
            nativeComponent.NativeMethod(7, out result);
            Console.WriteLine(result);
        }
    }
}