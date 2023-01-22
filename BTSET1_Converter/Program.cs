using System.Reflection;
using BTSET1_Converter;
using Newtonsoft.Json;

#if DEBUG
BTSET1 set1 = new();
await set1.Parse("T_BTSET1._DT");

LeJson json = new(set1);

string bonuses = JsonConvert.SerializeObject(json.BonusesTable, Formatting.Indented);
string models = JsonConvert.SerializeObject(json.ModelTable, Formatting.Indented);
string placements = JsonConvert.SerializeObject(json.PlacementTable, Formatting.Indented);
string battles1 = JsonConvert.SerializeObject(json.Battles1, Formatting.Indented);
string autobattles = JsonConvert.SerializeObject(json.AutoBattles, Formatting.Indented);
string battles2 = JsonConvert.SerializeObject(json.Battles2, Formatting.Indented);

string dir = "T_BTSET1";
Directory.CreateDirectory(dir);
await File.WriteAllTextAsync(Path.Combine(dir, "Bonuses.json"), bonuses);
await File.WriteAllTextAsync(Path.Combine(dir, "Models.json"), models);
await File.WriteAllTextAsync(Path.Combine(dir, "Placements.json"), placements);
await File.WriteAllTextAsync(Path.Combine(dir, "Battles1.json"), battles1);
await File.WriteAllTextAsync(Path.Combine(dir, "AutoBattles.json"), autobattles);
await File.WriteAllTextAsync(Path.Combine(dir, "Battles2.json"), battles2);

LeJson newJson = await LeJson.Deserialize(dir);
byte[] compl = newJson.Compile();

await File.WriteAllBytesAsync("T_BTSET1_TEST._DT", compl);

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

            Console.WriteLine($"BTSET1 Converter v{versionString}");
            Console.WriteLine("-------------");
            Console.WriteLine("\nUsage:");
            Console.WriteLine("1)  [file]");
            Console.WriteLine("    try to decompile T_BTSET1._DT to it's components in json format");
            Console.WriteLine("2)  [folder]");
            Console.WriteLine("    try to compile folder to T_BTSET1._DT");
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
            string missingFiles = "";
            if (!fNames.Contains(Bonuses)) missingFiles += $"{Bonuses}\n";
            if (!fNames.Contains(Models)) missingFiles += $"{Models}\n";
            if (!fNames.Contains(Placements)) missingFiles += $"{Placements}\n";
            if (!fNames.Contains(Battles1)) missingFiles += $"{Battles1}\n";
            if (!fNames.Contains(AutoBattles)) missingFiles += $"{AutoBattles}\n";
            if (!fNames.Contains(Battles2)) missingFiles += $"{Battles2}\n";
            if (missingFiles != "")
            {
                Console.WriteLine($"Error: missing files:\n{missingFiles}");
                goto END;
            }
            byte[] compl;
            try
            {
                LeJson newJson = await LeJson.Deserialize(args[0]);
                compl = newJson.Compile();
            }
            catch(Exception e)
            {
                Console.WriteLine("An error occured while compiling T_BTSET1._DT");
                Console.WriteLine(e);
                goto END;
            }
            if (!Directory.Exists("./output"))
                Directory.CreateDirectory("./output");
            await File.WriteAllBytesAsync("./output/T_BTSET1._DT", compl);

            Console.WriteLine("Succesfully compiled T_BTSET1._DT!");
        }
        else
        {
            string fName = args[0].Split(Path.DirectorySeparatorChar).Last();
            if (fName != "T_BTSET1._DT")
            {
                Console.WriteLine($"Error: expected T_BTSET1._DT.\nGot {fName}");
                goto END;
            }
            BTSET1 set1 = new();
            try
            {
                await set1.Parse(args[0]);
            }
            catch(Exception e)
            {
                Console.WriteLine("An error occured while parsing T_BTSET1._DT");
                Console.WriteLine(e);
                goto END;
            }

            LeJson json = new(set1);

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
        }

    END:
        Console.WriteLine("Press enter to close...");
        Console.ReadLine();
    }
}
#endif