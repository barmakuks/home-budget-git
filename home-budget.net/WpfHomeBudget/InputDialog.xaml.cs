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

        public static bool Show(string caption, string text, ref string value) 
        {
            InputDialog dlg = new InputDialog();
            dlg.lblCaption.Content = caption;
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
