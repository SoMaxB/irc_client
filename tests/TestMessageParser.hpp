#pragma once

#include <QtCore/QObject>

class TestMessageParser : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testParseSimpleCommand();
    void testParseCommandWithPrefix();
    void testParseCommandWithTags();
    void testParseCommandWithTrailing();
    void testParseCommandWithParams();
    void testParseCommandWithAll();
    void testParseEmptyLine();
    void testParsePrivmsg();
    void testParseJoin();
    void testParseNumericReply();
    void testSerialize();
    void testSerializeWithTags();
    void testSerializeWithTrailing();
    void testBuildPrivmsg();
    void testBuildNotice();
    void testIsNumericReply();
    void testGetNumericCode();
    void testGetCommandType();
};