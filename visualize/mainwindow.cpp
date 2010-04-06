#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <global.h>
#include <analyzer/analyze/analyzer.h>
#include <analyzer/filter/numberfilter.h>
#include <analyzer/filter/stringfilter.h>
#include <analyzer/adapter/wlanadapter.h>
#include <analyzer/adapter/hddstore.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    workingFile("")
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_action_2_triggered()
{
    // open file

   /* QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "");
    if (!fileName.isEmpty()) {
        workingFile = fileName;
    } else return;
*/
    QString fileName = "/home/vden/work/PacketParser/bin/tests/t2.cap";
    workingFile = fileName;

    Analyzer* a = new Analyzer("/home/vden/work/PacketParser/bin/");


    StringFilter *f;
    PacketStore* ps;

    f = new StringFilter("wlan.mgmt.ssid = 123321");
    a->addFilter(dynamic_cast<Filter*>(f));

    f = new StringFilter("wlan.mode = b");
//	a->addFilter(dynamic_cast<Filter*>(f));

    ps = new WLANAdapter();
    a->addStore(dynamic_cast<PacketStore*>(ps));

    a->addSyncStore(dynamic_cast<PacketStore*>(ps));

    PacketInfo pi;
    pi["test1"] = "8123";
    pi["test2"] = "5";

    HDDStore *hs = new HDDStore("/tmp/hstore", 1);
    a->addStore(dynamic_cast<PacketStore*>(hs));


    char* buf = 0;
    fstream fs(fileName.toStdString().c_str());
    int len = 0;
    u_quad_t offset = sizeof(CAPHeader);

    while ( (len = readNextCAPBlockHeader(fs, offset)) > 0) {
        cout << "------- frame --------------" << endl;

        buf = new char[len];
        fs.read(buf, len);

        cout << "READ FROM cap: " << len << endl;

        offset += len + sizeof(CAPBlockHeader);
        a->nextPacket(buf, len);

        cout << "----------------------------" << endl;
        delete buf;
    }
}

void MainWindow::on_action_triggered()
{
    close();
}
