using System.Reflection;

public class Program
{
	public static void Main(string[] args)
	{
		if (args.Length == 0)
		{
			string? versionString = Assembly.GetEntryAssembly()?
									.GetCustomAttribute<AssemblyInformationalVersionAttribute>()?
									.InformationalVersion
									.ToString();

			Console.WriteLine($"T_MAGIC_Converter v{versionString}");
			Console.WriteLine("-------------");
			Console.WriteLine("\nUsage:");
			Console.WriteLine("  <file>");
			Console.WriteLine("Press enter to close...");
			Console.ReadLine();
			return;
		}
		else if (args.Length == 1)
		{
			if (!args[0].EndsWith("T_MAGIC ._DT"))
			{
				Console.WriteLine("Please provide the \"T_MAGIC ._DT\" file location");
				Console.WriteLine("Press enter to close...");
				Console.ReadLine();
				return;
			}
		}
		else
		{
			Console.WriteLine("Invalid arguments");
		}
	}
}