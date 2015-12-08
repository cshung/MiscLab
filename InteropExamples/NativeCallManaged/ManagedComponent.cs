namespace ManagedLibrary
{
    using System.Runtime.InteropServices;

    [Guid("78530175-5d2a-4403-a25d-2247a8b16a72")]
    public interface IManagedComponent
    {
        int ManagedMethod(int s);
    }

    [Guid("b015d72f-a8ea-4f8e-925a-d95cc25c51f9")]
    public class ManagedComponent : IManagedComponent
    {
        public int ManagedMethod(int s)
        {
            return s + 1;
        }
    }
}
