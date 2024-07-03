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
```

## 32/64 build

```
cmake -A x64 ..
cmake -A Win32 ..
```

## Usage

The tool accepts two commands
```
# specify the [id] when client is connected, 
select client [id]

# Disconnect from client/session
exit
```

## Screenshots
Waiting for clients to connect, two clients are connected

![Alt text](/screenshots/1.JPG?raw=true "Optional Title")

Client 0 is selected for communication

![Alt text](/screenshots/2.JPG?raw=true "Optional Title")

Here i'm spawing Interactive terminal on client 1

![Alt text](/screenshots/3.JPG?raw=true "Optional Title")

## License

[MIT](https://choosealicense.com/licenses/mit/)