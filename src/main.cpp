/*
 * This file is part of Pure Maps.
 *
 * SPDX-FileCopyrightText: 2021 Rinigus https://github.com/rinigus
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "config.h"
#include "governor.h"

#include <QCoreApplication>
#include <QDBusConnection>

#include <QDebug>

#include <csignal>
#include <iostream>

int main(int argc, char *argv[])
{
  QCoreApplication app(argc,argv);

  app.setApplicationName(APP_NAME);
  app.setOrganizationName(APP_NAME);
  app.setApplicationVersion(APP_VERSION);

  // register singlar handler
  signal(SIGTERM, [](int /*sig*/){ qApp->quit(); });
  signal(SIGINT, [](int /*sig*/){ qApp->quit(); });
  signal(SIGHUP, [](int /*sig*/){ qApp->quit(); });

  if (!QDBusConnection::systemBus().isConnected())
    {
      qWarning("Cannot connect to the D-Bus system bus.\n"
               "Please check your system settings and try again.\n");
      return 1;
    }

  // initialize objects
  Config config;
  if (config.empty())
    {
      std::cerr << "Exit: No configuration specified" << std::endl;
      return 0;
    }

  Governor gov(config);

  std::cout << "Starting zgovernor" << std::endl;

  int rc = app.exec();

  std::cout << "Exiting zgovernor" << std::endl;

  return rc;
}
