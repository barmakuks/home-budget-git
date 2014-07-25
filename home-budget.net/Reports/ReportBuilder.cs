using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using Microsoft.Windows.Controls;
using System.Windows.Controls;
using System.Windows.Data;

namespace Reports
{
    public class ReportBuilder
    {
        Report _report = null;
        DataGrid _grid = null;
        public ReportBuilder(Report report, DataGrid grid, Style rightColumnStyle) 
        {
            Report =report;
            SetGrid(grid, rightColumnStyle);
        }

        public void SetGrid(DataGrid grid, Style rightColumnStyle)
        {
            _grid = grid;
            CreateColumns(rightColumnStyle);
        }
        public Report Report 
        {
            get{
                return _report;
            }
            set{
                _report = value;
            }
            
        }
        public void CreateColumns(Style rightColumnStyle) 
        {
            _grid.Columns.Clear();
            // Столбец наименования
            {
                DataGridTemplateColumn column = new DataGridTemplateColumn();
                column.CellTemplate = new DataTemplate();
                FrameworkElementFactory element = new FrameworkElementFactory(typeof(TextBlock));
                element.SetValue(TextBlock.TextProperty, new Binding("DocTypeName"));
                element.SetValue(TextBlock.MarginProperty, new Binding("Margin"));
                element.SetValue(TextBlock.VerticalAlignmentProperty, VerticalAlignment.Center);
                column.CellTemplate.VisualTree = element;
                column.IsReadOnly = true;
                _grid.Columns.Add(column);
            }
            {
                // Столбец Общей сумммы
                DataGridTemplateColumn column = new DataGridTemplateColumn();
                column.CellTemplate = new DataTemplate();
                column.CellStyle = rightColumnStyle;
                FrameworkElementFactory items_panel = new FrameworkElementFactory(typeof(ItemsControl));
                column.CellTemplate.VisualTree = items_panel;
                column.Header = "Всего";
                items_panel.SetValue(ItemsControl.ItemsSourceProperty, new Binding("TotalReport"));
                DataTemplate items_template = new DataTemplate();

                FrameworkElementFactory cur_item = new FrameworkElementFactory(typeof(TextBlock));
                cur_item.SetValue(TextBlock.TextProperty, new Binding("."));

                FrameworkElementFactory stack_item = new FrameworkElementFactory(typeof(StackPanel));
                stack_item.SetValue(StackPanel.OrientationProperty, Orientation.Horizontal);
                stack_item.SetValue(StackPanel.MarginProperty, new Thickness(4, 0, 4, 0));
                stack_item.SetValue(StackPanel.HorizontalAlignmentProperty, HorizontalAlignment.Right);

                items_template.VisualTree = stack_item;
                items_template.VisualTree.AppendChild(cur_item);

                items_panel.SetValue(ItemsControl.ItemTemplateProperty, items_template);
                column.IsReadOnly = true;
                _grid.Columns.Add(column);
            }

            for (int i = 0; i < _report.Columns.Count(); i++)
            {
                {
                    // Столбец сумммы
                    DataGridTemplateColumn column = new DataGridTemplateColumn();
                    column.CellStyle = rightColumnStyle;                    
                    column.CellTemplate = new DataTemplate();                    

                    FrameworkElementFactory items_panel = new FrameworkElementFactory(typeof(ItemsControl));
                    column.CellTemplate.VisualTree = items_panel;                    
                    column.Header = _report.Columns[i].Caption;
                    items_panel.SetValue(ItemsControl.ItemsSourceProperty, new Binding(String.Format("DayReports[{0}].Report", i)));                    
                    DataTemplate items_template = new DataTemplate();

                    FrameworkElementFactory cur_item = new FrameworkElementFactory(typeof(TextBlock));
                    cur_item.SetValue(TextBlock.TextProperty, new Binding(".MoneyString"));

                    FrameworkElementFactory stack_item = new FrameworkElementFactory(typeof(StackPanel));
                    stack_item.SetValue(StackPanel.OrientationProperty, Orientation.Horizontal);
                    stack_item.SetValue(StackPanel.MarginProperty, new Thickness(4, 0, 4, 0));
                    stack_item.SetValue(StackPanel.HorizontalAlignmentProperty, HorizontalAlignment.Right);

                    items_template.VisualTree = stack_item;
                    items_template.VisualTree.AppendChild(cur_item);

                    items_panel.SetValue(ItemsControl.ItemTemplateProperty, items_template);
                    column.IsReadOnly = true;
                    _grid.Columns.Add(column);
                    //report.Rows[0].DayReports[0].Report[0].Currency;
                }
            }
            
        }
        public void FillData() 
        {
            Expand(null);
        }
        public void Expand(Reports.ReportRow parent_row) 
        {
            int index = 0;

            if (parent_row == null)
            {
                _grid.Items.Clear();                
            }
            else 
            {
                index = _grid.Items.IndexOf(parent_row) + 1;
            }

            int parentId = (parent_row == null) ? 0 : parent_row.DocType.Id;
            foreach (Reports.ReportRow row in _report.Rows)
            {
                if (row.DocType.ParentId == parentId) 
                {
                    if (parent_row != null && parent_row.IsExpanded)
                    {
                        row.IsExpanded = true;
                        Expand(row);
                        _grid.Items.Remove(row);
                    }
                    else
                    {
                        if (parent_row != null)
                            row.Level = parent_row.Level + 1;
                        else
                            row.Level = 0;
                        _grid.Items.Insert(index++, row);
                    }
                        
                }                
            }
            if (parent_row != null) 
            {
                parent_row.IsExpanded = !parent_row.IsExpanded;                
            }
                

            _grid.UpdateLayout();
        }
    }
}
