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
using System.Reflection;

namespace WpfHomeBudget
{
    /// <summary>
    /// Interaction logic for AccountDialog.xaml
    /// </summary>
    public partial class AccountDialog : Window
    {
        public AccountDialog()
        {
            InitializeComponent();
        }
        #region Move window routine
        Vector div = new Vector();
        private void lblCaption_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            div = Point.Subtract(new Point(this.Left, this.Top), this.PointToScreen(Mouse.GetPosition(this)));
            Mouse.Capture(lblCaption);
            //div.Negate();
        }
        private void lblCaption_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (div.X != 0 && div.Y != 0)
            {
                Point loc = Point.Add(this.PointToScreen(Mouse.GetPosition(this)), div);
                this.Left = loc.X;
                this.Top = loc.Y;
            }
        }
        private void lblCaption_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            div.X = 0;
            div.Y = 0;
            Mouse.Capture(null);
        }
        #endregion

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        public static bool Edit(Kernel.Account account)
        {
            AccountDialog dlg = new AccountDialog();
            dlg.SetData(account);
            if (dlg.ShowDialog().Value)
            {
                dlg.GetData(ref account);
                return true;
            }
            return false;
        }

        private void GetData(ref Kernel.Account account)
        {
            account.Name = txtName.Text;
            account.Description = txtDescription.Text;
            account.IsActive = cbxIsActive.IsChecked.Value;
            account.Bank = txtBank.Text;            
            account.DefaultCurrency = (cbxDefaultCurrency.SelectedItem as Kernel.Currency).Code;
            account.ForegroundColor = (cbxColor.SelectedItem as Kernel.ColorItem).Value;
        }

        private void SetData(Kernel.Account account)
        {
            txtName.Text = account.Name;
            txtDescription.Text = account.Description;
            cbxIsActive.IsChecked = account.IsActive;
            txtBank.Text = account.Bank;

            cbxColor.ItemsSource = Kernel.Globals.Colors;
            cbxColor.SelectedItem = Kernel.Globals.GetColor(account.ForegroundColor);

            foreach (string bank in db.Database.GetBanksList()) 
            {
                txtBank.Items.Add(bank);
            }            
            cbxDefaultCurrency.ItemsSource = Kernel.Globals.Currency.Values;
            if (Kernel.Globals.Currency.ContainsKey(account.DefaultCurrency))
                cbxDefaultCurrency.SelectedItem = Kernel.Globals.Currency[account.DefaultCurrency];
            else
                cbxDefaultCurrency.SelectedIndex = 0;
        }


    }
}
