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

import bb.cascades 1.2

Page {
    Container {
        layout: DockLayout {}
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        topPadding: 30
        bottomPadding: topPadding
        leftPadding: topPadding
        rightPadding: topPadding
        
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            Label {
                text: qsTr("Quickly press POWER button twice to start/stop flashlight") + Retranslate.onLocaleOrLanguageChanged
                multiline: true
                textStyle.fontSize: FontSize.XLarge
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: qsTr("Did you know that double clicking quickly on POWER button doesn't turn screen ON/OFF? I didn't, learned it while coding this app. Learning everyday!") + Retranslate.onLocaleOrLanguageChanged
                multiline: true
                textStyle.fontSize: FontSize.Large
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
        Button {
            text: qsTr("Shutdown (for developer only)") + Retranslate.onLocaleOrLanguageChanged
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            onClicked: { app.shutdown() }
        }
    }
}
