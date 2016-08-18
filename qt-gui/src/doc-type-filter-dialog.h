#pragma once

#include <QDialog>
#include <QAbstractButton>

namespace Ui
{
class DocTypeFilterDialog;
}

class DocTypeFilterModel;

class DocTypeFilterDialog : public QDialog
{
    Q_OBJECT

public:
    static bool ShowFilter(DocTypeFilterModel* model);

public:

    explicit DocTypeFilterDialog(QWidget* parent = 0);
    ~DocTypeFilterDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DocTypeFilterDialog* ui;
    DocTypeFilterModel*      m_model;
};
