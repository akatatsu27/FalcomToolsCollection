using Newtonsoft.Json;

namespace BTSET1_Converter
{
    internal class BTSET1Json
    {
        public Dictionary<string, Bonuses> BonusesTable = new();
        public Dictionary<string, ModelEntry> ModelTable = new();
        public Dictionary<string, Placement[]> PlacementTable = new();
        public Dictionary<string, BattleEntry> Battles1 = new();
        public Dictionary<string, AutoBattleEntry> AutoBattles = new();
        public Dictionary<string, BattleEntry> Battles2 = new();

        //For Serialization
        [JsonIgnore] private Dictionary<ushort, string> OffsetToNameMap = new();
        //For Deserialization
        [JsonIgnore] private Dictionary<string, ushort> NameToOffsetMap = new();

        [JsonIgnore] private ushort BattleEntryOffset_Offset; //The offset of the offset table

        private BTSET1Json() { }
        public BTSET1Json(BTSET1 file)
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
                    variation.BonusesName = OffsetToNameMap[(ushort)variation.BonusesOffset];
                    variation.PlacementsNames = OffsetToNameMap[variation.PlacementTableOffset];
                    variation.SurprisePlacementsNames = OffsetToNameMap[variation.SurpriseTableOffset];
                }
                name = $"{str}{count++}";
                Battles1[name] = battle;
            }
            str = "AutoBattle";
            count = 0;
            foreach (var battle in file.AutoBattles)
            {
                name = $"{str}{count++}";
                AutoBattles[name] = battle.Value;
            }
            str = "Type2Battle";
            count = 0;
            foreach (var battle in file.Battles2.Values)
            {
                foreach (var variation in battle.Variations)
                {
                    if (variation == null) continue;
                    variation.BonusesName = OffsetToNameMap[(ushort)variation.BonusesOffset];
                    variation.PlacementsNames = OffsetToNameMap[variation.PlacementTableOffset];
                    variation.SurprisePlacementsNames = OffsetToNameMap[variation.SurpriseTableOffset];
                }
                name = $"{str}{count++}";
                Battles2[name] = battle;
            }
        }

        internal static async Task<BTSET1Json> Deserialize(string dirPath)
        {
            string bonuses = Program.Bonuses;
            string models = Program.Models;
            string placements = Program.Placements;
            string battles1 = Program.Battles1;
            string autobattles = Program.AutoBattles;
            string battles2 = Program.Battles2;

            BTSET1Json json = new();
            json.BonusesTable = JsonConvert.DeserializeObject<Dictionary<string, Bonuses>>(await File.ReadAllTextAsync(Path.Combine(dirPath, bonuses))) ?? throw new FileNotFoundException(bonuses);
            json.ModelTable = JsonConvert.DeserializeObject<Dictionary<string, ModelEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, models))) ?? throw new FileNotFoundException(models);
            json.PlacementTable = JsonConvert.DeserializeObject<Dictionary<string, Placement[]>>(await File.ReadAllTextAsync(Path.Combine(dirPath, placements))) ?? throw new FileNotFoundException(placements);
            json.Battles1 = JsonConvert.DeserializeObject<Dictionary<string, BattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, battles1))) ?? throw new FileNotFoundException(battles1);
            json.AutoBattles = JsonConvert.DeserializeObject<Dictionary<string, AutoBattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, autobattles))) ?? throw new FileNotFoundException(autobattles);
            json.Battles2 = JsonConvert.DeserializeObject<Dictionary<string, BattleEntry>>(await File.ReadAllTextAsync(Path.Combine(dirPath, battles2))) ?? throw new FileNotFoundException(battles2);

            foreach (var placementTable in json.PlacementTable)
            {
                if (placementTable.Value.Length != 8) throw new InvalidDataException($"Error in {placementTable.Key}: Each placement table must have exactly 8 entries!");
            }
            foreach (var model in json.ModelTable)
            {
                model.Value.Filename = model.Key;
            }
            foreach (var battle in json.Battles1.Concat(json.Battles2))
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
                        enemy.DataTable = ushort.Parse(enemy.DataTableName[6..], System.Globalization.NumberStyles.HexNumber);
                    }
                }
            }
            foreach (var battle in json.AutoBattles)
            {
                if (battle.Value.Side1.Length != 8 || battle.Value.Side2.Length != 8)
                    throw new InvalidDataException($"Error in {battle.Key}: Each autobattle side must have exactly 8 enemy entries, even if empty!");
                foreach (EnemyEntry enemy in battle.Value.Side1.Concat(battle.Value.Side2))
                {
                    enemy.DataTable = ushort.Parse(enemy.DataTableName[6..], System.Globalization.NumberStyles.HexNumber);
                }
            }

            return json;
        }

        private void CalculateOffsets()
        {
            //Skipping first UInt16 which is the BattleEntryTableOffset
            ushort curOffset = 2;
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
                entry.NameOffset = (ushort)(curOffset + 4); // sizeof(unk00) + sizeof(NameOffset)
                OffsetToNameMap[curOffset] = name;
                NameToOffsetMap[name] = curOffset;
                curOffset += (ushort)(4 + name.Length + 1); // sizeof(unk00) + sizeof(NameOffset) + name.Length + null byte
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

            //NOTE: in offset table Battles2 -> Battles1 -> AutoBattles
            //BUT in order of appearance in the file Battles1 -> AutoBattles -> Battles2
            foreach (var battle in Battles2)
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
#if DEBUG
                //For some reason these battles are not present in the offset table
                if (battle.Value.Id == 3623 || battle.Value.Id == 529 || battle.Value.Id == 530) continue;
#endif
                curOffset += 2;
            }
            foreach (var battle in Battles1)
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
#if DEBUG
                //For some reason these battles are not present in the offset table
                if (battle.Value.Id == 3623 || battle.Value.Id == 529 || battle.Value.Id == 530) continue;
