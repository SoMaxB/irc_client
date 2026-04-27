#include "MainWindow.hpp"
#include "MessageParser.hpp"
#include "IrcNumericHandler.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QSplitter>
#include <QAbstractItemView>
#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QFont>
#include <QDebug>
#include <QDateTime>
#include <QIcon>
#include <QMessageBox>
#include <QSizePolicy>
#include <QSystemTrayIcon>

namespace {

bool isChannelTarget(const QString& name) {
    return name.startsWith('#') ||
           name.startsWith('&') ||
           name.startsWith('+') ||
           name.startsWith('!');
}

}

namespace ThemeColors {

struct Palette {
    QString name;
    QString background;
    QString backgroundSecondary;
    QString inputBg;
    QString border;
    QString text;
    QString textSecondary;
    QString accentPrimary;
    QString accentHover;
    QString accentPressed;
    QString accentSecondary;
    QString success;
    QString successBg;
    QString warning;
    QString warningBg;
    QString error;
    QString errorBg;
    QString highlight;
    QString splitter;
    int borderRadius;
    int borderRadiusSmall;
    int splitterWidth;
};

Palette darkDefault() {
    Palette p;
    p.name = "Dark Blue";
    p.background = "#121417";
    p.backgroundSecondary = "#171a21";
    p.inputBg = "#20242d";
    p.border = "#303644";
    p.text = "#f2f4f8";
    p.textSecondary = "#94a3b8";
    p.accentPrimary = "#2b5cff";
    p.accentHover = "#3b6bff";
    p.accentPressed = "#234bd1";
    p.accentSecondary = "#263042";
    p.success = "#9be7b4";
    p.successBg = "#123524";
    p.warning = "#f0c080";
    p.warningBg = "#3a2a10";
    p.error = "#f1b3b3";
    p.errorBg = "#3a1f1f";
    p.highlight = "#27406f";
    p.splitter = "#252a35";
    p.borderRadius = 12;
    p.borderRadiusSmall = 8;
    p.splitterWidth = 6;
    return p;
}

Palette githubDark() {
    Palette p;
    p.name = "GitHub Dark";
    p.background = "#0d1117";
    p.backgroundSecondary = "#161b22";
    p.inputBg = "#21262d";
    p.border = "#30363d";
    p.text = "#f0f6fc";
    p.textSecondary = "#8b949e";
    p.accentPrimary = "#58a6ff";
    p.accentHover = "#79b8ff";
    p.accentPressed = "#388bfd";
    p.accentSecondary = "#21262d";
    p.success = "#3fb950";
    p.successBg = "rgba(63, 185, 80, 0.15)";
    p.warning = "#d29922";
    p.warningBg = "rgba(210, 153, 34, 0.15)";
    p.error = "#f85149";
    p.errorBg = "rgba(248, 81, 73, 0.15)";
    p.highlight = "#1f6feb";
    p.splitter = "#30363d";
    p.borderRadius = 12;
    p.borderRadiusSmall = 8;
    p.splitterWidth = 6;
    return p;
}

Palette dracula() {
    Palette p;
    p.name = "Dracula";
    p.background = "#282a36";
    p.backgroundSecondary = "#343746";
    p.inputBg = "#44475a";
    p.border = "#6272a4";
    p.text = "#f8f8f2";
    p.textSecondary = "#bfbfbf";
    p.accentPrimary = "#bd93f9";
    p.accentHover = "#ff79c6";
    p.accentPressed = "#8be9fd";
    p.accentSecondary = "#44475a";
    p.success = "#50fa7b";
    p.successBg = "rgba(80, 250, 123, 0.15)";
    p.warning = "#f1fa8c";
    p.warningBg = "rgba(241, 250, 140, 0.15)";
    p.error = "#ff5555";
    p.errorBg = "rgba(255, 85, 85, 0.15)";
    p.highlight = "#6272a4";
    p.splitter = "#44475a";
    p.borderRadius = 12;
    p.borderRadiusSmall = 8;
    p.splitterWidth = 6;
    return p;
}

Palette monokai() {
    Palette p;
    p.name = "Monokai";
    p.background = "#272822";
    p.backgroundSecondary = "#3e3d32";
    p.inputBg = "#49483e";
    p.border = "#75715e";
    p.text = "#f8f8f2";
    p.textSecondary = "#a59f85";
    p.accentPrimary = "#f92672";
    p.accentHover = "#fd5b51";
    p.accentPressed = "#ae81ff";
    p.accentSecondary = "#49483e";
    p.success = "#a6e22e";
    p.successBg = "rgba(166, 226, 46, 0.15)";
    p.warning = "#e6db74";
    p.warningBg = "rgba(230, 219, 116, 0.15)";
    p.error = "#f92672";
    p.errorBg = "rgba(249, 38, 114, 0.15)";
    p.highlight = "#49483e";
    p.splitter = "#49483e";
    p.borderRadius = 12;
    p.borderRadiusSmall = 8;
    p.splitterWidth = 6;
    return p;
}

Palette solarizedDark() {
    Palette p;
    p.name = "Solarized Dark";
    p.background = "#002b36";
    p.backgroundSecondary = "#073642";
    p.inputBg = "#094350";
    p.border = "#586e75";
    p.text = "#93a1a1";
    p.textSecondary = "#657b83";
    p.accentPrimary = "#268bd2";
    p.accentHover = "#2aa7f3";
    p.accentPressed = "#1c6ea8";
    p.accentSecondary = "#094350";
    p.success = "#859900";
    p.successBg = "rgba(133, 153, 0, 0.15)";
    p.warning = "#b58900";
    p.warningBg = "rgba(181, 137, 0, 0.15)";
    p.error = "#dc322f";
    p.errorBg = "rgba(220, 50, 47, 0.15)";
    p.highlight = "#2aa7f3";
    p.splitter = "#073642";
    p.borderRadius = 12;
    p.borderRadiusSmall = 8;
    p.splitterWidth = 6;
    return p;
}

Palette cyberpunk() {
    Palette p;
    p.name = "Cyberpunk";
    p.background = "#0d0d0d";
    p.backgroundSecondary = "#1a1a1a";
    p.inputBg = "#262626";
    p.border = "#ff00ff";
    p.text = "#ffffff";
    p.textSecondary = "#bfbfbf";
    p.accentPrimary = "#ff00ff";
    p.accentHover = "#ff66ff";
    p.accentPressed = "#cc00cc";
    p.accentSecondary = "#262626";
    p.success = "#00ff00";
    p.successBg = "rgba(0, 255, 0, 0.15)";
    p.warning = "#ffff00";
    p.warningBg = "rgba(255, 255, 0, 0.15)";
    p.error = "#ff0000";
    p.errorBg = "rgba(255, 0, 0, 0.15)";
    p.highlight = "#00ffff";
    p.splitter = "#333333";
    p.borderRadius = 12;
    p.borderRadiusSmall = 8;
    p.splitterWidth = 6;
    return p;
}

QVector<Palette> allPalettes() {
    return QVector<Palette>{darkDefault(), githubDark(), dracula(), monokai(), solarizedDark(), cyberpunk()};
}

Palette getPalette(int index) {
    QVector<Palette> themes = allPalettes();
    if (index < 0 || index >= themes.size()) {
        return darkDefault();
    }
    return themes[index];
}

}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_centralWidget(new QWidget(this)),
      m_channelModel(std::make_unique<ChannelModel>(this)),
      m_userModel(std::make_unique<UserModel>(this)),
      m_connection(std::make_unique<IrcConnection>(this)),
      m_configManager(std::make_unique<ConfigManager>()),
      m_reconnectTimer(std::make_unique<QTimer>(this)),
      m_isConnected(false),
      m_registrationRequired(false),
      m_registrationComplete(false) {
    
    setWindowTitle("IRC Client");
    setWindowIcon(QIcon()); // TODO: Add icon
    resize(1200, 750);
    m_reconnectTimer->setSingleShot(true);
    setCentralWidget(m_centralWidget);
    setupTrayIcon();
    setupUI();
    loadConfigurationSettings();
    setupConnections();
    applyStyle();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event) {
    m_manualDisconnectRequested = true;
    stopAutoReconnect();
    if (m_isConnected) {
        m_connection->sendQuit("Client closing");
        m_connection->disconnect();
    }
    QMainWindow::closeEvent(event);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == m_autoReconnectIndicatorLabel && event->type() == QEvent::MouseButtonPress) {
        m_autoReconnectEnabled = !m_autoReconnectEnabled;
        m_configManager->saveAutoReconnectEnabled(m_autoReconnectEnabled);
        updateAutoReconnectIndicator();
        if (!m_autoReconnectEnabled) {
            stopAutoReconnect();
            if (m_connectionStatusLabel && !m_connection->isConnected()) {
                m_connectionStatusLabel->setText("Auto-reconnect disabled");
            }
        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::setupConnections() {
    connect(m_connection.get(), &IrcConnection::connected,
            this, &MainWindow::onConnected);
    connect(m_connection.get(), &IrcConnection::disconnected,
            this, &MainWindow::onDisconnected);
    connect(m_connection.get(), &IrcConnection::connectionError,
            this, &MainWindow::onConnectionError);
    connect(m_connection.get(), &IrcConnection::messageReceived,
            this, &MainWindow::onMessageReceived);
    
    // Auto-reconnect timer
    connect(m_reconnectTimer.get(), &QTimer::timeout,
            this, &MainWindow::onReconnectAttempt);
}

void MainWindow::setupUI() {
    createMainUI();
    updateConnectionUi(false);
}

void MainWindow::applyStyle() {
    ThemeColors::Palette p = ThemeColors::getPalette(m_themeIndex);
    QString style = QString(R"(
        QMainWindow {
            background: %1;
            color: %2;
        }
        QDialog {
            background: %3;
            color: %2;
        }
        QLabel {
            color: %2;
        }
        QLineEdit {
            background: %4;
            color: %2;
            border: 1px solid %5;
            border-radius: %6px;
            padding: 8px 10px;
            selection-background-color: %7;
        }
        QLineEdit:focus {
            border: 1px solid %7;
        }
        QPushButton {
            background: %8;
            color: white;
            border: none;
            border-radius: %6px;
            padding: 8px 14px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: %9;
        }
        QPushButton:pressed {
            background: %10;
        }
        QListView, QTextEdit {
            background: %3;
            color: %2;
            border: 1px solid %5;
            border-radius: %6px;
        }
        QListView::item {
            padding: 8px 10px;
        }
        QListView::item:selected {
            background: %11;
            color: white;
        }
        QTextEdit {
            padding: 10px;
        }
        QSplitter::handle {
            background: %12;
        }
    )")
        .arg(p.background)
        .arg(p.text)
        .arg(p.backgroundSecondary)
        .arg(p.inputBg)
        .arg(p.border)
        .arg(p.borderRadius)
        .arg(p.accentPrimary)
        .arg(p.accentPrimary)
        .arg(p.accentHover)
        .arg(p.accentPressed)
        .arg(p.highlight)
        .arg(p.splitter);

    setStyleSheet(style);
}

void MainWindow::createMainUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    QWidget* connectionToggleRow = new QWidget(m_centralWidget);
    QHBoxLayout* connectionToggleLayout = new QHBoxLayout(connectionToggleRow);
    connectionToggleLayout->setContentsMargins(0, 0, 0, 0);
    connectionToggleLayout->addStretch();

    m_connectionPanelToggleButton = new QPushButton(connectionToggleRow);
    m_connectionPanelToggleButton->setFixedSize(34, 24);
    m_connectionPanelToggleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_connectionPanelToggleButton->setStyleSheet(
        "QPushButton {"
        "  border-radius: 12px;"
        "  padding: 0px;"
        "  font-weight: 700;"
        "  background: #263042;"
        "  color: #dbe4f3;"
        "}"
        "QPushButton:hover {"
        "  background: #33405a;"
        "}"
    );
    connectionToggleLayout->addWidget(m_connectionPanelToggleButton);
    connectionToggleLayout->addStretch();
    mainLayout->addWidget(connectionToggleRow);

    m_connectionPanel = new QWidget(m_centralWidget);
    QFormLayout* connectionLayout = new QFormLayout(m_connectionPanel);
    connectionLayout->setContentsMargins(0, 0, 0, 0);
    connectionLayout->setHorizontalSpacing(12);
    connectionLayout->setVerticalSpacing(8);

    QLabel* connectionTitle = new QLabel("Connection");
    QFont titleFont = connectionTitle->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    connectionTitle->setFont(titleFont);

    m_serverPreset = new QComboBox(m_connectionPanel);
    m_serverPreset->addItem("Custom");
    m_serverPreset->addItem("Libera.Chat TLS (recommended)", QVariantList{QString("irc.libera.chat"), 6697, true});
    m_serverPreset->addItem("OFTC TLS", QVariantList{QString("irc.oftc.net"), 6697, true});
    m_serverPreset->addItem("Libera.Chat plain", QVariantList{QString("irc.libera.chat"), 6667, false});
    m_serverPreset->addItem("OFTC plain", QVariantList{QString("irc.oftc.net"), 6667, false});

    m_tlsCheckBox = new QCheckBox("Use TLS", m_connectionPanel);

    m_serverInput = new QLineEdit(m_connectionPanel);
    m_serverInput->setPlaceholderText("irc.libera.chat");
    m_serverInput->setText("irc.libera.chat");

    m_portInput = new QSpinBox(m_connectionPanel);
    m_portInput->setRange(1, 65535);
    m_portInput->setValue(6667);
    m_portInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_portInput->setMaximumWidth(110);

    m_nickInput = new QLineEdit(m_connectionPanel);
    m_nickInput->setPlaceholderText("YourNick");
    m_nickInput->setText("QtClient");

    m_themeComboBox = new QComboBox(m_connectionPanel);
    m_themeComboBox->setFixedWidth(180);
    QVector<ThemeColors::Palette> themes = ThemeColors::allPalettes();
    for (const auto& theme : themes) {
        m_themeComboBox->addItem(theme.name);
    }
    m_themeComboBox->setCurrentIndex(m_themeIndex);

    connect(m_themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        setTheme(index);
        m_configManager->saveThemePreference(index);
    });

    m_connectButton = new QPushButton("Connect", m_connectionPanel);
    m_connectionStatusLabel = new QLabel("Disconnected", m_connectionPanel);
    m_autoReconnectIndicatorLabel = new QLabel(m_connectionPanel);
    m_registrationIndicatorLabel = new QLabel(m_connectionPanel);
    m_notificationsCheckBox = new QCheckBox("Notifications", m_connectionPanel);
    m_notificationsCheckBox->setChecked(m_notificationsEnabled);
    connect(m_notificationsCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        m_notificationsEnabled = checked;
    });

    connect(m_connectButton, &QPushButton::clicked, this, &MainWindow::onConnect);

    connect(m_serverPreset, &QComboBox::currentIndexChanged, this, [this](int) {
        const QVariantList values = m_serverPreset->currentData().toList();
        if (values.size() >= 3) {
            m_serverInput->setText(values[0].toString());
            m_portInput->setValue(values[1].toInt());
            m_tlsCheckBox->setChecked(values[2].toBool());
            m_serverInput->setEnabled(false);
            m_portInput->setEnabled(false);
            m_tlsCheckBox->setEnabled(false);
        } else {
            m_serverInput->setEnabled(true);
            m_portInput->setEnabled(true);
            m_tlsCheckBox->setEnabled(true);
        }
    });

    connectionLayout->addRow(connectionTitle);

    QWidget* presetThemeRow = new QWidget(m_connectionPanel);
    QHBoxLayout* presetThemeLayout = new QHBoxLayout(presetThemeRow);
    presetThemeLayout->setContentsMargins(0, 0, 0, 0);
    presetThemeLayout->setSpacing(8);
    presetThemeLayout->addWidget(m_serverPreset, 1);
    presetThemeLayout->addWidget(new QLabel("Theme:", m_connectionPanel));
    presetThemeLayout->addWidget(m_themeComboBox);
    connectionLayout->addRow("Preset:", presetThemeRow);

    QWidget* serverPortRow = new QWidget(m_connectionPanel);
    QHBoxLayout* serverPortLayout = new QHBoxLayout(serverPortRow);
    serverPortLayout->setContentsMargins(0, 0, 0, 0);
    serverPortLayout->setSpacing(8);
    serverPortLayout->addWidget(m_serverInput, 1);
    serverPortLayout->addWidget(m_portInput);
    serverPortLayout->addWidget(m_tlsCheckBox);
    connectionLayout->addRow("Server / Port / TLS:", serverPortRow);

    connectionLayout->addRow("Nickname:", m_nickInput);

    m_serverPreset->setCurrentIndex(0);
    updateAutoReconnectIndicator();
    updateRegistrationIndicator();

    mainLayout->addWidget(m_connectionPanel);

    connect(m_connectionPanelToggleButton, &QPushButton::clicked, this, [this]() {
        setConnectionPanelExpanded(!m_connectionPanelExpanded);
    });
    setConnectionPanelExpanded(true);

    // Connection status bar (outside connection panel)
    QHBoxLayout* connectionStatusBar = new QHBoxLayout();
    connectionStatusBar->setSpacing(12);
    connectionStatusBar->addWidget(m_connectButton);
    connectionStatusBar->addWidget(m_connectionStatusLabel);
    connectionStatusBar->addStretch();
    connectionStatusBar->addWidget(m_autoReconnectIndicatorLabel);
    connectionStatusBar->addWidget(m_registrationIndicatorLabel);
    connectionStatusBar->addWidget(m_notificationsCheckBox);
    mainLayout->addLayout(connectionStatusBar);

    // Top bar with topic
    m_topicLabel = new QLabel("Not connected", m_centralWidget);
    m_topicLabel->setStyleSheet("background-color: #0f1117; padding: 8px 10px; border-radius: 8px;");
    mainLayout->addWidget(m_topicLabel);

    // Main content: 3-pane layout
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(8);

    // Left: Channel list
    m_channelList = new QListView(this);
    m_channelList->setModel(m_channelModel.get());
    m_channelList->setMaximumWidth(200);
    m_channelList->setMinimumWidth(150);
    m_channelList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_channelList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_channelList->setAlternatingRowColors(true);
    connect(m_channelList, &QListView::clicked, this, &MainWindow::onChannelClicked);
    splitter->addWidget(m_channelList);

    QWidget* channelControls = new QWidget(this);
    QHBoxLayout* channelControlLayout = new QHBoxLayout(channelControls);
    channelControlLayout->setContentsMargins(0, 0, 0, 0);
    channelControlLayout->setSpacing(12);

    m_joinChannelInput = new QLineEdit(this);
    m_joinChannelInput->setPlaceholderText("#channel");

    m_joinChannelButton = new QPushButton("Join", this);
    m_removeChannelButton = new QPushButton("Remove", this);
    m_removeChannelButton->setEnabled(false);

    connect(m_joinChannelButton, &QPushButton::clicked, this, &MainWindow::onJoinChannelRequested);
    connect(m_removeChannelButton, &QPushButton::clicked, this, &MainWindow::onRemoveChannelRequested);
    connect(m_channelList->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex& current, const QModelIndex&) {
        m_removeChannelButton->setEnabled(current.isValid() || m_channelModel->rowCount() > 0);
    });

    channelControlLayout->addWidget(m_joinChannelInput, 1);
    channelControlLayout->addWidget(m_joinChannelButton);
    channelControlLayout->addWidget(m_removeChannelButton);
    mainLayout->addWidget(channelControls);

    // Center: Chat display
    m_chatDisplay = new QTextEdit(this);
    m_chatDisplay->setReadOnly(true);
    m_chatDisplay->setAcceptRichText(false);
    m_chatDisplay->setStyleSheet("font-family: 'DejaVu Sans Mono', monospace; font-size: 11pt;");
    splitter->addWidget(m_chatDisplay);

    // Right: User list
    m_userList = new QListView(this);
    m_userList->setModel(m_userModel.get());
    m_userList->setMaximumWidth(150);
    m_userList->setMinimumWidth(120);
    m_userList->setSelectionMode(QAbstractItemView::NoSelection);
    m_userList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    splitter->addWidget(m_userList);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    splitter->setStretchFactor(2, 1);

    mainLayout->addWidget(splitter, 1);

    // Bottom: Message input bar
    QHBoxLayout* inputLayout = new QHBoxLayout();
    m_messageInput = new QLineEdit(this);
    m_messageInput->setPlaceholderText("Type a message and press Enter...");
    m_messageInput->setMinimumHeight(40);
    m_messageInput->setClearButtonEnabled(true);
    connect(m_messageInput, &QLineEdit::returnPressed, this, &MainWindow::onMessageSent);

    inputLayout->addWidget(m_messageInput);
    mainLayout->addLayout(inputLayout);

    // Status bar
    m_statusBar = new QLabel(this);
    m_statusBar->setText("Disconnected. Enter a server above and connect.");
    m_statusBar->setStyleSheet("border-top: 1px solid #2a3040; padding: 8px 10px; color: #94a3b8;");
    mainLayout->addWidget(m_statusBar);

    m_centralWidget->setLayout(mainLayout);

    m_channelList->setEnabled(false);
    m_userList->setEnabled(false);
    m_chatDisplay->setEnabled(false);
    m_messageInput->setEnabled(false);
}

