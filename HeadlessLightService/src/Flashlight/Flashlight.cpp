/*
 * Flashlight.cpp
 *
 *  Created on: 2016-06-09
 *      Author: Roger
 */

#include <src/Flashlight/Flashlight.h>
#include <src/Logger/Logger.h>

Flashlight::Flashlight(QObject *_parent) :
    QObject(_parent),
    m_cameraHandle(CAMERA_HANDLE_INVALID),
    m_flashlightAvailableOnDevice(false),
    m_flashlightOn(false)
{
    this->setFlashlightOn(m_flashlightOn);

    m_cameraUnit = CAMERA_UNIT_REAR;
    camera_open(m_cameraUnit, CAMERA_MODE_RW, &m_cameraHandle);
    bool flashlightAvailable = camera_has_feature(m_cameraHandle, CAMERA_FEATURE_VIDEOLIGHT);
    camera_close(m_cameraHandle);
    this->setFlashlightAvailableOnDevice(flashlightAvailable);
}

Flashlight::~Flashlight()
{
}

void Flashlight::setFlashlightAvailableOnDevice(const bool& newValue) {
    if (m_flashlightAvailableOnDevice != newValue) {
        m_flashlightAvailableOnDevice = newValue;
        emit this->flashlightAvailableOnDeviceChanged(m_flashlightAvailableOnDevice);
    }
}

void Flashlight::setFlashlightOn(const bool& newValue) {
    if (m_flashlightOn != newValue) {
        if (!m_flashlightOn)
            this->turnOnFlashlight();
        else
            this->turnOffFlashlight();
    }
}

void Flashlight::toggleFlashlight() {
    this->setFlashlightOn(!this->flashlightOn());
}

void Flashlight::turnOffFlashlight() {
    Logger::logThis("Flashlight::turnOffFlashlight()");

    if (camera_config_videolight(m_cameraHandle, CAMERA_VIDEOLIGHT_OFF) != CAMERA_EOK) return;
    if (camera_close(m_cameraHandle) != CAMERA_EOK) return;

    m_flashlightOn = false;
    emit this->flashlightOnChanged(m_flashlightOn);
    Logger::logThis("Flashlight::turnOffFlashlight() ended");
}

void Flashlight::turnOnFlashlight() {
    Logger::logThis("Flashlight::turnOnFlashlight()");

    if (this->flashlightAvailableOnDevice()) {
        camera_error_t open_error = camera_open(m_cameraUnit, CAMERA_MODE_RW, &m_cameraHandle);
        camera_error_t config_error = camera_config_videolight(m_cameraHandle, CAMERA_VIDEOLIGHT_ON);

        if ((open_error == CAMERA_EOK) && (config_error == CAMERA_EOK)) {
            m_flashlightOn = true;
            emit this->flashlightOnChanged(m_flashlightOn);
            Logger::logThis("Flashlight ON success!");
        }
        else {
            Logger::logThis("Flashlight ON error... open_error #" + QString::number((int)open_error) + " config_error #" + QString::number((int)config_error));
        }
    }
    else {
        Logger::logThis("Flashlight ON error... handle is invalid or no camera supports videolight");
    }
}
