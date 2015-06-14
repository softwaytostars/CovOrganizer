#-------------------------------------------------
#
# Project ANDROID created by QtCreator 2013-09-13T10:18:22
#
#-------------------------------------------------

QT       += core gui widgets sql

TARGET = CovOrganizer
TEMPLATE = app


android {

    INCLUDEPATH += /home/alexis/Gecode4.2/include


    LIBS += -L/home/alexis/Gecode4.2/lib/ \
         -lgecodesupport \
          -lgecodekernel \
          -lgecodedriver \
          -lgecodefloat \
          -lgecodesearch \
          -lgecodeint \
          -lgecodeset \
          -lgecodeminimodel
}

linux {
    INCLUDEPATH += /home/alexis/Gecode4.2ForLinux/include


    LIBS += -L/home/alexis/Gecode4.2ForLinux/lib/ \
         -lgecodesupport \
          -lgecodekernel \
          -lgecodedriver \
          -lgecodefloat \
          -lgecodesearch \
          -lgecodeint \
          -lgecodeset \
          -lgecodeminimodel
}

SOURCES += main.cpp\
        Algo/organizer.cpp\
        mainwindow.cpp \
    ClientGraphique/covcalendarview.cpp \
    ClientGraphique/covcalendarscene.cpp \
    ClientGraphique/widgetscenecalendar.cpp \
    Data/data.cpp \
    ClientGraphique/framelisteetcalendar.cpp \
    ClientGraphique/frameentreedata.cpp \
    ClientGraphique/tableviewpers.cpp \
    Data/databasemanager.cpp \
    ClientGraphique/listeviewpers.cpp \
    ControleurAffichage/igestionframecalendar.cpp \
    utilitaires.cpp \
    ControleurAffichage/gestionframeindispos.cpp \
    ClientGraphique/framecalendar.cpp \
    iabonnegestioncalendar.cpp \
    ControleurAffichage/factorygestioncalendar.cpp \
    ControleurAffichage/gestionframesolution.cpp \
    Algo/SharedObjects/dataalgo.cpp \
    ClientGraphique/iactualisable.cpp

HEADERS  += Algo/organizer.h \
    mainwindow.h \
    ClientGraphique/covcalendarview.h \
    ClientGraphique/covcalendarscene.h \
    ClientGraphique/widgetscenecalendar.h \
    ClientGraphique/Types/TypeInfosBloc.hpp \
    Algo/Types/TypePropositionJour.hpp \
    Algo/Types/TypeSolution.hpp \
    Data/data.h \
    ClientGraphique/framelisteetcalendar.h \
    ClientGraphique/frameentreedata.h \
    ClientGraphique/tableviewpers.h \
    Data/databasemanager.h \
    Data/Types/TypeData.h \
    ClientGraphique/listeviewpers.h \
    ControleurAffichage/igestionframecalendar.h \
    utilitaires.h \
    ControleurAffichage/gestionframeindispos.h \
    ClientGraphique/framecalendar.h \
    iabonnegestioncalendar.h \
    ClientGraphique/Types/TypeAffichageCalendar.hpp \
    ControleurAffichage/factorygestioncalendar.h \
    ControleurAffichage/gestionframesolution.h \
    Algo/SharedObjects/dataalgo.h \
    ClientGraphique/iactualisable.h

FORMS    += mainwindow.ui \
    ClientGraphique/framelisteetcalendar.ui \
    ClientGraphique/frameentreedata.ui \
    ClientGraphique/framecalendar.ui

android {

OTHER_FILES += \
    android/version.xml \
    android/res/values-el/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-et/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/AndroidManifest.xml \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl
}

ANDROID_EXTRA_LIBS = ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodedriver.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodeflatzinc.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodefloat.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodeint.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodekernel.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodeminimodel.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodesearch.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodeset.so ../../OLDGecode4.2ForAndroid/lib/tmp/libgecodesupport.so
