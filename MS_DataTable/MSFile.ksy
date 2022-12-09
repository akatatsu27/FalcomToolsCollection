meta:
  id: ms_file
  endian: le
  license: GPLv2
  imports:
    - ED6_CRAFT_AI_INFO
    - AIType
    - Gender
    - skill_data
seq:
  - id: as_file_address
    type: u4
  - id: character_level
    type: u2
  - id: character_max_hp
    type: u4
  - id: character_initial_hp
    type: u4
  - id: ep_max
    type: u2
  - id: ep_initial # 0x10
    type: u2
  - id: cp_max
    type: u2
  - id: cp_initial 
    type: u2
  - id: spd
    type: u2
  - id: move_spd
    type: u2
  - id: mov
    type: u2
  - id: str
    type: u2
  - id: def 
    type: u2
  - id: ats # 0x20
    type: u2
  - id: adf
    type: u2
  - id: dex
    type: u2
  - id: agl
    type: u2
  - id: rng # range
    type: u2
  - id: unknown0
    type: u2
  - id: exp
    type: u2
  - id: unknown
    type: u2
  - id: ai_type # 0x30
    type: ai_type_enum 
  - id: unknown2
    type: u1
    repeat: expr
    repeat-expr: 6
  - id: flags
    type: u1
  - id: death_flags
    type: u1
  - id: under_attack_flags
    type: u1
  - id: unknown3
    type: u1
    repeat: expr
    repeat-expr: 5
  - id: sex # 0x40
    type: gender_enum
  - id: unknown4
    type: u1
    repeat: expr
    repeat-expr: 9
  - id: char_size
    type: u2
  - id: unknown5
    type: u1
    repeat: expr
    repeat-expr: 0xA
  - id: symbol_texture_file_index
    type: u4
  - id: resistance
    type: u4
  - id: unknown6
    type: u1
    repeat: expr
    repeat-expr: 0xB
  - id: condition_resistance
    type: u2
    repeat: expr
    repeat-expr: 7
  - id: sepith
    type: u1
    repeat: expr
    repeat-expr: 7
  - id: unknown7
    type: u1
    repeat: expr
    repeat-expr: 6
  - id: equip
    type: u2
    repeat: expr
    repeat-expr: 5
  - id: orb
    type: u2
    repeat: expr
    repeat-expr: 4
  - id: normal_attack
    type: ed6_craft_ai_info
  - id: should_be_zero_9
    type: u1
    repeat: expr
    repeat-expr: 8
  - id: magic_count
    type: u1
  - id: magic
    type: ed6_craft_ai_info
    repeat: expr
    repeat-expr: magic_count
  - id: craft_count
    type: u1
  - id: craft
    type: ed6_craft_ai_info
    repeat: expr
    repeat-expr: craft_count
  - id: scraft_count
    type: u1
  - id: scraft
    type: ed6_craft_ai_info
    repeat: expr
    repeat-expr: scraft_count
  - id: skill_table_count
    type: u1
  - id: skill_table
    type: skill_data
    repeat: expr
    repeat-expr: skill_table_count
  - id: should_be_zero2
    contents: [0x00, 0x00, 0x00, 0x00]
  - id: name
    type: strz
    encoding: SHIFT-JIS
  - id: description
    type: strz
    encoding: SHIFT-JIS