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
    /// Interaction logic for WindowCaption.xaml
    /// </summary>
    public partial class WindowCaption : UserControl
    {
        Window _wind = null;
        public WindowCaption()
        {
            InitializeComponent();
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            if(_wind != null)
                _wind.Close();
        }

        private void btnMaximize_Click(object sender, RoutedEventArgs e)
        {
            if (_wind != null && btnMaximize.Visibility == Visibility.Visible) 
            {
                if(_wind.WindowState == WindowState.Maximized)
                {
                    _wind.WindowState = WindowState.Normal;
                }
                else
                {
                    _wind.WindowState = WindowState.Maximized;
                }
                
            }
                
        }

        private void btnMinimize_Click(object sender, RoutedEventArgs e)
        {
            if (_wind != null)
                _wind.WindowState = WindowState.Minimized;
        }

        Vector _div = new Vector();
        private void lblCaption_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (_div.X != 0 && _div.Y != 0)
            {
                Point loc = Point.Add(_wind.PointToScreen(Mouse.GetPosition(_wind)), _div);
                _wind.Left = loc.X;
                _wind.Top = loc.Y;
            }
        }

        private void lblCaption_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            _div.X = 0;
            _div.Y = 0;
            Mouse.Capture(null);
        }

        private void lblCaption_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if(e.ClickCount >= 2)
                btnMaximize_Click(sender, RoutedEventArgs.Empty as RoutedEventArgs);
        }

        private void lblCaption_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {            
            if (_wind != null) 
            {
                _div = Point.Subtract(new Point(_wind.Left, _wind.Top), this.PointToScreen(Mouse.GetPosition(this)));
                Mouse.Capture(lblCaption);
            }

        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            FrameworkElement parent = Parent as FrameworkElement;
            while (parent is FrameworkElement && !(parent is Window))
            {
                parent = parent.Parent as FrameworkElement;
            }
            if (parent != null) 
            {
                _wind = parent as Window;
                lblCaption.Content = _wind.Title;
                switch (_wind.ResizeMode)
                {
                    case ResizeMode.CanMinimize:
                        btnMaximize.Visibility = Visibility.Collapsed;
                        break;
                    case ResizeMode.CanResize:
                        break;
                    case ResizeMode.CanResizeWithGrip:
                        break;
                    case ResizeMode.NoResize:
                        btnMinimize.Visibility = Visibility.Collapsed;
                        btnMaximize.Visibility = Visibility.Collapsed;
                        break;
                }
            }
                
        }
    }
}
