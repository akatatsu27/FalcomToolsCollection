meta:
  id: t_cook2
  endian: le
seq:
  - id: offset_of_end_of_offset_array
    type: u2
  - id: offsets_of_entries
    type: u2
    repeat: expr
    repeat-expr: (offset_of_end_of_offset_array - 1)/2
  - id: recipes
    type: recipe
    repeat: eos
types:
  recipe:
    seq:
    - id: recipe_id
      type: u2
    - id: ingredients
      type: ingredient
      repeat: expr
      repeat-expr: 8 # hardcoded MAX 8 ingredients. if less, the rest of the entries are null
    - id: flags
      type: u2
    - id: effect_id # stored in another file?
      type: u2
    - id: padding
      type: u2
    - id: heal
      type: u2
    - id: offset_of_name
      type: u2
    - id: offset_of_description
      type: u2
    - id: name
      type: strz
      encoding: SHIFT-JIS
    - id: description
      type: strz
      encoding: SHIFT-JIS
  ingredient:
    seq:
    - id: item_id
      type: u2
    - id: num
      type: u2
enums:
  recipe_flag:
    1: to_go
    2: revive
    4: doom
    8: cp
    80: dummy
