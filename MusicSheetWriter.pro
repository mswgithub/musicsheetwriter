#-------------------------------------------------
#
# Project created by QtCreator 2016-02-03T10:25:04
#
#-------------------------------------------------

QT       += core gui xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MusicSheetWriter
TEMPLATE = app

include(QMidi/QMidi.pri)

SOURCES +=\
    ProjectFactory.cpp \
    MusicSheetWriter.cpp \
    Note.cpp \
    Project.cpp \
    Score.cpp \
    Voice.cpp \
    Converter.cpp \
    main.cpp \
    AMenu.cpp \
    FileMenu.cpp \
    ProjectManager.cpp \
    EditMenu.cpp \
    AProjectManager.cpp \
    NewProjectDialog.cpp \
    ProjectTreeWidget.cpp \
    EditionToolbar.cpp \
    AEditionManager.cpp \
    EditionManager.cpp \
    ScoreEditor.cpp \
    ScoreViewerWidget.cpp \
    AMusicElement.cpp \
    Rest.cpp \
    Tag.cpp \
    MusicElementFactory.cpp \
    AEvent.cpp \
    Player.cpp \
    analysis/AudioInput.cpp \
    analysis/ASoundAnalyzer.cpp \
    analysis/SoundAnalyzerManager.cpp \
    analysis/AccousticAnalyzer.cpp \
    analysis/FrequencyTable.cpp \
    Chord.cpp \
    Parse.cpp \
    PlayerManager.cpp \
    MidiPlayer.cpp \
    midi/rtmidi.cpp \
    midi/rtmidi_c.cpp \
    midi/MidiAnalyser.cpp \
    userexception.cpp \
    MetronomePlayer.cpp \
    MetronomeManager.cpp \
    Metronome.cpp \
    AndroidSwitch.cpp \
    GraphicItemsView.cpp \
    MultipageToolbar.cpp

HEADERS  += \
    ProjectFactory.h \
    MusicSheetWriter.h \
    Note.h \
    Project.h \
    Score.h \
    Voice.h \
    Converter.h \
    AMenu.h \
    FileMenu.h \
    ProjectManager.h \
    EditMenu.h \
    AProjectManager.h \
    NewProjectDialog.h \
    ProjectTreeWidget.h \
    EditionToolbar.h \
    AEditionManager.h \
    EditionManager.h \
    ScoreEditor.h \
    ScoreViewerWidget.h \
    IScoreEditor.h \
    AMusicElement.h \
    Rest.h \
    Tag.h \
    IGuidoAbstractObject.h \
    MusicElementFactory.h \
    AEvent.h \
    Player.h \
    analysis/ASoundAnalyzer.h \
    analysis/AudioInput.h \
    analysis/AudioData.h \
    analysis/SoundAnalyzerManager.h \
    analysis/AccousticAnalyzer.h \
    analysis/FrequencyTable.h \
    Chord.h \
    Parse.h \
    PlayerManager.h \
    MidiPlayer.h \
    midi/rtmidi.h \
    midi/rtmidi_c.h \
    midi/MidiAnalyser.h \
    userexception.h \
    MetronomePlayer.h \
    MetronomeManager.h \
    Metronome.h \
    AndroidSwitch.h \
    GraphicItemsView.h \
    MultipageToolbar.h

FORMS    += \
    newProjectDialog.ui \
    musicSheetWriter.ui \
    editionToolbar.ui \
    generatorToolbar.ui \
    midiPlayer.ui \
    metronomePlayer.ui

RESOURCES += \
    ressources.qrc

TRANSLATIONS += \
    englishTranslation.ts \
    frenchTranslation.ts

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/GuidoEngine/release/ -lGUIDOEngine64D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/GuidoEngine/debug/ -lGUIDOEngine64D

INCLUDEPATH += $$PWD/libs/GuidoEngine/include
DEPENDPATH += $$PWD/libs/GuidoEngine/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/GuidoQt/release/ -lGuidoQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/GuidoQt/debug/ -lGuidoQt

INCLUDEPATH += $$PWD/libs/GuidoQt/include
DEPENDPATH += $$PWD/libs/GuidoQt/include

win32: LIBS += -L$$PWD/libs/LibMusicXML/ -llibmusicxml2

INCLUDEPATH += $$PWD/libs/LibMusicXML/include
DEPENDPATH += $$PWD/libs/LibMusicXML/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/PortAudio/release/ -lportaudio_x64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/PortAudio/debug/ -lportaudio_x64

INCLUDEPATH += $$PWD/libs/PortAudio/include
DEPENDPATH += $$PWD/libs/PortAudio/include

win32: LIBS += -L$$PWD/libs/Midisharelight/ -lmidisharelight64

INCLUDEPATH += $$PWD/libs/Midisharelight/include
DEPENDPATH += $$PWD/libs/Midisharelight/include

win32: LIBS += -lwinmm