void MainWindow::updateConnectionUi(bool connected) {
    m_isConnected = connected;
    m_connectButton->setText(connected ? "Disconnect" : "Connect");
    m_connectButton->setEnabled(true);
    m_serverInput->setEnabled(!connected);
    m_portInput->setEnabled(!connected);
    m_nickInput->setEnabled(!connected);
    m_tlsCheckBox->setEnabled(!connected);
    m_channelList->setEnabled(connected);
    updateSendControls();
    m_userList->setEnabled(connected);
    m_chatDisplay->setEnabled(connected);
    m_channelList->setEnabled(connected);
    updateSendControls();
    if (m_topicLabel) {
        m_topicLabel->setText(connected ? "Connected" : "Not connected");
    }

    ThemeColors::Palette p = ThemeColors::getPalette(m_themeIndex);
    QString buttonColor = connected ? p.accentSecondary : p.accentPrimary;
    QString buttonHover = connected ? p.accentSecondary : p.accentHover;
    m_connectButton->setStyleSheet(QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: %2px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: %3;"
        "}"
        "QPushButton:pressed {"
        "   background-color: %4;"
        "}"
        "QPushButton:disabled {"
        "   background-color: %5;"
        "   color: %6;"
        "}"
    ).arg(buttonColor).arg(p.borderRadiusSmall).arg(buttonHover).arg(p.accentPressed).arg(p.backgroundSecondary).arg(p.textSecondary));
}

