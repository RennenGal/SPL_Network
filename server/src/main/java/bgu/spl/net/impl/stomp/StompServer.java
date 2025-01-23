package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        // TODO: implement this
        if (args.length < 2) {
            System.out.println("Usage: StompServer <port> <server type (tpc/reactor)> [<number of threads for reactor>]");
            return;
        }

        int port;
        try {
            port = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            System.out.println("Invalid port number.");
            return;
        }

        String serverType = args[1].toLowerCase();
        Connections<String> connections = new ConnectionsImpl<>();
        switch (serverType) {
            case "tpc":
                Server.threadPerClient(
                        port,
                        StompMessagingProtocolImpl::new,
                        StompEncoderDecoder::new,
                        connections
                ).serve();
                break;

            case "reactor":
                if (args.length < 3) {
                    System.out.println("For reactor server, please provide the number of threads.");
                    return;
                }
                int nThreads;
                try {
                    nThreads = Integer.parseInt(args[2]);
                } catch (NumberFormatException e) {
                    System.out.println("Invalid number of threads.");
                    return;
                }

                Server.reactor(
                        nThreads,
                        port,
                        StompMessagingProtocolImpl::new,
                        StompEncoderDecoder::new,
                        connections
                ).serve();
                break;

            default:
                System.out.println("Invalid server type. Use 'tpc' for Thread-Per-Client or 'reactor' for Reactor.");
        }
    }
}
