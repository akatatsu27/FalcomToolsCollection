using Newtonsoft.Json;

namespace BTSET1_Converter
{
    internal class BTSET2Json
    {
        public Dictionary<string, Bonuses> BonusesTable = new();
        public Dictionary<string, ModelEntry> ModelTable = new();
        public Dictionary<string, Placement[]> PlacementTable = new();
        public Dictionary<string, BattleEntry> Battles = new();
        public Dictionary<string, AutoBattleEntry> AutoBattles = new();

        //For Serialization
        [JsonIgnore] private Dictionary<UInt16, string> OffsetToNameMap = new();
        //For Deserialization
        [JsonIgnore] private Dictionary<string, UInt16> NameToOffsetMap = new();

        [JsonIgnore] private UInt16 BattleEntryOffset_Offset; //The offset of the offset table
        [JsonIgnore] private UInt16 AutoBattleEntryOffset_Offset; //The offset of the offset table

        private BTSET2Json() { }
        public BTSET2Json(BTSET2 file)
        {
            string name;
            string str = "Bonuses";
            uint count = 0;
            foreach (var bonuses in file.BonusesTable.Values)
            {
                name = $"{str}{count++}";
                BonusesTable[name] = bonuses;
                OffsetToNameMap[bonuses.Offset] = name;
            }
            foreach (var model in file.ModelTable)
            {
                ModelTable[$"{model.Value.Filename}"] = model.Value;
            }
            str = "Placements";
            count = 0;
            foreach (var placements in file.PlacementTable.Values)
            {
                name = $"{str}{count++}";
                PlacementTable[name] = placements;
                OffsetToNameMap[placements[0].Offset] = name;
            }
            str = "Type1Battle";
            count = 0;
            foreach (var battle in file.Battles1.Values)
            {
                foreach (var variation in battle.Variations)
                {
                    if (variation == null) continue;
                    variation.BonusesName = OffsetToNameMap[(UInt16)variation.BonusesOffset];
                    variation.PlacementsNames = OffsetToNameMap[variation.PlacementTableOffset];
                    variation.SurprisePlacementsNames = OffsetToNameMap[variation.SurpriseTableOffset];
                }
                name = $"{str}{count++}";
                Battles[name] = battle;
            }
            str = "AutoBattle";
            count = 0;
            foreach (var battle in file.AutoBattles)
            {
                name = $"{str}{count++}";
                AutoBattles[name] = battle.Value;
            }
        }

