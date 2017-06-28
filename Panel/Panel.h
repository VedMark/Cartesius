#pragma once

#include <QDialog>

class QKeySequence;
class QLineEdit;
class QPushButton;
class QString;
class QTreeView;
class QWidget;

class CalculationTreeModel;
class Parser;

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
    int findLastFuncPos();
    int findLastNumberPos();

    Q_SLOT void slotButtonClicked();
    Q_SLOT void slotMoveToTopLeft();


private:
    QTreeView *m_pCalcucationTree;
    QLineEdit *m_pOutPanel;
    QWidget *m_pIngineeringPanel;
    Mode m_mode;
    bool m_calculatedPushed;

    CalculationTreeModel *m_pModel;
    Parser *m_pParser;
};
