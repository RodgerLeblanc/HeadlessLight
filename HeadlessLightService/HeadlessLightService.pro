APP_NAME = HeadlessLightService

CONFIG += qt warn_on

include(config.pri)

LIBS += -lbb -lbbdata -lbbplatform -lbbsystem -lcamapi
QT += declarative
