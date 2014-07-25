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
using System.Threading;

namespace WpfHomeBudget
{
    /// <summary>
    /// Interaction logic for CommunalDialog.xaml
    /// </summary>
    public partial class CommunalDialog : Window
    {
        private bool _ignore_changing = false;
        private Communal.Document _document = null;
        public CommunalDialog()
        {
            InitializeComponent();
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            if (CheckData()) 
            {
                DialogResult = true;
                Close();
            }
        }

        private bool CheckData()
        {
            if (cbxType.SelectedItem == null)
            {
                cbxType.Focus();
                return false;
            }

            if (String.IsNullOrEmpty(txtSumma.Text))
            {
                txtSumma.Focus();
                return false;
            }

            if (String.IsNullOrEmpty(txtTariff.Text))
            {
                txtTariff.Focus();
                return false;
            }
            
            return true;
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {

        }
        public static bool Edit(Communal.Document document, Window owner) 
        {
#if COMMUNAL
            CommunalDialog dlg = new CommunalDialog();
            dlg.Owner = owner;
            dlg.SetData(document);
            if (dlg.ShowDialog().Value) 
            {
                dlg.GetData(document);
                db.Database.WriteDocument(document);
                return true;
            }
#endif
            return false;
        }

        private string[] GetMonthList() 
        {
            string[] monthes = new string[12];
            //System.Globalization.DateTimeFormatInfo dti = new System.Globalization.DateTimeFormatInfo();
            for (int i = 0; i < 12; i++) 
            {
                monthes[i] = Thread.CurrentThread.CurrentUICulture.DateTimeFormat.GetMonthName(i+1);
            }
            return monthes;
        }
        private void SetData(Communal.Document document)
        {
            try
            {
                _ignore_changing = true;
                _document = document;
                cbxPeriodMonth.ItemsSource = GetMonthList();
                
                for(int i = -10;i <= 10; i++)
                {
                    cbxPeriodYear.Items.Add(DateTime.Today.Year + i);
                }
                SelectedPeriod = document.PeriodBegin;
                dateDate.SelectedDate = document.Date;
                cbxType.ItemsSource = Communal.Globals.CommunalTypes.Values;
                if (document.Type is Communal.CommunalType)
                {
                    Communal.CommunalType type = Communal.Globals.CommunalTypes[document.Type.Id];
                    cbxType.SelectedItem = type;
                    lblTariff.Content = Communal.MeasureTypes.GetName(type.Measure);
                }
                else 
                {
                    lblTariff.Content = "";
                }
                txtCounter.Text = (document.Counter == 0) ? "" : document.Counter.ToString();
                txtCounterAfter.Text = (document.CounterAfter == 0) ? "" : document.CounterAfter.ToString();
                txtCounterBefore.Text = (document.CounterBefore == 0) ? "" : document.CounterBefore.ToString();
                txtSumma.Text = document.Amount.ToString();
                txtSummaAlt.Text = (document.AmountAlt == 0.0) ? "" : document.AmountAlt.ToString();
            }
            finally 
            {
                _ignore_changing = false;
            }            
        }
        private void GetData(Communal.Document document)
        {
            document.Amount = Kernel.Amount.Parse(txtSumma.Text, 0);
            document.AmountAlt = Kernel.Amount.Parse(txtSummaAlt.Text, 0);
            document.Counter = Kernel.Amount.Parse(txtCounter.Text, 0) / 100;
            document.CounterAfter = Kernel.Amount.Parse(txtCounterAfter.Text, 0) / 100;
            document.CounterBefore = Kernel.Amount.Parse(txtCounterBefore.Text, 0) / 100;
            document.Date = dateDate.SelectedDate.Value;
            document.PeriodBegin = SelectedPeriod;
            document.Tariff = Double.Parse(txtTariff.Text);
            document.Type = cbxType.SelectedItem as Communal.CommunalType;
        }

        private DateTime SelectedPeriod {
            get { 
                return new DateTime((int)(cbxPeriodYear.SelectedItem), cbxPeriodMonth.SelectedIndex + 1, 1);
            }
            set {
                cbxPeriodYear.SelectedItem = value.Year;
                cbxPeriodMonth.SelectedIndex = value.Month - 1; 
            }
        }
        private void cbxPeriod_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
#if COMMUNAL
            if (_ignore_changing) return;
            if (cbxType.SelectedItem is Communal.CommunalType) 
            {
                
                Communal.CommunalType type = (cbxType.SelectedItem as Communal.CommunalType);
                double tariff = Communal.Globals.ActualTariffs.GetTariffOnDate(type.Id, SelectedPeriod);
                txtTariff.Text = tariff.ToString();
                lblTariff.Content = " / " + Communal.MeasureTypes.GetName(type.Measure);
                if (_document.Id <= 0) 
                {
                    _ignore_changing = true;
                    if (type.DefaultValue != 0.0)
                    {
                        txtCounterBefore.IsEnabled = false;
                        txtCounterAfter.IsEnabled = false;
                        txtCounter.IsEnabled = false;
                        txtSumma.Text = Kernel.Amount.ToMoney(type.DefaultValue * tariff);
                    }
                    else 
                    {
                        txtCounterBefore.IsEnabled = true;
                        txtCounterAfter.IsEnabled = true;
                        txtCounter.IsEnabled = true;
                        txtCounterBefore.Text = db.Database.GetLastCounterValue(type.Id, SelectedPeriod).ToString();
                        int before = 0;
                        int after = 0;
                        if (Int32.TryParse(txtCounterBefore.Text, out before) && Int32.TryParse(txtCounterAfter.Text, out after))
                        {
                            txtCounter.Text = (after - before).ToString();
                            txtSumma.Text = Kernel.Amount.ToMoney(tariff * (after - before));
                        }
                        else txtSumma.Text = "";
                    }
                    _ignore_changing = false;
                }
            }
#endif
        }

        private void txtCounterAfter_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (_ignore_changing)
                return;
            bool _init_old = _ignore_changing;
            try
            {
                _ignore_changing = true;
                int before = 0;
                int after = 0;
                if (Int32.TryParse(txtCounterBefore.Text, out before) && Int32.TryParse(txtCounterAfter.Text, out after))
                {
                    txtCounter.Text = (after - before).ToString();
                    double tariff = 0.0;
                    if (Double.TryParse(txtTariff.Text, out tariff))
                    {
                        txtSumma.Text = Kernel.Amount.ToMoney(Convert.ToInt32(Math.Round(tariff * (after - before) * 100.0)));
                    }
                    else txtSumma.Text = "";
                }                
            }
            finally 
            {
                _ignore_changing = _init_old;
            }
        }

        private void txtCounter_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (_ignore_changing)
                return;
            bool _init_old = _ignore_changing;
            try
            {
                _ignore_changing = true;
                int before = 0;
                int diff = 0;
                if (Int32.TryParse(txtCounterBefore.Text, out before) && Int32.TryParse(txtCounter.Text, out diff))
                {
                    txtCounterAfter.Text = (before + diff).ToString();
                    double tariff = 0.0;
                    if (Double.TryParse(txtTariff.Text, out tariff)) 
                    {
                        txtSumma.Text = Kernel.Amount.ToMoney(Convert.ToInt32(Math.Round(tariff * diff * 100.0)));
                    }
                    else txtSumma.Text = "";
                }
            }
            finally
            {
                _ignore_changing = _init_old;
            }
        }
    }
}
