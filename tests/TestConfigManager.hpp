#pragma once

#include <QtCore/QObject>

class TestConfigManager : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSaveAndLoadLastConnection();
    void testLoadLastConnectionNoData();
    void testSaveAndLoadChannels();
    void testRemoveChannel();
    void testClearChannels();
    void testSaveAndLoadAutoReconnect();
    void testSaveAndLoadThemePreference();
    void testSaveAndLoadCustomPresets();
    void testRemoveCustomPreset();
    void testClearAllSettings();
};