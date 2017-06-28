#include "Panel.h"

#include "Model/CalculationTreeModel.h"
#include "Calculator/Parser.h"

#include <QtAlgorithms>
#include <QApplication>
#include <QDesktopWidget>
#include <QHeaderView>
#include <QKeySequence>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExp>
#include <QAbstractTableModel>
#include <QTreeView>
#include <QFileSystemModel>
#include <QWidget>
#include <algorithm>

Panel::Panel(QWidget *parent)
    : QDialog(parent), m_mode(NORMAL), m_calculatedPushed(false)
{
    m_pModel = new CalculationTreeModel(this);
    m_pParser = new Parser();

    m_pOutPanel = new QLineEdit("0");
    m_pOutPanel->setAlignment(Qt::AlignRight);
    m_pOutPanel->setReadOnly(true);
    m_pOutPanel->setMinimumSize(200, 40);

    auto pLayout = new QGridLayout;
    pLayout->addWidget(createButton(("↩"),    tr("Назад [Ctrl+Z]"), QKeySequence("Ctrl+Z")            ), 1, 0);
    pLayout->addWidget(createButton(("CE"),   tr("Очистить [Escape]"), QKeySequence("Escape")         ), 1, 1);
    pLayout->addWidget(createButton(("("),    tr("Открыть блок [(]"), QKeySequence("(")               ), 1, 2);
    pLayout->addWidget(createButton((")"),    tr("Закрыть блок [)]"), QKeySequence(")")               ), 1, 3);
    pLayout->addWidget(createButton(("√"),    tr("Квадратный корень [Ctrl+R]"), QKeySequence("Ctrl+R")), 1, 4);
    pLayout->addWidget(createButton(("7"),    tr(""), QKeySequence("7")                               ), 2, 0);
    pLayout->addWidget(createButton(("8"),    tr(""), QKeySequence("8")                               ), 2, 1);
    pLayout->addWidget(createButton(("9"),    tr(""), QKeySequence("9")                               ), 2, 2);
    pLayout->addWidget(createButton(("÷"),    tr("Разделить [/]"), QKeySequence("/")                  ), 2, 3);
    pLayout->addWidget(createButton(("%"),    tr("Процент [%]"), QKeySequence("%")                    ), 2, 4);
    pLayout->addWidget(createButton(("4"),    tr(""), QKeySequence("4")                               ), 3, 0);
    pLayout->addWidget(createButton(("5"),    tr(""), QKeySequence("5")                               ), 3, 1);
    pLayout->addWidget(createButton(("6"),    tr(""), QKeySequence("6")                               ), 3, 2);
    pLayout->addWidget(createButton(("×"),    tr("Умножить [*]"), QKeySequence("*")                   ), 3, 3);
    pLayout->addWidget(createButton(("1/x"),  tr("Разделить на x [Ctrl+F]"), QKeySequence("Ctrl+F")   ), 3, 4);
    pLayout->addWidget(createButton(("1"),    tr(""), QKeySequence("1")                               ), 4, 0);
    pLayout->addWidget(createButton(("2"),    tr(""), QKeySequence("2")                               ), 4, 1);
    pLayout->addWidget(createButton(("3"),    tr(""), QKeySequence("3")                               ), 4, 2);
    pLayout->addWidget(createButton(("-"),    tr("Вычесть [-]"), QKeySequence("-")                    ), 4, 3);
    pLayout->addWidget(createButton(("="),    tr("Вычислить результат"), QKeySequence("Return")       ), 4, 4, 2, 1);
    pLayout->addWidget(createButton(("0"),    tr(""), QKeySequence("0")                               ), 5, 0, 1, 2);
    pLayout->addWidget(createButton(("."),    tr(""), QKeySequence(".")                               ), 5, 2);
    pLayout->addWidget(createButton(("+"),    tr("Сложить [+]"), QKeySequence("+")                    ), 5, 3);

    m_pIngineeringPanel = new QWidget;
    m_pIngineeringPanel->hide();
    auto pIngLayout = new QGridLayout(m_pIngineeringPanel);
    pIngLayout->addWidget(createButton(("sin")), 0, 0);
    pIngLayout->addWidget(createButton(("cos")), 1, 0);
    pIngLayout->addWidget(createButton(("tg")), 2, 0);
    pIngLayout->addWidget(createButton(("ctg")), 3, 0);
    pIngLayout->addWidget(createButton(("asin")), 0, 1);
    pIngLayout->addWidget(createButton(("acos")), 1, 1);
    pIngLayout->addWidget(createButton(("atg")), 2, 1);
    pIngLayout->addWidget(createButton(("actg")), 3, 1);

    auto modeMenu = new QMenu(tr("&Режим"));
    auto actionNormal = new QAction(tr("Обычный"));
    auto actionEngineering = new QAction(tr("Инженерный"));
    connect(actionNormal, &QAction::triggered, [this] { slotSetMode(NORMAL); });
    connect(actionEngineering, &QAction::triggered, [this] { slotSetMode(INGINEERING); });
    modeMenu->addAction(actionNormal);
    modeMenu->addAction(actionEngineering);
    auto menuBar = new QMenuBar;
    menuBar->addMenu(modeMenu);

    auto inputAreaLayout = new QVBoxLayout;
    auto pButtonsLayout = new QHBoxLayout;
    pButtonsLayout->addLayout(pLayout);
    pButtonsLayout->addWidget(m_pIngineeringPanel);
    inputAreaLayout->addWidget(menuBar);
    inputAreaLayout->addWidget(m_pOutPanel);
    inputAreaLayout->addLayout(pButtonsLayout);

    m_pCalcucationTree = new QTreeView;
    m_pCalcucationTree->setModel(m_pModel);
    m_pCalcucationTree->header()->setVisible(false);

    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_pCalcucationTree);
    mainLayout->addLayout(inputAreaLayout);

    setLayout(mainLayout);

    layout()->setSizeConstraint(QLayout::SetFixedSize);
    slotMoveToTopLeft();
    setWindowIcon(QIcon(":/images/calculator.png"));
    setWindowTitle(tr("Cartesius"));
}