void MainWindow::updateAutoReconnectIndicator() {
    if (!m_autoReconnectIndicatorLabel) {
        return;
    }

    ThemeColors::Palette p = ThemeColors::getPalette(m_themeIndex);

    if (m_autoReconnectEnabled) {
        m_autoReconnectIndicatorLabel->setText("Auto-reconnect: ON");
        m_autoReconnectIndicatorLabel->setStyleSheet(QString(
            "padding: 4px 12px; border-radius: %1px; background: %2; color: %3; font-weight: 600; cursor: pointer;"
        ).arg(p.borderRadiusSmall).arg(p.successBg).arg(p.success));
    } else {
        m_autoReconnectIndicatorLabel->setText("Auto-reconnect: OFF");
        m_autoReconnectIndicatorLabel->setStyleSheet(QString(
            "padding: 4px 12px; border-radius: %1px; background: %2; color: %3; font-weight: 600; cursor: pointer;"
        ).arg(p.borderRadiusSmall).arg(p.errorBg).arg(p.error));
    }

    m_autoReconnectIndicatorLabel->installEventFilter(this);
}

void MainWindow::updateRegistrationIndicator() {
    if (!m_registrationIndicatorLabel) {
        return;
    }

    ThemeColors::Palette p = ThemeColors::getPalette(m_themeIndex);

    if (!m_isConnected) {
        m_registrationIndicatorLabel->setText("");
        m_registrationIndicatorLabel->setStyleSheet("");
        return;
    }

    if (m_registrationComplete) {
        m_registrationIndicatorLabel->setText("Registered");
        m_registrationIndicatorLabel->setStyleSheet(QString("padding: 2px 8px; border-radius: %1px; background: %2; color: %3; font-weight: 600;")
            .arg(p.borderRadiusSmall).arg(p.successBg).arg(p.success));
    } else if (m_registrationRequired) {
        m_registrationIndicatorLabel->setText("Registration required");
        m_registrationIndicatorLabel->setStyleSheet(QString("padding: 2px 8px; border-radius: %1px; background: %2; color: %3; font-weight: 600;")
            .arg(p.borderRadiusSmall).arg(p.warningBg).arg(p.warning));
    } else {
        m_registrationIndicatorLabel->setText("Registering...");
        m_registrationIndicatorLabel->setStyleSheet(QString("padding: 2px 8px; border-radius: %1px; background: %2; color: %3; font-weight: 600;")
            .arg(p.borderRadiusSmall).arg(p.backgroundSecondary).arg(p.textSecondary));
    }
}

