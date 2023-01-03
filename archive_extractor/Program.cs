using System.Reflection;
using System.Text;
using archive_extractor;
using Shared;
using static Shared.dir_file;

internal class Program
{
    static async Task Main(string[] args)
    {
#if false
        await ExtractArchive("ED6_DT33");
#else
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
            Console.WriteLine("    try to extract from file(s)");
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
                goto END;
            }

            await ExtractArchiveMany(filenames);
        }
        else // check for individual .dat in args
        {
            await ExtractArchiveMany(args);
        }
#endif
    END:
        Console.WriteLine("Press enter to close...");
        Console.ReadLine();
    }   

    static async Task ExtractArchiveMany(IEnumerable<string> filepaths)
    {
        IEnumerable<string> datPaths = filepaths.Where(w => w.Contains(".dat"));
        var tasks = new List<Task>();
        if(!datPaths.Any())
        {
            Console.WriteLine("no .dat files provided!");
            return;
        }
        foreach (string dat in datPaths)
        {
            string noExt = dat[..(dat.Length - 4)];
            if (!File.Exists($"{noExt}.dir"))
            {
                Console.WriteLine($"{dat}: Could not locate dir.");
                continue;
            }
            tasks.Add(ExtractArchive(noExt));
        }
        Console.WriteLine("Extracting archives...");
        await Task.WhenAll(tasks);
        Console.WriteLine("Finished extracting archives!");
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

        // Make tab separated file with all the dir entry info
        string tsv = "file_name\ttimestamp1\ttimestamp2\tdecompressed_size\toriginal_compressed_size\toriginal_dat_offset\n";
        
        for (int i = 0; i < dir.entries.Length; i++)
        {
            Entry entry = dir.entries[i];
            tsv += $"{entry.file_name}\t{entry.timestamp1}\t{entry.timestamp2}\t0X{entry.decompressed_size.ToString("X")}\t0X{entry.compressed_size.ToString("X")}\t0X{entry.dat_offset.ToString("X")}\n";
        }
        await File.WriteAllTextAsync($"{name}.tsv", tsv);

        // local function
        async Task CreateFile(Entry entry)
        {
            string entry_name = entry.file_name;
            if (entry_name == "/_______.___" || entry.compressed_size == 0) return;
            if (!dat.body.entries.TryGetValue(entry.dat_offset, out byte[] data))
            {
                Console.WriteLine($"{name}:\n   tried to access inexistent entry in dat.\n   name in dir: {entry_name}\n   offset: 0X{entry.dat_offset.ToString("X")}");
                return;
            }
            if (data.Length != entry.compressed_size)
            {
                Console.WriteLine($"{name}:\n   <entry {entry_name}|offset 0X{entry.dat_offset.ToString("X")}> length mismatch.\n   dir entry compressed length: 0X{entry.compressed_size.ToString("X")}\n   dat entry compressed length: 0X{data.Length.ToString("X")}");
                return;
            }
            var widen = new decompressor();
            byte[] decmpr;
            if (entry_name.Contains(".WAV") || entry_name.Contains("._DS"))
            {
                decmpr = data;
                goto NOT_COMPRESSED;
            }
            try
            {
                decmpr = await widen.decompress(data);
            }
            catch (Exception e)
            {
                Console.WriteLine($"{name}:\n   error decompressing entry: {entry_name} at offset 0X{entry.dat_offset.ToString("X")}\n   compressed length: 0X{data.Length.ToString("X")}");
#if DEBUG
                Console.WriteLine(e);
#endif
                return;
            }
            NOT_COMPRESSED:
            if(entry.timestamp1 != 0)
            {
                Console.WriteLine($"{name}:\n   entry: {entry_name} at offset 0X{entry.dat_offset.ToString("X")}\n   unk1 is 0X{entry.timestamp1.ToString("X")}! :DDDDD");
            }
            try
            {
                await File.WriteAllBytesAsync(Path.Combine(name, entry_name), decmpr);
            }
            catch(Exception e)
            {
                Console.WriteLine($"{name}:\n   error creating file from entry:{entry_name} at offset 0X{entry.dat_offset.ToString("X")}\n decompressed length: 0X{decmpr.Length.ToString("X")}");
#if DEBUG
                Console.WriteLine(e);
#endif
            }
        }
    }
}
