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
		string curDir = AppContext.BaseDirectory;
		Directory.SetCurrentDirectory(curDir);
		if (!Directory.Exists("./output"))
			Directory.CreateDirectory("./output");
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
			string filename = Path.GetFileNameWithoutExtension(args[0]);
			if (args[0].EndsWith("._DT") || args[0].EndsWith("._dt"))
			{
				T_Magic_File file = await T_Magic_File.FromFile(args[0]);
				string json = JsonConvert.SerializeObject(file, Formatting.Indented, JsonSettings);
				await File.WriteAllTextAsync($"./output/{filename}.json", json);	
			}
			else if (args[0].EndsWith(".json"))
			{
				T_Magic_File fileA = JsonConvert.DeserializeObject<T_Magic_File>(await File.ReadAllTextAsync(args[0]), JsonSettings)!;
				await File.WriteAllBytesAsync($"./output/{filename}._DT", await fileA.ToByteArray());
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
			Console.WriteLine("Press enter to close...");
			Console.ReadLine();
			return;
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