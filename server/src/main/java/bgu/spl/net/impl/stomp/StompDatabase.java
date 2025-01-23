package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class StompDatabase {
    private final ConcurrentHashMap<String, String> users;
    private final ConcurrentHashMap<String, String> activeUsers;
    private final AtomicInteger messageId;
    private final AtomicInteger connectionId;
    private StompDatabase(){
        users = new ConcurrentHashMap<>();
        activeUsers = new ConcurrentHashMap<>();
        messageId = new AtomicInteger(0);
        connectionId = new AtomicInteger(0);
    }
    private static class StompDatabaseHolder{
        private static final StompDatabase instance = new StompDatabase();
    }
    public static StompDatabase getInstance(){
        return StompDatabaseHolder.instance;
    }
    public void addUser(String username, String password){
        users.put(username, password);
    }
    public boolean isUserValid(String username, String password){
        return users.containsKey(username) && users.get(username).equals(password);
    }
    public void addActiveUser(String username){
        activeUsers.put(username, username);
    }
    public void removeActiveUser(String username){
        activeUsers.remove(username);
    }
    public boolean isActiveUser(String username){
        return activeUsers.containsKey(username);
    }
    public boolean isUserExist(String username){
        return users.containsKey(username);
    }
    public int getNextMessageId(){
        return messageId.incrementAndGet();
    }
    public int generateConnectionId(){
        return connectionId.incrementAndGet();
    }
}
