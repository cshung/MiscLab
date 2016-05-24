using System.Collections.Generic;

namespace LayoutExample
{
    internal class MainWindowViewModel
    {
        public MainWindowViewModel()
        {
        }

        public List<string> Data
        {
            get
            {
                List<string> result = new List<string>();
                for (int i =0; i < 100; i++)
                {
                    result.Add(i + "");
                }
                return result;
            }
        }
    }
}