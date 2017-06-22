#pragma once

#include <QDialog>

class QKeySequence;
class QLineEdit;
class QPushButton;
class QString;
class QTreeView;
class QWidget;

enum Mode{
    NORMAL,
    INGINEERING,
};

class Panel : public QDialog
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = Q_NULLPTR);

    Q_SLOT void slotSetMode(Mode);


private:
    QPushButton *createButton(QString, QString hint = QString(), QKeySequence shortcut = QKeySequence());

    Q_SLOT void slotMoveToRightBottom();
    Q_SLOT void slotButtonClicked();


private:
    QTreeView *m_pCalcucationTree;
    QLineEdit *m_pOutPanel;
    QWidget *m_pIngineeringPanel;
    Mode m_mode;
};
