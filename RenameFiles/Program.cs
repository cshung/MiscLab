using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RenameFiles
{
    class Program
    {
        static void Main(string[] args)
        {
            const string prefix = "NR ";
            const int numDigits = 3;

            var oldFullPaths = Directory.GetFiles(@"C:\Users\andrewau\Dropbox\openCV job\Backup\#3 NR");
            int fileId = 1;
            foreach (var oldFullPath in oldFullPaths)
            {
                string directory = Path.GetDirectoryName(oldFullPath);
                string extension = Path.GetExtension(oldFullPath);
                string newFileName = prefix + ZeroLead(fileId, numDigits) + extension;
                
                string newFullPath = Path.Combine(directory, newFileName);
                File.Move(oldFullPath, newFullPath);
                fileId++;
            }
        }

        private static string ZeroLead(int fileId, int numDigits)
        {
            StringBuilder sb = new StringBuilder(fileId.ToString());
            while (sb.Length < numDigits)
            {
                sb.Insert(0, '0');
            }
            return sb.ToString();
        }
    }
}
