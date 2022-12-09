meta:
  id: skill_data
  endian: le
seq:
  - id: data
    type: u1
    repeat: expr
    repeat-expr: 28
  - id: skill_name
    type: strz
    encoding: SHIFT-JIS
  - id: skill_description
    type: strz
    encoding: SHIFT-JIS