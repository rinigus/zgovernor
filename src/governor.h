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
  void onMCEDispayStatus(QString message);
  void onComposerTopWindow(int topWindow);

protected:
  void applyConfig(const QString &cname);

protected:
  Config &m_config;
  bool m_displayOn = true;
  bool m_composerOnTop = false;

  BackgroundActivity *m_activity;
};

#endif // GOVERNOR_H
