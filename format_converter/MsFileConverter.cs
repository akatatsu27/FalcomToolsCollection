using System.Diagnostics;
using System.Text;
using System.Text.RegularExpressions;
using Kaitai;
using Newtonsoft.Json;

namespace format_converter
{
    internal static class MsFileConverter
    {
        public static async Task ToJsonFromFolderAsync(string folderPath)
        {
            IEnumerable<string> files = Directory.EnumerateFiles(folderPath);
            files = files.Where(w => w.Split(Path.DirectorySeparatorChar).Last().Contains("MS"));
            foreach(string file in files)
            {
                await ToJsonFromFileAsync(file);
            }
        }

        public static async Task ToJsonFromFileAsync(string filePath)
        {
            string fileName = filePath.Split(Path.DirectorySeparatorChar).Last();
            try
            {
                JsonSerializerSettings settings = new JsonSerializerSettings();
                settings.ContractResolver = new CustomResolver();

                var ms = MsFile.FromFile(filePath);
                string newName = ms.Name + "-" + fileName;
                string json = JsonConvert.SerializeObject(ms, Formatting.Indented, settings);
                await File.WriteAllTextAsync($"./MSJson/{newName}.json", json);
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
            foreach(string file in files)
            {
                await ToDataTableFromFileAsync(file);
            }
        }

        public static async Task ToDataTableFromFileAsync(string filePath)
        {
#region serialization
            JsonSerializerSettings settings = new JsonSerializerSettings();
            settings.ContractResolver = new CustomResolver();
            var ms = JsonConvert.DeserializeObject<MsFile>(await File.ReadAllTextAsync(filePath), settings);
            var encoding = Encoding.GetEncoding("shift-jis");
            BinaryWriter bw = new(new MemoryStream(), Encoding.GetEncoding("shift-jis"));
            bw.Write(ms.AsFileAddress);
            bw.Write(ms.CharacterLevel);
            bw.Write(ms.CharacterMaxHp);
            bw.Write(ms.CharacterInitialHp);
            bw.Write(ms.EpMax);
            bw.Write(ms.EpInitial);
            bw.Write(ms.CpMax);
            bw.Write(ms.CpInitial);
            bw.Write(ms.Spd);
            bw.Write(ms.MoveSpd);
            bw.Write(ms.Mov);
            bw.Write(ms.Str);
            bw.Write(ms.Def);
            bw.Write(ms.Ats);
            bw.Write(ms.Adf);
            bw.Write(ms.Dex);
            bw.Write(ms.Agl);
            bw.Write(ms.Rng);
            bw.Write(ms.Unknown0);
            bw.Write(ms.Exp);
            bw.Write(ms.Unknown);
            bw.Write((ushort)ms.AiType.Value);
            bw.Write(ms.Unknown2.ToArray());
            bw.Write(ms.Flags);
            bw.Write(ms.DeathFlags);
            bw.Write(ms.UnderAttackFlags);
            bw.Write(ms.Unknown3.ToArray());
            bw.Write((byte)ms.Sex.Value);
            bw.Write(ms.Unknown4.ToArray());
            bw.Write(ms.CharSize);
            bw.Write(ms.Unknown5.ToArray());
            bw.Write(ms.SymbolTextureFileIndex);
            bw.Write(ms.Resistance);
            bw.Write(ms.Unknown6.ToArray());
            for (int i = 0; i < 7; i++)
            {
                bw.Write(ms.ConditionResistance[i]);
            }
            for (int i = 0; i < 7; i++)
            {
                bw.Write(ms.Sepith[i]);
            }
            bw.Write(ms.Unknown7.ToArray());
            for (int i = 0; i < 5; i++)
            {
                bw.Write(ms.Equip[i]);
            }
            for (int i = 0; i < 4; i++)
            {
                bw.Write(ms.Orb[i]);
            }
            bw.Write(ms.NormalAttack.ToByteArray());
            bw.Write(ms.ShouldBeZero9.ToArray());
            bw.Write(ms.MagicCount);
            for (int i = 0; i < ms.MagicCount; i++)
            {
                bw.Write(ms.Magic[i].ToByteArray());
            }
            bw.Write(ms.CraftCount);
            for (int i = 0; i < ms.CraftCount; i++)
            {
                bw.Write(ms.Craft[i].ToByteArray());
            }
            bw.Write(ms.ScraftCount);
            for (int i = 0; i < ms.ScraftCount; i++)
            {
                bw.Write(ms.Scraft[i].ToByteArray());
            }
            bw.Write(ms.SkillTableCount);
            for (int i = 0; i < ms.SkillTableCount; i++)
            {
                bw.Write(ms.SkillTable[i].ToByteArray());
            }
            bw.Write(ms.ShouldBeZero2);
            bw.Write(encoding.GetBytes(ms.Name));
            bw.Write((byte)0x00);
            bw.Write(encoding.GetBytes(ms.Description));
            bw.Write((byte)0x00);
#endregion
            byte[] asBytes = (bw.BaseStream as MemoryStream).ToArray();
            bw.Close();
            string fileName = filePath.Split(Path.DirectorySeparatorChar).Last();
            fileName = Regex.Replace(fileName, ".json", "");
            fileName = fileName.Split("-").Last();
            await File.WriteAllBytesAsync($"./MSDT/{fileName}", asBytes);
            Console.WriteLine($"successfully converted to DataTable {fileName}");
        }
    }
}

namespace Kaitai
{
    public partial class Ed6CraftAiInfo
    {
        [JsonConstructor]
        public Ed6CraftAiInfo(KaitaiStruct p__parent = null, Ed6CraftAiInfo p__root = null) : base(new(new byte[1]))
        {
            _unknown = new();
            m_parent = p__parent;
            m_root = p__root ?? this;
        }
        public byte[] ToByteArray()
        {
            BinaryWriter bw = new(new MemoryStream(), Encoding.GetEncoding("shift-jis"));
            bw.Write(Condition);
            bw.Write(Probability);
            bw.Write(Target);
            bw.Write(TargetCondition);
            bw.Write(MagicChantAsEffectIndex);
            bw.Write(AsEffectIndex);
            bw.Write(SkillInfoIndex);
            bw.Write(Unknown.ToArray());
            byte[] asBytes = (bw.BaseStream as MemoryStream).ToArray();
            bw.Close();
            return asBytes;
        }
    }

