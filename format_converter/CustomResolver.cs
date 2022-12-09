using System.Reflection;
using Kaitai;
using Mono.Reflection;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace format_converter
{

        
    class CustomResolver : DefaultContractResolver
    {
        readonly string[] ToIgnore = new string[] { "M_Root", "M_Parent", "M_Io", "m_root", "m_parent", "m_io" };
        protected override JsonProperty CreateProperty(MemberInfo member,
                                            MemberSerialization memberSerialization)
        {
            JsonProperty property = base.CreateProperty(member, memberSerialization);

            if (typeof(KaitaiStruct).IsAssignableFrom(property.PropertyType) &&
                ToIgnore.Contains(property.PropertyName))
            {
                property.ShouldSerialize = instanceOfProblematic => false;
            }
            if (typeof(KaitaiStream).IsAssignableFrom(property.PropertyType))
            {
                property.ShouldSerialize = instanceOfProblematic => false;
            }
            if (typeof(Stream).IsAssignableFrom(property.PropertyType))
            {
                property.Ignored = true;
            }
            if (!property.Writable && !property.Ignored)
            {                
                var propInfo = property.DeclaringType.GetProperty(property.PropertyName);
                var fieldInfo = BackingFieldResolver.GetBackingField(propInfo);
                if (fieldInfo != null)
                {
                    property.ValueProvider = new ReflectionValueProvider(fieldInfo);
                    property.Writable = true;
                }
            }
            return property;
        }
    }
}
