# Intro

A Simple C++ TCP Server for Windows which handle multiple client/session for reverse connection(s)

## Download

```bash
git clone https://github.com/tajiknomi/TCPServerCLI.git
```
## How to build

```
cd TCPServerCLI && mkdir build && cd build
cmake ..
cmake --build .
```

## 32/64 build

```
cmake -A x64 ..
cmake -A Win32 ..
```

You can specify the CMake generator to create configuration files for your build system e.g. To generate VS19 project files for a 64-bit build

```
cmake -G "Visual Studio 16 2019" -A x64 ..

```
## Usage
```
TCPServerCLI.exe [PortNumber]
```
The tool accepts two commands
```
# Specify the [id] of the connected client/session
1) select client [id]

# Disconnect from client/session
2) exit
```

## Screenshots
Waiting for clients to connect, two clients are shown below as connected

![Alt text](/screenshots/1.JPG?raw=true "Optional Title")

Client 0 is selected for communication

![Alt text](/screenshots/2.JPG?raw=true "Optional Title")

Upgrading a simple reverse shell to a fully interactive tty on client 1

![Alt text](/screenshots/3.JPG?raw=true "Optional Title")

# Note
It is better to use [MINGW](https://www.mingw-w64.org/)/[GIT](https://git-scm.com/download/win)/[WSL](https://learn.microsoft.com/en-us/windows/wsl/about) terminal because it can handle interactive shell for all OS's, while the native Windows command prompt i.e. cmd and PowerShell environments do not natively support PTYs in the same way as Linux. They use different mechanisms for terminal emulation and may not fully support all PTY features expected by applications like pty.spawn()

## License

[MIT](https://choosealicense.com/licenses/mit/)