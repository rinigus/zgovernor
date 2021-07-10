#include "config.h"

#include <QSettings>
#include <QStringList>

#include <QDebug>

#include <iostream>

Config::Config(QObject *parent) : QObject(parent)
{
  QSettings settings(CONFIG_FILE, QSettings::IniFormat);

  QStringList conditions = {CONFIG_DISPLAY_OFF, CONFIG_DISPLAY_ON, CONFIG_COMPOSITOR_MAIN, CONFIG_COMPOSITOR_WINDOW_SHOWN};

  for (const QString &c: conditions)
    {
      settings.beginGroup(c);
      loadSetting(c, settings.value("set_files").toString());
      settings.endGroup();
    }

  // Print loaded configuration
  std::cout << "Loaded configuration\n";
  for (const QString &c: m_settings.keys())
    {
      std::cout << c.toStdString() << ":\n";
      for (const ConfigSetting &s: m_settings[c])
        std::cout << s.filename.toStdString() << " -> " << s.content.toStdString() << "\n";
      std::cout  << "\n";
    }
}

bool Config::empty() const
{
  for (auto &k: m_settings)
    if (!k.empty())
      return false;
  return true;
}

void Config::loadSetting(const QString &condition, const QString &content)
{
  QList<ConfigSetting> s;
  QStringList lines = content.split("\n");
  for (const QString &l: lines)
    {
      // handle empty split results for older Qt
      QStringList p0 = l.split(" ");
      QStringList p;
      for (QString c: p0)
        {
          c = c.trimmed();
          if (!c.isEmpty())
            p.append(c);
        }

      if (p.size() > 0 && p[0] == "#")
        {
          // pass comment line
        }
      else if (p.size() == 2)
        s.append(ConfigSetting(p[0], p[1]));
      else if (p.size() == 0)
        {
          // pass empty line
        }
      else
        std::cerr << "Ignoring faulty line in configuration: " << condition.toStdString() << " - " << l.toStdString() << "\n";
    }

  m_settings[condition] = s;
}

const QList<ConfigSetting> Config::get(const QString &config) const
{
  return m_settings[config];
}