void MainWindow::updateSendControls() {
    if (!m_messageInput) {
        return;
    }

    m_messageInput->setEnabled(m_isConnected);
}

void MainWindow::setConnectionPanelExpanded(bool expanded) {
    m_connectionPanelExpanded = expanded;

    if (m_connectionPanel) {
        m_connectionPanel->setVisible(expanded);
    }

    if (m_connectionPanelToggleButton) {
        if (expanded) {
            m_connectionPanelToggleButton->setText("^");
            m_connectionPanelToggleButton->setToolTip("Hide connection panel");
        } else {
            m_connectionPanelToggleButton->setText("v");
            m_connectionPanelToggleButton->setToolTip("Show connection panel");
        }
    }
}

void MainWindow::onConnect() {
    if (m_isConnected) {
        onDisconnect();
        return;
    }

    QString server = m_serverInput->text().trimmed();
    quint16 port = static_cast<quint16>(m_portInput->value());
    bool useTls = m_tlsCheckBox->isChecked();
    m_currentNickname = m_nickInput->text().trimmed();

    if (server.isEmpty() || m_currentNickname.isEmpty()) {
        m_connectionStatusLabel->setText("Server and nickname are required");
        return;
    }

    // Save connection settings for next session
    m_configManager->saveLastConnection(server, port, m_currentNickname, useTls);
    
    // Save for auto-reconnect
    m_lastConnectedServer = server;
    m_lastConnectedPort = port;
    m_lastConnectedUseTls = useTls;
    m_manualDisconnectRequested = false;
    resetReconnectAttempts();

    m_connectionStatusLabel->setText("Connecting...");
    if (!m_connection->connect(server, port, useTls)) {
        onConnectionError("Failed to connect: " + server + ":" + QString::number(port));
        return;
    }

    m_connection->sendNick(m_currentNickname);
    m_connection->sendUser(m_currentNickname, "Qt IRC Client");
}

void MainWindow::onDisconnect() {
    m_manualDisconnectRequested = true;
    m_pendingRejoinChannels.clear();
    stopAutoReconnect();
    setConnectionPanelExpanded(true);
    if (m_connection->isConnected()) {
        m_connection->sendQuit("Client disconnecting");
        m_connection->disconnect();
    }
    updateConnectionUi(false);
    m_connectionStatusLabel->setText("Disconnected");
    if (m_statusBar) {
        m_statusBar->setText("Disconnected. Enter a server above and connect.");
    }
}

void MainWindow::onJoinChannelRequested() {
    if (joinChannelFromInput(m_joinChannelInput->text(), "Enter a channel name, for example #libera")) {
        m_joinChannelInput->clear();
    }
}

