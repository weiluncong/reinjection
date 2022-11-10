#include "replay_window.h"
#include <QDebug>

ReplayWindow::ReplayWindow(QWidget *parent) : QMainWindow(parent)
{
    init_ui();
    qRegisterMetaType<QString>("QString");
    init_signal_slots();
    // std::thread list_widget_show(&ReplayWindow::update_list_widget, this);
    // list_widget_show.detach();
}

ReplayWindow::~ReplayWindow()
{
}

void ReplayWindow::init_ui()
{
    font.setFamily("微软雅黑");
    font.setPointSize(12);

    resize(QSize(1000, 800));
    setWindowTitle("ca_replay");
    //组件布局
    center = new QWidget(this);
    setCentralWidget(center);
    main_lay_out = new QHBoxLayout(center);

    splitter_h = new QSplitter(Qt::Horizontal);
    splitter_h->setGeometry(25, 25, 950, 750);

    list_gb_ = new QGroupBox(tr("List"), splitter_h);
    splitter_v = new QSplitter(Qt::Vertical, splitter_h);
    splitter_h->setSizes(list_hb);
    action_gb_ = new QGroupBox(tr("Action"), splitter_v);
    splitter_v->setSizes(list_vb);

    list_gb_->setFont(font);
    action_gb_->setFont(font);

    //listWidget布局
    QHBoxLayout *list_gb_hb_ = new QHBoxLayout(list_gb_);
    list_widget = new QListWidget(list_gb_);
    list_gb_hb_->addWidget(list_widget);

    //Action-groupbox布局
    QGridLayout *action_grid_ = new QGridLayout(action_gb_);

    bar_ = new QProgressBar(action_gb_);
    bar_->setMaximum(100);
    bar_->setFont(font);
    bar_->setMinimum(0);
    bar_->setMinimumWidth(300);
    bar_->setOrientation(Qt::Horizontal);
    bar_->setValue(0);
    bar_->setAlignment(Qt::AlignCenter | Qt::AlignRight);

    QString qss = "QProgressBar{"
                  "border: none;"
                  "background: rgb(54,54,54);"
                  "border-radius: 5px; "
                  "text-align:center;"
                  "color: rgb(229, 229, 229);}"
                  "QProgressBar::chunk {"
                  "border-radius: 4px; "
                  "background-color:rgb(58, 154, 255);}";
    bar_->setStyleSheet(qss);

    action_grid_->addWidget(bar_, 0, 0, 1, 2);
    send_btn_ = new QPushButton(tr("Send"), action_gb_);
    send_btn_->setFont(font);
    send_btn_->setMinimumHeight(50);
    send_btn_->setEnabled(false);
    action_grid_->addWidget(send_btn_, 1, 0, 1, 1);
    stop_btn_ = new QPushButton(tr("Stop"), action_gb_);
    stop_btn_->setFont(font);
    stop_btn_->setMinimumHeight(50);
    stop_btn_->setEnabled(false);
    action_grid_->addWidget(stop_btn_, 1, 1, 1, 1);

    eval_iacc_btn_ = new QPushButton(tr("IACC激活测试"), action_gb_);
    eval_iacc_btn_->setFont(font);
    eval_iacc_btn_->setMinimumHeight(50);
    eval_iacc_btn_->setEnabled(true);
    action_grid_->addWidget(eval_iacc_btn_, 2, 0, 1, 1);

    active_iacc_btn_ = new QPushButton(tr("IACC手动激活"), action_gb_);
    active_iacc_btn_->setFont(font);
    active_iacc_btn_->setMinimumHeight(50);
    active_iacc_btn_->setEnabled(false);
    action_grid_->addWidget(active_iacc_btn_, 2, 1, 1, 1);

    eval_alc_btn_ = new QPushButton(tr("ALC激活测试"), action_gb_);
    eval_alc_btn_->setFont(font);
    eval_alc_btn_->setMinimumHeight(50);
    eval_alc_btn_->setEnabled(true);
    action_grid_->addWidget(eval_alc_btn_, 3, 0);

    eval_nid_btn_ = new QPushButton(tr("NID激活测试"), action_gb_);
    eval_nid_btn_->setFont(font);
    eval_nid_btn_->setMinimumHeight(50);
    eval_nid_btn_->setEnabled(true);
    action_grid_->addWidget(eval_nid_btn_, 3, 1);

    action_grid_->setRowStretch(4, 1);

    //创建Action
    file = new QAction(tr("&File"), this);
    file->setShortcut(tr("Ctrl+P"));
    file->setStatusTip(tr("Select dat file path!!!"));
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(file);

    //全局布局添加splitter
    main_lay_out->addWidget(splitter_h);
    //设置全局布局
    // setLayout(main_lay_out);
}

