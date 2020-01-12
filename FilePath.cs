using System;
using System.IO;
using System.Text;

namespace FilePath
{
    
    public class FilePath
    {
        static int Main()
        {
            File.WriteAllText("fileName1", "我.txt");
            File.WriteAllBytes("fileName", Encoding.GetEncoding("gbk").GetBytes("我.txt"));
            return 0;
        }
    }
}
