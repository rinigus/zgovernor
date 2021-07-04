#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

#include <QMap>
#include <QList>
#include <QString>

#define CONFIG_DISPLAY_ON  "display on"
#define CONFIG_DISPLAY_OFF "display off"

struct ConfigSetting {
  ConfigSetting(const QString &fname, const QString &cont): filename(fname), content(cont) {}

  QString filename;
  QString content;
};

class Config : public QObject
{
  Q_OBJECT

public:
  explicit Config(QObject *parent = nullptr);

  bool empty() const;

  const QList<ConfigSetting> get(const QString &config) const;

signals:

protected:
  void loadSetting(const QString &condition, const QString &content);

protected:
  QMap< QString, QList<ConfigSetting> > m_settings;
};

#endif // CONFIG_H
