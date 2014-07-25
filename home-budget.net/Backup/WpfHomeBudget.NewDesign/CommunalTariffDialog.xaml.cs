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
    /// Interaction logic for CommunalTariffDialog.xaml
    /// </summary>
    public partial class CommunalTariffDialog : Window
    {
        public CommunalTariffDialog()
        {
            InitializeComponent();
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            if(cbxType.SelectedItem is Communal.CommunalType)
                DialogResult = true;
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private void cbxType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cbxType.SelectedItem is Communal.CommunalType)
                lblMeasure.Content = " грн / " + Communal.MeasureTypes.GetName((cbxType.SelectedItem as Communal.CommunalType).Measure);
            else
                lblMeasure.Content = "";
        }
        public static bool Edit(Communal.Tariff tariff, Window owner) 
        {
#if COMMUNAL
            CommunalTariffDialog dlg = new CommunalTariffDialog();
            dlg.Owner = owner;
            dlg.SetData(tariff);
            if (dlg.ShowDialog().Value) 
            {
                dlg.GetData(tariff);
                db.Database.WriteTariff(tariff);
                db.Database.UpdateGlobals();
                return true;
            }
#endif
            return false;
        }

        private void GetData(Communal.Tariff tariff)
        {
            tariff.Type = cbxType.SelectedItem as Communal.CommunalType;
            tariff.Date = dateDate.SelectedDate.Value;
            tariff.Value = Convert.ToDouble(txtValue.Text);
        }

        private void SetData(Communal.Tariff tariff)
        {
            cbxType.ItemsSource = Communal.Globals.CommunalTypes.Values;
            if (tariff.Type != null)
                cbxType.SelectedItem = Communal.Globals.CommunalTypes[tariff.Type.Id];
            else
                cbxType.SelectedItem = null;
            dateDate.SelectedDate = tariff.Date;
            txtValue.Text = tariff.Value.ToString();
        }
    }
}
