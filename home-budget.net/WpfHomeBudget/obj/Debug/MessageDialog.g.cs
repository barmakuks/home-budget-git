﻿#pragma checksum "..\..\MessageDialog.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "0C0D618EE1A897513FB594CF85223EFB"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.3607
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using Microsoft.Windows.Controls;
using Microsoft.Windows.Controls.Primitives;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace WpfHomeBudget {
    
    
    /// <summary>
    /// MessageDialog
    /// </summary>
    public partial class MessageDialog : System.Windows.Window, System.Windows.Markup.IComponentConnector {
        
        
        #line 20 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.DockPanel pnlCaption;
        
        #line default
        #line hidden
        
        
        #line 22 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.Label lblCaption;
        
        #line default
        #line hidden
        
        
        #line 30 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.DockPanel pnlLeft;
        
        #line default
        #line hidden
        
        
        #line 33 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.Button btnOk;
        
        #line default
        #line hidden
        
        
        #line 36 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.Button btnCancel;
        
        #line default
        #line hidden
        
        
        #line 39 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.Button btnYes;
        
        #line default
        #line hidden
        
        
        #line 42 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.Button btnNo;
        
        #line default
        #line hidden
        
        
        #line 49 "..\..\MessageDialog.xaml"
        internal System.Windows.Controls.Label lblText;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/WpfHomeBudget;component/messagedialog.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\MessageDialog.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.pnlCaption = ((System.Windows.Controls.DockPanel)(target));
            return;
            case 2:
            
            #line 21 "..\..\MessageDialog.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.btnCancel_Click);
            
            #line default
            #line hidden
            return;
            case 3:
            this.lblCaption = ((System.Windows.Controls.Label)(target));
            
            #line 22 "..\..\MessageDialog.xaml"
            this.lblCaption.PreviewMouseDown += new System.Windows.Input.MouseButtonEventHandler(this.lblCaption_PreviewMouseDown);
            
            #line default
            #line hidden
            
            #line 22 "..\..\MessageDialog.xaml"
            this.lblCaption.PreviewMouseMove += new System.Windows.Input.MouseEventHandler(this.lblCaption_PreviewMouseMove);
            
            #line default
            #line hidden
            
            #line 22 "..\..\MessageDialog.xaml"
            this.lblCaption.PreviewMouseUp += new System.Windows.Input.MouseButtonEventHandler(this.lblCaption_PreviewMouseUp);
            
            #line default
            #line hidden
            return;
            case 4:
            this.pnlLeft = ((System.Windows.Controls.DockPanel)(target));
            return;
            case 5:
            this.btnOk = ((System.Windows.Controls.Button)(target));
            
            #line 33 "..\..\MessageDialog.xaml"
            this.btnOk.Click += new System.Windows.RoutedEventHandler(this.btnOk_Click);
            
            #line default
            #line hidden
            return;
            case 6:
            this.btnCancel = ((System.Windows.Controls.Button)(target));
            
            #line 36 "..\..\MessageDialog.xaml"
            this.btnCancel.Click += new System.Windows.RoutedEventHandler(this.btnCancel_Click);
            
            #line default
            #line hidden
            return;
            case 7:
            this.btnYes = ((System.Windows.Controls.Button)(target));
            
            #line 39 "..\..\MessageDialog.xaml"
            this.btnYes.Click += new System.Windows.RoutedEventHandler(this.btnYes_Click);
            
            #line default
            #line hidden
            return;
            case 8:
            this.btnNo = ((System.Windows.Controls.Button)(target));
            
            #line 42 "..\..\MessageDialog.xaml"
            this.btnNo.Click += new System.Windows.RoutedEventHandler(this.btnNo_Click);
            
            #line default
            #line hidden
            return;
            case 9:
            this.lblText = ((System.Windows.Controls.Label)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}
