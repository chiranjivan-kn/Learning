# Qt CAN Bus 

## 1. What is CAN?

**Controller Area Network (CAN)** is a serial communication protocol developed by Bosch, commonly used in automotive, industrial automation, robotics, and embedded systems.

CAN allows multiple electronic control units (ECUs) or microcontrollers to communicate over a shared two-wire bus without requiring a central controller.

### Key Characteristics

- **Multi-master communication:** Any node can initiate transmission when the bus is idle.
- **Message-based protocol:** Messages are identified by CAN IDs rather than destination addresses.
- **Differential signaling:** Uses `CAN_H` and `CAN_L` for improved noise immunity.
- **Priority-based arbitration:** Lower numerical CAN IDs have higher priority.
- **Error handling:** Built-in error detection and retransmission mechanisms.

---

## 2. CAN Network Architecture

A typical CAN network consists of multiple nodes connected to a shared bus.

### Network Diagram

```text
                 CAN BUS
        CAN_H =========================
        CAN_L =========================
                  |       |       |
                Node A  Node B  Node C
                  |       |       |
                 MCU     MCU     MCU
```

### Components of a CAN Node

Each physical CAN node typically contains:

1. **Microcontroller (MCU)**
   - Runs the application logic.

2. **CAN Controller**
   - Constructs CAN frames.
   - Handles arbitration, transmission, and reception.
   - Detects and manages protocol errors.

3. **CAN Transceiver**
   - Converts controller signals into differential electrical signals on the CAN bus.

### CAN Bus Termination

A high-speed CAN bus typically requires **120 Ω termination resistors** at each physical end.

With power off, the resistance measured between `CAN_H` and `CAN_L` is approximately **60 Ω**, assuming no additional parallel loads.

---

## 3. CAN Frame Structure

A Classical CAN data frame contains the following main fields:

```text
| SOF | Arbitration | Control | Data | CRC | ACK | EOF |
```

### Frame Fields

| Field | Description |
|---|---|
| SOF | Start of Frame |
| Arbitration | CAN identifier and arbitration information |
| Control | Includes data length information |
| Data | 0–8 bytes for Classical CAN |
| CRC | Error detection |
| ACK | Receiver acknowledgment |
| EOF | End of Frame |

### CAN Identifier Formats

CAN supports two identifier formats:

| Format | Identifier Size | Range |
|---|---|---|
| Standard CAN | 11-bit | `0x000`–`0x7FF` |
| Extended CAN | 29-bit | `0x00000000`–`0x1FFFFFFF` |

### Example CAN Frame

```text
CAN ID : 0x123
DLC    : 8
DATA   : 01 02 03 04 05 06 07 08
```

**Explanation:**

- **CAN ID (`0x123`):** Identifies the message.
- **DLC (Data Length Code):** A DLC of 8 represents eight data bytes in Classical CAN.
- **DATA:** Contains the application payload.

> [!NOTE]
> A CAN ID does not inherently identify the sending device. Its meaning is defined by the application's communication protocol.

---

## 4. CAN Arbitration

CAN uses **non-destructive bitwise arbitration**, allowing multiple nodes to begin transmitting without corrupting the winning message.

### CAN Bus States

| State | Bit Value | Description |
|---|---|---|
| Dominant | `0` | Overrides a recessive bit |
| Recessive | `1` | Can be overridden by a dominant bit |

### How Arbitration Works

When two nodes transmit simultaneously, the message with the lower numerical identifier wins arbitration, assuming comparable frame formats.

**Example:**

```text
Node A → CAN ID: 0x100
Node B → CAN ID: 0x200

Winner → Node A (Higher Priority)
```

The losing node stops transmitting and retries when the bus becomes available.

### Key Points

- Arbitration occurs bit by bit.
- Dominant (`0`) overrides recessive (`1`).
- Lower numerical CAN IDs generally have higher priority.
- The winning frame continues without corruption.

---

## 5. CAN Bitrate and Bus Length

All nodes on a Classical CAN bus must be configured with compatible bit timing and nominal bitrate.

### Common CAN Bitrates

| Bitrate | Common Usage |
|---|---|
| 125 kbit/s | Longer-distance networks |
| 250 kbit/s | Industrial and vehicle networks |
| 500 kbit/s | Automotive and embedded systems |
| 1 Mbit/s | Shorter high-speed CAN networks |

### Factors Affecting Bus Length

The supported cable length depends on:

- CAN bitrate
- Cable characteristics
- CAN transceivers
- Network topology
- Propagation delay

> [!IMPORTANT]
> Higher bitrates generally require shorter buses.

---

## 6. CAN Error Handling

CAN includes several mechanisms for detecting communication errors.

### Error Detection Mechanisms

| Mechanism | Description |
|---|---|
| Bit Monitoring | Detects unexpected bus values |
| Bit Stuffing Check | Detects invalid bit-stuffing sequences |
| CRC Check | Detects corrupted frames |
| Form Check | Detects invalid fixed-format fields |
| ACK Check | Detects when a transmitted frame is not acknowledged |

### CAN Error States

CAN controllers maintain transmit and receive error counters and may enter the following states:

| State | Description |
|---|---|
| Error Active | Normal communication with active error signaling |
| Error Passive | Communication continues with restricted error signaling |
| Bus Off | Controller stops participating in normal CAN traffic |

### Key Points

