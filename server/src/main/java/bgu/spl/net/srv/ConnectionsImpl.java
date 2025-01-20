package bgu.spl.net.srv;

import java.io.IOException;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T> implements Connections<T> {

    private final ConcurrentHashMap<Integer, ConnectionHandler<T>> connections;
    private final ConcurrentHashMap<String, Set<Integer>> channels;
    // optimization for disconnect
    private final ConcurrentHashMap<Integer, Set<String>> connecionToChannels;

    public ConnectionsImpl() {
        this.connections = new ConcurrentHashMap<>();
        this.channels = new ConcurrentHashMap<>();
        // optimization for disconnect
        this.connecionToChannels = new ConcurrentHashMap<>();
    }

    @Override
    public boolean send(int connectionId, T msg) {
        ConnectionHandler<T> handler = connections.get(connectionId);
        if (handler != null){
            handler.send(msg);
            return true;
        }
        return false;
    }

    @Override
    public void send(String channel, T msg) {
        Set<Integer> subscribers = channels.get(channel);
        for (Integer subscriber : subscribers){
            send(subscriber, msg);
        }
    }

    @Override
    public void disconnect(int connectionId) throws IOException {
        ConnectionHandler<T> handler = connections.remove(connectionId);
        if (handler != null) {
            handler.close();
        }
        for (String channel : channels.keySet()){
            unsubscribe(channel, connectionId);
        }
        // optimization for disconnect
        Set<String> subscribedChannels = connecionToChannels.remove(connectionId);
        if (subscribedChannels!= null){
            for (String channel : subscribedChannels){
                unsubscribe(channel, connectionId);
            }
        }
    }

    // Functions outside the interface
    public void newConnection(int connectionId, ConnectionHandler<T> handler) {
        connections.putIfAbsent(connectionId, handler);
    }
    public void subscribe(String channel, int connectionTd){
        channels.computeIfAbsent(channel, k -> ConcurrentHashMap.newKeySet()).add(connectionTd);
        // optimization for disconnect
        connecionToChannels.computeIfAbsent(connectionTd, k -> ConcurrentHashMap.newKeySet()).add(channel);
    }
    public void unsubscribe(String channel, int connectionTd){
        Set<Integer> connections = channels.get(channel);
        if (connections != null){
            connections.remove(connectionTd);
            if (connections.isEmpty()) {
                channels.remove(channel);
            }
        }
    }
}
