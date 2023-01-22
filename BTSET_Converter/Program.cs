using System.Reflection;
using BTSET1_Converter;
using Newtonsoft.Json;

#if DEBUG
BTSET2 set2 = new();
await set2.Parse("T_BTSET2._DT");

BTSET2Json json = new(set2);

string bonuses = JsonConvert.SerializeObject(json.BonusesTable, Formatting.Indented);
string models = JsonConvert.SerializeObject(json.ModelTable, Formatting.Indented);
string placements = JsonConvert.SerializeObject(json.PlacementTable, Formatting.Indented);
string battles1 = JsonConvert.SerializeObject(json.Battles, Formatting.Indented);
string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);
//string battles2 = JsonConvert.SerializeObject(json.Battles2, Formatting.Indented);

string dir = "T_BTSET2";
Directory.CreateDirectory(dir);
await File.WriteAllTextAsync(Path.Combine(dir, Bonuses), bonuses);
await File.WriteAllTextAsync(Path.Combine(dir, Models), models);
await File.WriteAllTextAsync(Path.Combine(dir, Placements), placements);
await File.WriteAllTextAsync(Path.Combine(dir, Battles1), battles1);
await File.WriteAllTextAsync(Path.Combine(dir, AutoBattles), autobattles);
//await File.WriteAllTextAsync(Path.Combine(dir, "Battles2.json"), battles2);

BTSET2Json newJson = await BTSET2Json.Deserialize(dir);
byte[] compl = newJson.Compile();

await File.WriteAllBytesAsync("T_BTSET2_TEST._DT", compl);

Console.WriteLine("Done!");

internal partial class Program
{
    internal const string Bonuses = "Bonuses.json";
    internal const string Models = "Models.json";
    internal const string Placements = "Placements.json";
    internal const string Battles1 = "Battles1.json";
    internal const string AutoBattles = "AutoBattles.json";
    internal const string Battles2 = "Battles2.json";
}

#else

internal class Program
{
    internal const string Bonuses = "Bonuses.json";
    internal const string Models = "Models.json";
    internal const string Placements = "Placements.json";
    internal const string Battles1 = "Battles1.json";
    internal const string AutoBattles = "AutoBattles.json";
    internal const string Battles2 = "Battles2.json";
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
        byte oneOrTwo = 0;
        if (attr.HasFlag(FileAttributes.Directory))
        {           
            if (args[0].Split(Path.DirectorySeparatorChar).Last() == "T_BTSET1")
                oneOrTwo = 1;
            else if (args[0].Split(Path.DirectorySeparatorChar).Last() == "T_BTSET2")
                oneOrTwo = 2;
            else
            {
                Console.WriteLine("Error: expected folder name T_BTSET1 or T_BTSET2");
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
            if (!fNames.Contains(Battles1)) missingFiles += $"{Battles1}\n";
            if (!fNames.Contains(AutoBattles)) missingFiles += $"{AutoBattles}\n";

            if (oneOrTwo == 1)
                if (!fNames.Contains(Battles2)) missingFiles += $"{Battles2}\n";
            if (missingFiles != "")
            {
                Console.WriteLine($"Error: missing files:\n{missingFiles}");
                goto END;
            }
            if (oneOrTwo == 1)
                await CompileBTSET1(args[0]);
            else
                await CompileBTSET2(args[0]);
        }
        else
        {
            string fName = args[0].Split(Path.DirectorySeparatorChar).Last();
            if (fName == "T_BTSET1._DT")
            {
                oneOrTwo = 1;   
            }
            else if (fName == "T_BTSET2._DT")
            {
                oneOrTwo = 2;
            }
            else
            {
                Console.WriteLine($"Error: expected T_BTSET1._DT or T_BTSET2._DT.\nGot {fName}");
                goto END;
            }
            if (oneOrTwo == 1)
                await DecompileBTSET1(args[0]);
            else
                await DecompileBTSET2(args[0]);
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
        string battles1 = JsonConvert.SerializeObject(json.Battles1, Formatting.Indented);
        string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);
        string battles2 = JsonConvert.SerializeObject(json.Battles2, Formatting.Indented);

        string dir = "T_BTSET1";
        Directory.CreateDirectory(dir);
        await File.WriteAllTextAsync(Path.Combine(dir, Bonuses), bonuses);
        await File.WriteAllTextAsync(Path.Combine(dir, Models), models);
        await File.WriteAllTextAsync(Path.Combine(dir, Placements), placements);
        await File.WriteAllTextAsync(Path.Combine(dir, Battles1), battles1);
        await File.WriteAllTextAsync(Path.Combine(dir, AutoBattles), autobattles);
        await File.WriteAllTextAsync(Path.Combine(dir, Battles2), battles2);

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
        string battles1 = JsonConvert.SerializeObject(json.Battles, Formatting.Indented);
        string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);

        string dir = "T_BTSET2";
        Directory.CreateDirectory(dir);
        await File.WriteAllTextAsync(Path.Combine(dir, Bonuses), bonuses);
        await File.WriteAllTextAsync(Path.Combine(dir, Models), models);
        await File.WriteAllTextAsync(Path.Combine(dir, Placements), placements);
        await File.WriteAllTextAsync(Path.Combine(dir, Battles1), battles1);
        await File.WriteAllTextAsync(Path.Combine(dir, AutoBattles), autobattles);

        Console.WriteLine("Successfully decompiled T_BTSET2._DT!");
        return true;
    }
}
#endif