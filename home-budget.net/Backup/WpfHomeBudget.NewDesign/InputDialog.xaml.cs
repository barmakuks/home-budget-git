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
    public partial class InputDialog : Window
    {
        public InputDialog()
        {
            InitializeComponent();
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        public static bool Show(string caption, string text, ref string value) 
        {
            InputDialog dlg = new InputDialog();
            dlg.Title = caption;
            dlg.wincapcaption.lblCaption.Content = text;
            dlg.lblText.Content = text;
            dlg.txtValue.Text = value;
            dlg.txtValue.Focus();
            dlg.txtValue.SelectAll();
            if (dlg.ShowDialog().Value)
            {
                value = dlg.txtValue.Text;
                return true;
            }
            return false;            
        }
    }
}
