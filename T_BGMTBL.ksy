meta:
  id: t_bgmtbl
  endian: le
seq:
  - id: bgm_list
    type: bgm
    repeat: eos
types:
  bgm:
    seq:
    - id: bgm_id
      type: u4
    - id: name
      type: strz
      encoding: SHIFT-JIS
    - id: padding
      contents: [0x00]
    - id: b_loop
      type: u4