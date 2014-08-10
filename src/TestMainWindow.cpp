#include "TestMainWindow.h"

TestMainWindow::TestMainWindow(){}

TestMainWindow::~TestMainWindow(){}

void TestMainWindow::sayhello()
{
    std::cerr << "Hello PyGrip!" << std::endl;
}

void TestMainWindow::TestSlot()
{
    std::cerr << "Testing PyGrip Slot" << std::endl;
}
