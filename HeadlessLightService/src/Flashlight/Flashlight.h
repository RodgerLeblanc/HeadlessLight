/*
 * Flashlight.h
 *
 *  Created on: 2016-06-09
 *      Author: Roger
 */

#ifndef FLASHLIGHT_H_
#define FLASHLIGHT_H_

#include <QObject>
#include <camera/camera_api.h>

class Flashlight : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool flashlightOn READ flashlightOn NOTIFY flashlightOnChanged);
    Q_PROPERTY(bool flashlightAvailableOnDevice READ flashlightAvailableOnDevice NOTIFY flashlightAvailableOnDeviceChanged);

public:
    Flashlight(QObject *_parent = 0);
    virtual ~Flashlight();

    bool flashlightOn() { return m_flashlightOn; }
    void toggleFlashlight();

private:
    bool flashlightAvailableOnDevice() { return m_flashlightAvailableOnDevice; }
    void setFlashlightAvailableOnDevice(const bool& newValue);

    void setFlashlightOn(const bool& newValue);

    void turnOffFlashlight();
    void turnOnFlashlight();

    camera_handle_t m_cameraHandle;
    camera_unit_t m_cameraUnit;

    bool m_flashlightAvailableOnDevice, m_flashlightOn;

signals:
    void flashlightAvailableOnDeviceChanged(const bool&);
    void flashlightOnChanged(const bool&);
};

#endif /* FLASHLIGHT_H_ */
