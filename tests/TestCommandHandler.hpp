#pragma once

#include <QtCore/QObject>

class TestCommandHandler : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testParseJoin();
    void testParseJoinWithHash();
    void testParseJoinMissingChannel();
    void testParsePart();
    void testParsePartWithoutChannel();
    void testParseMsg();
    void testParseMsgMissingArgs();
    void testParseQuery();
    void testParseQueryMissingNick();
    void testParseMe();
    void testParseMeMissingAction();
    void testParseRaw();
    void testParseRawMissingCommand();
    void testParseSearch();
    void testParseSearchMissingText();
    void testParseTopicGet();
    void testParseTopicSet();
    void testParseNick();
    void testParseNickMissingNewNick();
    void testParseQuit();
    void testParseQuitWithMessage();
    void testParsePass();
    void testParsePassMissingPassword();
    void testParseUser();
    void testParseUserMissingArgs();
    void testParseUserWithQuotes();
    void testParseUserValid();
    void testParseInvite();
    void testParseInviteMissingNick();
    void testParseMode();
    void testParseModeMissingArgs();
    void testParseNotSlashCommand();
    void testParseEmptyCommand();
    void testParseUnknownCommand();
    void testSupportedCommandsSummary();
};