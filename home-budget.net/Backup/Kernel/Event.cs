using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
        /// <summary>
        /// Периодичность появления сообщения
        /// </summary>
        public class EventPeriodicity 
        {
            /// <summary>
            /// Периодичность
            /// </summary>
            public enum Periodicity { Exact, OneTime, Dayly, Weekly, Monthly, Quarterly, Yearly }
            public static string[] PeriodicityStrings = new string[] { "Указать значение", "Один раз", "Ежедневно", "Еженедельно", "Ежемесячно", "Ежеквартально", "Ежегодно" };
            public Periodicity Period { get; set; }
            public int ExactValue;
        }

    public class Event
    {
        private EventPeriodicity _event_periodicity = new EventPeriodicity();

        public Event() 
        {
            Id = 0;
            ActionId = 0;
            IsActive = true;
            Date = DateTime.Today;
            Periodicity.ExactValue = 1;
            Periodicity.Period = EventPeriodicity.Periodicity.OneTime;
            Duration = 0;
        }

        public EventPeriodicity Periodicity { get 
            {
                return _event_periodicity;
            }
        }
        /// <summary>
        /// Идентификатор сообщения
        /// </summary>
        public int Id { get; set; }

        public bool IsActive { get; set; }
        /// <summary>
        /// Дата первого сообщения
        /// </summary>
        public DateTime Date { get; set; }
        /// <summary>
        /// Продолжительнгость показа сообщения в днях
        /// </summary>
        public int Duration { get; set; }

        /// <summary>
        /// Дата последнего сообщения, прочитанного пользователем
        /// </summary>
        public DateTime LastDate { get; set; }
        
        /// <summary>
        /// Текст сообщения
        /// </summary>
        public string Message { get; set; }
        /// <summary>
        /// Идентификатор связанного с сообщение документа, который необходимо будет создать
        /// при получении сообщения
        /// </summary>
        public int ActionId { get; set; }

        /// <summary>
        /// Вычисляет дату следующего события
        /// </summary>
        /// <param name="minDate"></param>
        /// <returns></returns>
        public DateTime CalculateNextDate(DateTime minDate)
        {
            DateTime next_date = this.Date;
            while (next_date.AddDays(Duration) < minDate) // или while (next_date.AddDays(Duration) <= minDate) 
            {
                switch (Periodicity.Period)
                {
                    case EventPeriodicity.Periodicity.Dayly:
                        next_date = next_date.AddDays(1);
                        break;
                    case EventPeriodicity.Periodicity.Exact:
                        next_date = next_date.AddDays(Periodicity.ExactValue);
                        break;
                    case EventPeriodicity.Periodicity.Monthly:
                        next_date = next_date.AddMonths(1);
                        break;
                    case EventPeriodicity.Periodicity.OneTime:
                        IsActive = false;
                        return Date;
                    case EventPeriodicity.Periodicity.Quarterly:
                        next_date = next_date.AddMonths(3);
                        break;
                    case EventPeriodicity.Periodicity.Weekly:
                        next_date = next_date.AddDays(7);
                        break;
                    case EventPeriodicity.Periodicity.Yearly:
                        next_date = next_date.AddYears(1);
                        break;
                }
            }
            Date = next_date;            
            return next_date;
        }

        /// <summary>
        /// Проверяет, выпадает ли напоминание в указанную дату
        /// </summary>
        /// <param name="date">Дата</param>
        /// <returns></returns>
        public bool CheckDate(DateTime date) 
        {
            return IsActive && date >= Date && date <= Date.AddDays(Duration);
        }
    }

    public class EventsList : List<Event> 
    {
        public IEnumerable<Event> GetDateEvents(DateTime date) 
        {
            List<Event> date_events = new List<Event>();
            foreach(Event evnt in this)
            {
                if (evnt.CheckDate(date))
                    date_events.Add(evnt);
            }
            return date_events.ToArray();
        }
    }
}
