using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reports
{
    public class Report
    {
        private List<ColumnDateHeader> _columns = new List<ColumnDateHeader>();
        private Dictionary<int,ReportRow> _rows = new Dictionary<int,ReportRow>();
        public enum Period { Dayly, Weekly, Monthly, Quarterly, Yearly};
        public Report(DateTime minDate, DateTime maxDate, Period period)
        {
            // swap dates if necessary
            DateTime date;
            if (minDate > maxDate)
            {
                date = maxDate;
                maxDate = minDate;
                minDate = date;
            }
            // create columns
            _columns.Clear();
            date = minDate.Date;
            DateTime next_date = minDate.Date;
            while (date <= maxDate)
            {
                #region NEXT DATE
                switch (period)
                {
                    case Period.Dayly:
                        next_date = date.AddDays(1);
                        break;
                    case Period.Weekly:
                        next_date = date.AddDays(7);
                        break;
                    case Period.Monthly:
                        next_date = date.AddMonths(1);
                        break;
                    case Period.Quarterly:
                        next_date = date.AddMonths(3);
                        break;
                    case Period.Yearly:
                        next_date = date.AddYears(1);
                        break;
                }
                #endregion
                ColumnDateHeader col = new ColumnDateHeader();
                col.MinDate = date;
                col.MaxDate = next_date.AddDays(-1);
                if (col.MaxDate > maxDate)
                    col.MaxDate = maxDate;

                col.Caption = date.ToString("dd.MM.yyyy");
                if(col.MinDate != col.MaxDate)
                    col.Caption = col.MinDate.ToString("dd.MM.yyyy\n") + col.MaxDate.ToString("dd.MM.yyyy");
                
                _columns.Add(col);
                date = next_date;
            }
            // create rows

        }

        public void AddBalance(int id, int parent_id, string name, DateTime date, string currency, int money) 
        {
            if (!_rows.ContainsKey(id)) 
            {
                AddRow(id, parent_id, name);
            }
                
            _rows[id].AddBalance(date, currency, money);

            // Увеличиваем сумму для всех предков этой записи
            HierarchyAddBalance(_rows[id].DocType.ParentId, date, currency, money);
            

        }

        private int GetParentCount(int parent_id)
        {
            if (_rows.ContainsKey(parent_id))
                return GetParentCount(_rows[parent_id].DocType.ParentId) + 1;
            else return 0;
        }

        private void HierarchyAddBalance(int parentId, DateTime date, string currency, int money)
        {
            if (parentId != 0 && _rows.ContainsKey(parentId)) 
            {
                _rows[parentId].AddBalance(date, currency, money);
                HierarchyAddBalance(_rows[parentId].DocType.ParentId, date, currency, money);
            }
        }

        public ReportRow[] Rows 
        {
            get {
                return _rows.Values.ToArray();
            }
        }
        public ColumnDateHeader[] Columns 
        {
            get {
                return _columns.ToArray();
            }
        }

        public bool ContainsId(int id)
        {
            return _rows.ContainsKey(id);
        }

        public void AddRow(int id, int parent_id, string name)
        {
            if (!_rows.ContainsKey(id))
            {
                _rows.Add(id, new ReportRow(new DocTypeInfo(id, name, parent_id), _columns.ToArray()));
                // Добавляем сумму всех дочерних этой записи
                foreach (ReportRow row in _rows.Values)
                {
                    if (row.DocType.ParentId == id)
                    {
                        foreach (DayReport report in row.DayReports)
                        {
                            foreach (CurrencyReport cur_rep in report.Report)
                                _rows[id].AddBalance(report.MinDate, cur_rep.Currency, cur_rep.Money);
                        }
                        row.Level++;
                    }
                }
                _rows[id].Level = GetParentCount(parent_id);
            }
        }
    }
}