void ReplayWindow::init_signal_slots()
{
    connect(file, &QAction::triggered, this, &ReplayWindow::select_files);
    connect(send_btn_, &QPushButton::clicked, this, &ReplayWindow::send_data);
    connect(stop_btn_, &QPushButton::clicked, this, &ReplayWindow::stop_send_data);

    connect(eval_iacc_btn_, &QPushButton::clicked, this, &ReplayWindow::eval_iacc_slot);
    connect(active_iacc_btn_, &QPushButton::clicked, this, &ReplayWindow::active_iacc_slot);
    connect(eval_alc_btn_, &QPushButton::clicked, this, &ReplayWindow::eval_alc_slot);
    connect(eval_nid_btn_, &QPushButton::clicked, this, &ReplayWindow::eval_nid_slot);
}

void ReplayWindow::select_files()
{
    fileList.clear();
    send_btn_->setEnabled(true);
    QString cur_path = QDir::currentPath();
    QString dlg_title = "选择多个dat文件";
    QString filter = "dat文件(*.dat);;所有文件(*.*)";
    fileList = QFileDialog::getOpenFileNames(this, dlg_title, cur_path, filter);
    list_widget->clear();
    list_widget->addItems(fileList);
}

void ReplayWindow::send_data()
{
    std::cout << "send_data function!!!";
    td_send = new std::thread(&ReplayWindow::send_data1, this, true);
    send_btn_->setEnabled(false);
    file->setEnabled(false);
    this_thread::sleep_for(std::chrono::milliseconds(100));
    stop_btn_->setEnabled(true);

    eval_iacc_btn_->setEnabled(false);
    eval_alc_btn_->setEnabled(false);
    eval_nid_btn_->setEnabled(false);
}

void ReplayWindow::send_data1(bool repeated)
{
    if (send_data_ == nullptr)
        send_data_ = new Send_Data();
    send_data_->stop_flag = false;
    int i = 0;
    while (1)
    {
        QStringList tmp;
        for (auto file_name : fileList)
        {
            QStringList tmp1;
            if (widget_list.count() > 200)
                widget_list.clear();
            list_widget->addItem(QString("current inject file name : ") + file_name);
            tmp1 << send_data_->Set_File_Name(file_name.toStdString());
            list_widget->addItems(tmp1);
            time_total = tmp1[3].split(":")[1].toDouble();
            list_widget->addItem(QString(" injecting !!!"));
            time_start = KTime().getTime();
            // std::thread t1(&ReplayWindow::update_list_widget, this);
            // t1.detach();
            active_iacc_btn_->setEnabled(true);
            list_widget->addItems(send_data_->Send());
            active_iacc_btn_->setEnabled(false);
            if (send_data_->stop_flag)
                return;
            i++;
            qDebug() << "i : " << i;
        }
        time_total = 0;
        list_widget->addItem(QString::number(i) + " files have been injected!!!");
        list_widget->addItem("-------------------------------");
        if (!repeated)
        {
            stop_send_data();
            break;
        }
    }
}

void ReplayWindow::stop_send_data()
{
    if (send_data_ != nullptr)
    {
        send_data_->stop_flag = true;
    }
    this_thread::sleep_for(std::chrono::milliseconds(500));
    send_btn_->setEnabled(true);
    stop_btn_->setEnabled(false);
    file->setEnabled(true);
    time_total = 0;
    widget_list << "stop !!!";

    if (reveive_parser_data_ != nullptr)
    {
        reveive_parser_data_->setStop();
        send_btn_->setEnabled(false);
    }

    if (pyManager_ != nullptr)
    {
        pyManager_->setStop();
        send_btn_->setEnabled(false);
        // pyManager_->deleteLater();
    }

    active_iacc_btn_->setEnabled(false);
    eval_iacc_btn_->setEnabled(true);
    eval_alc_btn_->setEnabled(true);
    eval_nid_btn_->setEnabled(true);
}

