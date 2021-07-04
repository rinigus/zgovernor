#include "governor.h"

#include <QDBusConnection>

#include <QDebug>

#include <iostream>
#include <fstream>

Governor::Governor(Config &c) :
  QObject(),
  m_config(c)
{
  m_activity = new BackgroundActivity(this);
  m_activity->setWakeupFrequency(BackgroundActivity::ThirtySeconds);

  // establish connection
  QDBusConnection::systemBus().connect("com.nokia.mce", "/com/nokia/mce/signal",
                                       "com.nokia.mce.signal", "display_status_ind",
                                       this, SLOT(onDBusMessage(QString)));
}

void Governor::onDBusMessage(QString message)
{
  if (message == "on")
    applyConfig(CONFIG_DISPLAY_ON);
  else if (message == "off")
    applyConfig(CONFIG_DISPLAY_OFF);
}

void Governor::applyConfig(const QString &cname)
{
  const QList<ConfigSetting> settings = m_config.get(cname);

  m_activity->run();
  std::cout << "Apply: " << cname.toStdString() << std::endl;

  for (const ConfigSetting& c: settings)
    {
      std::ofstream f(c.filename.toStdString().c_str());
      f << c.content.toStdString().c_str();
    }

  std::cout << "Applied: " << cname.toStdString() << std::endl;
  m_activity->stop();
}
