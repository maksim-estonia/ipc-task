# IPC task

The program shall accept the following command-line arguments:

- "--help" shall print out a help text containing short description of all supported command-line arguments
- "--message TBD" shall use message passing as IPC method
- "--queue TBD" shall use message queue as IPC method
- "--pipe TBD" shall use pipes as IPC method
- "--shm [buffer size in kB]" shall use shared memory buffer as IPC method (remember mutexes/semaphores!)
- "--file [filename]" file used to read/write data

## To do

- [x] setup command-line arguments
  - [x] send
  - [x] receive
- [ ] add message passing functionality
  - [ ] send
  - [ ] receive
- [ ] add message queue functionality
  - [ ] send 
  - [ ] receive
- [ ] add pipe functionality
  - [ ] send
  - [ ] receive
- [ ] add shared memory functionality
  - [ ] send
  - [ ] receive

## Command line arguments

`ipc_receivefile --help`

![receive-help](images/receive-help.png)

`ipc_receivefile --message --file filename`

![receive-message](images/receive-message.png)

`ipc_sendfile --help`

![send-help](images/send-help.png)

`ipc_sendfile --message --file filename`

![send-message](images/send-message.png)