#ifndef QFOCUSEDLINEEDIT_H
#define QFOCUSEDLINEEDIT_H

#include <QLineEdit>

class QFocusedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    QFocusedLineEdit(QWidget* parent = 0);
    ~QFocusedLineEdit();

signals:
    void focussed(bool hasFocus);

protected:
    void focusInEvent(QFocusEvent* e) override;
    void focusOutEvent(QFocusEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
private:
    bool m_just_has_focused;
};

#endif // QFOCUSEDLINEEDIT_H
