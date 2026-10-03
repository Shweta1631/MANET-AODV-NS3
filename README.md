# MANET Routing Protocol Simulation using NS-3

## Project Description

This project demonstrates the simulation of a Mobile Ad-hoc Network (MANET) routing protocol using NS-3.

The project uses the AODV (Ad hoc On-Demand Distance Vector) routing protocol to establish communication between wireless nodes. UDP traffic is transmitted from a source node to a destination node, and FlowMonitor is used to measure the network performance.

## Objectives

- To simulate a wireless ad-hoc network using NS-3.
- To implement the AODV routing protocol.
- To establish communication between source and destination nodes.
- To transmit UDP packets through the network.
- To measure network performance using Packet Delivery Ratio, Throughput, and Average Delay.

## Tools and Technologies

- NS-3
- C++
- AODV Routing Protocol
- Wi-Fi
- UDP
- FlowMonitor
- Ubuntu Linux
- WSL (Windows Subsystem for Linux)

## Network Configuration

| Parameter | Value |
|---|---|
| Number of Nodes | 5 |
| Routing Protocol | AODV |
| Wireless Technology | Wi-Fi 802.11b |
| Traffic Protocol | UDP |
| Packet Size | 512 bytes |
| Data Rate | 100 Kbps |
| Simulation Time | 10 seconds |
| Source Node | Node 0 |
| Destination Node | Node 4 |

## Working of the Project

1. Five wireless nodes are created.
2. Wi-Fi is configured in ad-hoc mode.
3. AODV is installed as the routing protocol.
4. IP addresses are assigned to the nodes.
5. A UDP server is installed on Node 4.
6. A UDP client is installed on Node 0.
7. Node 0 sends packets to Node 4.
8. FlowMonitor collects the communication statistics.
9. Packet Delivery Ratio, Throughput, and Average Delay are calculated and displayed.

## Main Simulation Result

For the main communication flow from Node 0 to Node 4:

- Packets Sent: 73
- Packets Received: 68
- Packets Not Received: 5
- Packet Delivery Ratio: 93.15%
- Throughput: 41.97 Kbps
- Average Delay: 14.48 ms

## Performance Metrics

### Packet Delivery Ratio

Packet Delivery Ratio (PDR) represents the percentage of transmitted packets successfully received at the destination.

**PDR = (Packets Received / Packets Sent) × 100**

### Throughput

Throughput represents the amount of data successfully received by the destination per unit of time.

### Average Delay

Average Delay represents the average time taken by packets to travel from the source to the destination.

## Project File

- `manet-aodv.cc` - Main NS-3 simulation program.

## How to Run

Make sure NS-3 is installed and configured.

Run the following commands from the NS-3 directory:

```bash
./ns3 build
./ns3 run manet-aodv
