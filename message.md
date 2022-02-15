# Message IPC

## Receive

- [ ] create a channel
  
  ```c
  name_attach_t *att;
  attach = name_attach (NULL, "message-ipc", 0);
  ```

- [ ] wait for a message 
  
  ```c
  rcvid = MsgReceive(att->chid, &msg, sizeof(msg), NULL);
  if (rcvid == -1)
  {
    perror("MsgReceive");
    exit(EXIT_FAILURE);
  }
  ```
  - [ ] open file for writing output

    ```c
    int fd;
    fd = creat("filename.dat", S_IRUSR | S_IWUSR);
    ```

  - [ ] perform processing (write to filename.dat)

    ```c
    size_written = write(fd, msg, sizeof(msg));

    /* test for error */
    if (size_written != sizeof(buffer))
    {
      perror("Error writing to file");
      return EXIT_FAILURE;
    }
    ```
  
  - [ ] close file

    ```c
    close(fd);
    ```

  - [ ] reply (with checksum)
  
    ```c
    checksum = calculate_checksum(rbuf.msg.string_to_cksum);
    status = MsgReply(rcvid, EOK, &checksum, sizeof(checksum));
    if (status == -1)
    {
      perror("MsgReply");
      exit(EXIT_FAILURE);
    }
    ```

- [ ] loop back to "wait for message"

## Send

## Test