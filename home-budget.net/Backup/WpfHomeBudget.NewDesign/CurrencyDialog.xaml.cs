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
    /// Interaction logic for SimpleDialog.xaml
    /// </summary>
    public partial class CurrencyDialog : Window
    {
        public CurrencyDialog()
        {
            InitializeComponent();
        }
        public static bool Edit(Kernel.Currency currency) 
        {
            CurrencyDialog dlg = new CurrencyDialog();
            dlg.SetData(currency);
            if (dlg.ShowDialog().Value) 
            {
                dlg.GetData(ref currency);
                return true;
            }
            return false;
        }

        private void GetData(ref Kernel.Currency currency)
        {
            currency.Code = Convert.ToInt32(txtCode.Text);
            currency.ISO = txtISO.Text;
            currency.ShortName = txtShortName.Text;
            currency.Symbol = txtSymbol.Text;
            currency.SymbolPlace = cbxSymbolPlace.IsChecked.Value ? Kernel.Currency.Place.Before : Kernel.Currency.Place.After;
        }

        private void SetData(Kernel.Currency currency)
        {
            txtCode.Text = currency.Code.ToString();
            txtISO.Text = currency.ISO;
            txtShortName.Text = currency.ShortName;
            txtSymbol.Text = currency.Symbol;
            cbxSymbolPlace.IsChecked = (currency.SymbolPlace == Kernel.Currency.Place.Before);
        }


        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            int res = 0;
            if(Int32.TryParse(txtCode.Text, out res))
            {
                if (txtISO.Text != String.Empty && txtShortName.Text != String.Empty)
                {
                    DialogResult = true;
                    Close();
                }                    
            }            
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

    }
}
