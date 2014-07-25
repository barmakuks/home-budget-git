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
using System.Windows.Shapes;

namespace WpfHomeBudget
{
    /// <summary>
    /// Interaction logic for EventDialog.xaml
    /// </summary>
    public partial class EventDialog : Window
    {
        public EventDialog()
        {
            InitializeComponent();
        }
        public static bool Edit(Kernel.Event evnt) 
        {
            EventDialog dlg = new EventDialog();
            dlg.SetData(evnt);
            if (dlg.ShowDialog().Value) 
            {
                dlg.GetData(evnt);
                evnt.CalculateNextDate(DateTime.Today);
                db.Database.WriteEvent(evnt);
                return true;
            }
            return false;
        }

        private void GetData(Kernel.Event evnt)
        {
            evnt.Date = dateDate.SelectedDate.Value;
            evnt.Message = txtText.Text;
            evnt.Periodicity.Period = (Kernel.EventPeriodicity.Periodicity)(cbxPeriodicity.SelectedIndex);
            evnt.Periodicity.ExactValue = Convert.ToInt32(txtPeriodValue.Text);
            evnt.Duration = Convert.ToInt32(txtDuration.Text) - 1;
        }

        private void SetData(Kernel.Event evnt)
        {
            dateDate.SelectedDate = evnt.Date;
            txtText.Text = evnt.Message;
            cbxPeriodicity.ItemsSource = Kernel.EventPeriodicity.PeriodicityStrings;
            cbxPeriodicity.SelectedIndex = Convert.ToInt32(evnt.Periodicity.Period);
            txtPeriodValue.Text = evnt.Periodicity.ExactValue.ToString();
            txtDuration.Text = (evnt.Duration + 1).ToString();
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private void cbxPeriodicity_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            txtPeriodValue.Visibility = (cbxPeriodicity.SelectedIndex == 0) ? Visibility.Visible : Visibility.Hidden;
        }
    }
}
