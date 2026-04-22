# IRC Client - Qt6 & C++17

A modern, minimalist IRC client built with Qt6 and C++17. This project is a graphical interface client for IRC servers.

## Features

- **Modern Qt6 Interface**: Clean, responsive GUI with intuitive layout
- **Real-time Chat**: Connect to IRC servers and chat in real time
- **TLS + Plain Connections**: Switch between secure TLS and plain IRC
- **Conversation Buffers**: Sidebar buffers for channels (`[CH]`) and private messages (`[PM]`)
- **Unread Counters**: Per-conversation unread badges showing number of unread messages
- **Channel Management**: Join/part/remove channels directly from the UI
- **User List by Channel**: Right pane user list updates dynamically for the active channel
- **Auto-normalization**: Channel names automatically prefixed with `#` if missing
- **Private Messages**: Distinguished from channel messages and routed to dedicated private buffers
- **Slash Commands**: Supports `/join`, `/part`, `/msg`, `/query`, `/me`, `/topic`, `/nick`, `/user`, `/pass`, `/raw`, `/search`, and `/quit`
- **History Search**: Search and jump through the active conversation history
- **Configuration Persistence**: Saves last server, port, nickname, and joined channels automatically
- **Auto-reconnect**: Automatically reconnects on disconnect with exponential backoff (up to 10 attempts)
- **Memory Safe**: RAII principles, no memory leaks
- **C++17 Features**: Modern C++ with smart pointers and move semantics

## Architecture

```
irc_client_cpp/
├── include/               # Header files
│   ├── MainWindow.hpp     # Main UI window and interaction logic
│   ├── IrcConnection.hpp  # IRC transport layer over QSslSocket
│   ├── MessageParser.hpp  # IRC line parser/serializer
│   ├── ChannelModel.hpp   # Qt model for channels + unread counters
│   ├── UserModel.hpp      # Qt model for users in active channel
│   ├── ConfigManager.hpp  # QSettings persistence helpers
│   └── CommandHandler.hpp # Slash command parser/validator
├── src/                   # Implementation files
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── IrcConnection.cpp
│   ├── MessageParser.cpp
│   ├── ChannelModel.cpp
│   ├── UserModel.cpp
│   ├── ConfigManager.cpp
│   └── CommandHandler.cpp
├── build/                 # Build output directory
└── CMakeLists.txt         # Build configuration
```

## Requirements

- **Compiler**: GCC 10+, Clang 14+, or MSVC 2019+
- **CMake**: 3.21 or later
- **Qt6**: Full development libraries
  - Qt6Core
  - Qt6Gui
  - Qt6Widgets
  - Qt6Network
  - Qt6Concurrent

### Ubuntu/Debian

```bash
sudo apt-get install cmake qt6-base-dev libqt6core6 libqt6gui6 libqt6widgets6 libqt6network6
```

### macOS

```bash
brew install cmake qt@6
```

### Windows

