using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reports
{
    /// <summary>
    /// Описывает заголовок таблицы
    /// </summary>
    public class ColumnHeader
    {
        public string Caption { get; set; } 
    }

    /// <summary>
    /// Описывает заголовок таблицы с датой
    /// </summary>
    public class ColumnDateHeader : ColumnHeader
    {
        public DateTime MinDate { get; set; }
        public DateTime MaxDate { get; set; }
    }
}
