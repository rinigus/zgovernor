#ifndef GOVERNOR_H
#define GOVERNOR_H

#include <QObject>

#include "config.h"

#include <backgroundactivity.h>

class Governor : public QObject
{
  Q_OBJECT
public:
  explicit Governor(Config &c);

signals:

public slots:
  void onDBusMessage(QString message);

protected:

  void applyConfig(const QString &cname);

protected:
  Config &m_config;

  BackgroundActivity *m_activity;
};

#endif // GOVERNOR_H