- CAN detects transmission and reception errors.
- Error counters track communication reliability.
- Repeated errors may cause a controller to enter the **Bus Off** state.
- Controllers can recover from Bus Off according to their configured recovery behavior.

---

## 7. Classical CAN vs. CAN FD

**CAN FD (Flexible Data-rate)** extends Classical CAN to support larger payloads and optionally faster data transfer.

### Comparison

| Feature | Classical CAN | CAN FD |
|---|---|---|
| Maximum Payload | 8 bytes | 64 bytes |
| Identifier | 11 or 29 bits | 11 or 29 bits |
| Data-phase Bitrate | Same as nominal bitrate | Can be higher with bit-rate switching |
| CRC | 15 bits | 17 or 21 bits |
| Typical Use | Basic control messages | Larger or more frequent data transfers |

### Advantages of CAN FD

- Supports up to **64 bytes of payload** per frame.
- Can use a faster bitrate during the data phase.
- Improves efficiency for larger data transfers.
- Provides enhanced CRC error detection.

> [!NOTE]
> CAN FD requires compatible controllers and network configurations.

---

## 8. Summary

| Concept | Key Takeaway |
|---|---|
| CAN | Multi-master, message-based serial communication protocol |
| Physical Layer | Differential signaling using `CAN_H` and `CAN_L` |
| CAN ID | Identifies messages and determines arbitration priority |
| Arbitration | Dominant bits override recessive bits |
| Classical CAN Payload | Maximum 8 bytes |
| CAN FD Payload | Maximum 64 bytes |
| Termination | Typically 120 Ω at each end |
| Error Handling | Built-in detection, counters, and fault-confinement states |



## POC
A simple **CAN Bus communication application** developed using **C++17, Qt 6, and SocketCAN** on Linux.
This project uses a virtual CAN interface (`vcan0`) for development and testing without requiring physical CAN hardware.

## Features

- Qt Widgets-based graphical user interface
- Connect and disconnect from a CAN interface
- Send standard 11-bit CAN frames
- Receive and display incoming CAN frames
- Basic CAN communication error handling
- Virtual CAN testing using Linux SocketCAN

## Requirements

- GCC with C++17 support
- CMake 3.16 or newer
- Qt 6 (Widgets and SerialBus)
- Linux SocketCAN
- `can-utils`

## 1. Install Dependencies

Install the required development packages:

```bash
sudo apt update

sudo apt install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-serialbus-dev \
    can-utils
```

## 2. Configure Virtual CAN

Linux provides a virtual CAN driver that allows CAN communication to be tested without physical hardware.

Load the virtual CAN kernel module:

```bash
sudo modprobe vcan
```

Create a virtual CAN interface:

```bash
sudo ip link add dev vcan0 type vcan
```

Enable the interface:

```bash
sudo ip link set vcan0 up
```

Verify the interface:

```bash
ip -details link show vcan0
```

The interface should report `UP`.

**Note:** Virtual CAN interfaces do not require a bitrate configuration because they do not use a physical CAN controller.

## 4. Build the Application

From the project root directory:

```bash
cmake -S . -B build
```

Compile the application:

```bash
cmake --build build -j$(nproc)
```

The executable will be generated at:

```text
build/QtCANPOC
```

## 5. Run the Application

Start the application:

```bash
./build/QtCANPOC
```

The GUI provides three buttons:

| Button | Description |
|---|---|
| Connect to vcan0 | Connects to the virtual CAN interface |
| Send CAN Frame | Sends a predefined CAN message |
| Disconnect | Disconnects from the CAN interface |

The application also contains a text area for CAN transmission, reception, and error logs.

## 6. CAN Frame Configuration

The application currently transmits a predefined standard CAN frame.

| Parameter | Value |
|---|---|
| CAN interface | `vcan0` |
| CAN ID | `0x123` |
| Frame format | Standard CAN (11-bit ID) |
| Payload length | 8 bytes |
| Payload | `01 02 03 04 05 06 07 08` |
| Protocol | Classical CAN |

The Qt SerialBus module handles CAN communication through the SocketCAN backend.

## 7. Test CAN Transmission

Open another terminal and monitor the virtual CAN interface:

```bash
candump vcan0
```

In the Qt application:

1. Click **Connect to vcan0**.
2. Click **Send CAN Frame**.
3. Observe the transmitted frame in the terminal.

## 8. Test CAN Reception

Use `cansend` to transmit a CAN frame from another terminal:

```bash
cansend vcan0 456#AABBCCDD
```

The Qt application should receive the frame and display:

```text
RX ID: 0x456 Data: aa bb cc dd
```

This verifies that the application can receive CAN frames sent by another SocketCAN application.

## 9. Using a Physical CAN Interface

The application can also communicate through a physical CAN adapter supported by Linux SocketCAN.

For example, configure a CAN interface with a bitrate of 500 kbit/s:

```bash
sudo ip link set can0 down
sudo ip link set can0 type can bitrate 500000
sudo ip link set can0 up
```

Verify:

```bash
ip -details link show can0
```

Update the interface name in `main.cpp`:

```cpp
device = QCanBus::instance()->createDevice(
    "socketcan", "can0", &error);
```

Rebuild and run the application.

**Important:** Physical CAN communication requires compatible CAN hardware, a correctly wired CAN bus, appropriate termination, and matching bitrates across nodes.
