using System.Reflection;
using BTSET_Converter;
using Newtonsoft.Json;

#if DEBUG
FC_BTLSET set = new();
await set.Parse("T_BTLSET._DT");

FC_BTLSETJson json = new(set);

string bonuses = JsonConvert.SerializeObject(json.BonusesTable, Formatting.Indented);
string models = JsonConvert.SerializeObject(json.ModelTable, Formatting.Indented);
string placements = JsonConvert.SerializeObject(json.PlacementTable, Formatting.Indented);
string battles = JsonConvert.SerializeObject(json.Battles, Formatting.Indented);
//string battles1 = JsonConvert.SerializeObject(json.Type1Battles, Formatting.Indented);
string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);
//string battles2 = JsonConvert.SerializeObject(json.Type2Battles, Formatting.Indented);

string dir = "T_BTLSET";
Directory.CreateDirectory(dir);
await File.WriteAllTextAsync(Path.Combine(dir, Bonuses), bonuses);
await File.WriteAllTextAsync(Path.Combine(dir, Models), models);
await File.WriteAllTextAsync(Path.Combine(dir, Placements), placements);
await File.WriteAllTextAsync(Path.Combine(dir, Battles), battles);
//await File.WriteAllTextAsync(Path.Combine(dir, Type1Battles), battles1);
await File.WriteAllTextAsync(Path.Combine(dir, AutoBattles), autobattles);
//await File.WriteAllTextAsync(Path.Combine(dir, Type2Battles), battles2);

FC_BTLSETJson newJson = await FC_BTLSETJson.Deserialize(dir);
byte[] compl = newJson.Compile();

await File.WriteAllBytesAsync("T_BTLSET_TEST._DT", compl);

Console.WriteLine("Done!");

#endif
internal partial class Program
{
	internal const string Bonuses = "Bonuses.json";
	internal const string Models = "Models.json";
	internal const string Placements = "Placements.json";
	internal const string Type1Battles = "Type1Battles.json";
	internal const string AutoBattles = "AutoBattles.json";
	internal const string Type2Battles = "Type2Battles.json";

    internal const string Battles = "Battles.json";
}
#if !DEBUG
internal partial class Program
{
    static async Task Main(string[] args)
    {
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
            Console.WriteLine("    try to decompile T_BTSET1._DT or T_BTSET2._DT to it's components in json format");
            Console.WriteLine("2)  [folder]");
            Console.WriteLine("    try to compile folder to T_BTSET1._DT or T_BTSET2._DT (checks folder name)");
            Console.WriteLine("Press enter to close...");
            Console.ReadLine();
            return;
        }
        FileAttributes attr = File.GetAttributes(args[0]);
        byte oneOrTwoOrFC = 0;
        if (attr.HasFlag(FileAttributes.Directory))
        {
            string dirName = args[0].Split(Path.DirectorySeparatorChar).Last();
			if (dirName == "T_BTSET1")
                oneOrTwoOrFC = 1;
            else if (dirName == "T_BTSET2")
                oneOrTwoOrFC = 2;
            else if (dirName == "T_BTLSET")
            {
                oneOrTwoOrFC = 3;
            }
            else
            {
                Console.WriteLine("Error: expected folder name T_BTSET1 or T_BTSET2 or T_BTLSET");
                goto END;
            }

            IEnumerable<string> filenames = Directory.GetFiles(args[0]);
            if (!filenames.Any())
            {
                Console.WriteLine($"empty folder: {args[0]}");
                goto END;
            }
            IEnumerable<string> fNames = filenames.Select(w => w.Split(Path.DirectorySeparatorChar).Last());
            string missingFiles = "";
            if (!fNames.Contains(Bonuses)) missingFiles += $"{Bonuses}\n";
            if (!fNames.Contains(Models)) missingFiles += $"{Models}\n";
            if (!fNames.Contains(Placements)) missingFiles += $"{Placements}\n";
            if (!fNames.Contains(AutoBattles)) missingFiles += $"{AutoBattles}\n";
			if (oneOrTwoOrFC == 1 || oneOrTwoOrFC == 2)
				if (!fNames.Contains(Type2Battles)) missingFiles += $"{Type1Battles}\n";
			if (oneOrTwoOrFC == 1)
                if (!fNames.Contains(Type2Battles)) missingFiles += $"{Type2Battles}\n";
			if (oneOrTwoOrFC == 3)
				if (!fNames.Contains(Battles)) missingFiles += $"{Battles}\n";
			if (missingFiles != "")
            {
                Console.WriteLine($"Error: missing files:\n{missingFiles}");
                goto END;
            }
            if (oneOrTwoOrFC == 1)
                await CompileBTSET1(args[0]);
            else if (oneOrTwoOrFC == 2)
                await CompileBTSET2(args[0]);
            else
                await CompileFC_BTLSET(args[0]);
        }
        else
        {
            string fName = args[0].Split(Path.DirectorySeparatorChar).Last();
            if (fName == "T_BTSET1._DT")
            {
                oneOrTwoOrFC = 1;   
            }
            else if (fName == "T_BTSET2._DT")
            {
                oneOrTwoOrFC = 2;
            }
            else if (fName == "T_BTLSET._DT")
            {
                oneOrTwoOrFC = 3;
            }
            else
            {
                Console.WriteLine($"Error: expected T_BTSET1._DT or T_BTSET2._DT or T_BTLSET._DT.\nGot {fName}");
                goto END;
            }
            if (oneOrTwoOrFC == 1)
                await DecompileBTSET1(args[0]);
            else if (oneOrTwoOrFC == 2)
                await DecompileBTSET2(args[0]);
            else
                await DecompileFC_BTLSET(args[0]);
        }

