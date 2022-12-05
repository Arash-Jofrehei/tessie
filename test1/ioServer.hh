#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <QtCore/QObject>

#include "tMosq.hh"

class driveHardware;
// ----------------------------------------------------------------------
class ioServer: public QObject {

  Q_OBJECT

public:
  ioServer(driveHardware *);
  ~ioServer();
  void run();
  void printFromServer(std::string msg);

public slots:
  void sentToServer(std::string msg);
  void startServer();

signals:
  void sendFromServer(std::string msg);

private:
  tMosq         *fCtrlTessie;
  driveHardware *fHardware;
};

#endif // RPCSERVER_H
