meta:
  id: t_world
  endian: le
seq:
  - id: scenas
    type: scena
    repeat: eos
types:
  scena:
    seq:
    - id: scena_id
      type: u4
    - id: x
      type: u4
    - id: y
      type: u4