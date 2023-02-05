meta:
  id: as_instructions_third
  endian: le
seq: []
types:
  as_00_end: # no operands
    seq: []
  as_01_goto:
    seq:
      - id: op1
        type: u2
  as_02_select_sub_chip:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_03_deg:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
  as_04:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
  as_05:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
  as_06_sleep:
    seq:
      - id: op1
        type: u4
  as_07_clear_effect: # no operands
    seq: []
  as_08_teleport:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
  as_09:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
  as_0a:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u1
      - id: op4
        type: u4
  as_0b_turn:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
  as_0c:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
      - id: op4
        type: u2
      - id: op5
        type: u1
  as_0d_jump:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
      - id: op6
        type: u2
      - id: op7
        type: u2
  as_0e_drop_down:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u2
      - id: op6
        type: u2
  as_0f_jump_to_target:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u2
  as_10_jump_back:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u2
  as_11_move:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
      - id: op6
        type: u4
      - id: op7
        type: u1
  as_12_add_effect:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: strz
        encoding: shift-jis
  as_13_release_effect:
    seq:
      - id: op1
        type: u2
  as_14:
    seq:
      - id: op1
        type: u2
  as_15_wait_effect:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_16_finish_effect:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_17_cancel_effect:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_18_show_effect:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u1
      - id: op4
        type: u2
      - id: op5
        type: u4
      - id: op6
        type: u4
      - id: op7
        type: u4
      - id: op8
        type: u2
      - id: op9
        type: u2
      - id: op10
        type: u2
      - id: op11
        type: u2
      - id: op12
        type: u2 
      - id: op13
        type: u2
      - id: op14
        type: u1   
  as_19_show_3d_effect:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: strz
        encoding: shift-jis
      - id: op4
        type: u2
      - id: op5
        type: u4
      - id: op6
        type: u4
      - id: op7
        type: u4
      - id: op8
        type: u2
      - id: op9
        type: u2
      - id: op10
        type: u2
      - id: op11
        type: u2
      - id: op12
        type: u2
      - id: op13
        type: u2
      - id: op14
        type: u1
  as_1a:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
  as_1b_select_chip:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_1c_damage:
    seq:
      - id: op1
        type: u1
  as_1d_damage_anime:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
  as_1e:
    seq:
      - id: op1
        type: u4
  as_1f:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u2
      - id: op3
        type: u1
  as_20:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u1
      - id: op4
        type: u1
      - id: op5
        type: u4
      - id: op6
        type: u4
  as_21:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
  as_22_begin_thread:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
      - id: op4
        type: u1
  as_23_wait_thread:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_24_set_chip_mode_flag:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u1
  as_25_clear_chip_mode_flag:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
  as_26:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
  as_27:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
  as_28_char_say:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: strz
        encoding: shift-jis
      - id: op3
        type: u2
  as_29:
    seq:
      - id: op1
        type: u1
  as_2a_tip_text:
    seq:
      - id: op1
        type: strz
        encoding: shift-jis
      - id: op2
        type: u4
  as_2b: # no operands
    seq: []
  as_2c_shadow_begin:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
      - id: op3
        type: u2
  as_2d_shadow_end:
    seq:
      - id: op1
        type: u1
  as_2e_shake_char:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
  as_2f_suspend_thread:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_30: # NOT IMPLEMENTED
    seq: []
  as_31:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_32:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_33:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_34: # no operands
    seq: []
  as_35_keep_angle:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
  as_36:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
  as_37_rotation_angle:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
  as_38_rotation_angle_v:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
  as_39_set_angle:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
  as_3a_tilt_angle:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
  as_3b_rotation_angle_h:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
  as_3c:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u4
  as_3d_shake_screen:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
  as_3e:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u4
  as_3f:
    seq:
      - id: op1
        type: u1
  as_40:
    seq:
      - id: op1
        type: u1
  as_41_lock_angle:
    seq:
      - id: op1
        type: u1
  as_42:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u1
  as_43_set_bk_color:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
  as_44_zoom_angle:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
  as_45:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_46:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
  as_47:
    seq:
      - id: op1
        type: u1
  as_48:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_49_set_control:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
  as_4a:
    seq:
      - id: op1
        type: u1
  as_4b_random:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u2
  as_4c_loop_target_beg:
    seq:
      - id: op1
        type: u2
  as_4d_reset_loop_target: # no operands
    seq: []
  as_4e_loop_target_end: # no operands
    seq: []
  as_4f:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_50_call:
    seq:
      - id: op1
        type: u2
  as_51_ret: # no operands
    seq: []
  as_52:
    seq:
      - id: op1
        type: u1
  as_53:
    seq:
      - id: op1
        type: u1
  as_54:
    seq:
      - id: op1
        type: u1
  as_55_magic_cast_begin:
    seq:
      - id: op1
        type: u2
  as_56_magic_cast_end: # no operands
    seq: []
  as_57:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_58_beat_back:
    seq:
      - id: op1
        type: u1
  as_59:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
  as_5a:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
  as_5b:
    seq:
      - id: op1
        type: u4
  as_5c_show:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_5d_hide:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_5e:
    seq:
      - id: op1
        type: u1
  as_5f:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_60:
    seq:
      - id: op1
        type: u1
  as_61_set_battle_speed:
    seq:
      - id: op1
        type: u4
  as_62:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u1
      - id: op4
        type: u1
      - id: op5
        type: u2
  as_63:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_64_sound_effect:
    seq:
      - id: op1
        type: u2
  as_65_sound_effect_ex:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u1
  as_66:
    seq:
      - id: op1
        type: u2
  as_67_scraft_cut_in:
    seq:
      - id: op1
        type: strz
        encoding: shift-jis
  as_68: # no operands
    seq: []
  as_69_release_texture: # no operands
    seq: []
  as_6a_load_schip:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
  as_6b_reset_scraft_chip: # no operands
    seq: []
  as_6c_die: # no operands
    seq: []
  as_6d:
    seq:
      - id: op1
        type: u4
  as_6e:
    seq:
      - id: op1
        type: u4
  as_6f:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_70:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
      - id: op4
        type: u2
  as_71:
    seq:
      - id: op1
        type: u1
  as_72:
    seq:
      - id: op1
        type: u1
  as_73:
    seq:
      - id: op1
        type: u1
  as_74:
    seq:
      - id: op1
        type: u1
  as_75:
    seq:
      - id: op1
        type: u2
  as_76:
    seq:
      - id: op1
        type: u1
  as_77:
    seq:
      - id: op1
        type: u1
  as_78_set_eff_state:
    seq:
      - id: op1
        type: u1
  as_79:
    seq:
      - id: op1
        type: u1
  as_7a_craft_end:
    seq:
      - id: op1
        type: u1
  as_7b_set_craft_end_flag:
    seq:
      - id: op1
        type: u2
  as_7c:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_7d:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_7e:
    seq:
      - id: op1
        type: u4
  as_7f_blur_screen:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u1
      - id: op5
        type: u4
  as_80:
    seq:
      - id: op1
        type: u4
  as_81:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u2
  as_82: # no operands
    seq: []
  as_83_sort_target:
    seq:
      - id: op1
        type: u1
  as_84_rotate_char:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
      - id: op3
        type: u2
      - id: op4
        type: u2
      - id: op5
        type: u4
      - id: op6
        type: u1
  as_85:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
  as_86:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u2
      - id: op3
        type: u2
      - id: op4
        type: u1
      - id: op5
        type: u4
  as_87:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u1
  as_88_voice:
    seq:
      - id: op1
        type: u2
  as_89_save_cur_pos:
    seq:
      - id: op1
        type: u1
  as_8a_clone:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_8b_use_item_begin: # no operands
    seq: []
  as_8c_use_item_end: # no operands
    seq: []
  as_8d_zoom:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
  as_8e_load_x_file: #TODO
    seq: []
  as_8f:
    seq:
      - id: op1
        type: u1
  as_90:
    seq:
      - id: op1
        type: u1
  as_91:
    seq:
      - id: op1
        type: u1
  as_92:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
      - id: op6
        type: u2
      - id: op7
        type: u4
  as_93:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: strz
        encoding: shift-jis
  as_94:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: strz
        encoding: shift-jis
      - id: op3
        type: u4
  as_95: # no operands
    seq: []
  as_96_set_angle_target:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: strz
        encoding: shift-jis
      - id: op3
        type: u2
  as_97_move_angle:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
      - id: op3
        type: u2
  as_98:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
  as_99:
    seq:
      - id: op1
        type: u1
  as_9a:
    seq:
      - id: op1
        type: u4
  as_9b:
    seq:
      - id: op1
        type: u1
  as_9c_reset_chip_status:
    seq:
      - id: op1
        type: u1
  as_9d:
    seq:
      - id: op1
        type: u1
  as_9e_set_timer:
    seq:
      - id: op1
        type: u1
  as_9f_set_battle_mode:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_a0:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
      - id: op3
        type: u2
      - id: op4
        type: strz
        encoding: shift-jis
  as_a1:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u4
  as_a2:
    seq:
      - id: op1
        type: u1
  as_a3:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_a4: #TODO
    seq: []
  as_a5:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u1
  as_a6:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u1
      - id: op4
        type: u4
      - id: op5
        type: u4
      - id: op6
        type: u4
      - id: op7
        type: u4
  as_a7_battle_effect_end:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
  as_a8_damage_voice:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
  as_a9:
    seq:
      - id: op1
        type: u4
  as_aa:
    seq:
      - id: op1
        type: u4
      - id: op2
        type: u4
  as_ab:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u1
  as_ac:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u1
  as_ad:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
  as_ae:
    seq:
      - id: op1
        type: u2
      - id: op2
        type: u4
  as_af:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u1
      - id: op3
        type: u4
      - id: op4
        type: u4
      - id: op5
        type: u4
  as_b0:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
  as_b1:
    seq:
      - id: op1
        type: u1
      - id: op2
        type: u2
  instruction:
    seq:
      - id: opcode
        type: u1
      - id: params
        type: 
          switch-on: opcode
          cases:
            0x00: as_00_end
            0x01: as_01_goto
            0x02: as_02_select_sub_chip
            0x03: as_03_deg
            0x04: as_04
            0x05: as_05
            0x06: as_06_sleep
            0x07: as_07_clear_effect
            0x08: as_08_teleport
            0x09: as_09
            0x0A: as_0a
            0x0B: as_0b_turn
            0x0C: as_0c
            0x0D: as_0d_jump
            0x0E: as_0e_drop_down
            0x0F: as_0f_jump_to_target
            0x10: as_10_jump_back
            0x11: as_11_move
            0x12: as_12_add_effect
            0x13: as_13_release_effect
            0x14: as_14
            0x15: as_15_wait_effect
            0x16: as_16_finish_effect
            0x17: as_17_cancel_effect
            0x18: as_18_show_effect
            0x19: as_19_show_3d_effect
            0x1a: as_1a
            0x1b: as_1b_select_chip
            0x1c: as_1c_damage
            0x1d: as_1d_damage_anime
            0x1e: as_1e
            0x1f: as_1f
            0x20: as_20
            0x21: as_21
            0x22: as_22_begin_thread
            0x23: as_23_wait_thread
            0x24: as_24_set_chip_mode_flag
            0x25: as_25_clear_chip_mode_flag
            0x26: as_26
            0x27: as_27
            0x28: as_28_char_say
            0x29: as_29
            0x2a: as_2a_tip_text
            0x2b: as_2b
            0x2c: as_2c_shadow_begin
            0x2d: as_2d_shadow_end
            0x2e: as_2e_shake_char
            0x2f: as_2f_suspend_thread
            0x30: as_30
            0x31: as_31
            0x32: as_32
            0x33: as_33
            0x34: as_34
            0x35: as_35_keep_angle
            0x36: as_36
            0x37: as_37_rotation_angle
            0x38: as_38_rotation_angle_v
            0x39: as_39_set_angle
            0x3a: as_3a_tilt_angle
            0x3b: as_3b_rotation_angle_h
            0x3c: as_3c
            0x3d: as_3d_shake_screen
            0x3e: as_3e
            0x3f: as_3f
            0x40: as_40
            0x41: as_41_lock_angle
            0x42: as_42
            0x43: as_43_set_bk_color
            0x44: as_44_zoom_angle
            0x45: as_45
            0x46: as_46
            0x47: as_47
            0x48: as_48
            0x49: as_49_set_control
            0x4a: as_4a
            0x4b: as_4b_random
            0x4c: as_4c_loop_target_beg
            0x4d: as_4d_reset_loop_target
            0x4e: as_4e_loop_target_end
            0x4f: as_4f
            0x50: as_50_call
            0x51: as_51_ret
            0x52: as_52
            0x53: as_53
            0x54: as_54
            0x55: as_55_magic_cast_begin
            0x56: as_56_magic_cast_end
            0x57: as_57
            0x58: as_58_beat_back
            0x59: as_59
            0x5a: as_5a
            0x5b: as_5b
            0x5c: as_5c_show
            0x5d: as_5d_hide
            0x5e: as_5e
            0x5f: as_5f
            0x60: as_60
            0x61: as_61_set_battle_speed
            0x62: as_62
            0x63: as_63
            0x64: as_64_sound_effect
            0x65: as_65_sound_effect_ex
            0x66: as_66
            0x67: as_67_scraft_cut_in
            0x68: as_68
            0x69: as_69_release_texture
            0x6a: as_6a_load_schip
            0x6b: as_6b_reset_scraft_chip
            0x6c: as_6c_die
            0x6d: as_6d
            0x6e: as_6e
            0x6f: as_6f
            0x70: as_70
            0x71: as_71
            0x72: as_72
            0x73: as_73
            0x74: as_74
            0x75: as_75
            0x76: as_76
            0x77: as_77
            0x78: as_78_set_eff_state
            0x79: as_79
            0x7a: as_7a_craft_end
            0x7b: as_7b_set_craft_end_flag
            0x7c: as_7c
            0x7d: as_7d
            0x7e: as_7e
            0x7f: as_7f_blur_screen
            0x80: as_80
            0x81: as_81
            0x82: as_82
            0x83: as_83_sort_target
            0x84: as_84_rotate_char
            0x85: as_85
            0x86: as_86
            0x87: as_87
            0x88: as_88_voice
            0x89: as_89_save_cur_pos
            0x8a: as_8a_clone
            0x8b: as_8b_use_item_begin
            0x8c: as_8c_use_item_end
            0x8d: as_8d_zoom
            0x8e: as_8e_load_x_file
            0x8f: as_8f
            0x90: as_90
            0x91: as_91
            0x92: as_92
            0x93: as_93
            0x94: as_94
            0x95: as_95
            0x96: as_96_set_angle_target
            0x97: as_97_move_angle
            0x98: as_98
            0x99: as_99
            0x9a: as_9a
            0x9b: as_9b
            0x9c: as_9c_reset_chip_status
            0x9d: as_9d
            0x9e: as_9e_set_timer
            0x9f: as_9f_set_battle_mode
            0xa0: as_a0
            0xa1: as_a1
            0xa2: as_a2
            0xa3: as_a3
            0xa4: as_a4
            0xa5: as_a5
            0xa6: as_a6
            0xa7: as_a7_battle_effect_end
            0xa8: as_a8_damage_voice
            0xa9: as_a9
            0xaa: as_aa
            0xab: as_ab
            0xac: as_ac
            0xad: as_ad
            0xae: as_ae
            0xaf: as_af
            0xb0: as_b0
            0xb1: as_b1