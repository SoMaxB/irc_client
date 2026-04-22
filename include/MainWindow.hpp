#pragma once

#include <QMainWindow>
#include <QListView>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QTextCursor>
#include <QVector>
#include <QModelIndex>
#include <QCloseEvent>
#include <QMap>
#include <QStringList>
#include <memory>
#include "IrcConnection.hpp"
#include "ChannelModel.hpp"
#include "UserModel.hpp"
#include "ConfigManager.hpp"
#include "CommandHandler.hpp"

struct IrcMessage;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString& error);
    void onMessageReceived(const QString& rawMessage);
    
    void onConnect();
    void onDisconnect();
    void onJoinChannelRequested();
    void onRemoveChannelRequested();
    void onChannelClicked(const QModelIndex& index);
    void onMessageSent();
    void onReconnectAttempt();  // Auto-reconnect timer callback
    void onSearchRequested();
    void onSearchNextRequested();
    void onSearchPreviousRequested();

private:
    void setupUI();
    void setupConnections();
    void applyStyle();
    void createMainUI();
    void loadConfigurationSettings();
    void updateConnectionUi(bool connected);
    void displayMessage(const QString& channel, const QString& sender, const QString& message);
    void appendSystemMessage(const QString& message);
    void appendConversationLine(const QString& conversation, const QString& line);
    bool handleServerPing(const IrcMessage& msg);
    bool handleNumericMessage(const IrcMessage& msg);
    void refreshCurrentConversationView();
    QString extractNickFromPrefix(const QString& prefix) const;
    void updateChannelInfo();
    void updateUserList();
    void updateAutoReconnectIndicator();
    void updateRegistrationIndicator();
    void updateSendControls();
    void setConnectionPanelExpanded(bool expanded);
    void startAutoReconnect();
    void stopAutoReconnect();
    void resetReconnectAttempts();
    void captureChannelsForReconnect();
    void rejoinPendingChannelsIfAny();
    void updateSearchResults(const QString& query);
    void jumpToSearchResult(int index);
    QString currentConversationKey() const;
    QString normalizeChannelName(const QString& channel) const;
    bool joinChannelFromInput(const QString& rawChannel, const QString& emptyErrorMessage);
    bool partChannelFromInput(const QString& rawChannel, const QString& emptyErrorMessage);
    void setCurrentConversation(const QString& conversation);
    void removeConversation(const QString& channel, const QString& systemMessage, bool removeFromConfig);
    bool handleSlashCommand(const QString& message);

    void setTheme(bool useThemeB);

    // UI Components
    QWidget* m_centralWidget = nullptr;
    
    // Connection panel components
    QWidget* m_connectionPanel = nullptr;
    QPushButton* m_connectionPanelToggleButton = nullptr;
    bool m_connectionPanelExpanded = true;
    QComboBox* m_serverPreset = nullptr;
    QCheckBox* m_tlsCheckBox = nullptr;
    QLineEdit* m_serverInput = nullptr;
    QSpinBox* m_portInput = nullptr;
    QLineEdit* m_nickInput = nullptr;
    QCheckBox* m_autoReconnectCheckBox = nullptr;
    QCheckBox* m_themeToggleCheckBox = nullptr;
    QPushButton* m_connectButton = nullptr;
    QPushButton* m_disconnectButton = nullptr;
    QLabel* m_connectionStatusLabel = nullptr;
    QLabel* m_autoReconnectIndicatorLabel = nullptr;
    QLabel* m_registrationIndicatorLabel = nullptr;
    
    // Main UI components
    QListView* m_channelList = nullptr;           // Left panel: channels
    QLineEdit* m_joinChannelInput = nullptr;      // Left panel: join target
    QPushButton* m_joinChannelButton = nullptr;
    QPushButton* m_removeChannelButton = nullptr;
    QTextEdit* m_chatDisplay = nullptr;           // Center: message history
    QListView* m_userList = nullptr;              // Right panel: users
    QLineEdit* m_messageInput = nullptr;          // Bottom: message input
    QLineEdit* m_searchInput = nullptr;           // Search current buffer
    QPushButton* m_searchPrevButton = nullptr;
    QPushButton* m_searchNextButton = nullptr;
    QLabel* m_searchStatusLabel = nullptr;
    QLabel* m_statusBar = nullptr;
    QLabel* m_topicLabel = nullptr;               // Channel topic display
    
    // Models
    std::unique_ptr<ChannelModel> m_channelModel;
    std::unique_ptr<UserModel> m_userModel;
    
    // Connection
    std::unique_ptr<IrcConnection> m_connection;
    
    // Configuration
    std::unique_ptr<ConfigManager> m_configManager;
    
    // Auto-reconnect state
    std::unique_ptr<QTimer> m_reconnectTimer;
    int m_reconnectAttempts = 0;
    int m_maxReconnectAttempts = 10;  // Max attempts before giving up
    bool m_autoReconnectEnabled = true;  // User can disable if needed
    bool m_manualDisconnectRequested = false;
    QStringList m_pendingRejoinChannels;
    QString m_lastConnectedServer;
    quint16 m_lastConnectedPort = 0;
    bool m_lastConnectedUseTls = false;
    QString m_searchQuery;
    QVector<int> m_searchMatches;
    int m_searchMatchIndex = -1;
    
    // State
    bool m_isConnected;
    bool m_registrationRequired;
    bool m_registrationComplete;
    bool m_useThemeB = false;
    QString m_currentChannel;
    QString m_currentNickname;
    QMap<QString, QStringList> m_conversationHistory;
};
