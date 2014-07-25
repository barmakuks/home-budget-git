using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reports
{
    /// <summary>
    /// Полный отчет по одному из видов документов
    /// </summary>
    public class ReportRow
    {
        internal bool IsExpanded = false;
        internal int Level = 0;
        public string Margin 
        {
            get {
                return String.Format("{0} 0 4 0", Level * 15 + 4);
            }
        }        
        private Dictionary<string, CurrencyReport> _currency = new Dictionary<string, CurrencyReport>();
        private List<DayReport> _day_reports = new List<DayReport>();

        public DocTypeInfo DocType { get; set; }
        public string DocTypeName {
            get {
                return DocType.Name;
            }
        }
        public ReportRow(DocTypeInfo doc_type_info, ColumnDateHeader[] date_columns) 
        {
            DocType = doc_type_info;
            foreach (ColumnDateHeader day in date_columns) 
            {
                _day_reports.Add(new DayReport(day.MinDate, day.MaxDate));
            }
        }
        /// <summary>
        /// Добавлет в отчет одну строку операции
        /// </summary>
        /// <param name="date">Дата операции</param>
        /// <param name="currency">Валюта операции</param>
        /// <param name="money">Сумма операции</param>
        public void AddBalance(DateTime date, string currency, int money) 
        {
            if (_currency.ContainsKey(currency))
            {   // такая валюта уже есть в корзине валют
                _currency[currency].Money += money;
                foreach (DayReport day in _day_reports)                 
                {
                    if (date >= day.MinDate && date <= day.MaxDate) 
                    {
                        day.AddMoneyValue(currency, money);
                        return;
                    }
                }
            }
            else 
            {   // нужно добавить валюту в корзину валют
                _currency.Add(currency, new CurrencyReport(currency));
                _currency[currency].Money += money;
                foreach (DayReport day in _day_reports)
                {
                    if (date >= day.MinDate && date <= day.MaxDate)
                    {
                        day.AddMoneyValue(currency, money);
                    }
                    else
                        day.AddMoneyValue(currency, 0);
                }
            }
        }

        public DayReport[] DayReports 
        {
            get {
                return _day_reports.ToArray();
            }
        }
        public CurrencyReport[] TotalReport 
        {
            get { return _currency.Values.ToArray(); }
        }
    }
}
