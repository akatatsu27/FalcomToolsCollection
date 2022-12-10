meta:
  id: t_book
  endian: le
seq:
  - id: hex_14
    contents: [0x14]
  - id: unknown
    type: u2
  - id: rest_of_header
    type: u4
    repeat: expr
    repeat-expr: 4
  - id: books
    type: book
    repeat: eos
types:
  book:
    seq:
    - id: title
      type: strz
      encoding: SHIFT-JIS
    - id: pages # 10 * 0x8140 (_) is used as a page separator
      type: strz
      encoding: SHIFT-JIS