void MainWindow::onRemoveChannelRequested() {
    const QStringList channelNames = m_channelModel->getChannelNames();
    if (channelNames.isEmpty()) {
        m_statusBar->setText("Select a channel to remove it from the list");
        return;
    }

    QString channel;
    const QModelIndex currentIndex = m_channelList->currentIndex();
    if (currentIndex.isValid() && currentIndex.row() >= 0 && currentIndex.row() < channelNames.size()) {
        channel = channelNames[currentIndex.row()];
    } else if (!m_currentChannel.isEmpty() && channelNames.contains(m_currentChannel)) {
        channel = m_currentChannel;
    } else if (channelNames.size() == 1) {
        channel = channelNames.first();
    } else {
        m_statusBar->setText("Select a channel to remove it from the list");
        return;
    }

    if (m_isConnected) {
        const QMessageBox::StandardButton answer = QMessageBox::question(
            this,
            "Leave Channel",
            "You are connected. Remove and leave channel " + channel + "?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

        if (answer != QMessageBox::Yes) {
            return;
        }

        m_connection->sendPart(channel);
    }

    removeConversation(channel, "Removed " + channel + " from the list", true);
    m_removeChannelButton->setEnabled(m_channelModel->rowCount() > 0);
}

void MainWindow::onConnected() {
    m_manualDisconnectRequested = false;
    stopAutoReconnect();
    setConnectionPanelExpanded(false);
    updateConnectionUi(true);
    m_registrationRequired = true;
    m_registrationComplete = false;
    updateRegistrationIndicator();
    const QString scheme = m_connection->isTlsEnabled() ? "TLS" : "plain";
    m_connectionStatusLabel->setText("Connected to " + m_connection->host() + ":" + QString::number(m_connection->port()) + " (" + scheme + ")");
    if (m_statusBar) {
        m_statusBar->setText("Connected. Join a channel to start chatting.");
    }
    rejoinPendingChannelsIfAny();
    qDebug() << "Connected signal received";
}

void MainWindow::onDisconnected() {
    updateConnectionUi(false);
    m_registrationRequired = false;
    m_registrationComplete = false;
    updateRegistrationIndicator();
    if (m_connectionStatusLabel) {
        m_connectionStatusLabel->setText("Disconnected");
    }
    if (m_statusBar) {
        m_statusBar->setText("Disconnected. Enter a server above and connect.");
    }
    qDebug() << "Disconnected from server";

    if (m_manualDisconnectRequested) {
        m_manualDisconnectRequested = false;
        return;
    }

    // Snapshot channels before the reconnect cycle mutates local state.
    captureChannelsForReconnect();
    
    // Start auto-reconnect if enabled and credentials are available
    if (m_autoReconnectEnabled && !m_lastConnectedServer.isEmpty()) {
        startAutoReconnect();
    }
}

void MainWindow::onConnectionError(const QString& error) {
    qWarning() << "Connection error:" << error;
    updateConnectionUi(false);
    if (m_connectionStatusLabel) {
        m_connectionStatusLabel->setText(error);
    }
    if (m_statusBar) {
        m_statusBar->setText("Error: " + error);
    }
}

void MainWindow::onMessageReceived(const QString& rawMessage) {
    qDebug() << "Message received:" << rawMessage;

    IrcMessage msg = MessageParser::parse(rawMessage);
    if (!msg.isValid()) {
        return;
    }

    if (handleServerPing(msg)) {
        return;
    }

    if (handleNumericMessage(msg)) {
        return;
    }

    const QString senderNick = extractNickFromPrefix(msg.source);

    // Handle NOTICE from server (registration prompts, info messages)
    if (msg.command == "NOTICE") {
        QString target = msg.params.value(0, QString());
        QString content = !msg.trailing.isEmpty() ? msg.trailing : msg.params.value(1, QString());

        if (content.isEmpty()) {
            return;
        }

        if (target.isEmpty() || target.compare("AUTH", Qt::CaseInsensitive) == 0) {
            QString displaySender = senderNick.isEmpty() ? QString("server") : senderNick;
            appendConversationLine("*", QString("-[%1] %2").arg(displaySender, content));
        } else if (!m_currentNickname.isEmpty() && target.compare(m_currentNickname, Qt::CaseInsensitive) == 0) {
            displayMessage(senderNick, senderNick, content);
        } else {
            displayMessage(target, senderNick, content);
        }
        return;
    }

    // Handle different message types
    if (msg.command == "PRIVMSG") {
        if (msg.params.size() >= 2) {
            QString target = msg.params[0];
            QString content = msg.params[1];
            QString conversation = target;
            bool isActionMessage = false;

            if (content.startsWith(QChar(1) + QStringLiteral("ACTION ")) && content.endsWith(QChar(1))) {
                content = content.mid(8, content.size() - 9);
                isActionMessage = true;
            }

            // Direct messages are addressed to our nick; route them to sender's query buffer.
            if (!m_currentNickname.isEmpty() && target.compare(m_currentNickname, Qt::CaseInsensitive) == 0) {
                conversation = senderNick;
            }

            if (!conversation.isEmpty() && !m_channelModel->hasChannel(conversation)) {
                m_channelModel->addChannel(conversation);
            }

            if (!conversation.isEmpty() && conversation != m_currentChannel) {
                m_channelModel->incrementUnread(conversation);

                // Show notification for mentions or private messages
                if (m_notificationsEnabled && !m_currentNickname.isEmpty()) {
                    const bool isMention = content.contains(m_currentNickname, Qt::CaseInsensitive);
                    const bool isPrivate = (target.compare(m_currentNickname, Qt::CaseInsensitive) == 0);

                    if (isPrivate) {
                        showNotification("Private message from " + senderNick, content);
                    } else if (isMention) {
                        showNotification("Mentioned in " + conversation, senderNick + ": " + content);
                    }
                }
            }

            if (isActionMessage) {
                displayMessage(conversation, senderNick, "* " + content);
            } else {
                displayMessage(conversation, senderNick, content);
            }
        }
    } else if (msg.command == "JOIN") {
        if (msg.params.isEmpty()) {
            return;
        }

        QString channel = msg.params[0];
        if (senderNick.compare(m_currentNickname, Qt::CaseInsensitive) == 0 || m_channelModel->hasChannel(channel)) {
            m_channelModel->addChannel(channel);
            m_channelModel->addUserToChannel(channel, senderNick);
            if (channel == m_currentChannel) {
                updateUserList();
            }
        }
    } else if (msg.command == "PART" || msg.command == "QUIT") {
        if (msg.command == "PART" && msg.params.size() > 0) {
            QString channel = msg.params[0];
            if (senderNick.compare(m_currentNickname, Qt::CaseInsensitive) == 0) {
                m_channelModel->removeChannel(channel);
                m_conversationHistory.remove(channel);
                if (channel == m_currentChannel) {
                    m_currentChannel.clear();
                    refreshCurrentConversationView();
                    updateChannelInfo();
                    updateUserList();
                }
            } else {
                m_channelModel->removeUserFromChannel(channel, senderNick);
                if (channel == m_currentChannel) {
                    updateUserList();
                }
            }
        } else if (msg.command == "QUIT") {
            const QStringList channels = m_channelModel->getChannelNames();
            for (const QString& channel : channels) {
                if (isChannelTarget(channel)) {
                    m_channelModel->removeUserFromChannel(channel, senderNick);
                }
            }
            updateUserList();
        }
    } else if (msg.command == "TOPIC") {
        if (msg.params.size() >= 2) {
            QString channel = msg.params[0];
            QString topic = msg.params[1];
            m_channelModel->setChannelTopic(channel, topic);
            if (channel == m_currentChannel) {
                m_topicLabel->setText("Topic: " + topic);
            }
        }
    }
}

bool MainWindow::handleServerPing(const IrcMessage& msg) {
    if (msg.command != "PING") {
        return false;
    }

    const QString token = !msg.trailing.isEmpty()
        ? msg.trailing
        : (msg.params.isEmpty() ? QString() : msg.params.first());

    if (!token.isEmpty()) {
        m_connection->sendCommand("PONG :" + token);
    } else {
        m_connection->sendCommand("PONG");
    }
    return true;
}

bool MainWindow::handleNumericMessage(const IrcMessage& msg) {
    IrcNumericHandler::Dependencies deps;
    deps.channelModel = m_channelModel.get();
    deps.userModel = m_userModel.get();
    deps.connection = m_connection.get();
    deps.nickInput = m_nickInput;
    deps.statusBar = m_statusBar;
    deps.topicLabel = m_topicLabel;
    deps.currentNickname = &m_currentNickname;
    deps.currentChannel = &m_currentChannel;
    deps.appendSystemMessage = [this](const QString& text) {
        appendSystemMessage(text);
    };
    deps.removeConversation = [this](const QString& channel, const QString& systemMessage, bool removeFromConfig) {
        removeConversation(channel, systemMessage, removeFromConfig);
    };
    deps.updateUserList = [this]() {
        updateUserList();
    };
    deps.setRegistrationRequired = [this](bool required) {
        m_registrationRequired = required;
        if (!required) {
            m_registrationComplete = true;
        }
        updateRegistrationIndicator();
        updateSendControls();
    };
    deps.displayBurstLine = [this](const QString& line) {
        appendConversationLine("*", "-[server] -> " + line);
    };

    return IrcNumericHandler::handle(msg, deps);
}

void MainWindow::onChannelClicked(const QModelIndex& index) {
    const QStringList channelNames = m_channelModel->getChannelNames();
    if (!index.isValid() || index.row() < 0 || index.row() >= channelNames.size()) {
        return;
    }

    QString channelName = channelNames[index.row()];
    m_currentChannel = channelName;
    m_channelModel->clearUnread(channelName);
    refreshCurrentConversationView();
    updateChannelInfo();
    updateUserList();
}

void MainWindow::onMessageSent() {
    const QString message = m_messageInput->text();

    if (!m_isConnected) {
        return;
    }

    if (message.isEmpty()) {
        if (!m_searchQuery.isEmpty() && !m_searchMatches.isEmpty()) {
            jumpToSearchMatch(m_searchMatchIndex + 1);
            m_statusBar->setText(QString::number(m_searchMatchIndex + 1) + "/" + QString::number(m_searchMatches.size()) + " matches");
        }
        return;
    }

    if (!m_searchQuery.isEmpty()) {
        m_searchQuery.clear();
        m_searchMatches.clear();
        m_searchMatchIndex = -1;
    }

    if (message.startsWith('/')) {
        if (handleSlashCommand(message)) {
            m_messageInput->clear();
        }
        return;
    }

    if (m_registrationRequired && !m_registrationComplete) {
        m_statusBar->setText("Registration required. Use /pass, /user, /nick to register before sending messages.");
        return;
    }

    if (m_currentChannel.isEmpty()) {
        m_statusBar->setText("Join a channel first with /join #channel or use /msg target message");
        return;
    }

    m_connection->sendPrivmsg(m_currentChannel, message);
    displayMessage(m_currentChannel, m_currentNickname, message);
    m_messageInput->clear();
}

QString MainWindow::normalizeChannelName(const QString& channel) const {
    QString normalized = channel.trimmed();
    if (!normalized.isEmpty() &&
        !normalized.startsWith('#') &&
        !normalized.startsWith('&') &&
        !normalized.startsWith('+') &&
        !normalized.startsWith('!')) {
        normalized.prepend('#');
    }
    return normalized;
}

void MainWindow::setCurrentConversation(const QString& conversation) {
    m_currentChannel = conversation;
    if (!conversation.isEmpty()) {
        m_channelModel->clearUnread(conversation);
    }
    refreshCurrentConversationView();
    updateChannelInfo();
    updateUserList();
}

void MainWindow::removeConversation(const QString& channel, const QString& systemMessage, bool removeFromConfig) {
    if (channel.isEmpty()) {
        return;
    }

    m_channelModel->removeChannel(channel);
    m_conversationHistory.remove(channel);

    if (channel == m_currentChannel) {
        m_currentChannel.clear();
        refreshCurrentConversationView();
        updateChannelInfo();
        updateUserList();
    }

    if (removeFromConfig) {
        m_configManager->removeJoinedChannel(channel);
    }

    if (!systemMessage.isEmpty()) {
        appendSystemMessage(systemMessage);
    }
}

bool MainWindow::joinChannelFromInput(const QString& rawChannel, const QString& emptyErrorMessage) {
    if (!m_isConnected) {
        m_statusBar->setText("Connect to a server before joining channels");
        return false;
    }

    const QString channel = normalizeChannelName(rawChannel);
    if (channel.isEmpty()) {
        m_statusBar->setText(emptyErrorMessage);
        return false;
    }

    m_connection->sendJoin(channel);
    m_channelModel->addChannel(channel);
    m_channelModel->addUserToChannel(channel, m_currentNickname);
    setCurrentConversation(channel);
    appendSystemMessage("Joining " + channel);

    // Save channel to config for next session
    m_configManager->addJoinedChannel(channel);
    return true;
}

bool MainWindow::partChannelFromInput(const QString& rawChannel, const QString& emptyErrorMessage) {
    const QString channel = rawChannel.trimmed().isEmpty() ? m_currentChannel : rawChannel.trimmed();
    if (channel.isEmpty()) {
        m_statusBar->setText(emptyErrorMessage);
        return false;
    }

    m_connection->sendPart(channel);
    removeConversation(channel, "Parted " + channel, true);
    return true;
}

bool MainWindow::handleSlashCommand(const QString& message) {
    const CommandHandler::ParsedCommand parsed = CommandHandler::parse(message);

    if (!parsed.valid) {
        m_statusBar->setText(parsed.errorMessage);
        return false;
    }

    if (parsed.type == CommandHandler::Type::Join) {
        return joinChannelFromInput(parsed.arg1, "Usage: /join #channel");
    }

    if (parsed.type == CommandHandler::Type::Part) {
        return partChannelFromInput(parsed.arg1, "Usage: /part #channel");
    }

    if (parsed.type == CommandHandler::Type::Msg) {
        const QString& target = parsed.arg1;
        const QString& body = parsed.arg2;

        m_connection->sendPrivmsg(target, body);
        if (!m_channelModel->hasChannel(target)) {
            m_channelModel->addChannel(target);
        }
        if (m_currentChannel.isEmpty()) {
            setCurrentConversation(target);
        }
        displayMessage(target, m_currentNickname, body);
        return true;
    }

    if (parsed.type == CommandHandler::Type::Query) {
        const QString& target = parsed.arg1;
        if (!m_channelModel->hasChannel(target)) {
            m_channelModel->addChannel(target);
        }
        setCurrentConversation(target);
        appendSystemMessage("Opened private conversation with " + target);
        return true;
    }

    if (parsed.type == CommandHandler::Type::Me) {
        const QString& body = parsed.arg1;
        if (m_currentChannel.isEmpty()) {
            m_statusBar->setText("Join a channel first or use /msg target message");
            return false;
        }

        m_connection->sendPrivmsg(m_currentChannel, QString(QChar(1)) + "ACTION " + body + QChar(1));
        displayMessage(m_currentChannel, m_currentNickname, "* " + body);
        return true;
    }

    if (parsed.type == CommandHandler::Type::Raw) {
        m_connection->sendCommand(parsed.arg1);
        appendSystemMessage(">> " + parsed.arg1);
        return true;
    }

    if (parsed.type == CommandHandler::Type::Search) {
        QString query = parsed.arg1.trimmed();
        if (query.isEmpty()) {
            m_statusBar->setText("Usage: /search text");
            return true;
        }

        m_searchQuery = query;
        m_searchMatches.clear();
        m_searchMatchIndex = -1;

        const QString text = m_chatDisplay->toPlainText();
        int position = 0;
        while ((position = text.indexOf(m_searchQuery, position, Qt::CaseInsensitive)) != -1) {
            m_searchMatches.append(position);
            position += qMax(1, m_searchQuery.size());
        }

        if (m_searchMatches.isEmpty()) {
            m_statusBar->setText("No matches found for: " + query);
            return true;
        }

        jumpToSearchMatch(0);
        m_statusBar->setText(QString::number(m_searchMatches.size()) + " matches - Enter for next");
        return true;
    }

    if (parsed.type == CommandHandler::Type::Topic) {
        auto isChannelName = [](const QString& value) {
            return value.startsWith('#') || value.startsWith('&') ||
                   value.startsWith('+') || value.startsWith('!');
        };

        QString firstToken = parsed.arg1.trimmed();
        QString restText = parsed.arg2;
        QString targetChannel;
        QString topicText;

        if (firstToken.isEmpty()) {
            // /topic -> request topic for current channel
            targetChannel = m_currentChannel;
        } else if (isChannelName(firstToken)) {
            // /topic #chan [new topic]
            targetChannel = firstToken;
            topicText = restText;
        } else {
            // /topic new topic text -> set topic on current channel
            targetChannel = m_currentChannel;
            topicText = parsed.arg1;
            if (!restText.isEmpty()) {
                topicText += " " + restText;
            }
        }

        if (targetChannel.isEmpty() || !isChannelName(targetChannel)) {
            m_statusBar->setText("Usage: /topic [#channel] [new topic]");
            return false;
        }

        m_connection->sendTopic(targetChannel, topicText);

        if (topicText.trimmed().isEmpty()) {
            appendSystemMessage("Requesting topic for " + targetChannel);
        } else {
            appendSystemMessage("Setting topic for " + targetChannel + ": " + topicText);
            m_channelModel->setChannelTopic(targetChannel, topicText);
            if (targetChannel == m_currentChannel) {
                m_topicLabel->setText("Topic: " + topicText);
            }
        }
        return true;
    }

    if (parsed.type == CommandHandler::Type::Nick) {
        m_connection->sendNick(parsed.arg1);
        appendConversationLine("*", "-[server] -> NICK " + parsed.arg1);
        return true;
    }

    if (parsed.type == CommandHandler::Type::Quit) {
        m_connection->sendQuit(parsed.arg1);
        m_connection->disconnect();
        return true;
    }

    if (parsed.type == CommandHandler::Type::Pass) {
        m_connection->sendPass(parsed.arg1);
        appendSystemMessage("Password sent to server");
        appendConversationLine("*", "-[server] -> Registration steps:");
        QString displayNick = m_currentNickname.isEmpty() ? QString("<nickname>") : m_currentNickname;
        QString displayUser = m_currentNickname.isEmpty() ? QString("<username>") : m_currentNickname;
        appendConversationLine("*", "-[server] -> PASS <password>");
        appendConversationLine("*", "-[server] -> NICK " + displayNick);
        appendConversationLine("*", "-[server] -> USER " + displayUser + " 0 * :Qt IRC Client");
        return true;
    }

    if (parsed.type == CommandHandler::Type::User) {
        m_connection->sendUser(parsed.arg1, parsed.arg2);
        appendSystemMessage("User credentials sent to server");
        appendConversationLine("*", "-[server] -> USER " + parsed.arg1 + " 0 * :" + parsed.arg2);
        return true;
    }

    if (parsed.type == CommandHandler::Type::Invite) {
        QString target = parsed.arg1;
        QString channel = m_currentChannel;
        if (parsed.arg2.isEmpty() && channel.isEmpty()) {
            m_statusBar->setText("Usage: /invite nick #channel");
            return true;
        }
        if (!parsed.arg2.isEmpty()) {
            channel = parsed.arg2;
        }
        m_connection->sendInvite(target, channel);
        appendSystemMessage("INVITE " + target + " " + channel + " sent to server");
        return true;
    }

    if (parsed.type == CommandHandler::Type::Mode) {
        QString target = parsed.arg1;
        QString modes = parsed.arg2;
        if (target.isEmpty()) {
            m_statusBar->setText("Usage: /mode #channel [+modes] [params]");
            return true;
        }
        if (!target.startsWith('#') && !target.startsWith('&')) {
            target = m_currentChannel.isEmpty() ? QString("#") + target : m_currentChannel;
        }
        m_connection->sendMode(target, modes);
        appendSystemMessage("MODE " + target + (modes.isEmpty() ? "" : " " + modes) + " sent to server");
        return true;
    }

    m_statusBar->setText("Unknown command state. Available: " + CommandHandler::supportedCommandsSummary());
    return false;
}

void MainWindow::appendSystemMessage(const QString& message) {
    const QString target = m_currentChannel.isEmpty() ? QString("*") : m_currentChannel;
    appendConversationLine(target, "[system] " + message);
}

void MainWindow::displayMessage(const QString& channel, const QString& sender, const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString conversation = channel.isEmpty() ? QString("*") : channel;
    QString formatted = QString("[%1] [%2] <%3> %4").arg(timestamp, conversation, sender, message);

    appendConversationLine(conversation, formatted);
}

void MainWindow::appendConversationLine(const QString& conversation, const QString& line) {
    m_conversationHistory[conversation].append(line);

    if (conversation == m_currentChannel || (m_currentChannel.isEmpty() && conversation == "*")) {
        refreshCurrentConversationView();
    }
}

void MainWindow::refreshCurrentConversationView() {
    if (!m_chatDisplay) {
        return;
    }

    m_chatDisplay->clear();

    const QString key = m_currentChannel.isEmpty() ? QString("*") : m_currentChannel;
    const QStringList lines = m_conversationHistory.value(key);
    for (const QString& line : lines) {
        m_chatDisplay->append(line);
    }

    if (!m_searchQuery.isEmpty() && !m_searchMatches.isEmpty()) {
        jumpToSearchMatch(m_searchMatchIndex);
    }
}

void MainWindow::jumpToSearchMatch(int index) {
    if (!m_chatDisplay || m_searchMatches.isEmpty() || m_searchQuery.isEmpty()) {
        return;
    }

    const int normalizedIndex = (index % m_searchMatches.size() + m_searchMatches.size()) % m_searchMatches.size();
    m_searchMatchIndex = normalizedIndex;

    QTextCursor cursor(m_chatDisplay->document());
    const int start = m_searchMatches.at(normalizedIndex);
    cursor.setPosition(start);
    cursor.setPosition(start + m_searchQuery.size(), QTextCursor::KeepAnchor);
    m_chatDisplay->setTextCursor(cursor);
    m_chatDisplay->ensureCursorVisible();
}

QString MainWindow::extractNickFromPrefix(const QString& prefix) const {
    if (prefix.isEmpty()) {
        return QString("unknown");
    }

    const int bangIndex = prefix.indexOf('!');
    if (bangIndex > 0) {
        return prefix.left(bangIndex);
    }

    return prefix;
}

void MainWindow::updateChannelInfo() {
    if (m_currentChannel.isEmpty()) {
        if (m_topicLabel) {
            m_topicLabel->setText("Select a channel to see its topic and users");
        }
        return;
    }

    auto channel = m_channelModel->getChannel(m_currentChannel);
    if (channel) {
        if (m_topicLabel) {
            m_topicLabel->setText("Channel: " + channel->name + " | Topic: " + channel->topic);
        }
    }
}

void MainWindow::updateUserList() {
    m_userModel->clear();

    if (m_currentChannel.isEmpty()) {
        return;
    }

    if (!m_currentChannel.startsWith('#') &&
        !m_currentChannel.startsWith('&') &&
        !m_currentChannel.startsWith('+') &&
        !m_currentChannel.startsWith('!')) {
        return;
    }

    auto channel = m_channelModel->getChannel(m_currentChannel);
    if (channel) {
        // Keep local user visible even if the server user list is delayed or incomplete.
        if (!m_currentNickname.isEmpty() && !channel->users.contains(m_currentNickname)) {
            m_channelModel->addUserToChannel(m_currentChannel, m_currentNickname);
        }

        for (const auto& user : channel->users) {
            m_userModel->addUser(user);
        }
        m_userModel->sortUsers();
    }
}

QString MainWindow::currentConversationKey() const {
    return m_currentChannel.isEmpty() ? QString("*") : m_currentChannel;
}

void MainWindow::loadConfigurationSettings() {
    // Load last connection settings
    QString server;
    int port;
    QString nickname;
    bool useTls;

    if (m_configManager->loadLastConnection(server, port, nickname, useTls)) {
        m_serverInput->setText(server);
        m_portInput->setValue(port);
        m_nickInput->setText(nickname);
        m_tlsCheckBox->setChecked(useTls);
        qDebug() << "Loaded last connection settings";
    }

    m_autoReconnectEnabled = m_configManager->loadAutoReconnectEnabled();
    updateAutoReconnectIndicator();

    m_themeIndex = m_configManager->loadThemePreference();
    setTheme(m_themeIndex);
    if (m_themeComboBox) {
        m_themeComboBox->setCurrentIndex(m_themeIndex);
    }

    // Load previously joined channels
    QStringList previousChannels = m_configManager->getJoinedChannels();
    if (!previousChannels.isEmpty()) {
        for (const auto& channel : previousChannels) {
            m_channelModel->addChannel(channel);
        }
        qDebug() << "Loaded" << previousChannels.count() << "previously joined channels";
    }
}

void MainWindow::startAutoReconnect() {
    if (!m_autoReconnectEnabled || m_lastConnectedServer.isEmpty()) {
        return;
    }

    if (m_reconnectTimer->isActive()) {
        return;
    }

    if (m_reconnectAttempts >= m_maxReconnectAttempts) {
        appendSystemMessage("Auto-reconnect: Max attempts (" + QString::number(m_maxReconnectAttempts) + ") reached. Giving up.");
        m_connectionStatusLabel->setText("Failed to reconnect after " + QString::number(m_maxReconnectAttempts) + " attempts");
        return;
    }

    // Exponential backoff: 2^attempts seconds, capped at 60 seconds
    int delaySeconds = (1 << m_reconnectAttempts);  // 2^n
    if (delaySeconds > 60) {
        delaySeconds = 60;
    }

    m_reconnectAttempts++;
    appendSystemMessage("Auto-reconnect: Attempt " + QString::number(m_reconnectAttempts) + "/" + 
                       QString::number(m_maxReconnectAttempts) + " in " + QString::number(delaySeconds) + " seconds...");
    
    m_connectionStatusLabel->setText("Reconnecting in " + QString::number(delaySeconds) + "s (attempt " + 
                                    QString::number(m_reconnectAttempts) + ")");
    
    m_reconnectTimer->start(delaySeconds * 1000);  // Convert to milliseconds
}

void MainWindow::stopAutoReconnect() {
    if (m_reconnectTimer->isActive()) {
        m_reconnectTimer->stop();
    }
    resetReconnectAttempts();
}

void MainWindow::resetReconnectAttempts() {
    m_reconnectAttempts = 0;
}

void MainWindow::captureChannelsForReconnect() {
    m_pendingRejoinChannels.clear();

    const QStringList channelNames = m_channelModel->getChannelNames();
    for (const QString& channel : channelNames) {
        if (isChannelTarget(channel) && !m_pendingRejoinChannels.contains(channel)) {
            m_pendingRejoinChannels.append(channel);
        }
    }
}

void MainWindow::rejoinPendingChannelsIfAny() {
    if (m_pendingRejoinChannels.isEmpty()) {
        return;
    }

    int rejoinCount = 0;
    for (const QString& channel : m_pendingRejoinChannels) {
        if (channel.isEmpty()) {
            continue;
        }

        m_connection->sendJoin(channel);
        m_channelModel->addChannel(channel);
        m_channelModel->addUserToChannel(channel, m_currentNickname);
        rejoinCount++;
    }

    if (rejoinCount > 0) {
        appendSystemMessage("Auto-reconnect: Rejoining " + QString::number(rejoinCount) + " channel(s)...");

        if (m_currentChannel.isEmpty() && !m_pendingRejoinChannels.isEmpty()) {
            setCurrentConversation(m_pendingRejoinChannels.first());
        } else {
            refreshCurrentConversationView();
            updateChannelInfo();
            updateUserList();
        }
    }

    m_pendingRejoinChannels.clear();
}

void MainWindow::onReconnectAttempt() {
    if (m_lastConnectedServer.isEmpty()) {
        stopAutoReconnect();
        return;
    }

    if (m_connection->isConnected()) {
        stopAutoReconnect();
        return;
    }

    qDebug() << "Attempting auto-reconnect to" << m_lastConnectedServer << ":" << m_lastConnectedPort;
    
    if (!m_connection->connect(m_lastConnectedServer, m_lastConnectedPort, m_lastConnectedUseTls)) {
        appendSystemMessage("Auto-reconnect: Connection attempt failed, will retry...");
        startAutoReconnect();  // Schedule next attempt
        return;
    }

    if (m_currentNickname.isEmpty()) {
        m_currentNickname = m_nickInput ? m_nickInput->text().trimmed() : QString();
        if (m_currentNickname.isEmpty()) {
            m_currentNickname = "QtClient";
        }
    }

    m_connection->sendNick(m_currentNickname);
    m_connection->sendUser(m_currentNickname, "Qt IRC Client");
    appendSystemMessage("Auto-reconnect: Reconnection attempt in progress...");
}

void MainWindow::setTheme(int index) {
    m_themeIndex = index;
    ThemeColors::Palette p = ThemeColors::getPalette(m_themeIndex);

    if (m_topicLabel) {
        m_topicLabel->setStyleSheet(QString("background-color: %1; padding: 8px 10px; border-radius: %2px;").arg(p.backgroundSecondary).arg(p.borderRadius - 2));
    }

    if (m_chatDisplay) {
        m_chatDisplay->setStyleSheet(QString("font-family: 'DejaVu Sans Mono', monospace; font-size: 11pt; background: %1; color: %2; border: 1px solid %3; border-radius: %4px;")
            .arg(p.backgroundSecondary).arg(p.text).arg(p.border).arg(p.borderRadius));
    }

    if (m_statusBar) {
        m_statusBar->setStyleSheet(QString("border-top: 1px solid %1; padding: 8px 10px; color: %2;").arg(p.border).arg(p.textSecondary));
    }

    if (m_connectionPanelToggleButton) {
        m_connectionPanelToggleButton->setStyleSheet(QString(
            "QPushButton {"
            "  border-radius: %1px;"
            "  padding: 0px;"
            "  font-weight: 700;"
            "  background: %2;"
            "  color: %3;"
            "}"
            "QPushButton:hover {"
            "  background: %4;"
            "}"
        ).arg(p.borderRadiusSmall).arg(p.accentSecondary).arg(p.text).arg(p.border));
    }

applyStyle();
    updateAutoReconnectIndicator();
    updateRegistrationIndicator();

    if (m_trayIcon) {
        m_trayIcon->setToolTip("IRC Client");
    }
}

void MainWindow::setupTrayIcon() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qDebug() << "System tray not available on this platform";
        return;
    }

    m_trayIcon = std::make_unique<QSystemTrayIcon>(this);
    m_trayIcon->setToolTip("IRC Client");

    QIcon icon;
    m_trayIcon->setIcon(icon);
    m_trayIcon->setVisible(true);

    connect(m_trayIcon.get(), &QSystemTrayIcon::activated,
            this, &MainWindow::onTrayIconActivated);
}

void MainWindow::showNotification(const QString& title, const QString& message) {
    if (!m_trayIcon || !m_notificationsEnabled) {
        return;
    }

    if (m_trayIcon->isVisible()) {
        m_trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 5000);
    }
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
        if (isMinimized()) {
            showNormal();
        }
        raise();
        activateWindow();
    }
}