Download and install from [Qt Online Installer](https://www.qt.io/download-open-source)

## Building

```bash
# From the project root
mkdir -p build
cmake -S . -B build
cmake --build build --config Release
```

## Running

```bash
./build/irc_client_qt
```

### Connection Panel

The application opens directly into the main window with a connection panel at the top.

1. **Preset**: Select a known network or use custom settings
2. **Server / Port**: IRC server hostname and port in one row
3. **TLS**: Enable encrypted transport when required
4. **Nickname**: Your IRC nickname
5. Click **Connect**

Until a connection is established, the chat panels remain visible but disabled, which avoids the old blank-window behavior.

For quick testing, the client includes known-server presets for public networks that still accept plain IRC connections:

- **Libera.Chat TLS**: `irc.libera.chat:6697`
- **OFTC TLS**: `irc.oftc.net:6697`
- **Libera.Chat plain**: `irc.libera.chat:6667`
- **OFTC plain**: `irc.oftc.net:6667`

Use the TLS checkbox for encrypted connections. The client verifies certificates through the system trust store, so make sure your CA bundle is current.

## Usage

- **Connect First**: Use the top connection panel before trying to chat
- **Join Channel**: Use the Join button in the sidebar or type `/join #channelname`
- **Send Message**: Type and press Enter in the bottom input
- **Private Message**: Use `/msg nick message` to open/use a direct conversation buffer
- **History Search**: Use the search bar below the chat or type `/search text`
- **Conversation Types**: `[CH]` for channels and `[PM]` for private buffers
- **Unread State**: Buffers not currently focused accumulate `{N}` unread count
- **Remove Channel**: Remove from sidebar (with confirmation when connected)

### Supported IRC Commands

The IRC protocol supports the following commands via the GUI and slash commands:

- `JOIN #channel` - Join a channel (use the Join button or type channel name)
- `PART [#channel]` - Leave a channel (click Remove or use `/part`)
- `PRIVMSG target message` - Send private message or channel message
- `QUERY nick` - Open a private conversation buffer without sending a message
- `ME action` - Send an IRC action message in the active buffer
- `SEARCH text` - Find text in the current conversation history and jump between matches
- `TOPIC [#channel] [new topic]` - View or set channel topic (uses current channel when omitted)
- `NICK newnick` - Change nickname
- `RAW command` - Send a raw IRC command for debugging or advanced use
- `QUIT [reason]` - Disconnect and close (Disconnect button)

### Registration & Authentication Commands

For IRC servers that require registration before joining channels:

- `PASS password` - Send server password (if required by the server)
  - Usage: `/pass mypassword`
  - Some servers require this as the first command

- `USER username realname` - Set user credentials for registration
  - Usage: `/user "myusername" "My Real Name"`
  - The format sent to the server is: `USER myusername 0 * :My Real Name`
  - Both username and realname are required
  - Quote parameters are optional but recommended

- `NICK newnick` - Set or change your nickname
  - Usage: `/nick mynickname`
  - Must be done before joining channels if username is not set

**Registration Flow Example:**

If you connect to a server that requires registration:

1. Connect to the server (via the connection panel)
2. Send your password: `/pass serverpassword` (if required)
3. Set your user credentials: `/user "myusername" "Real Name"`
4. Set your nickname: `/nick mynickname`
5. Now you should be able to join channels: `/join #channel`

The server will notify you in the chat window if it requires authentication or additional steps.

## Safety Features

- **Memory Management**: Smart pointers (unique_ptr, shared_ptr)
- **Input Validation**: Nick and channel name sanitization
- **RAII Pattern**: Automatic resource cleanup
- **No Sensitive Data**: Passwords and tokens not logged

## Testing

Currently no unit tests. Future: Add CppUnit or Catch2 framework.

## Code Structure

### IrcConnection.cpp/hpp (Network & Protocol Layer)
- QSslSocket with TLS/plain support for IRC transport
- IRC command send helpers (PASS, NICK, USER, JOIN, PART, PRIVMSG, QUIT)
- Message receiving with CRLF buffering and line framing
- Emits connection and message signals for UI orchestration
- TLS error propagation to the UI status layer

### CommandHandler.cpp/hpp (Slash Command Parsing)
- Dedicated parser/validator for slash commands
- Centralized command argument extraction and usage errors
- Keeps UI orchestration in `MainWindow` while isolating command syntax concerns

### MainWindow.cpp/hpp (UI & Message Management)
- 3-pane layout: Channels Sidebar | Chat View | Users Sidebar
- Integrated connection panel (server/port/nickname/TLS + presets)
- Conversation history per buffer (channels, PMs, and `*` system buffer)
- Unread counters per conversation in the sidebar
- Search UI and `/search` support over active conversation history
- Auto-reconnect with exponential backoff (max attempts configurable in code)
- Slash command execution wired through `CommandHandler` parsing + UI actions

### ConfigManager.cpp/hpp (Persistence)
- QSettings-based persistence for last connection settings
- Joined-channel list persistence
- Auto-reconnect preference persistence
- Custom preset APIs available (currently backend-ready)

### ChannelModel.cpp/hpp (Channel/Conversation List Model)
- Qt AbstractListModel for conversation sidebar display
- Channel + PM entries with `[CH]` / `[PM]` labels
- Unread counters and unread styling
- Topic/user-count tracking per channel

### UserModel.cpp/hpp (Users List Model)
- Qt AbstractListModel for active-channel user display
- Prefix/operator/voice metadata
- Sorting (operators first, then voiced, then alphabetical)

## Reusing Code from ft_irc

This project can leverage utilities from `/home/max/code/irc/`:
- Message parsing logic
- Protocol command structure
- User and Channel data models
- Numeric reply constants (RFC 1459)

## Performance Considerations

- **Threading**: Message receiving in Qt event loop (future: move to QThread)
- **Buffer Management**: CRLF framing prevents buffer overflow attacks
- **Model Updates**: Qt model system batches UI updates efficiently

## Security Considerations

1. **No plaintext password logging**
2. **Input sanitization for nicks/channels**
3. **RAII prevents resource leaks**
4. **No shell execution from user input**
5. **TLS support available** via Qt SSL stack

## Known Limitations

- **No SASL authentication**: SASL mechanisms are not implemented
- **No DCC file transfer**: File sharing is not implemented
- **No unread "mentions" distinction**: Unread count does not currently separate mentions from general unread
- **No ignore lists**: Cannot block users or hide messages
- **No message logging to disk**: In-memory conversation history is not persisted as chat logs
- **No IRCv3 capability negotiation**: Modern capability features are not yet implemented
- **No desktop notifications**: Mentions/PMs do not trigger OS-level notifications
- **Limited numeric reply handling**: Several IRC numerics and richer error surfaces are still pending
- **No automated test suite yet**: There are currently no unit/integration tests

## Future Enhancements

### [PRIORITY 1] Core Features
- **Custom Preset Management UI**: Create/edit/remove server presets from the interface
- **Reconnect Recovery**: Optional auto-rejoin of previously active channels after reconnect (implemented)
- **Protocol Reply Coverage**: Improve handling of common IRC numerics and user-facing error feedback (in progress: welcome, names/topic, nick-in-use, join/send failures, registration/password/channel-ops errors, fallback for generic 4xx/5xx, and unified numeric message format)
- **Command Handling Refactor**: Continue separating command parsing/dispatch from UI orchestration for easier testing and maintenance

### [PRIORITY 2] UI/UX Polish
- **Themes**: Light/dark theme toggle
- **Better Icons**: Custom icons for channel/private/online/offline states
- **Message Timestamps**: Show time each message was sent
- **Mention Highlighting**: Highlight when your nick is mentioned
- **Auto-scroll**: Auto-scroll to latest message when new message arrives
- **Font Resizing**: User-configurable font size

### [PRIORITY 3] Protocol & Security
- **SASL Authentication**: Support for SASL plain/scram mechanisms
- **IRCv3 Capabilities**: Partial support for modern IRC extensions
- **Message Logging**: Option to log conversations to file
- **Ignore/Block Lists**: Silently hide messages from specific users

### [PRIORITY 4] Testing & Quality
- **Unit Tests**: Add Catch2 or CppUnit test framework
- **Integration Tests**: Test with live IRC server (Libera.Chat, etc)
- **Memory Profiling**: Valgrind checks for leaks
- **Performance Benchmarks**: Measure network latency and UI responsiveness

### [PRIORITY 5] Advanced Features
- **Channel Favorites/Bookmarks**: Mark important channels for quick access
- **DCC Protocol**: Support file transfers
- **User Profiles**: View user info and joined channels
- **Notifications**: Desktop notifications for mentions and new PMs

## Maintenance Checklist

- [ ] Sync README.md when adding/removing IRC commands or UI features
- [ ] Keep architecture and code-structure sections aligned with real file/class names
- [ ] Update preset servers if network addresses change
- [ ] Revisit Known Limitations after completing each priority
- [ ] Add release notes to this README when user-visible behavior changes
- [ ] Test with live IRC server (Libera.Chat, OFTC) after significant changes
- [ ] Run `cmake --build build -j2` to verify compilation before commits
- [ ] Add/expand tests as protocol and UI logic grow

## License

Personal project for educational purposes.

## Contributing

Team project - feedback and improvements welcome.

## Authors

- Main developer: SoMaxB
- Based on 42 project ft_irc server from GitHub Users: 4Bi4 - Lucasfdp & SoMaxB