void Panel::slotSetMode(Mode mode)
{
    m_mode = mode;
    switch (mode) {
    case NORMAL:
        m_pIngineeringPanel->hide();
        break;
    case INGINEERING:
        m_pIngineeringPanel->show();
        break;
    }
    slotMoveToTopLeft();
}

QPushButton *Panel::createButton(QString head, QString hint, QKeySequence shortcut)
{
    auto button = new QPushButton(head);
    button->setAutoDefault(false);
    if(head != "0")
        button->setMaximumSize(40, 100);
    button->setToolTip(hint);
    button->setShortcut(shortcut);
    connect(button, SIGNAL( clicked(bool) ), SLOT( slotButtonClicked() ) );
    return button;
}

int Panel::findLastFuncPos()
{
    auto text = m_pOutPanel->text();
    auto num_brace = 0;
    auto i = m_pOutPanel->text().size() - 1;
    if(text[i] == ")")
        num_brace++;
    for(i--; i >= 0 && num_brace > 0; --i){
        if(text[i] == ")")
            num_brace++;
        if(text[i] == "(")
            num_brace--;
    }
    while(QRegExp("a-z").exactMatch(text.at(i--)));
    return i - 1;
}

int Panel::findLastNumberPos()
{
    int i = m_pOutPanel->text().size() - 1;
    for(; i >= 0; --i){
        if(!QRegExp("[0-9.]").exactMatch(m_pOutPanel->text().at(i))){
            if(i == m_pOutPanel->text().size() - 1 && i)
                return findLastFuncPos();
            return i + 1;
        }
        else if(i == 0)
            return i;
    }
    return -1;
}

void Panel::slotButtonClicked()
{
    auto text = static_cast<QPushButton*>(sender())->text();
    if(m_calculatedPushed && text != "="){
        m_pOutPanel->setText("0");
        m_calculatedPushed = false;
    }
    if(text == "CE"){
        m_pOutPanel->setText("0");
        m_pModel->clear();
    }
    else if(text == "↩"){
        if(m_pOutPanel->text().size() == 1){
            m_pOutPanel->setText("0");
        }
        else
            m_pOutPanel->setText(m_pOutPanel->text().remove(m_pOutPanel->text().size() - 1, 1));
    }
    else if(text == "1/x"){
        m_pOutPanel->setText(m_pOutPanel->text().insert(findLastNumberPos(), "1÷"));

    }
    else if(text == "sin" || text == "cos" || text == "tg" || text == "ctg" ||
            text == "asin" || text == "acos" || text == "atg" || text == "actg"){
        m_pOutPanel->setText(m_pOutPanel->text().insert(findLastNumberPos(), text + "("));
        m_pOutPanel->setText(m_pOutPanel->text().insert(m_pOutPanel->text().size(), ")"));
    }
    else if(text == "="){
        try{
            auto calcText = m_pOutPanel->text();
            m_pOutPanel->setText(QString::number(m_pParser->calcucateExpression(calcText)));
            m_pModel->setupModelData(calcText);
            m_calculatedPushed = true;
        }
        catch(SyntaxError){
            QMessageBox::critical(this,
                                  QStringLiteral("Error"),
                                  QStringLiteral("Wrong expression!"),
                                  QMessageBox::Ok);
        }
        catch(...){
            QMessageBox::critical(this,
                                  QStringLiteral("Error"),
                                  QStringLiteral("Unknown error!"),
                                  QMessageBox::Ok);
        }
    }
    else if(QRegExp("[0-9√]").exactMatch(text)){
        m_pOutPanel->text().size() == 1 && m_pOutPanel->text() == "0"
                ? m_pOutPanel->setText(text)
                : m_pOutPanel->setText(m_pOutPanel->text().append(text));
    }
    else if(QRegExp("[()]").exactMatch(text)){
        m_pOutPanel->text().size() == 1 && m_pOutPanel->text() == "0" ?
                    m_pOutPanel->setText(text) :
                    m_pOutPanel->setText(m_pOutPanel->text().append(text));
    }
    else if(QRegExp("[+-×÷.%]").exactMatch(text)){
        m_pOutPanel->setText(m_pOutPanel->text().append(text));
    }
    m_pCalcucationTree->setFocus();
}

void Panel::slotMoveToTopLeft()
{
    move(0, 0);
}

