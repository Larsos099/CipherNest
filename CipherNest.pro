QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23
macx{
    DEFINES += MAC_PLATFORM
    OPENSSL_PATH = /opt/homebrew/Cellar/openssl@3/3.4.1
}

win32 {
    DEFINES += WINDOWS_PLATFORM
    OPENSSL_PATH = D:\\Qt\\Tools\\OpenSSLv3\\Win_x64
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    encryptionservice.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    encryptionservice.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$OPENSSL_PATH/lib/ -lcrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OPENSSL_PATH/lib/ -lcrypto
else:unix: LIBS += -L$$OPENSSL_PATH/lib/ -lcrypto.3

INCLUDEPATH += $$OPENSSL_PATH/include
DEPENDPATH += $$OPENSSL_PATH/include

win32:CONFIG(release, debug|release): LIBS += -L$$OPENSSL_PATH/lib/ -lssl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OPENSSL_PATH/lib/ -lssl
else:unix: LIBS += -L$$OPENSSL_PATH/lib/ -lssl
