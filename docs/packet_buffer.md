## Packet buffer.
This class is for to receive fixed length packets.

```
STX AAA BBB CCC DDD ETX CHK.
# --> total 7 bytes per packet.
```

### Simple usage.

```cpp
struct packet_t {
    uint8_t data[7]; // --> STX, AAA, BBB, CCC, DDD, ETX, CHK.
};

// --> uart RX buffer and packet queue.
packet_buffer<7> uart_rx_buf;
circular_queue<packet_t, uint8_t, 32> uart_rx_que;

bool start_rx() {
    uint8_t* buf = uart_rx_buf.back();
    uint16_t len = uart_rx_buf.remains();

    if (!len) {
        // buffer is full.
        return false;
    }

    return HAL_UART_Receive_IT(&huart1, buf, len) == HAL_OK;
}

void HAL_UART_RxCpltCallback(...) {
    // --> accept received bytes to front.
    uart_rx_buf.accept(uart_rx_buf.remains());

    // --> then, handle buffered bytes.
    handle_buf();

    // --> restart RX.
    start_rx();
}

void handle_buf() {
    int32_t stx = uart_rx_buf.find(PKT_STX);
    if (stx < 0) {
        // --> no STX found.
        uart_rx_buf.reset();
        return;
    }

    if (stx > 0) {
        // --> STX is not aligned as first byte.
        uart_rx_buf.consume(stx);
        return;
    }

    const uint8_t* buf = uart_rx_buf.front();

    // --> if no STX or no ETX, or checksum mismatches.
    if ((buf[0] != PKT_STX && buf[5] != PKT_ETX)
        || checksum(buf) != buf[6])
    {
        if ((stx = uart_rx_buf.find(PKT_STX, 1)) < 1) {
            // --> no next STX found: discard all bytes.
            uart_rx_buf.reset();
            return;
        }

        // --> align the next STX to be as first byte.
        uart_rx_buf.consume(stx);
        return;
    }

    packet_t packet;

    memcpy(packet.data, buf, 7);
    uart_rx_que.enqueue(packet);

    uart_rx_buf.reset();
}

void uart_task() {
    packet_t packet;

    while (uart_rx_que.dequeue(packet)) {
        // TODO: handle the packet.
    }
}
```