void ReplayWindow::update_list_widget()
{
    while (!send_data_->stop_flag)
    {
        // if (!widget_list.isEmpty())
        //     list_widget->addItems(widget_list);
        // widget_list.clear();
        // qDebug() << "(KTime().getTime() - time_start) * 100 / time_total : " << (KTime().getTime() - time_start) * 100 / time_total;
        // if (time_total != 0)
        //     bar_->setValue((KTime().getTime() - time_start) * 100 * 1000 / time_total);
        // qDebug() << ((KTime().getTime() - time_start) * 100 * 1000 / time_total);
        // if ((KTime().getTime() - time_start) * 100 / time_total >= 99)
        //     break;
        // if (widget_list.count() > 200)
        //     widget_list.clear();
        // this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void ReplayWindow::active_iacc_slot()
{
    if (send_data_ != nullptr)
    {
        send_data_->setIaccActive();
    }
}
void ReplayWindow::eval_iacc_slot()
{
    evaluation("iaccCase");
}
void ReplayWindow::eval_alc_slot()
{
    evaluation("alcCase");
}
void ReplayWindow::eval_nid_slot()
{
    evaluation("nidCase");
}

void ReplayWindow::closeEvent(QCloseEvent *event)
{
    if (send_data_ != nullptr)
        send_data_->stop_flag = true;
    this_thread::sleep_for(std::chrono::milliseconds(200));
    this->hide();
}

void ReplayWindow::evaluation(const std::string &caseName)
{
    if (reveive_parser_data_ == nullptr)
        reveive_parser_data_ = new ReveiveParserData();

    qDebug() << TOQSTR(caseName).remove("Case").toUpper() + "激活测试";

    LoadConfig loadConfig;
    auto caseConfig = loadConfig.loadCaseConfig("./testpy/config/" + caseName + ".yaml");
    reveive_parser_data_->setSignalFilter(caseConfig.signals_);

    pyManager_ = new pycaseManager();
    pyManager_->setConfigInfo(caseName, caseConfig);

    qRegisterMetaType<QMap<QString, double>>("QMap<QString, double>");
    qRegisterMetaType<std::map<std::string, std::string>>("std::map<std::string, std::string>");

    connect(reveive_parser_data_, &ReveiveParserData::dataUpdate, pyManager_, &pycaseManager::addData, Qt::ConnectionType::DirectConnection);
    connect(pyManager_, &pycaseManager::UpdateReport, this, &ReplayWindow::update_report);

    fileList.clear();
    fileList.append(caseConfig.dataFile.data());
    // list_widget->clear();
    list_widget->addItem("-------------------------------------------------");
    list_widget->addItem(TOQSTR(caseName).remove("Case").toUpper() + "激活测试");
    list_widget->addItems(fileList);

    td_send = new std::thread(&ReplayWindow::send_data1, this, false);
    td_eval = new std::thread(&pycaseManager::run, pyManager_, caseName, 180);
    td_recv = new std::thread(&ReveiveParserData::receiveData, reveive_parser_data_);

    send_btn_->setEnabled(false);
    file->setEnabled(false);
    this_thread::sleep_for(std::chrono::milliseconds(100));
    stop_btn_->setEnabled(true);

    eval_iacc_btn_->setEnabled(false);
    eval_alc_btn_->setEnabled(false);
    eval_nid_btn_->setEnabled(false);
}
void ReplayWindow::update_report(const std::map<std::string, std::string> &report)
{
    QStringList reportList;
    reportList.append(TOQSTR("result:" + report.at("result")));
    reportList.append(TOQSTR("reason:" + report.at("reason")));
    reportList.append(TOQSTR("dateTime:" + report.at("dateTime")));

    qDebug() << reportList;
    list_widget->addItems(reportList);

    stop_send_data();
}