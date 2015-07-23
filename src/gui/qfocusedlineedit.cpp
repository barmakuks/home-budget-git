#include "qfocusedlineedit.h"

QFocusedLineEdit::QFocusedLineEdit(QWidget* parent):
    QLineEdit(parent),
    m_just_has_focused(false)
{

}

QFocusedLineEdit::~QFocusedLineEdit()
{

}

void QFocusedLineEdit::focusInEvent(QFocusEvent* e)
{
    QLineEdit::focusInEvent(e);
    emit(focussed(true));
    m_just_has_focused = true;
    selectAll();
}

void QFocusedLineEdit::focusOutEvent(QFocusEvent* e)
{
    QLineEdit::focusOutEvent(e);
    emit(focussed(false));
    m_just_has_focused = false;
}

void QFocusedLineEdit::mousePressEvent(QMouseEvent* e)
{
    if (!m_just_has_focused)
    {
        QLineEdit::mousePressEvent(e);
    }

    m_just_has_focused = false;
}

