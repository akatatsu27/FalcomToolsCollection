using Newtonsoft.Json;

namespace Shared;

public class ByteArrayConverter : JsonConverter<byte[]>
{
	public override void WriteJson(JsonWriter writer, byte[]? value, JsonSerializer serializer)
	{
		if (value == null)
		{
			writer.WriteNull();
			return;
		}

		// Compose an array.
		writer.WriteStartArray();

		for (var i = 0; i < value.Length; i++)
		{
			writer.WriteValue(value[i]);
		}

		writer.WriteEndArray();
	}

	public override byte[]? ReadJson(JsonReader reader, Type objectType, byte[]? existingValue, bool hasExistingValue, JsonSerializer serializer)
	{
		if (reader.TokenType == JsonToken.StartArray)
		{
			List<byte> byteList = new();

			while (reader.Read())
			{
				switch (reader.TokenType)
				{
					case JsonToken.Integer:
						byteList.Add(Convert.ToByte(reader.Value));
						break;
					case JsonToken.EndArray:
						return byteList.ToArray();
					case JsonToken.Comment:
						// skip
						break;
					default:
						throw new Exception(
						string.Format(
							"Unexpected token when reading bytes: {0}",
							reader.TokenType));
				}
			}

			throw new Exception("Unexpected end when reading bytes.");
		}
		else
		{
			throw new Exception(
				string.Format(
					"Unexpected token parsing binary. "
					+ "Expected StartArray, got {0}.",
					reader.TokenType));
		}
	}
}