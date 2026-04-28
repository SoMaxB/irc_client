#include <QtTest/QtTest>
#include <QCoreApplication>

#include "TestMessageParser.hpp"
#include "TestCommandHandler.hpp"
#include "TestConfigManager.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    TestMessageParser testMsg;
    int msgResult = QTest::qExec(&testMsg, argc, argv);

    TestCommandHandler testCmd;
    int cmdResult = QTest::qExec(&testCmd, argc, argv);

    TestConfigManager testConfig;
    int configResult = QTest::qExec(&testConfig, argc, argv);

    return (msgResult == 0 && cmdResult == 0 && configResult == 0) ? 0 : 1;
}