using System.IO;
using System.Reflection;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Shared;
using T_MAGIC_Converter;

public class Program
{
	private static readonly JsonSerializerSettings JsonSettings = new()
	{
		Converters = new List<JsonConverter>() { new ByteArrayConverter(), new StringEnumConverter() }
	};
	public static async Task Main(string[] args)
	{
		string curDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
		Directory.SetCurrentDirectory(curDir);
#if !DEBUG
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
			if (args[0].EndsWith("._DT") || args[0].EndsWith("._dt"))
			{
				T_Magic_File file = await T_Magic_File.FromFile(args[0]);
				string json = JsonConvert.SerializeObject(file, Formatting.Indented, JsonSettings);
				await File.WriteAllTextAsync($"./{args[0]}.json", json);	
			}
			else if (args[0].EndsWith(".json"))
			{
				//TODO
			}
			else
			{
				Console.WriteLine("Invalid file extension");
				Console.WriteLine("Press enter to close...");
				Console.ReadLine();
				return;
			}			
		}
		else
		{
			Console.WriteLine("Invalid arguments");
		}
#else
		T_Magic_File file = await T_Magic_File.FromFile("T_MAGIC ._DT");
		string json = JsonConvert.SerializeObject(file, Formatting.Indented, JsonSettings);
		await File.WriteAllTextAsync($"./T_MAGIC ._DT.json", json);
		T_Magic_File fileA = JsonConvert.DeserializeObject<T_Magic_File>(await File.ReadAllTextAsync("./T_MAGIC ._DT.json"), JsonSettings)!;
		await File.WriteAllBytesAsync("./T_MAGIC .DT.new", await fileA.ToByteArray());
#endif
	}
}