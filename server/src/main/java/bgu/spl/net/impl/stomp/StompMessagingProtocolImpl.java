package bgu.spl.net.impl.stomp;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;
import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<String> {
    private Connections<String> connections;
    private String username;
    private int connectionId;
    private boolean terminated = false;
    private final StompDatabase db = StompDatabase.getInstance();
    private ConnectionHandler<String> connectionHandler;
    private Map<String, Integer> subscriptionsToId;
    private Map<Integer, String> idToSubscriptions;

    @Override
    public void start(int connectionId, Connections<String> connections, ConnectionHandler<String> connectionHandler) {
        this.connections = connections;
        this.connectionId = connectionId;
        this.connectionHandler = connectionHandler;
        this.subscriptionsToId = new HashMap<>();
        this.idToSubscriptions = new HashMap<>();
    }


    @Override
    public void process(String message) {
        String[] lines = message.split("\n");
        String command = lines[0];
        switch (command) {
            case "CONNECT":
                handleConnect(lines);
                break;
            case "SUBSCRIBE":
                handleSubscribe(lines);
                break;
            case "UNSUBSCRIBE":
                handleUnsubscribe(lines);
                break;
            case "SEND":
                handleSend(lines);
                break;
            case "DISCONNECT":
                handleDisconnect(lines);
                break;
            default:
                sendError("Unknown command: " + command);
        }
    }
    private void handleConnect(String[] lines) {
        String username = null;
        String password = null;
        for (String line : lines) {
            if (line.startsWith("login:")) {
                username = line.substring(6); // Extract username after "login:"
            } else if (line.startsWith("passcode:")) {
                password = line.substring(9); // Extract password after "passcode:"
            }
        }
        if (username == null || password == null) {
            sendError("ERROR\n" +
                    "Missing login or passcode\n\u0000");
            return;
        }
        if (db.isUserExist(username)) {
            if (!db.isUserValid(username, password)) {
                sendError("ERROR\n" +
                        "Wrong password\n\u0000");
                return;
            }
            if (db.isActiveUser(username)) {
                sendError("ERROR\n" +
                        "User is already logged in\n\u0000");
                return;
            }
        }
        this.username = username;
        db.addUser(username, password);
        db.addActiveUser(username);
        connections.newConnection(connectionId, connectionHandler);
        connections.send(connectionId, "CONNECTED\nversion: 1.2\n\n\u0000");
    }

    private void handleSubscribe(String[] lines) {
        String subscription = null;
        int id = Integer.MIN_VALUE;
        String receipt = null;
        for (String line : lines) {
            if (line.startsWith("destination:")) {
                subscription = line.substring(6);
            } else if (line.startsWith("id:")) {
                id = Integer.parseInt(line.substring(3)); // Extract password after "id:"
            } else if (line.startsWith("receipt:")) {
                receipt = line.substring(8); // Extract receipt after "receipt:"
            }
        }
        if (receipt == null) {
            if (subscription == null) {
                sendError("ERROR\n" +
                        "Missing destination in SUBSCRIBE\n\u0000");
                return;
            }
            if (id == Integer.MIN_VALUE) {
                sendError("ERROR\n" +
                        "Missing id in SUBSCRIBE\n\u0000");
                return;
            }
            if (subscriptionsToId.containsKey(subscription)) {
                sendError("ERROR\n" +
                        "Allready subscribed to this channel \n\u0000");
                return;
            }
            subscriptionsToId.put(subscription, id);
            idToSubscriptions.put(id, subscription);
            connections.subscribe(subscription, connectionId);
        }
        else{
            if (subscription == null) {
                sendError("ERROR\n" +
                        "receipt-id:"+receipt + "\n" +
                        "Missing destination in SUBSCRIBE\n\u0000");
                return;
            }
            if (id == Integer.MIN_VALUE) {
                sendError("ERROR\n" +
                        "receipt-id:"+receipt + "\n" +
                        "Missing id in SUBSCRIBE\n\u0000");
                return;
            }
            if (subscriptionsToId.containsKey(subscription)) {
                sendError("ERROR\n" +
                        "receipt-id:"+receipt + "\n" +
                        "Allready subscribed to this channel \n\u0000");
                return;
            }
            subscriptionsToId.put(subscription, id);
            idToSubscriptions.put(id, subscription);
            connections.subscribe(subscription, connectionId);
            connections.send(connectionId, "RECEIPT\nreceipt-id:" + receipt + "\n\n\u0000");
        }
    }

    private void handleUnsubscribe(String[] lines) {
        int id = Integer.MIN_VALUE;
        String receipt = null;
        for (String line : lines) {
            if (line.startsWith("id:")) {
                id = Integer.parseInt(line.substring(3)); // Extract password after "id:"
            } else if (line.startsWith("receipt:")) {
                receipt = line.substring(8); // Extract receipt after "receipt:"
            }
        }
        if (receipt == null) {
            if (id == Integer.MIN_VALUE) {
                sendError("ERROR\n" +
                        "Missing id in SUBSCRIBE\n\u0000");
                return;
            }
            if (!idToSubscriptions.containsKey(id)) {
                sendError("ERROR\n" +
                        "Not subscribed to this channel \n\u0000");
                return;
            }
            String channel = idToSubscriptions.remove(id);
            subscriptionsToId.remove(channel);
            connections.unsubscribe(idToSubscriptions.get(id), connectionId);
        }
        else{
            if (id == Integer.MIN_VALUE) {
                sendError("ERROR\n" +
                        "receipt-id:"+receipt + "\n" +
                        "Missing id in SUBSCRIBE\n\u0000");
                return;
            }
            if (!idToSubscriptions.containsKey(id)) {
                sendError("ERROR\n" +
                        "receipt-id:"+receipt + "\n" +
                        "Not subscribed to this channel \n\u0000");
                return;
            }
            String channel = idToSubscriptions.remove(id);
            subscriptionsToId.remove(channel);
            connections.unsubscribe(idToSubscriptions.get(id), connectionId);
            connections.send(connectionId, "RECEIPT\nreceipt-id:" + receipt + "\n\n\u0000");
        }
    }

    private void handleSend(String[] lines) {
        String destination = null;
        String body = null;
        String receipt = null;
        for (String line : lines) {
            if (line.startsWith("destination:")) {
                destination = line.substring(12); // Extract destination
            } else if (line.startsWith("receipt:")) {
            receipt = line.substring(8); // Extract receipt after "receipt:"
        }
        }
        String message = String.join("\n", lines); // Reconstruct the original message
        int bodyStartIndex = message.indexOf("\n\n");
        if (bodyStartIndex != -1) {
            body = message.substring(bodyStartIndex + 2, message.indexOf('\u0000')).trim(); // Extract body
        }
        if (receipt == null) {
            if (destination == null) {
                sendError("ERROR\n" +
                        "Missing destination in SEND\n\u0000");
                return;
            }
            if (!subscriptionsToId.containsKey(destination)) {
                sendError("ERROR\n" +
                        "Not subscribed to this channel \n\u0000");
                return;
            }
            String msg = "MESSAGE\n" +
                    "subscription:" + subscriptionsToId.get(destination) + "\n" +
                    "message-id:" + db.getNextMessageId() + "\n" +
                    "destination:" + destination + "\n" +
                    body + "\n\u0000";
            connections.send(destination, msg);
        }
        else{
            if (destination == null) {
                sendError("ERROR\n" +
                        "receipt-id:"+receipt + "\n" +
                        "Missing destination in SEND\n\u0000");
                return;
            }
            if (!subscriptionsToId.containsKey(destination)) {
                sendError("ERROR\n" +
                        "receipt-id:"+receipt + "\n" +
                        "Not subscribed to this channel \n\u0000");
                return;
            }
            String msg = "MESSAGE\n" +
                    "subscription:" + subscriptionsToId.get(destination) + "\n" +
                    "message-id:" + db.getNextMessageId() + "\n" +
                    "destination:" + destination + "\n" +
                    body + "\n\u0000";
            connections.send(destination, msg);
            connections.send(connectionId, "RECEIPT\nreceipt-id:" + receipt + "\n\n\u0000");
        }
    }

    private void handleDisconnect(String[] lines) {
        terminated = true;
        String receipt = null;
        for (String line : lines) {
            if (line.startsWith("receipt:")) {
                receipt = line.substring(8); // Extract receipt after "receipt:"
            }
        }
        String message;
        if (receipt == null){
            message = "DISCONNECT\n" +
                    "no receipt was asked\n\n\u0000";
        }
        else{
            message = "DISCONNECT\n" +
                    "receipt-id:"+ receipt + "\n\n\u0000";
        }
        db.removeActiveUser(username);
        connections.send(connectionId, message);
        connections.disconnect(connectionId);
    }

    private void sendError(String s) {
        connections.send(connectionId, s);
        db.removeActiveUser(username);
        connections.disconnect(connectionId);
    }

    @Override
    public boolean shouldTerminate() {
        return terminated;
    }
}
