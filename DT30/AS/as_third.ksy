meta:
  id: as_third
  endian: le
  imports:
   - as_instructions_third
seq:
  - id: craft_offset_table_offset
    type: u2
  - id: craft_offset_table_offset_end
    type: u2
  - id: bones_3d_offset
    type: u2
  - id: chip_entries # pairs of _ch and _cp, terminated by 0xFFFF
    type: chip_entry
    repeat: until
    repeat-until: _.ch.index == 0xFFFF
  - id: model_3d
    type: strz
    encoding: ascii
    repeat: until
    repeat-until: _.length == 0
  - id: bones_3d
    type: bones
    if: bones_3d_offset != 0
  - id: function_offset_table
    type: u2
    repeat: expr
    repeat-expr: (craft_offset_table_offset_end - craft_offset_table_offset)/2
  - id: unk_bytes # 0x80B0 * 8
    type: unk_bytes_entry
    repeat: expr
    repeat-expr: 8
  - id: instructions
    type: as_instructions_third::instruction
    repeat: eos
types:
  subroutine:
    seq:
      - id: start
        type: as_instructions_third::instruction
      - id: instructions
        type: as_instructions_third::instruction
        repeat: until
        repeat-until: _.opcode == 0 # as_end instruction  
  unk_bytes_entry:
    seq: 
      - id: unk00
        type: u1
      - id: unk01
        type: u1
  chip_entry:
    seq:
      - id: ch
        type: dt_entry
      - id: cp
        type: dt_entry
        if: ch.index != 0xFFFF
  dt_entry:
    seq:
     - id: index
       type: u2
     - id: dt_table
       type: u2
  bones:
    seq:
     - id: unk00
       type: u1
     - id: bones_3d
       type: strz
       encoding: ascii
       repeat: until
       repeat-until: _.length == 0 