#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/aodv-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

int main()
{
    double simulationTime = 10.0;

    // Create 5 MANET nodes
    NodeContainer nodes;
    nodes.Create(5);

    // Wi-Fi configuration
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211b);

    YansWifiPhyHelper phy;
    YansWifiChannelHelper channel =
        YansWifiChannelHelper::Default();

    phy.SetChannel(channel.Create());

    WifiMacHelper mac;
    mac.SetType("ns3::AdhocWifiMac");

    NetDeviceContainer devices =
        wifi.Install(phy, mac, nodes);

    // Node positions
    MobilityHelper mobility;

    mobility.SetPositionAllocator(
        "ns3::GridPositionAllocator",
        "MinX", DoubleValue(0.0),
        "MinY", DoubleValue(0.0),
        "DeltaX", DoubleValue(30.0),
        "DeltaY", DoubleValue(0.0),
        "GridWidth", UintegerValue(5),
        "LayoutType", StringValue("RowFirst")
    );

    mobility.SetMobilityModel(
        "ns3::ConstantPositionMobilityModel"
    );

    mobility.Install(nodes);

    // Install AODV routing
    AodvHelper aodv;
    InternetStackHelper internet;

    internet.SetRoutingHelper(aodv);
    internet.Install(nodes);

    // IP addresses
    Ipv4AddressHelper address;

    address.SetBase(
        "10.0.0.0",
        "255.255.255.0"
    );

    Ipv4InterfaceContainer interfaces =
        address.Assign(devices);

    // UDP server on Node 4
    uint16_t port = 5000;

    PacketSinkHelper server(
        "ns3::UdpSocketFactory",
        InetSocketAddress(
            Ipv4Address::GetAny(),
            port
        )
    );

    ApplicationContainer serverApp =
        server.Install(nodes.Get(4));

    serverApp.Start(Seconds(1.0));
    serverApp.Stop(Seconds(simulationTime));

    // UDP client on Node 0
    OnOffHelper client(
        "ns3::UdpSocketFactory",
        InetSocketAddress(
            interfaces.GetAddress(4),
            port
        )
    );

    client.SetAttribute(
        "DataRate",
        DataRateValue(
            DataRate("100kbps")
        )
    );

    client.SetAttribute(
        "PacketSize",
        UintegerValue(512)
    );

    ApplicationContainer clientApp =
        client.Install(nodes.Get(0));

    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(9.0));

    // Flow Monitor
    FlowMonitorHelper flowmon;

    Ptr<FlowMonitor> monitor =
        flowmon.InstallAll();

    // Run simulation
    Simulator::Stop(
        Seconds(simulationTime)
    );

    std::cout
        << "\nStarting MANET AODV simulation...\n";

    Simulator::Run();

    std::cout
        << "\nSimulation completed successfully!\n";

    // Check packets
    monitor->CheckForLostPackets();

    Ptr<Ipv4FlowClassifier> classifier =
        DynamicCast<Ipv4FlowClassifier>(
            flowmon.GetClassifier()
        );

    std::map<FlowId, FlowMonitor::FlowStats> stats =
        monitor->GetFlowStats();

    // Display results
    for (auto const &flow : stats)
    {
        Ipv4FlowClassifier::FiveTuple t =
            classifier->FindFlow(flow.first);

        std::cout
            << "\n-----------------------------\n";

        std::cout
            << "Flow ID: "
            << flow.first
            << "\n";

        std::cout
            << "Source: "
            << t.sourceAddress
            << "\n";

        std::cout
            << "Destination: "
            << t.destinationAddress
            << "\n";

        std::cout
            << "Packets Sent: "
            << flow.second.txPackets
            << "\n";

        std::cout
            << "Packets Received: "
            << flow.second.rxPackets
            << "\n";

        // Packets not received
        uint32_t notReceived =
            flow.second.txPackets -
            flow.second.rxPackets;

        std::cout
            << "Packets Not Received: "
            << notReceived
            << "\n";

        // Packet Delivery Ratio
        if (flow.second.txPackets > 0)
        {
            double pdr =
                (double)flow.second.rxPackets /
                flow.second.txPackets * 100.0;

            std::cout
                << "Packet Delivery Ratio: "
                << pdr
                << "%\n";
        }

        // Throughput
        if (flow.second.rxBytes > 0)
        {
            double throughput =
                flow.second.rxBytes *
                8.0 /
                7.0 /
                1000.0;

            std::cout
                << "Throughput: "
                << throughput
                << " Kbps\n";
        }

        // Average Delay
        if (flow.second.rxPackets > 0)
        {
            double delay =
                flow.second.delaySum.GetSeconds() /
                flow.second.rxPackets;

            std::cout
                << "Average Delay: "
                << delay
                << " seconds\n";
        }

        std::cout
            << "-----------------------------\n";
    }

    Simulator::Destroy();

    return 0;
}
