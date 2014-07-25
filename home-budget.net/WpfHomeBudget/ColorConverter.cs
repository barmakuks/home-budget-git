using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Data;
using System.Windows.Media;
using System.Globalization;

namespace WpfHomeBudget
{
    [ValueConversion(typeof(Color), typeof(Brush))]
    public class MyColorConverter : IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return new SolidColorBrush(Kernel.ColorItem.UIntToColor((uint)value));
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return Colors.Red;
        }

    }
}
