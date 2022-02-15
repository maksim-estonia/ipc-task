# IPC task

The program shall accept the following command-line arguments:

- "--help" shall print out a help text containing short description of all supported command-line arguments
- "--message TBD" shall use message passing as IPC method
- "--queue TBD" shall use message queue as IPC method
- "--pipe TBD" shall use pipes as IPC method
- "--shm [buffer size in kB]" shall use shared memory buffer as IPC method (remember mutexes/semaphores!)
- "--file [filename]" file used to read/write data

## to do

- [x] setup command-line arguments
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