        internal static async Task<BTSET2Json> Deserialize(string dirPath)
        {
            string bonuses = Program.Bonuses;
            string models = Program.Models;
            string placements = Program.Placements;
            string battles1 = Program.Battles1;
            string autobattles = Program.AutoBattles;

            BTSET2Json json = new();
            json.BonusesTable = JsonConvert.DeserializeObject<Dictionary<string, Bonuses>>(await File.ReadAllTextAsync(Path.Combine(dirPath, bonuses))) ?? throw new FileNotFoundException(bonuses);
            json.ModelTable = JsonConvert.DeserializeObject<Dictionary<string, ModelEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, models))) ?? throw new FileNotFoundException(models);
            json.PlacementTable = JsonConvert.DeserializeObject<Dictionary<string, Placement[]>>(await File.ReadAllTextAsync(Path.Combine(dirPath, placements))) ?? throw new FileNotFoundException(placements);
            json.Battles = JsonConvert.DeserializeObject<Dictionary<string, BattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, battles1))) ?? throw new FileNotFoundException(battles1);
            json.AutoBattles = JsonConvert.DeserializeObject<Dictionary<string, AutoBattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, autobattles))) ?? throw new FileNotFoundException(autobattles);

            foreach (var placementTable in json.PlacementTable)
            {
                if (placementTable.Value.Length != 8) throw new InvalidDataException($"Error in {placementTable.Key}: Each placement table must have exactly 8 entries!");
            }
            foreach (var model in json.ModelTable)
            {
                model.Value.Filename = model.Key;
            }
            foreach (var battle in json.Battles)
            {
                battle.Value.VariationWeights = battle.Value.JsonBytes.Select(w => (byte)w).ToArray();
                if (battle.Value.VariationWeights.Length != 4) throw new InvalidDataException($"Error in {battle.Key}: Each battle must have exactly 4 variation weights");
                if (battle.Value.Variations.Length > 4) throw new InvalidDataException($"Error in {battle.Key}: Each battle must have at most 4 variations");
                foreach (var variation in battle.Value.Variations)
                {
                    if (variation == null) continue;
                    if (variation.Enemies.Length != 8) throw new InvalidDataException($"Error in {battle.Key}: Each battle variation must have exactly 8 enemy entries, even if empty!");
                    foreach (EnemyEntry enemy in variation.Enemies)
                    {
                        enemy.DataTable = UInt16.Parse(enemy.DataTableName[6..], System.Globalization.NumberStyles.HexNumber);
                    }
                }
            }
            foreach (var battle in json.AutoBattles)
            {
                if (battle.Value.Side1.Length != 8 || battle.Value.Side2.Length != 8)
                    throw new InvalidDataException($"Error in {battle.Key}: Each autobattle side must have exactly 8 enemy entries, even if empty!");
                foreach (EnemyEntry enemy in battle.Value.Side1.Concat(battle.Value.Side2))
                {
                    enemy.DataTable = UInt16.Parse(enemy.DataTableName[6..], System.Globalization.NumberStyles.HexNumber);
                }
            }

            return json;
        }

        private void CalculateOffsets()
        {
            //Skipping first 2 UInt16 which are the BattleEntryTableOffset and the AutoBattleEntryTableOffset
            UInt16 curOffset = 4;
#if !DEBUG
            //Debug header (#bonuses + #models + #placements + #battles)
            curOffset += 2 + 2 + 2 + 2;
#endif
            foreach (var bonuses in BonusesTable)
            {
                string name = bonuses.Key;
                Bonuses entry = bonuses.Value;
                entry.Offset = curOffset;
                OffsetToNameMap[curOffset] = name;
                NameToOffsetMap[name] = curOffset;
                curOffset += 18; //sizeof(Bonuses)
            }
            foreach (var model in ModelTable)
            {
                string name = model.Key;
                ModelEntry entry = model.Value;
                entry.Offset = curOffset;
                entry.NameOffset = (UInt16)(curOffset + 4); // sizeof(unk00) + sizeof(NameOffset)
                OffsetToNameMap[curOffset] = name;
                NameToOffsetMap[name] = curOffset;
                curOffset += (UInt16)(4 + name.Length + 1); // sizeof(unk00) + sizeof(NameOffset) + name.Length + null byte
            }
            foreach (var placements in PlacementTable)
            {
                string name = placements.Key;
                Placement[] entry = placements.Value;
                entry[0].Offset = curOffset;
                OffsetToNameMap[curOffset] = name;
                NameToOffsetMap[name] = curOffset;
                curOffset += (1 + 1 + 2) * 8; // (sizeof(X) + sizeof(Y) + sizeof(Rot)) * 8 placements
            }

            BattleEntryOffset_Offset = curOffset;
            foreach (var battle in Battles)
            {
                string name = battle.Key;
                BattleEntry entry = battle.Value;
                entry.FieldOffset = NameToOffsetMap[entry.BattlefieldName];
                foreach (var variation in entry.Variations)
                {
                    if (variation == null) continue;
                    variation.BonusesOffset = NameToOffsetMap[variation.BonusesName];
                    variation.PlacementTableOffset = NameToOffsetMap[variation.PlacementsNames];
                    variation.SurpriseTableOffset = NameToOffsetMap[variation.SurprisePlacementsNames];
                }

                curOffset += 2;
            }
            AutoBattleEntryOffset_Offset = curOffset;
            foreach (var battle in AutoBattles)
            {
                string name = battle.Key;
                AutoBattleEntry entry = battle.Value;
                entry.FieldOffset = NameToOffsetMap[entry.BattlefieldName];

                curOffset += 2;
            }

            foreach (var battle in Battles)
            {
                string name = battle.Key;
                BattleEntry entry = battle.Value;
                entry.Offset = curOffset;
                curOffset += 16;
                foreach (var variation in entry.Variations)
                {
                    if (variation == null) continue;
                    curOffset += 48;
                }
            }
            foreach (var battle in AutoBattles)
            {
                string name = battle.Key;
                AutoBattleEntry entry = battle.Value;
                entry.Offset = curOffset;

                curOffset += 76; // sizeof(AutoBattleEntry)
            }

            if (curOffset > UInt16.MaxValue) throw new InvalidDataException("Compiled file too big!");
        }

        internal byte[] Compile()
        {
            CalculateOffsets();
            using var ms = new MemoryStream(0x32F0);
            using var bw = new BinaryWriter(ms);

            bw.Write(BattleEntryOffset_Offset);
            bw.Write(AutoBattleEntryOffset_Offset);
#if !DEBUG
            //Debug Header
            bw.Write((UInt16)BonusesTable.Count);
            bw.Write((UInt16)ModelTable.Count);
            bw.Write((UInt16)PlacementTable.Count);
            bw.Write((UInt16)(Battles.Count + AutoBattles.Count));
#endif
            foreach (var bonuses in BonusesTable.Values)
            {
                bw.Write(bonuses.ToByteArray());
            }
            foreach (var model in ModelTable.Values)
            {
                bw.Write(model.ToByteArray());
            }
            foreach (var placements in PlacementTable.Values)
            {
                foreach (Placement plac in placements)
                {
                    bw.Write(plac.ToByteArray());
                }
            }

            foreach (var battle in Battles.Values)
            {
                bw.Write(battle.Offset);
            }
            foreach (var battle in AutoBattles.Values)
            {
                bw.Write(battle.Offset);
            }

            foreach (var battle in Battles.Values)
            {
                bw.Write(battle.ToByteArray());
            }
            foreach (var battle in AutoBattles.Values)
            {
                bw.Write(battle.ToByteArray());
            }

            return ms.ToArray();
        }
    }
}
