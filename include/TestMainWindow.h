#ifndef TEST_MAINWINDOW_H
#define TEST_MAINWINDOW_H

// Qt includes
#include <QMainWindow>
#include <QDir>
#include <QtXml>

// C++ Standard includes
#include <iostream>
#include <cstdio>

// Forward declarations
class QAction;
class QActionGroup;
class QLabel;
class QMenu;

/**
 * \class MainWindow MainWindow.h
 * \brief Main window for robot simulation interface. A virtual base class
 * that subclasses QMainWindow
 */
class TestMainWindow : public QMainWindow
{
    /// Meta object macro to use signals and slots
    Q_OBJECT

public:

    TestMainWindow();

    ~TestMainWindow();

    static void sayhello();

public slots:

    static void TestSlot();
};

#endif // TEST_MAINWINDOW_H
