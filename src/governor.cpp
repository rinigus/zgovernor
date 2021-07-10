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

  // establish connections
  QDBusConnection::systemBus().connect("com.nokia.mce", "/com/nokia/mce/signal",
                                       "com.nokia.mce.signal", "display_status_ind",
                                       this, SLOT(onMCEDispayStatus(QString)));
  QDBusConnection::systemBus().connect("", "/",
                                       "org.nemomobile.compositor", "privateTopmostWindowProcessIdChanged",
                                       this, SLOT(onComposerTopWindow(int)));
}

void Governor::onMCEDispayStatus(QString message)
{
  if (message == "on")
    m_displayOn = true;
  else if (message == "off")
    m_displayOn = false;
  else
    return;

  if (m_displayOn)
    applyConfig(CONFIG_DISPLAY_ON);
  else
    applyConfig(CONFIG_DISPLAY_OFF);

  if (m_composerOnTop && m_displayOn)
    applyConfig(CONFIG_COMPOSITOR_MAIN);
}

void Governor::onComposerTopWindow(int topWindow)
{
  m_composerOnTop = (topWindow == 0);
  if (m_displayOn)
    {
      if (m_composerOnTop)
	applyConfig(CONFIG_COMPOSITOR_MAIN);
      else
	applyConfig(CONFIG_COMPOSITOR_WINDOW_SHOWN);
    }
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
