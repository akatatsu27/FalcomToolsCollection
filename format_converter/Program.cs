using System.Reflection;
using System.Text;
using format_converter;

internal class Program
{
    static async Task Main(string[] args)
    {
        Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
        if (!Directory.Exists("./MSJson"))
            Directory.CreateDirectory("./MSJson");
        if (!Directory.Exists("./MSDT"))
            Directory.CreateDirectory("./MSDT");
        if (args.Length == 0)
        {

            var versionString = Assembly.GetEntryAssembly()?
                                    .GetCustomAttribute<AssemblyInformationalVersionAttribute>()?
                                    .InformationalVersion
                                    .ToString();

            Console.WriteLine($"format converter v{versionString}");
            Console.WriteLine("-------------");
            Console.WriteLine("\nUsage:");
            Console.WriteLine("  <folder or file>");
            Console.WriteLine("Press enter to close...");
            Console.ReadLine();
            return;
        }
        else if(args.Length == 1)
        {
            FileAttributes attr = File.GetAttributes(args[0]);

            if (attr.HasFlag(FileAttributes.Directory))
            {
                if (Directory.GetFiles(args[0]).Any() && Directory.GetFiles(args[0]).First().Contains(".json"))
                {
                    await MsFileConverter.ToDataTableFromFolderAsync(args[0]);
                }
                else
                {                    
                    await MsFileConverter.ToJsonFromFolderAsync(args[0]);
                }
            }
            else
            {
                if (args[0].Contains(".json"))
                {
                    await MsFileConverter.ToDataTableFromFileAsync(args[0]);
                }
                else if (args[0].Contains("._DT"))
                {
                    await MsFileConverter.ToJsonFromFileAsync(args[0]);
                }
                else
                    Console.WriteLine("invalid arguments");
            }
            Console.WriteLine("Press enter to close...");
            Console.ReadLine();
        }
        else
        {
            Console.WriteLine("invalid arguments");
        }
    }
}