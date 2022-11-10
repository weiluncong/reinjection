// #include "receiveparserdata.h"
// #include "pycase_manager.h"
// #include "load_config.h"
#include "send_data.h"
#include "ktime.h"
#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QGroupBox>
#include <QPushButton>
#include <iostream>
#include <QRadioButton>
#include <QProgressBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileDialog>
#include "receiveparserdata.h"
#include "pycase_manager.h"
#include "load_config.h"

using namespace std;

class ReplayWindow : public QMainWindow
{
    // Q_OBJECT
private:
    /* data */
    void init_ui();
    void init_signal_slots();
    void update_list_widget();
    void send_data1(bool repeated);
    void evaluation(const std::string &caseName);

    double time_total = 0;
    double time_start = 0;

    std::thread *td_send;
    std::thread *td_recv;
    std::thread *td_eval;

    QFont font;
    QFont f;

    //布局变量
    QWidget *center;
    QHBoxLayout *main_lay_out;
    QSplitter *splitter_h;
    QSplitter *splitter_v;
    QGroupBox *list_gb_;
    QGroupBox *setting_gb_;
    QGroupBox *model_gb_;
    QGroupBox *action_gb_;
    QListWidget *list_widget;
    QList<int> list_hb = {650, 250};
    QList<int> list_vb = {20, 20, 150};
    //action_groupbox控件变量
    QPushButton *send_btn_;
    QPushButton *stop_btn_;
    QProgressBar *bar_;

    //评测按钮控件变量
    QPushButton *eval_iacc_btn_;
    QPushButton *active_iacc_btn_;
    QPushButton *eval_alc_btn_;
    QPushButton *eval_nid_btn_;

    //Action
    QAction *file;
    QMenu *fileMenu;

    //txt
    QStringList fileList;
    QStringList widget_list;

    Send_Data *send_data_ = nullptr;
    ReveiveParserData *reveive_parser_data_ = nullptr;
    pycaseManager *pyManager_ = nullptr;

private slots:
    void send_data();
    void select_files();
    void stop_send_data();

    void eval_iacc_slot();
    void active_iacc_slot();
    void eval_alc_slot();
    void eval_nid_slot();

    void update_report(const std::map<std::string, std::string> &report);

protected:
    void closeEvent(QCloseEvent *event);

public:
    explicit ReplayWindow(QWidget *parent);
    ~ReplayWindow();
};
