using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Communal
{
    /// <summary>
    /// Список единиц измерения 
    /// </summary>
    public class MeasureTypes 
    {
        private static string[] _strings = { "", "чел.", "м³", "м²", "кВт/ч", "мес" };
        public enum Measure { None, PerPerson, PerVolume, PerSquare, KilowattHour, PerMonth }
        public static string[] Names
        {
            get {
                return _strings;
            }
        }
        public static string GetName(Measure type) 
        {
            return _strings[Convert.ToInt32(type)];
        }
        public static Measure GetType(string name) 
        {
            int index = 0;
            foreach(string nm in _strings)
            {
                if(nm.ToUpper() == name.ToUpper())
                    return (Measure)index;
                index++;
            }
            return Measure.None;
        }
        public static Measure GetType(object type_id) 
        {
            return (Measure)(Convert.ToInt32(type_id));
        }
    }
}
