
#include "MusicSheetWriter.h"
#include <QApplication>
#include "midi/MidiAnalyser.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Prepare the application
    QApplication a(argc, argv);

    // Show the windows
    MusicSheetWriter w;
    w.show();

    // Launch the application (GUI loop)

    return a.exec();
    return (0);
}

