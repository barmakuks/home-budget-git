using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Communal
{
    /// <summary>
    /// Изменение тарифа на услугу
    /// </summary>
    public class Tariff
    {
        public Tariff() 
        {
            Id = 0; Date = DateTime.Today; Value = 0.0;
        }
        public int Id { get; set; }
        /// <summary>
        /// Вид коммунальной услуги
        /// </summary>
        public CommunalType Type { get; set; }
        /// <summary>
        /// Дата введения нового тарифа
        /// </summary>
        public DateTime Date { get; set; }
        /// <summary>
        /// Значение стоимости услуги
        /// </summary>
        public double Value { get; set; }
        public string ValueString {
            get {
                return Value.ToString() + " грн/" + MeasureTypes.GetName(Type.Measure);
            }
        }
    }
    /// <summary>
    /// Список тарифов актуальных на данный момент времени
    /// </summary>
    public class Tariffs : List<Tariff> 
    {
        public Dictionary<int,Tariff> GetTariffsOnDate(DateTime date)
        {
            Dictionary<int, Tariff> tariffs = new Dictionary<int,Tariff>();
            foreach(Tariff tariff in this)
            {
                if (tariff.Date <= date) 
                {
                    if (tariffs.ContainsKey(tariff.Type.Id))
                    {
                        if (tariffs[tariff.Type.Id].Date < tariff.Date)
                            tariffs[tariff.Type.Id] = tariff;
                    }
                    else tariffs.Add(tariff.Type.Id, tariff);
                }
            }
            return tariffs;
        }

        public double GetTariffOnDate(int type_id, DateTime period)
        {
            Dictionary<int, Tariff> tariffs = GetTariffsOnDate(period);
            if(tariffs.ContainsKey(type_id))
                return tariffs[type_id].Value;
            else 
                return 0.0;
        }
    }
}
