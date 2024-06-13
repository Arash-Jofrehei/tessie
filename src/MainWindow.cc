#include "MainWindow.hh"

#include <sstream>
#include <string.h>
#include <stdio.h>
#include <fstream>

#include <QtWidgets/QLineEdit>
#include <QtWidgets>

#include <qpushbutton.h>
#include <unistd.h>

#include "tLog.hh"
#include "sha256.hh"
#include "util.hh"

using namespace std;

// -------------------------------------------------------------------------------
MainWindow::MainWindow(tLog &x, driveHardware *h, QWidget *parent) :
  QMainWindow(parent), fLOG(x), fpHw(h) {

  fFont1.setFamilies({QString::fromUtf8("Menlo")});
  fFont1.setPointSize(16);

  QWidget *wdg = new QWidget(this);
  wdg->setFocusPolicy(Qt::NoFocus);
  // -- Top vertical stack of layouts
  QVBoxLayout *vlayTop = new QVBoxLayout(wdg);

  // -- top row, containing (left) info block and (right) env block
  QHBoxLayout *hlay0 = new QHBoxLayout(wdg);
  vlayTop->addLayout(hlay0);

  // -- Info block
  QGridLayout *glay00 = new QGridLayout();
  wdg->setLayout(glay00);

  QLabel *lblA = new QLabel("CANbus errors"); setupLBL(lblA);
  QLabel *lblB = new QLabel("I2C errors"); setupLBL(lblB);
  QLabel *lblC = new QLabel("Runtime"); setupLBL(lblC);
  QLabel *lblD = new QLabel("Version"); setupLBL(lblD);

  fqleCANbusErrors = new QLineEdit(wdg); setupQLE(fqleCANbusErrors);
  fqleI2CErrors    = new QLineEdit(wdg); setupQLE(fqleI2CErrors);
  fqleRunTime      = new QLineEdit(wdg); setupQLE(fqleRunTime);
  fqleVersion      = new QLineEdit(wdg); setupQLE(fqleVersion);
  ifstream INS;
  string sline;
  INS.open("version.txt");
  getline(INS, sline);
  fqleVersion->setText(sline.c_str());
  INS.close();

  glay00->addWidget(lblA,  0, 0, 1, 1);
  glay00->addWidget(fqleCANbusErrors, 0, 1, 1, 1);
  glay00->addWidget(lblB,  1, 0, 1, 1);
  glay00->addWidget(fqleI2CErrors, 1, 1, 1, 1);
  glay00->addWidget(lblC,  2, 0, 1, 1);
  glay00->addWidget(fqleRunTime, 2, 1, 1, 1);
  glay00->addWidget(lblD,  3, 0, 1, 1);
  glay00->addWidget(fqleVersion, 3, 1, 1, 1);

  hlay0->addLayout(glay00);

  // -- Environmental block
  QGridLayout *glay01 = new QGridLayout();
  wdg->setLayout(glay01);

  QLabel *lblA1 = new QLabel("Air Temp."); setupLBL(lblA1);
  QLabel *lblB1 = new QLabel("Water Temp."); setupLBL(lblB1);
  QLabel *lblC1 = new QLabel("Rel. Hum.");  setupLBL(lblC1);
  QLabel *lblD1 = new QLabel("Dew Point");  setupLBL(lblD1);

  fqleAT = new QLineEdit(wdg); setupQLE(fqleAT);
  fqleWT = new QLineEdit(wdg); setupQLE(fqleWT);
  fqleRH = new QLineEdit(wdg); setupQLE(fqleRH);
  fqleDP = new QLineEdit(wdg); setupQLE(fqleDP);

  glay01->addWidget(lblA1,  0, 0, 1, 1);
  glay01->addWidget(fqleAT, 0, 1, 1, 1);
  glay01->addWidget(lblB1,  1, 0, 1, 1);
  glay01->addWidget(fqleWT, 1, 1, 1, 1);
  glay01->addWidget(lblC1,  0, 2, 1, 1);
  glay01->addWidget(fqleRH, 0, 3, 1, 1);
  glay01->addWidget(lblD1,  1, 2, 1, 1);
  glay01->addWidget(fqleDP, 1, 3, 1, 1);

  hlay0->addLayout(glay01);


  // -- bottom row, containing (left) buttons and (right) TEC array
  QHBoxLayout *hlay1 = new QHBoxLayout(wdg);
  vlayTop->addLayout(hlay1);

  // -- buttons
  const QSize btnSize = QSize(100, 50);
  QVBoxLayout *vlay00 = new QVBoxLayout(wdg);

  fbtnValve0 = new QPushButton("Flush");
  fbtnValve0->setFocusPolicy(Qt::NoFocus);
  fbtnValve0->setFont(fFont1);
  fbtnValve0->setFixedSize(btnSize);
  fbtnValve0->setStyleSheet("QPushButton {background-color: gray; color: black;}");
  connect(fbtnValve0, &QPushButton::clicked, this, &MainWindow::btnValve0);
  vlay00->addWidget(fbtnValve0);

  fbtnValve1 = new QPushButton("Rinse");
  fbtnValve1->setFocusPolicy(Qt::NoFocus);
  fbtnValve1->setFont(fFont1);
  fbtnValve1->setFixedSize(btnSize);
  fbtnValve1->setStyleSheet("QPushButton {background-color: gray; color: black;}");
  connect(fbtnValve1, &QPushButton::clicked, this, &MainWindow::btnValve1);
  vlay00->addWidget(fbtnValve1);

  QPushButton *btn3 = new QPushButton("Quit"); btn3->setFocusPolicy(Qt::NoFocus);
  btn3->setFont(fFont1);
  btn3->setFixedSize(btnSize);
  connect(btn3, &QPushButton::clicked, this, &MainWindow::btnQuit);
  vlay00->addWidget(btn3);

  hlay1->addLayout(vlay00);


  // -- TEC block
  QGridLayout *glay02 = new QGridLayout();
  wdg->setLayout(glay02);

  QLabel *lbl1 = new QLabel("1"); setupLBL(lbl1);  lbl1->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  QLabel *lbl2 = new QLabel("2"); setupLBL(lbl2);  lbl2->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  QLabel *lbl3 = new QLabel("3"); setupLBL(lbl3);  lbl3->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  QLabel *lbl4 = new QLabel("4"); setupLBL(lbl4);  lbl4->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  QLabel *lbl5 = new QLabel("5"); setupLBL(lbl5);  lbl5->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  QLabel *lbl6 = new QLabel("6"); setupLBL(lbl6);  lbl6->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  QLabel *lbl7 = new QLabel("7"); setupLBL(lbl7);  lbl7->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  QLabel *lbl8 = new QLabel("8"); setupLBL(lbl8);  lbl8->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

  glay02->addWidget(lbl1,  0, 0, 1, 1);
  glay02->addWidget(lbl2,  0, 1, 1, 1);
  glay02->addWidget(lbl3,  0, 2, 1, 1);
  glay02->addWidget(lbl4,  0, 3, 1, 1);

  glay02->addWidget(lbl5,  1, 0, 1, 1);
  glay02->addWidget(lbl6,  1, 1, 1, 1);
  glay02->addWidget(lbl7,  1, 2, 1, 1);
  glay02->addWidget(lbl8,  1, 3, 1, 1);

  hlay1->addLayout(glay02);

  setCentralWidget(wdg);

  fqleCANbusErrors->clearFocus();

}


