# 🧪 IPC with `fork()` and `pipe()` in Linux

This repo demonstrates **Interprocess Communication (IPC)** in Linux using:
- `fork()` to create a child process
- `pipe()` to set up a unidirectional communication channel between parent and child
- Custom messaging using strings and commands

---

## 📂 Contents

| File                     | Description                                          |
|--------------------------|------------------------------------------------------|
| `fork_pipe_demo.c`       | Minimal working example using `fork()` and `pipe()` to send a simple string |
| `fork_custom_data_send.c`| Enhanced version that accepts command-line arguments like `0xFF`, `0xFE` and performs custom logic based on what the child receives |

---

## ✅ Requirements

- GCC (any recent version)
- Linux environment (Ubuntu, Debian, etc.)
- Terminal access

---

## 🔧 How to Compile

```bash
gcc fork.c -o fork
gcc fork_custom_data_send.c -o fork_custom_data_send
```
▶️ How to Run
1. Basic Example: fork.c
```bash
linux-o-phile@homeserver:~/Fork/IPC---Pipes$ ./fork
Sending data to the child
Receiving data from the parent
Parent sent: Hello from parent!
Child received: Hello from parent!
```
2. Custom Command Example: fork_custom_data_send.c
You can pass a command like 0xFF or 0xFE:
```bash
linux-o-phile@homeserver:~/Fork/IPC---Pipes$ ./fork_custom_data_send
Sending data to the child
Parent sent: Hello
Receiving data from the parent
Invalid option
linux-o-phile@homeserver:~/Fork/IPC---Pipes$ ./fork_custom_data_send 0xFE
Sending data to the child
Parent sent: 0xFE
Receiving data from the parent
Turning on the AC
```
🧠 How It Works
 - The parent writes a string (or custom command) into the pipe.
 - The child reads from the pipe, interprets the data, and prints/logs a message.
 - Pipes are unidirectional; parent writes, child reads.
 - String comparison is done using strcmp(), not ==.

📌 Key Concepts Demonstrated
 - Process creation with fork()
 - IPC with pipe()
 - Argument parsing with argv[]
 - Safe string handling using strdup()


             ┌───────────────────────────────┐
             │       Parent Process          │
             │-------------------------------│
             │ 1. Creates pipe()             │
             │    ┌──────────────┐           │
             │    │  pipefd[0]   │◄── read   │
             │    │  pipefd[1]   │──► write  │
             │    └──────────────┘           │
             │                               │
             │ 2. fork() → creates child     │
             └──────┬────────────┬───────────┘
                    │            │
                    │            ▼
         ┌──────────┴────────────┴───────────┐
         │           Kernel (shared)         │
         │-----------------------------------│
         │    Pipe buffer (in RAM)           │
         │    ┌────────────────────────────┐ │
         │    │  FIFO queue (byte stream)  │ │
         │    └────────────────────────────┘ │
         └──────────┬────────────┬───────────┘
                    │            │
                    ▼            │
             ┌───────────────────▼────────────┐
             │         Child Process          │
             │--------------------------------│
             │ 3. Closes pipefd[1] (write end)│
             │ 4. Reads data from pipefd[0]   │
             │ 5. Interprets message          │
             └────────────────────────────────┘

