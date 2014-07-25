using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reports
{
    /// <summary>
    /// Отчет по всем валютам за один день
    /// </summary>
    public class DayReport
    {
        private Dictionary<string, CurrencyReport> _currency_reports = new Dictionary<string,CurrencyReport>();

        public DateTime MinDate { get; set; }
        public DateTime MaxDate { get; set; }

        /// <summary>
        /// Список отчетов по всем валютам за текущий день
        /// </summary>
        public CurrencyReport[] Report {
            get {
                return _currency_reports.Values.ToArray();
            }
        }

        /// <summary>
        /// Увеличивает баланс валюты на значение money
        /// </summary>
        /// <param name="cur_index">индекс валюты в таблице валют</param>
        /// <param name="money">значение, на которое необходимо увеличить баланс</param>
        public void AddMoneyValue(string currency, int money) 
        {
            if (!_currency_reports.ContainsKey(currency))                
                _currency_reports.Add(currency, new CurrencyReport(currency));
            if(money != 0)
                _currency_reports[currency].Money += money;
        }

        public DayReport(DateTime minDate, DateTime maxDate) 
        {
            MinDate = minDate; MaxDate = maxDate;
        }
    }
}
