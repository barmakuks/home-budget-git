using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WpfHomeBudget
{
    class DocumentEditor
    {
        public static int Create(Kernel.DocumentType.DocTypeSign sign)
        {
            Kernel.Document doc = Kernel.Document.CreateEmptyDocument(sign);
            return Edit(sign, doc);
        }
        public static int Edit(Kernel.Document document)
        {
            return Edit(document.DocType.Sign, document);
        }
        private static int Edit(Kernel.DocumentType.DocTypeSign sign, Kernel.Document document)
        {
            if (sign == Kernel.DocumentType.DocTypeSign.Movement)
            {
                DocumentMoveDialog dlg = new DocumentMoveDialog(sign);
                return dlg.EditDocument(document);
            }
            else 
            {
                DocumentInOutDialog dlg = new DocumentInOutDialog(sign);
                return dlg.EditDocument(document);
            }
        }
    }
}