    END:
        Console.WriteLine("Press enter to close...");
        Console.ReadLine();
    }

    private static async Task<bool> CompileBTSET1(string dirPath)
    {
        byte[] compl;
        try
        {
            BTSET1Json newJson = await BTSET1Json.Deserialize(dirPath);
            compl = newJson.Compile();
        }
        catch (Exception e)
        {
            Console.WriteLine("An error occured while compiling T_BTSET1._DT");
            Console.WriteLine(e);
            return false;
        }
        if (!Directory.Exists("./output"))
            Directory.CreateDirectory("./output");
        await File.WriteAllBytesAsync("./output/T_BTSET1._DT", compl);

        Console.WriteLine("Succesfully compiled T_BTSET1._DT!");
        return true;
    }

    private static async Task<bool> DecompileBTSET1(string fPath)
    {
        BTSET1 set1 = new();
        try
        {
            await set1.Parse(fPath);
        }
        catch (Exception e)
        {
            Console.WriteLine("An error occured while parsing T_BTSET1._DT");
            Console.WriteLine(e);
            return false;
        }

        BTSET1Json json = new(set1);

        string bonuses = JsonConvert.SerializeObject(json.BonusesTable, Formatting.Indented);
        string models = JsonConvert.SerializeObject(json.ModelTable, Formatting.Indented);
        string placements = JsonConvert.SerializeObject(json.PlacementTable, Formatting.Indented);
        string battles1 = JsonConvert.SerializeObject(json.Type1Battles, Formatting.Indented);
        string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);
        string battles2 = JsonConvert.SerializeObject(json.Type2Battles, Formatting.Indented);

        string dir = "T_BTSET1";
        Directory.CreateDirectory(dir);
        await File.WriteAllTextAsync(Path.Combine(dir, Bonuses), bonuses);
        await File.WriteAllTextAsync(Path.Combine(dir, Models), models);
        await File.WriteAllTextAsync(Path.Combine(dir, Placements), placements);
        await File.WriteAllTextAsync(Path.Combine(dir, Type1Battles), battles1);
        await File.WriteAllTextAsync(Path.Combine(dir, AutoBattles), autobattles);
        await File.WriteAllTextAsync(Path.Combine(dir, Type2Battles), battles2);

        Console.WriteLine("Successfully decompiled T_BTSET1._DT!");
        return true;
    }

    private static async Task<bool> CompileBTSET2(string dirPath)
    {
        byte[] compl;
        try
        {
            BTSET2Json newJson = await BTSET2Json.Deserialize(dirPath);
            compl = newJson.Compile();
        }
        catch (Exception e)
        {
            Console.WriteLine("An error occured while compiling T_BTSET2._DT");
            Console.WriteLine(e);
            return false;
        }
        if (!Directory.Exists("./output"))
            Directory.CreateDirectory("./output");
        await File.WriteAllBytesAsync("./output/T_BTSET2._DT", compl);

        Console.WriteLine("Succesfully compiled T_BTSET2._DT!");
        return true;
    }

    private static async Task<bool> DecompileBTSET2(string fPath)
    {
        BTSET2 set2 = new();
        try
        {
            await set2.Parse(fPath);
        }
        catch (Exception e)
        {
            Console.WriteLine("An error occured while parsing T_BTSET2._DT");
            Console.WriteLine(e);
            return false;
        }

        BTSET2Json json = new(set2);

        string bonuses = JsonConvert.SerializeObject(json.BonusesTable, Formatting.Indented);
        string models = JsonConvert.SerializeObject(json.ModelTable, Formatting.Indented);
        string placements = JsonConvert.SerializeObject(json.PlacementTable, Formatting.Indented);
        string battles1 = JsonConvert.SerializeObject(json.Type1Battles, Formatting.Indented);
        string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);

        string dir = "T_BTSET2";
        Directory.CreateDirectory(dir);
        await File.WriteAllTextAsync(Path.Combine(dir, Bonuses), bonuses);
        await File.WriteAllTextAsync(Path.Combine(dir, Models), models);
        await File.WriteAllTextAsync(Path.Combine(dir, Placements), placements);
        await File.WriteAllTextAsync(Path.Combine(dir, Type1Battles), battles1);
        await File.WriteAllTextAsync(Path.Combine(dir, AutoBattles), autobattles);

        Console.WriteLine("Successfully decompiled T_BTSET2._DT!");
        return true;
    }

	private static async Task<bool> CompileFC_BTLSET(string dirPath)
	{
		byte[] compl;
		try
		{
			FC_BTLSETJson newJson = await FC_BTLSETJson.Deserialize(dirPath);
			compl = newJson.Compile();
		}
		catch (Exception e)
		{
			Console.WriteLine("An error occured while compiling T_BTLSET._DT");
			Console.WriteLine(e);
			return false;
		}
		if (!Directory.Exists("./output"))
			Directory.CreateDirectory("./output");
		await File.WriteAllBytesAsync("./output/T_BTLSET._DT", compl);

		Console.WriteLine("Succesfully compiled T_BTLSET._DT!");
		return true;
	}

	private static async Task<bool> DecompileFC_BTLSET(string fPath)
	{
		FC_BTLSET set = new();
		try
		{
			await set.Parse(fPath);
		}
		catch (Exception e)
		{
			Console.WriteLine("An error occured while parsing T_BTLSET._DT");
			Console.WriteLine(e);
			return false;
		}

		FC_BTLSETJson json = new(set);

		string bonuses = JsonConvert.SerializeObject(json.BonusesTable, Formatting.Indented);
		string models = JsonConvert.SerializeObject(json.ModelTable, Formatting.Indented);
		string placements = JsonConvert.SerializeObject(json.PlacementTable, Formatting.Indented);
		string battles = JsonConvert.SerializeObject(json.Battles, Formatting.Indented);
		string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);

		string dir = "T_BTLSET";
		Directory.CreateDirectory(dir);
		await File.WriteAllTextAsync(Path.Combine(dir, Bonuses), bonuses);
		await File.WriteAllTextAsync(Path.Combine(dir, Models), models);
		await File.WriteAllTextAsync(Path.Combine(dir, Placements), placements);
		await File.WriteAllTextAsync(Path.Combine(dir, Battles), battles);
		await File.WriteAllTextAsync(Path.Combine(dir, AutoBattles), autobattles);

		Console.WriteLine("Successfully decompiled T_BTLSET._DT!");
		return true;
	}
}
#endif