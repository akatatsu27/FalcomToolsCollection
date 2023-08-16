using System.Numerics;
using Newtonsoft.Json;

namespace Shared;

public class BitFlagsToArrayConverter<TEnum> : JsonConverter<TEnum> where TEnum : Enum
{
	public override void WriteJson(JsonWriter writer, TEnum value, JsonSerializer serializer)
	{
		IEnumerable<string> flags = value.ToString()
			.Split(new[] { ", " }, StringSplitOptions.RemoveEmptyEntries)
			.Select(f => $"\"{f}\"");

		writer.WriteRawValue($"[{string.Join(", ", flags)}]");
	}

	public override TEnum? ReadJson(JsonReader reader, Type objectType, TEnum? existingValue, bool hasExistingValue, JsonSerializer serializer)
	{
		Type underlyingType = Enum.GetUnderlyingType(typeof(TEnum));

		object outVal = typeof(BitFlagsToArrayConverter<TEnum>).GetMethod("AddValues", System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.NonPublic)!
			.MakeGenericMethod(underlyingType).Invoke(null, new object[] { reader, objectType, existingValue, hasExistingValue, serializer })!;
		return (TEnum)outVal!;
	}

	private static TNum AddValues<TNum>(JsonReader reader, Type objectType, TEnum? existingValue, bool hasExistingValue, JsonSerializer serializer) where TNum : IBinaryInteger<TNum>
	{
		TNum outVal = TNum.Zero;
		if (reader.TokenType == JsonToken.StartArray)
		{
			reader.Read();
			while (reader.TokenType != JsonToken.EndArray)
			{
				outVal += (TNum)Enum.Parse(objectType, reader.Value.ToString());
				reader.Read();
			}
		}
		return outVal;
	}
}
