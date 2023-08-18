﻿using Newtonsoft.Json;
using Shared;

namespace MS_Converter;

[Flags]
[JsonConverter(typeof(BitFlagsToArrayConverter<StatusResistanceEnum>))]
public enum StatusResistanceEnum : uint
{
	NONE =		0b0000_0000_0000_0000_0000_0000_0000_0000,
	POISON =	0b0000_0000_0000_0000_0000_0000_0000_0001,
	FREEZE =	0b0000_0000_0000_0000_0000_0000_0000_0010,
	PETRIFY =	0b0000_0000_0000_0000_0000_0000_0000_0100,
	SLEEP =		0b0000_0000_0000_0000_0000_0000_0000_1000,
	MUTE =		0b0000_0000_0000_0000_0000_0000_0001_0000,
	BLIND =		0b0000_0000_0000_0000_0000_0000_0010_0000,
	SEAL =		0b0000_0000_0000_0000_0000_0000_0100_0000,
	CONFUSE =	0b0000_0000_0000_0000_0000_0000_1000_0000,
	FAINT =		0b0000_0000_0000_0000_0000_0001_0000_0000,
	DEATHBLOW =	0b0000_0000_0000_0000_0000_0010_0000_0000,
	_400 =		0b0000_0000_0000_0000_0000_0100_0000_0000,
	RAGE =		0b0000_0000_0000_0000_0000_1000_0000_0000,
	_1000 =		0b0000_0000_0000_0000_0001_0000_0000_0000,
	_2000 =		0b0000_0000_0000_0000_0010_0000_0000_0000,
	_4000 =		0b0000_0000_0000_0000_0100_0000_0000_0000,
	MOV =		0b0000_0000_0000_0000_1000_0000_0000_0000,
	_10_000 =	0b0000_0000_0000_0001_0000_0000_0000_0000,
	STR =		0b0000_0000_0000_0010_0000_0000_0000_0000,
	_40_000 =	0b0000_0000_0000_0100_0000_0000_0000_0000,
	DEF =		0b0000_0000_0000_1000_0000_0000_0000_0000,
	_100_000 =	0b0000_0000_0001_0000_0000_0000_0000_0000,
	SPD =		0b0000_0000_0010_0000_0000_0000_0000_0000,
	_400_000 =	0b0000_0000_0100_0000_0000_0000_0000_0000,
	ADF =		0b0000_0000_1000_0000_0000_0000_0000_0000,
	_1_000_000= 0b0000_0001_0000_0000_0000_0000_0000_0000,
	_2_000_000= 0b0000_0010_0000_0000_0000_0000_0000_0000,
	_4_000_000= 0b0000_0100_0000_0000_0000_0000_0000_0000,
	_8_000_000= 0b0000_1000_0000_0000_0000_0000_0000_0000,
	_10_000_000=0b0001_0000_0000_0000_0000_0000_0000_0000,
	_20_000_000=0b0010_0000_0000_0000_0000_0000_0000_0000,
	_40_000_000=0b0100_0000_0000_0000_0000_0000_0000_0000,
	_80_000_000=0b1000_0000_0000_0000_0000_0000_0000_0000,
}