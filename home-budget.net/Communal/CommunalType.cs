using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Communal
{
    /// <summary>
    /// Вид коммунальной услуги
    /// </summary>
    public class CommunalType{
        public int Id { get; set; }
        /// <summary>
        /// Наименование услуги
        /// </summary>
        public string Name { get; set; }
        /// <summary>
        /// Вид измерения
        /// </summary>
        public MeasureTypes.Measure Measure { get; set; }
        /// <summary>
        /// Значение постоянного объема услуги
        /// </summary>
        public double DefaultValue { get; set; }

        public override string ToString()
        {
            return Name;
        }
    }

    /// <summary>
    /// Список видов коммунальных услуг
    /// </summary>
    public class CommunalTypesList : Dictionary<int, CommunalType> 
    {
    }
}