#endif
                curOffset += 2;
            }
            foreach (var battle in AutoBattles)
            {
                string name = battle.Key;
                AutoBattleEntry entry = battle.Value;
                entry.FieldOffset = NameToOffsetMap[entry.BattlefieldName];
#if DEBUG
                //For some reason these battles are not present in the offset table
                if (battle.Value.Id == 3623 || battle.Value.Id == 529 || battle.Value.Id == 530) continue;
#endif
                curOffset += 2;
            }
            //NOTE: in offset table Battles2 -> Battles1 -> AutoBattles
            //BUT in order of appearance in the file Battles1 -> AutoBattles -> Battles2
            foreach (var battle in Battles1)
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
            foreach (var battle in Battles2)
            {
                string name = battle.Key;
                BattleEntry entry = battle.Value;
                entry.Offset = curOffset;
                curOffset += 16;
                for (byte i = 0; i < 4; i++)
                {
                    var variation = entry.Variations[i];
                    if (variation == null && i > 0) continue;
                    curOffset += 48;
                }
            }

            if (curOffset > ushort.MaxValue) throw new InvalidDataException("Compiled file too big!");
        }

        internal byte[] Compile()
        {
            CalculateOffsets();
            using var ms = new MemoryStream(0xB4F2);
            using var bw = new BinaryWriter(ms);

            bw.Write(BattleEntryOffset_Offset);
#if !DEBUG
            //Debug Header
            bw.Write((ushort)BonusesTable.Count);
            bw.Write((ushort)ModelTable.Count);
            bw.Write((ushort)PlacementTable.Count);
            bw.Write((ushort)(Battles1.Count + AutoBattles.Count + Battles2.Count));
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

            //NOTE: in offset table Battles2 -> Battles1 -> AutoBattles
            //BUT in order of appearance in the file Battles1 -> AutoBattles -> Battles2
            foreach (var battle in Battles2.Values)
            {
#if DEBUG
                //For some reason these battles are not present in the offset table
                if (battle.Id == 3623 || battle.Id == 529 || battle.Id == 530) continue;
#endif
                bw.Write(battle.Offset);
            }
            foreach (var battle in Battles1.Values)
            {
#if DEBUG
                //For some reason these battles are not present in the offset table
                if (battle.Id == 3623 || battle.Id == 529 || battle.Id == 530) continue;
#endif
                bw.Write(battle.Offset);
            }
            foreach (var battle in AutoBattles.Values)
            {
#if DEBUG
                //For some reason these battles are not present in the offset table
                if (battle.Id == 3623 || battle.Id == 529 || battle.Id == 530) continue;
#endif
                bw.Write(battle.Offset);
            }

            //NOTE: in offset table Battles2 -> Battles1 -> AutoBattles
            //BUT in order of appearance in the file Battles1 -> AutoBattles -> Battles2
            foreach (var battle in Battles1.Values)
            {
                bw.Write(battle.ToByteArray());
            }
            foreach (var battle in AutoBattles.Values)
            {
                bw.Write(battle.ToByteArray());
            }
            foreach (var battle in Battles2.Values)
            {
                bw.Write(battle.ToByteArray());
            }

            return ms.ToArray();
        }
    }
}