// ----------------------------------------------------------------------
MainWindow::~MainWindow() {
}

// ----------------------------------------------------------------------
void MainWindow::updateHardwareDisplay() {
  cout << "MainWindow::updateHardwareDisplay()" << endl;
  fqleCANbusErrors->setText(QString::number(fpHw->getNCANbusErrors()));
  fqleI2CErrors->setText(QString::number(fpHw->getNI2CErrors()));
  fqleRunTime->setText(QString::number(fpHw->getRunTime()));

  fqleAT->setText(QString::number(fpHw->getTemperature(), 'f', 2));
  fqleWT->setText(QString::number(fpHw->getTECRegister(8, "Temp_W"), 'f', 2));
  fqleRH->setText(QString::number(fpHw->getRH(), 'f', 2));
  fqleDP->setText(QString::number(fpHw->getDP(), 'f', 2));
}

// ----------------------------------------------------------------------
void MainWindow::setupQLE(QLineEdit *qle) {
  qle->setFocusPolicy(Qt::NoFocus);
  qle->setAlignment(Qt::AlignRight);
  qle->setAlignment(Qt::AlignRight);
  qle->setFont(fFont1);
  qle->setFixedSize(QSize(140, 50));
}


// ----------------------------------------------------------------------
void MainWindow::setupLBL(QLabel *q) {
  q->setFocusPolicy(Qt::NoFocus);
  q->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  q->setFont(fFont1);
}


// ----------------------------------------------------------------------
void MainWindow::btnQuit() {
  static int cnt(0);
  cout << "MainWindow::signalQuitProgram(), cnt = " << cnt << endl;
  ++cnt;
  emit signalQuitProgram();
}


// ----------------------------------------------------------------------
void MainWindow::btnValve0() {
  // -- negate!
  if (!fpHw->getStatusValve0()) {
    fbtnValve0->setStyleSheet("QPushButton {background-color: #A3C1DA; color: black;}");
  } else {
    fbtnValve0->setStyleSheet("QPushButton {background-color: gray; color: black;}");
  }

  emit signalValve(1);
}


// ----------------------------------------------------------------------
void MainWindow::btnValve1() {
  // -- negate!
  if (!fpHw->getStatusValve1()) {
    fbtnValve1->setStyleSheet("QPushButton {background-color: #A3C1DA; color: black;}");
  } else {
    fbtnValve1->setStyleSheet("QPushButton {background-color: gray; color: black;}");
  }
  emit signalValve(2);
}
