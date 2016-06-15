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

    camera_feature_t features[] = { CAMERA_FEATURE_VIDEOLIGHT };

    camera_unit_t next = CAMERA_UNIT_NONE;
    unsigned int num = 0;
    camera_unit_t cams[CAMERA_UNIT_NUM_UNITS];

    while (camera_find_capable(features,
                               sizeof(features)/sizeof(*features),
                               next,
                               &next) == CAMERA_EOK) {
        cams[num++] = next;
        Logger::logThis("camera unit " + QString::number((int)next) + " supports flashlight");
    }

    if (num > 0)
        m_cameraUnit = cams[0];

    bool flashlightAvailable = (m_cameraUnit != CAMERA_HANDLE_INVALID);
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
