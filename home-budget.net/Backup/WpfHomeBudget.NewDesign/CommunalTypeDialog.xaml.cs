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
    /// Interaction logic for CommunalTypeDialog.xaml
    /// </summary>
    public partial class CommunalTypeDialog : Window
    {
        public CommunalTypeDialog()
        {
            InitializeComponent();
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            if (!String.IsNullOrEmpty(txtName.Text.Trim())) 
            {
                if (!string.IsNullOrEmpty(txtDefault.Text.Trim())) 
                {
                    double value;
                    if (!Double.TryParse(txtDefault.Text.Trim(), out value))
                        return;
                }
                DialogResult = true;
                Close();
            }            
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }
        public static bool Edit(Communal.CommunalType ctype, Window owner) 
        {
            CommunalTypeDialog dlg = new CommunalTypeDialog();
            dlg.Owner = owner;
            dlg.SetData(ctype);
            if (dlg.ShowDialog().Value) 
            {
                dlg.GetData(ctype);
                return true;
            }
            return false;
        }

        private void GetData(Communal.CommunalType ctype)
        {
            string measure = cbxMeasure.SelectedItem.ToString();
            ctype.Measure = Communal.MeasureTypes.GetType(measure);
            ctype.Name = txtName.Text;
            if(String.IsNullOrEmpty(txtDefault.Text.Trim()))
                ctype.DefaultValue = 0.0;
            else
                ctype.DefaultValue = Convert.ToDouble(txtDefault.Text.Trim());
        }

        private void SetData(Communal.CommunalType ctype)
        {
            cbxMeasure.ItemsSource = Communal.MeasureTypes.Names;
            cbxMeasure.SelectedItem = Communal.MeasureTypes.GetName(ctype.Measure);
            txtName.Text = ctype.Name;
            txtDefault.Text = (ctype.DefaultValue == 0.0) ? "" : ctype.DefaultValue.ToString();
        }
    }
}
