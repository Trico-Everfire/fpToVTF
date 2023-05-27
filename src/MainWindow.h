#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QComboBox>

class MainWindow : public QDialog
{
    Q_OBJECT
public:

    MainWindow();

    QPushButton* m_pFileButton;
    QTextEdit* m_pFileEdit;

    QSpinBox* m_pImageWidthSpinbox;
    QSpinBox* m_pImageHeightSpinbox;

    QComboBox* m_pImageRotateSpinbox;

    QComboBox *m_pImageTypeSpinbox;

    QPushButton *m_pSaveFileButton;

};
