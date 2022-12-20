using System.Reflection;
using System.Text;
using archive_extractor;
using static archive_extractor.dir_file;

internal class Program
{
    static async Task Main(string[] args)
    {
        if (args.Length == 0)
        {

            var versionString = Assembly.GetEntryAssembly()?
                                    .GetCustomAttribute<AssemblyInformationalVersionAttribute>()?
                                    .InformationalVersion
                                    .ToString();

            Console.WriteLine($"falcom data extractor v{versionString}");
            Console.WriteLine("-------------");
            Console.WriteLine("\nUsage:");
            Console.WriteLine("1)  [folder]");
            Console.WriteLine("    try to extract all archives in folder");
            Console.WriteLine("2)  [file]...");
            Console.WriteLine("    try to extract from multiple files");
            Console.WriteLine("Press enter to close...");
            Console.ReadLine();
            return;
        }
        FileAttributes attr = File.GetAttributes(args[0]);

        if (attr.HasFlag(FileAttributes.Directory)) // if user passed a whole folder as an argument
        {
            IEnumerable<string> filenames = Directory.GetFiles(args[0]);
            if (!filenames.Any())
            {
                Console.WriteLine($"empty folder: {args[0]}");
                return;
            }

            await ExtractArchiveMany(filenames);
        }
        else // check for individual .dat .dir pairs in args
        {
            await ExtractArchiveMany(args);
        }
        Console.WriteLine("Press enter to close...");
        Console.ReadLine();
    }   

    static async Task ExtractArchiveMany(IEnumerable<string> filenames)
    {
        filenames = filenames.Select(w => w.Split(Path.DirectorySeparatorChar).Last());
        IEnumerable<string> dats = filenames.Where(w => w.Contains(".dat"));
        var tasks = new List<Task>();
        if(!dats.Any())
        {
            Console.WriteLine("no .dat files provided!");
            return;
        }
        foreach (string dat in dats)
        {
            string noExt = dat[..(dat.Length - 4)];
            if (!File.Exists($"{noExt}.dir"))
            {
                Console.WriteLine($"{dat}: Could not locate dir.");
                continue;
            }
            tasks.Add(ExtractArchive(noExt));
        }
        await Task.WhenAll(tasks);
    }

    static async Task ExtractArchive(string name)
    {
        Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
        dir_file dir = new();
        dat_file dat = new();
        try
        {
            string dir_status = await dir.Parse($"{name}.dir");
            if (dir_status != "reached eos")
            {
                Console.WriteLine($"{name}.dir: {dir_status}");
                return;
            }
            string dat_status = await dat.Parse($"{name}.dat");
            if (dat_status != "reached eos")
            {
                Console.WriteLine($"{name}.dat: {dat_status}");
                return;
            }
        }
        catch (Exception e)
        {
#if DEBUG
            Console.WriteLine(e.StackTrace);
#else
            Console.WriteLine($"unknown error reading {name}");
#endif

        }
        if (!Directory.Exists(name))
            Directory.CreateDirectory(name);

        var tasks = new List<Task>();
        foreach (var entry in dir.entries)
        {
            tasks.Add(CreateFile(entry));
        }
        await Task.WhenAll(tasks);

        async Task CreateFile(Entry entry)
        {
            string entry_name = entry.file_name;
            if (entry_name == "/_______.___") return;
            if (!dat.body.entries.TryGetValue(entry.dat_offset, out byte[] data))
            {
                Console.WriteLine($"{name}:\n   tried to access inexistent entry in dat.\n  name in dir:{entry_name}\noffset: 0X{entry.dat_offset.ToString("X")}");
                return;
            }
            if (data.Length != entry.length)
            {
                Console.WriteLine($"{name}:\n   <entry {entry_name}|offset 0X{entry.dat_offset.ToString("X")}> length mismatch.\n   dir entry compressed length: 0X{entry.length.ToString("X")}\n   dat entry compressed length: 0X{data.Length.ToString("X")} ");
            }
            var widen = new decompressor();
            byte[] decmpr;
            try
            {
                decmpr = await widen.decompress(data);
            }
            catch (Exception e)
            {
                Console.WriteLine($"{name}:\n   error decompressing entry:{entry_name} at offset 0X{entry.dat_offset.ToString("X")}, compressed length: 0X{data.Length.ToString("X")}");
                return;
            }
            await File.WriteAllBytesAsync(Path.Combine(name, entry_name), decmpr);
        }
    }
}
