#include "TestCommandHandler.hpp"
#include "CommandHandler.hpp"
#include <QtTest/QtTest>

void TestCommandHandler::initTestCase() {}
void TestCommandHandler::cleanupTestCase() {}
void TestCommandHandler::init() {}
void TestCommandHandler::cleanup() {}

void TestCommandHandler::testParseJoin() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/join #channel");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Join);
    QVERIFY(cmd.arg1 == "#channel");
}

void TestCommandHandler::testParseJoinWithHash() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/join #test");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.arg1 == "#test");
}

void TestCommandHandler::testParseJoinMissingChannel() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/join");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Join);
    QVERIFY(!cmd.errorMessage.isEmpty());
}

void TestCommandHandler::testParsePart() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/part #channel");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Part);
    QVERIFY(cmd.arg1 == "#channel");
}

void TestCommandHandler::testParsePartWithoutChannel() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/part");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Part);
}

void TestCommandHandler::testParseMsg() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/msg nick message");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Msg);
    QVERIFY(cmd.arg1 == "nick");
    QVERIFY(cmd.arg2 == "message");
}

void TestCommandHandler::testParseMsgMissingArgs() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/msg");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Msg);
}

void TestCommandHandler::testParseQuery() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/query nick");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Query);
    QVERIFY(cmd.arg1 == "nick");
}

void TestCommandHandler::testParseQueryMissingNick() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/query");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Query);
}

void TestCommandHandler::testParseMe() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/me is excited");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Me);
    QVERIFY(cmd.arg1 == "is excited");
}

void TestCommandHandler::testParseMeMissingAction() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/me");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Me);
}

void TestCommandHandler::testParseRaw() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/raw PING");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Raw);
    QVERIFY(cmd.arg1 == "PING");
}

void TestCommandHandler::testParseRawMissingCommand() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/raw");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Raw);
}

void TestCommandHandler::testParseSearch() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/search keyword");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Search);
    QVERIFY(cmd.arg1 == "keyword");
}

void TestCommandHandler::testParseSearchMissingText() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/search");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Search);
}

void TestCommandHandler::testParseTopicGet() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/topic");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Topic);
}

void TestCommandHandler::testParseTopicSet() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/topic #channel New Topic");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Topic);
    QVERIFY(cmd.arg2 == "New Topic");
}

void TestCommandHandler::testParseNick() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/nick newnick");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Nick);
    QVERIFY(cmd.arg1 == "newnick");
}

void TestCommandHandler::testParseNickMissingNewNick() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/nick");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Nick);
}

void TestCommandHandler::testParseQuit() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/quit");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Quit);
}

void TestCommandHandler::testParseQuitWithMessage() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/quit Goodbye");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Quit);
    QVERIFY(cmd.arg1 == "Goodbye");
}

void TestCommandHandler::testParsePass() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/pass secret");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Pass);
    QVERIFY(cmd.arg1 == "secret");
}

void TestCommandHandler::testParsePassMissingPassword() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/pass");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Pass);
}

void TestCommandHandler::testParseUser() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/user myuser");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::User);
}

void TestCommandHandler::testParseUserMissingArgs() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/user");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::User);
}

void TestCommandHandler::testParseUserWithQuotes() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/user \"username\" \"Real Name\"");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.arg1 == "username");
    QVERIFY(cmd.arg2 == "Real Name");
}

void TestCommandHandler::testParseUserValid() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/user myuser My Real Name");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::User);
    QVERIFY(cmd.arg1 == "myuser");
}

void TestCommandHandler::testParseInvite() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/invite nick");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Invite);
    QVERIFY(cmd.arg1 == "nick");
}

void TestCommandHandler::testParseInviteMissingNick() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/invite");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Invite);
}

void TestCommandHandler::testParseMode() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/mode #channel +m");
    QVERIFY(cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Mode);
    QVERIFY(cmd.arg1 == "#channel");
    QVERIFY(cmd.arg2 == "+m");
}

void TestCommandHandler::testParseModeMissingArgs() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/mode");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Mode);
}

void TestCommandHandler::testParseNotSlashCommand() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("hello");
    QVERIFY(!cmd.valid);
    QVERIFY(!cmd.errorMessage.isEmpty());
}

void TestCommandHandler::testParseEmptyCommand() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/");
    QVERIFY(!cmd.valid);
    QVERIFY(!cmd.errorMessage.isEmpty());
}

void TestCommandHandler::testParseUnknownCommand() {
    CommandHandler::ParsedCommand cmd = CommandHandler::parse("/unknown");
    QVERIFY(!cmd.valid);
    QVERIFY(cmd.type == CommandHandler::Type::Unknown);
    QVERIFY(!cmd.errorMessage.isEmpty());
}

void TestCommandHandler::testSupportedCommandsSummary() {
    QString summary = CommandHandler::supportedCommandsSummary();
    QVERIFY(summary.contains("/join"));
    QVERIFY(summary.contains("/part"));
    QVERIFY(summary.contains("/msg"));
}