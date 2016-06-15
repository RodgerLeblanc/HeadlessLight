/*
 * Copyright (c) 2013-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "service.hpp"
#include <src/Logger/Logger.h>

#include <bb/Application>
#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/system/InvokeManager>

using namespace bb::platform;
using namespace bb::system;

Service::Service() :
        QObject(),
        m_flashlight(new Flashlight(this)),
        m_invokeManager(new InvokeManager(this)),
        m_notify(new Notification(this)),
        m_physicalButtons(new PhysicalButtons(this)),
        m_lastPowerPress(QDateTime(QDate(1970, 1, 1)))
{
    Logger::logThis("Headless started");

    connect(m_flashlight, SIGNAL(flashlightOnChanged(const bool&)), this, SLOT(onFlashlightOnChanged(const bool&)));

    connect(m_physicalButtons, SIGNAL(powerButtonPressed()), this, SLOT(onPowerButtonPressed()));

    m_invokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));


    NotificationDefaultApplicationSettings settings;
    settings.setPreview(NotificationPriorityPolicy::Allow);
    settings.apply();

    const QString iconPath = "file://" + QDir::currentPath() + "/app/public/native/icon.png";
    m_notify->setIconUrl(iconPath);
}

void Service::handleInvoke(const bb::system::InvokeRequest & request)
{
    Logger::logThis("Service::handleInvoke() " + request.action());

    QString action;
    if (!request.action().isEmpty())
        action = request.action().split(".").last();

    if (action == "SHUTDOWN") {
        bb::Application::instance()->quit();
    }
    else if (action == "SHUTDOWN_FLASHLIGHT") {
        if (m_flashlight->flashlightOn()) {
            m_flashlight->toggleFlashlight();
        }
    }
}

void Service::onFlashlightOnChanged(const bool& flashlightOn) {
    Logger::logThis("Service::onFlashlightOnChanged() " + QString(flashlightOn ? "ON" : "OFF"));

    if (flashlightOn) {
        // Show a Hub notification as the user might have forget about this app
        // and he's going to wonder why his camera flashlight just turned ON.

        m_notify->setTitle(bb::Application::applicationName() + tr(" turned ON your flashlight"));
        m_notify->setBody(tr("Don't panic, ") + bb::Application::applicationName() + tr(" just turned ON your flashlight because you doucle clicked quickly on POWER button. Click Open in the bottom of this screen if you want to turn OFF flashlight."));

        bb::system::InvokeRequest request;
        request.setTarget("com.CellNinja.HeadlessLightService");
        request.setAction("com.CellNinja.HeadlessLightService.SHUTDOWN_FLASHLIGHT");
        m_notify->setInvokeRequest(request);

        m_notify->notify();
    }
    else {
        Notification::clearEffectsForAll();
        m_notify->deleteFromInbox();
    }
}

void Service::onPowerButtonPressed() {
    Logger::logThis("Service::onPowerButtonPressed()");

    QDateTime now = QDateTime::currentDateTime();
    if (m_lastPowerPress.msecsTo(now) < 300) {
        m_flashlight->toggleFlashlight();

        // Reinit
        m_lastPowerPress = QDateTime(QDate(1970, 1, 1));
    }
    else {
        m_lastPowerPress = now;
    }
}
