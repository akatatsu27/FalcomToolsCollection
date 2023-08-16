using System.Text;
using System.Text.RegularExpressions;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Shared;

namespace MS_Converter;

internal static partial class MsFileConverter
{
	private static readonly JsonSerializerSettings JsonSettings = new()
	{
		ContractResolver = new KaitaiResolver(),
		Converters = new List<JsonConverter>() { new ByteArrayConverter(), new StringEnumConverter() }
	};

	public static async Task ToJsonFromFolderAsync(string folderPath)
	{
		IEnumerable<string> files = Directory.EnumerateFiles(folderPath, "MS*._DT");
		foreach (string file in files)
		{
			await ToJsonFromFileAsync(file);
		}
	}

	public static async Task ToJsonFromFileAsync(string filePath)
	{
		string fileName = filePath.Split(Path.DirectorySeparatorChar).Last();
		try
		{
			MS_File ms = await MS_File.FromFile(filePath);
			string json = JsonConvert.SerializeObject(ms, Formatting.Indented, JsonSettings);
			await File.WriteAllTextAsync($"./MSJson/{fileName}.json", json);
			Console.WriteLine($"successfully converted to json {fileName}");
		}
		catch (Exception e)
		{
#if DEBUG
			Console.WriteLine(e.StackTrace);
#endif
		}
	}
	public static async Task ToDataTableFromFolderAsync(string folderPath)
	{
		IEnumerable<string> files = Directory.EnumerateFiles(folderPath);
		files = files.Where(w => w.Split(Path.DirectorySeparatorChar).Last().Contains("json"));
		foreach (string file in files)
		{
			await ToDataTableFromFileAsync(file);
		}
	}

	public static async Task ToDataTableFromFileAsync(string filePath)
	{
		#region serialization
		MS_File ms = JsonConvert.DeserializeObject<MS_File>(await File.ReadAllTextAsync(filePath), JsonSettings);
		Encoding encoding = Encoding.GetEncoding("shift-jis");
		BinaryWriter bw = new(new MemoryStream(), Encoding.GetEncoding("shift-jis"));
		bw.Write(ms.AS_Index);
		bw.Write(ms.AS_dir);
		bw.Write(ms.Level);
		bw.Write(ms.HPMax);
		bw.Write(ms.HPInitial);
		bw.Write(ms.EPMax);
		bw.Write(ms.EPInitial);
		bw.Write(ms.CPMax);
		bw.Write(ms.CPInitial);
		bw.Write(ms.SPD);
		bw.Write(ms.MoveSpd);
		bw.Write(ms.MOV);
		bw.Write(ms.STR);
		bw.Write(ms.DEF);
		bw.Write(ms.ATS);
		bw.Write(ms.ADF);
		bw.Write(ms.DEX);
		bw.Write(ms.AGL);
		bw.Write(ms.RNG);
		bw.Write(ms.Unk_2A);
		bw.Write(ms.EXP);
		bw.Write(ms.Unk_2E);
		bw.Write((ushort)ms.AI_Type);
		bw.Write(ms.Unk_32);
		bw.Write(ms.Flags);
		bw.Write(ms.DeathFlags);
		bw.Write(ms.UnderAttackFlags);
		bw.Write(ms.Unk_3B);
		bw.Write((byte)ms.Gender);
		bw.Write(ms.Unk_41);
		bw.Write(ms.ActorSize);
		bw.Write(ms.Unk_4C);
		bw.Write(ms.SymbolTextureFileIndex);
		bw.Write(ms.SymbolTextureFileDir);
		bw.Write((uint)ms.StatusResistance);
		bw.Write(ms.Unk_5E);
		
		bw.Write(ms.EarthElementalEfficacy);
		bw.Write(ms.WaterElementalEfficacy);
		bw.Write(ms.FireElementalEfficacy);
		bw.Write(ms.WindElementalEfficacy);
		bw.Write(ms.TimeElementalEfficacy);
		bw.Write(ms.SpaceElementalEfficacy);
		bw.Write(ms.MirageElementalEfficacy);

		bw.Write(ms.EarthSepith);
		bw.Write(ms.WaterSepith);
		bw.Write(ms.FireSepith);
		bw.Write(ms.WindSepith);
		bw.Write(ms.TimeSepith);
		bw.Write(ms.SpaceSepith);
		bw.Write(ms.MirageSepith);

		bw.Write(ms.Unk_7E);
		for (int i = 0; i < 5; i++)
		{
			bw.Write(ms.Equip[i]);
		}
		for (int i = 0; i < 4; i++)
		{
			bw.Write(ms.Orb[i]);
		}
		bw.Write(ms.NormalAttack.ToByteArray());
		bw.Write(ms.ZeroPadding);
		bw.Write(ms.ArtsCount);
		for (int i = 0; i < ms.ArtsCount; i++)
		{
			bw.Write(ms.Arts[i].ToByteArray());
		}
		bw.Write(ms.CraftsCount);
		for (int i = 0; i < ms.CraftsCount; i++)
		{
			bw.Write(ms.Crafts[i].ToByteArray());
		}
		bw.Write(ms.SCraftsCount);
		for (int i = 0; i < ms.SCraftsCount; i++)
		{
			bw.Write(ms.SCrafts[i].ToByteArray());
		}
		bw.Write(ms.AbilityDataCount);
		for (int i = 0; i < ms.AbilityDataCount; i++)
		{
			bw.Write(ms.AbilityData[i].ToByteArray());
		}
		bw.Write(ms.UnkBeforeName);
		bw.Write(encoding.GetBytes(ms.Name));
		bw.Write((byte)0x00);
		bw.Write(encoding.GetBytes(ms.Description));
		bw.Write((byte)0x00);
		#endregion
		byte[] asBytes = (bw.BaseStream as MemoryStream).ToArray();
		bw.Close();
		string fileName = filePath.Split(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar).Last();
		fileName = MatchJson().Replace(fileName, "");
		await File.WriteAllBytesAsync($"./MSDT/{fileName}", asBytes);
		Console.WriteLine($"successfully converted to DataTable {fileName}");
	}

	[GeneratedRegex(".json")]
	private static partial Regex MatchJson();
}