    public partial class SkillData
    {
        [JsonConstructor]
        public SkillData(KaitaiStruct p__parent = null, SkillData p__root = null) : base(new(new byte[1]))
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
        }
        public byte[] ToByteArray()
        {
            var encoding = Encoding.GetEncoding("shift-jis");
            BinaryWriter bw = new(new MemoryStream(), encoding);
            bw.Write(Data.ToArray());
            bw.Write(encoding.GetBytes(SkillName));
            bw.Write((byte)0);
            bw.Write(encoding.GetBytes(SkillDescription));
            bw.Write((byte)0);
            byte[] asBytes = (bw.BaseStream as MemoryStream).ToArray();
            bw.Close();
            return asBytes;
        }
    }
    public partial class AiTypeEnum
    {
        [JsonConstructor]
        public AiTypeEnum(KaitaiStruct p__parent = null, AiTypeEnum p__root = null) : base(new(new byte[1]))
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
        }
    }
    public partial class GenderEnum
    {
        [JsonConstructor]
        public GenderEnum(KaitaiStruct p__parent = null, GenderEnum p__root = null) : base(new(new byte[1]))
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
        }
    }
    public partial class MsFile
    {
        [JsonConstructor]
        public MsFile(KaitaiStruct p__parent = null, MsFile p__root = null) : base(new(new byte[1]))
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
        }
    }
}
