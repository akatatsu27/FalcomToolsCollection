using System.Reflection;

namespace AS_Converter;

internal partial class Program
{
	static async Task Main(string[] args)
	{
#if DEBUG
		byte[] data = await File.ReadAllBytesAsync("AS32400 ._DT");
		AS_Third file = AS_Third.Read(data);
		bool foo = true;
#else
		if (args.Length == 0)
		{

			var versionString = Assembly.GetEntryAssembly()?
									.GetCustomAttribute<AssemblyInformationalVersionAttribute>()?
									.InformationalVersion
									.ToString();

			Console.WriteLine($"BTSET Converter v{versionString}");
			Console.WriteLine("-------------");
			Console.WriteLine("\nUsage:");
			Console.WriteLine("1)  [file]");
			Console.WriteLine("    try to decompile ASXXXXXX._DT to my custom assembly!");
			Console.WriteLine("2)  [folder]");
			Console.WriteLine("    try to decompile a folder of ASXXXXXX._DT to my custom assembly!");
			Console.WriteLine("Press enter to close...");
			Console.ReadLine();
			return;
		}
		FileAttributes attr = File.GetAttributes(args[0]);
		if (attr.HasFlag(FileAttributes.Directory))
		{
			IEnumerable<string> filenames = Directory.GetFiles(args[0]);
			if (!filenames.Any())
			{
				Console.WriteLine($"empty folder: {args[0]}");
				goto END;
			}
			IEnumerable<string> fNames = filenames.Select(w => w.Split(Path.DirectorySeparatorChar).Last());
		}
		else
		{
		}

	END:
		Console.WriteLine("Press enter to close...");
		Console.ReadLine();
#endif
	}
}