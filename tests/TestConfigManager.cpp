#include "TestConfigManager.hpp"
#include "ConfigManager.hpp"
#include <QTemporaryDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QtTest/QtTest>

void TestConfigManager::initTestCase() {
    QCoreApplication::setOrganizationName("IrcClientTest");
    QCoreApplication::setApplicationName("IrcClientTest");
}

void TestConfigManager::cleanupTestCase() {}
void TestConfigManager::init() {}
void TestConfigManager::cleanup() {
    ConfigManager config;
    config.clearAllSettings();
}

void TestConfigManager::testSaveAndLoadLastConnection() {
    ConfigManager config;

    config.saveLastConnection("irc.libera.chat", 6697, "TestUser", true);

    QString server;
    int port = 0;
    QString nickname;
    bool useTls = false;

    QVERIFY(config.loadLastConnection(server, port, nickname, useTls));
    QVERIFY(server == "irc.libera.chat");
    QVERIFY(port == 6697);
    QVERIFY(nickname == "TestUser");
    QVERIFY(useTls == true);
}

void TestConfigManager::testLoadLastConnectionNoData() {
    ConfigManager config;
    config.clearAllSettings();

    QString server;
    int port = 0;
    QString nickname;
    bool useTls = false;

    QVERIFY(!config.loadLastConnection(server, port, nickname, useTls));
}

void TestConfigManager::testSaveAndLoadChannels() {
    ConfigManager config;

    config.addJoinedChannel("#channel1");
    config.addJoinedChannel("#channel2");

    QStringList channels = config.getJoinedChannels();
    QVERIFY(channels.contains("#channel1"));
    QVERIFY(channels.contains("#channel2"));
}

void TestConfigManager::testRemoveChannel() {
    ConfigManager config;

    config.addJoinedChannel("#channel1");
    config.addJoinedChannel("#channel2");
    config.removeJoinedChannel("#channel1");

    QStringList channels = config.getJoinedChannels();
    QVERIFY(!channels.contains("#channel1"));
    QVERIFY(channels.contains("#channel2"));
}

void TestConfigManager::testClearChannels() {
    ConfigManager config;

    config.addJoinedChannel("#channel1");
    config.addJoinedChannel("#channel2");
    config.clearJoinedChannels();

    QStringList channels = config.getJoinedChannels();
    QVERIFY(channels.isEmpty());
}

void TestConfigManager::testSaveAndLoadAutoReconnect() {
    ConfigManager config;

    config.saveAutoReconnectEnabled(false);
    QVERIFY(config.loadAutoReconnectEnabled() == false);

    config.saveAutoReconnectEnabled(true);
    QVERIFY(config.loadAutoReconnectEnabled() == true);
}

void TestConfigManager::testSaveAndLoadThemePreference() {
    ConfigManager config;

    config.saveThemePreference(1);
    QVERIFY(config.loadThemePreference() == 1);

    config.saveThemePreference(2);
    QVERIFY(config.loadThemePreference() == 2);
}

void TestConfigManager::testSaveAndLoadCustomPresets() {
    ConfigManager config;

    config.saveCustomPreset("TestPreset", "irc.testserver.org", 6667, false);

    QStringList names;
    QStringList servers;
    QList<int> ports;
    QList<bool> tlsFlags;

    QVERIFY(config.loadCustomPresets(names, servers, ports, tlsFlags));
    QVERIFY(names.contains("TestPreset"));
    int idx = names.indexOf("TestPreset");
    QVERIFY(servers[idx] == "irc.testserver.org");
    QVERIFY(ports[idx] == 6667);
    QVERIFY(tlsFlags[idx] == false);
}

void TestConfigManager::testRemoveCustomPreset() {
    ConfigManager config;

    config.saveCustomPreset("TempPreset", "irc.example.org", 6697, true);
    config.removeCustomPreset("TempPreset");

    QStringList names;
    QStringList servers;
    QList<int> ports;
    QList<bool> tlsFlags;

    QVERIFY(!config.loadCustomPresets(names, servers, ports, tlsFlags));
}

void TestConfigManager::testClearAllSettings() {
    ConfigManager config;

    config.saveLastConnection("irc.example.org", 6667, "User", false);
    config.addJoinedChannel("#test");
    config.saveAutoReconnectEnabled(false);

    config.clearAllSettings();

    QString server;
    int port = 0;
    QString nickname;
    bool useTls = false;
    QVERIFY(!config.loadLastConnection(server, port, nickname, useTls));
    QVERIFY(config.getJoinedChannels().isEmpty());
}