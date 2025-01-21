package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    // New functions outside the provided interface
    void newConnection(int connectionId, ConnectionHandler<T> handler);

    void subscribe(String channel, int connectionTd);

    void unsubscribe(String channel, int connectionTd);
}
