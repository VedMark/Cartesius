#include "Panel.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeySequence>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QRegExp>
#include <QAbstractTableModel>
#include <QTreeView>
#include <QFileSystemModel>
#include <QWidget>

Panel::Panel(QWidget *parent)
    : QDialog(parent), m_mode(NORMAL)
{
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
    pLayout->addWidget(createButton(("0"),    tr(""), QKeySequence("0")                               ), 5, 0);
    pLayout->addWidget(createButton(("."),    tr(""), QKeySequence(".")                               ), 5, 1);
    pLayout->addWidget(createButton(QChar(0xB1), tr("Изменить знак [Ctrl+P]"), QKeySequence("Ctrl+P") ), 5, 2);
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
    auto fileSys = new QFileSystemModel();
    fileSys->setRootPath("/home/vedmark");

    m_pCalcucationTree->setModel(fileSys);

    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_pCalcucationTree);
    mainLayout->addLayout(inputAreaLayout);

    setLayout(mainLayout);

    layout()->setSizeConstraint(QLayout::SetFixedSize);
    slotMoveToRightBottom();
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
    m_pOutPanel->setText("0");
    slotMoveToRightBottom();
}

QPushButton *Panel::createButton(QString head, QString hint, QKeySequence shortcut)
{
    auto button = new QPushButton(head);
    button->setAutoDefault(false);
    button->setMaximumSize(40, 100);
    button->setToolTip(hint);
    button->setShortcut(shortcut);
    connect(button, SIGNAL( clicked(bool) ), SLOT( slotButtonClicked() ) );
    return button;
}

void Panel::slotButtonClicked()
{
    auto text = static_cast<QPushButton*>(sender())->text();
    if(text == "CE"){
        m_pOutPanel->setText("0");
    }
    else if(text == "↩"){

    }
    else if(text.contains(QRegExp("[0-9]")) || text == "."){

    }
}

void Panel::slotMoveToRightBottom()
{
    move(0, 0);
}
