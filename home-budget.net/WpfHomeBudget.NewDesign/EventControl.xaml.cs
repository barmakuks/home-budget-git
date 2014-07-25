using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfHomeBudget
{
    /// <summary>
    /// Interaction logic for MessageControl.xaml
    /// </summary>
    public partial class EventControl : UserControl
    {
        Kernel.Event _event = null;
        public EventControl()
        {
            InitializeComponent();
        }
        private void SetData(Kernel.Event msg) 
        {
            _event = msg;
            txtMessage.Text = msg.Message;
            btnDocument.Visibility = (msg.ActionId != 0) ? Visibility.Visible : Visibility.Collapsed;
        }
        public static EventControl Create(Kernel.Event msg) 
        {
            EventControl ctrl = new EventControl();
            ctrl.SetData(msg);
            return ctrl;
        }
        public event EventHandler EventStateChanged;

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            _event.LastDate = DateTime.Today;
            DateTime next_date = _event.Date.AddDays(_event.Duration + 1);
            if(DateTime.Today >= next_date)
                next_date = DateTime.Today.AddDays(1);
            _event.CalculateNextDate(next_date);
            db.Database.WriteEvent(_event);
            this.Visibility = Visibility.Collapsed;
            if (EventStateChanged != null)
                EventStateChanged(sender, EventArgs.Empty);
        }

        private void btnIgnore_Click(object sender, RoutedEventArgs e)
        {
            this.Visibility = Visibility.Collapsed;
            if (EventStateChanged != null)
                EventStateChanged(sender, EventArgs.Empty);
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            _event.IsActive = false;
            db.Database.WriteEvent(_event);
            this.Visibility = Visibility.Collapsed;
            if (EventStateChanged != null)
                EventStateChanged(sender, EventArgs.Empty);
        }

        private void btnDocument_Click(object sender, RoutedEventArgs e)
        {
            this.Visibility = Visibility.Collapsed;
            if (EventStateChanged != null)
                EventStateChanged(sender, EventArgs.Empty);
        }
    }
}
