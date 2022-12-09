meta:
  id: ed6_craft_ai_info
  endian: le
  license: GPLv2
seq:
  - id: condition
    type: u1
  - id: probability
    type: u1
  - id: target
    type: u1
  - id: target_condition
    type: u1
  - id: magic_chant_as_effect_index
    type: u1
  - id: as_effect_index
    type: u1
  - id: skill_info_index
    type: u2
  - id: should_be_zero
    contents: [ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,]