using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Communal
{
    /// <summary>
    /// Документ о начислении или оплате за коммунальную услугу
    /// </summary>
    public class Document
    {
        public Document() 
        {
            Id = 0; CounterAfter = CounterBefore = Counter = 0;
            Amount = AmountAlt = 0; this.Tariff = 0;
            Sign = 0;
            Date = DateTime.Today;
            DateTime last_month = DateTime.Today.AddMonths(-1);
            PeriodBegin = new DateTime(last_month.Year, last_month.Month, 1);
        }
        public int Id { get; set; }
        /// <summary>
        /// дата документа
        /// </summary>
        public DateTime Date { get; set; }
        /// <summary>
        /// Тип коммунальной услуги
        /// </summary>
        public CommunalType Type { get; set; }
        /// <summary>
        /// Период, за который отвечает документ
        /// </summary>
        public DateTime PeriodBegin { get; set; }
        /// <summary>
        /// Период, за который отвечает документ
        /// </summary>
        public DateTime PeriodEnd { get; set; }
        /// <summary>
        /// Показание счетчика до 
        /// </summary>
        public int CounterBefore { get; set; }
        /// <summary>
        /// Показание счетчика после
        /// </summary>
        public int CounterAfter { get; set; }
        /// <summary>
        /// Разница в показаниях счетчика
        /// </summary>
        public int Counter { get; set; }
        /// <summary>
        /// Сумма указанная в документе, получается при проведении расчетов
        /// </summary>
        public int Amount { get; set; }
        /// <summary>
        /// Сумма получаемая из коммунальных служб
        /// </summary>
        public int AmountAlt { get; set; }
        /// <summary>
        /// Тариф, по которому проводился расчет и платеж
        /// </summary>
        public double Tariff { get; set; }
        /// <summary>
        /// +1 - начисление; -1 - оплата
        /// </summary>
        public int Sign { get; set; }


        #region Функции для отображения данных в строку
        private static string ToMoney(int summa)
        {
            int abs = Math.Abs(summa);
            string sign = summa < 0 ? "-" : "";
            string res = "0.00";
            if (abs < 10)
                res = "0.0" + abs.ToString();
            else
                if (abs < 100)
                    res = "0." + abs.ToString();
                else 
                {
                    res = (abs / 100).ToString() + ".";
                    int kop = abs % 100;
                    if(kop < 10)
                        res = res + "0" + kop.ToString();
                    else
                        res = res + kop.ToString();
                }
            return sign + res;
        }

        public string strPeriod 
        {
            get {
                return Thread.CurrentThread.CurrentUICulture.DateTimeFormat.GetAbbreviatedMonthName(PeriodBegin.Month) + " " + PeriodBegin.Year.ToString();
                //return Period.ToString("MMM yyyy");
            }
        }
        public string strCounter
        {
            get
            {
                return Counter == 0 ? "" : Counter.ToString();
            }
        }
        public string strCounterAfter
        {
            get
            {
                return CounterAfter == 0 ? "" : CounterAfter.ToString();
            }
        }
        public string strCounterBefore
        {
            get
            {
                return CounterBefore == 0 ? "" : CounterBefore.ToString();
            }
        }
        public string strAmount{
            get {
                return ToMoney(Amount);
            }
        }
        public string strAmountAlt
        {
            get
            {
                return (AmountAlt != 0) ? ToMoney(Amount) : "";
            }
        }
        public string strTariff {
            get {
                return Tariff.ToString();
            }
        }
        #endregion
    }
}
