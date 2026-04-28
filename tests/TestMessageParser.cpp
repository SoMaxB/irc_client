#include "TestMessageParser.hpp"
#include "MessageParser.hpp"
#include <QtTest/QtTest>

void TestMessageParser::initTestCase() {}
void TestMessageParser::cleanupTestCase() {}
void TestMessageParser::init() {}
void TestMessageParser::cleanup() {}

void TestMessageParser::testParseSimpleCommand() {
    IrcMessage msg = MessageParser::parse("PING");
    QVERIFY(msg.isValid());
    QCOMPARE(msg.command, QString("PING"));
    QVERIFY(msg.source.isEmpty());
    QVERIFY(msg.params.isEmpty());
    QVERIFY(msg.tags.isEmpty());
}

void TestMessageParser::testParseCommandWithPrefix() {
    IrcMessage msg = MessageParser::parse(":nick!user@host PRIVMSG #channel :Hello world");
    QVERIFY(msg.isValid());
    QCOMPARE(msg.command, QString("PRIVMSG"));
    QCOMPARE(msg.source, QString("nick!user@host"));
    QVERIFY(msg.params.contains(QString("#channel")));
}

void TestMessageParser::testParseCommandWithTags() {
    IrcMessage msg = MessageParser::parse("@tag1=value1;tag2=value2 :server 001 nick :Welcome");
    QVERIFY(msg.isValid());
    QVERIFY(msg.tags.contains("tag1"));
    QVERIFY(msg.tags.contains("tag2"));
    QVERIFY(msg.tags["tag1"] == "value1");
    QVERIFY(msg.tags["tag2"] == "value2");
}

void TestMessageParser::testParseCommandWithTrailing() {
    IrcMessage msg = MessageParser::parse("PRIVMSG #channel :Hello world");
    QVERIFY(msg.isValid());
    QVERIFY(msg.trailing == "Hello world");
    QVERIFY(msg.params.contains("Hello world"));
}

void TestMessageParser::testParseCommandWithParams() {
    IrcMessage msg = MessageParser::parse("JOIN #channel");
    QVERIFY(msg.isValid());
    QVERIFY(msg.params.contains("#channel"));
}

void TestMessageParser::testParseCommandWithAll() {
    IrcMessage msg = MessageParser::parse("@tag=value :nick!user@host PRIVMSG #channel :Hello");
    QVERIFY(msg.isValid());
    QVERIFY(!msg.tags.isEmpty());
    QVERIFY(!msg.source.isEmpty());
    QVERIFY(!msg.params.isEmpty());
}

void TestMessageParser::testParseEmptyLine() {
    IrcMessage msg = MessageParser::parse("");
    QVERIFY(!msg.isValid());
}

void TestMessageParser::testParsePrivmsg() {
    IrcMessage msg = MessageParser::parse(":sender!user@host PRIVMSG #test :Testing message");
    QVERIFY(msg.isValid());
    QVERIFY(msg.command == "PRIVMSG");
    QVERIFY(msg.source == "sender!user@host");
    QVERIFY(msg.trailing == "Testing message");
}

void TestMessageParser::testParseJoin() {
    IrcMessage msg = MessageParser::parse(":nick!user@host JOIN #channel");
    QVERIFY(msg.isValid());
    QVERIFY(msg.command == "JOIN");
    QVERIFY(msg.params.contains("#channel"));
}

void TestMessageParser::testParseNumericReply() {
    IrcMessage msg = MessageParser::parse(":server 001 nick :Welcome to the network");
    QVERIFY(msg.isValid());
    QVERIFY(msg.command == "001");
    QVERIFY(MessageParser::isNumericReply(msg));
    QVERIFY(MessageParser::getNumericCode(msg) == 1);
}

void TestMessageParser::testSerialize() {
    IrcMessage msg;
    msg.command = "PRIVMSG";
    msg.params = {"#channel", "Hello"};
    QString serialized = MessageParser::serialize(msg);
    QVERIFY(serialized.contains("PRIVMSG"));
    QVERIFY(serialized.contains("#channel"));
}

void TestMessageParser::testSerializeWithTags() {
    IrcMessage msg;
    msg.tags["example.com/msg-id"] = "abc123";
    msg.command = "PRIVMSG";
    msg.params = {"#channel", "Hello"};
    QString serialized = MessageParser::serialize(msg);
    QVERIFY(serialized.startsWith("@"));
    QVERIFY(serialized.contains("PRIVMSG"));
}

void TestMessageParser::testSerializeWithTrailing() {
    IrcMessage msg;
    msg.command = "PRIVMSG";
    msg.params = {"#channel", "Hello world with spaces"};
    QString serialized = MessageParser::serialize(msg);
    QVERIFY(serialized.contains(" :"));
}

void TestMessageParser::testBuildPrivmsg() {
    QString result = MessageParser::buildPrivmsg("#channel", "Hello");
    QVERIFY(result.contains("PRIVMSG"));
    QVERIFY(result.contains("#channel"));
    QVERIFY(result.contains("Hello"));
}

void TestMessageParser::testBuildNotice() {
    QString result = MessageParser::buildNotice("#channel", "Notice message");
    QVERIFY(result.contains("NOTICE"));
    QVERIFY(result.contains("#channel"));
    QVERIFY(result.contains("Notice message"));
}

void TestMessageParser::testIsNumericReply() {
    QVERIFY(MessageParser::isNumericReply(MessageParser::parse("001")));
    QVERIFY(MessageParser::isNumericReply(MessageParser::parse("324")));
    QVERIFY(MessageParser::isNumericReply(MessageParser::parse("999")));
    QVERIFY(!MessageParser::isNumericReply(MessageParser::parse("JOIN")));
    QVERIFY(!MessageParser::isNumericReply(MessageParser::parse("12")));
    QVERIFY(!MessageParser::isNumericReply(MessageParser::parse("1234")));
}

void TestMessageParser::testGetNumericCode() {
    IrcMessage msg001 = MessageParser::parse("001");
    IrcMessage msg404 = MessageParser::parse("404");
    IrcMessage msgJoin = MessageParser::parse("JOIN");

    QVERIFY(MessageParser::getNumericCode(msg001) == 1);
    QVERIFY(MessageParser::getNumericCode(msg404) == 404);
    QVERIFY(MessageParser::getNumericCode(msgJoin) == -1);
}

void TestMessageParser::testGetCommandType() {
    IrcMessage msg = MessageParser::parse("PRIVMSG #channel :Hello");
    QVERIFY(MessageParser::getCommandType(msg) == "PRIVMSG");
}