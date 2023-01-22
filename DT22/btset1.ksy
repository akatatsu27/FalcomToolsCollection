meta:
  id: btset1
  endian: le
seq:
  - id: bt_offsets_array_offset
    type: u2
  - id: bonuses_table
    type: bonuses
    repeat: expr
    repeat-expr: 15
  - id: battlefield_models
    type: model_entry
    repeat: expr
    repeat-expr: 188
  - id: placements
    type: placement
    repeat: expr
    repeat-expr: 672
  - id: bt_offsets
    type: u2
    repeat: expr
    repeat-expr: 376
  - id: battles
    type: bt_entry
    repeat: until
    repeat-until: _.id == 0xFFFF
  - id: auto_battles
    type: auto_bt_entry
    repeat: until
    repeat-until: _.id == 0xFFFF
  - id: battles2
    type: bt_entry2
    repeat: expr
    repeat-expr: 363
types:
  bonuses:
    seq:
      - id: unk00
        type: u1
      - id: hp10
        type: u1
      - id: hp50
        type: u1
      - id: ep10
        type: u1
      - id: ep50
        type: u1
      - id: cp10
        type: u1
      - id: cp50
        type: u1
      - id: atk10
        type: u1
      - id: atk50
        type: u1
      - id: unk09
        type: u1
      - id: unk0a
        type: u1
      - id: sepith
        type: u1
      - id: crit
        type: u1
      - id: unk0d
        type: u1
      - id: unk0e
        type: u1
      - id: unk0f
        type: u1
      - id: unk10
        type: u1
      - id: unk11
        type: u1
  placement:
    seq:
      - id: x
        type: u1
      - id: y
        type: u1
      - id: rot
        type: u2
  model_entry:
    seq:
      - id: unk
        type: u2
      - id: offset
        type: u2
      - id: model
        type: strz
        encoding: SHIFT-JIS
  enemy:
    seq:
     - id: index
       type: u2
     - id: data_table
       type: u2
  variation:
    seq:
     - id: enemies
       type: enemy
       repeat: expr
       repeat-expr: 8
     - id: placement_table_offset
       type: u2
     - id: surprise_placement_table_offset
       type: u2
     - id: bgm1
       type: u4
     - id: bgm2
       type: u4
     - id: bonuses_offset
       type: u4
    instances:
      placements:
        pos: placement_table_offset
        type: placement
        repeat: expr
        repeat-expr: 8
      surprise_placements:
        pos: surprise_placement_table_offset
        type: placement
        repeat: expr
        repeat-expr: 8
      bonuses:
        pos: bonuses_offset
        type: bonuses
  bt_entry:
    seq:
     - id: id
       type: u2
     - id: flags
       type: u2
     - id: unk04
       type: u2
     - id: unk06
       type: u2
     - id: unk08
       type: u2
     - id: field_offset
       type: u2
     - id: variation_weights
       type: u1
       repeat: expr
       repeat-expr: 0x4
     - id: variation1
       type: variation
       if: variation_weights[0] != 0
     - id: variation2
       type: variation
       if: variation_weights[1] != 0
     - id: variation3
       type: variation
       if: variation_weights[2] != 0
     - id: variation4
       type: variation
       if: variation_weights[3] != 0
    instances:
      field:
        pos: field_offset
        type: model_entry
  auto_bt_entry:
    seq:
     - id: id
       type: u2
     - id: unk02
       type: u2       
     - id: field_offset
       type: u2
     - id: unk06
       type: u2
     - id: side1
       type: enemy
       repeat: expr
       repeat-expr: 8
     - id: side2
       type: enemy
       repeat: expr
       repeat-expr: 8
     - id: bgm
       type: u2
     - id: unk4a
       type: u2
    instances:
      field:
        pos: field_offset
        type: model_entry
  bt_entry2:
    seq:
     - id: id
       type: u2
     - id: flags
       type: u2
     - id: unk04
       type: u2
     - id: unk06
       type: u2
     - id: unk08
       type: u2
     - id: field_offset
       type: u2
     - id: variation_weights
       type: u1
       repeat: expr
       repeat-expr: 0x4
     - id: variation1
       type: variation
       #if: variation_weights[0] != 0
     - id: variation2
       type: variation
       if: variation_weights[1] != 0
     - id: variation3
       type: variation
       if: variation_weights[2] != 0
     - id: variation4
       type: variation
       if: variation_weights[3] != 0
    instances:
      field:
        pos: field_offset
        type: model_entry