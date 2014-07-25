using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace WpfHomeBudget
{
    class DocumentEditor
    {
        public static int Create(Kernel.DocumentType.DocTypeSign sign, Window windowOwner)
        {
            Kernel.Document doc = Kernel.Document.CreateEmptyDocument(sign);
            return Edit(sign, doc, windowOwner);
        }
        public static int Edit(Kernel.Document document, Window windowOwner)
        {
            return Edit(document.DocType.Sign, document, windowOwner);
        }
        private static int Edit(Kernel.DocumentType.DocTypeSign sign, Kernel.Document document, Window windowOwner)
        {
            if (sign == Kernel.DocumentType.DocTypeSign.Movement)
            {
                DocumentMoveDialog dlg = new DocumentMoveDialog(sign);
                return dlg.EditDocument(document);
            }
            else 
            {
                DocumentInOutDialog dlg = new DocumentInOutDialog(sign);
                return dlg.EditDocument(document, windowOwner);
            }
        }
    }
}
