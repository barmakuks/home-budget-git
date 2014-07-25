using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
    public class ReportBuilder
    {
        public enum Periodicity { Daily, Weekly, Monthly }
        private class CurrencyReport : Dictionary<int, int> { }
        private class DayReport : Dictionary<DateTime, CurrencyReport> { }
        private class Report : Dictionary<int, DayReport> { }
        public static string[][] CreateReport(ReportItem[] items, Periodicity periodicity) 
        {
            Report report = new Report();
            foreach (ReportItem item in items) 
            {
                if (!report.ContainsKey(item.DocType.Id))
                    report.Add(item.DocType.Id, new DayReport());
                if (!report[item.DocType.Id].ContainsKey(item.DocDate))
                    report[item.DocType.Id].Add(item.DocDate, new CurrencyReport());
                
                foreach(int cur_code in item.Amounts.Keys)
                {
                    if(!report[item.DocType.Id][item.DocDate].ContainsKey(cur_code))
                        report[item.DocType.Id][item.DocDate].Add(cur_code, 0);
                    report[item.DocType.Id][item.DocDate][cur_code] += item.Amounts[cur_code].Summa;
                }
            }
            return null;
        }
    }
}
