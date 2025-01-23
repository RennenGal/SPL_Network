package bgu.spl.net.srv;
import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import java.util.function.Supplier;
public class TPCServer<T> extends BaseServer<T>{

    public TPCServer(int port, Supplier<StompMessagingProtocol<T>> protocolFactory,
                     Supplier<MessageEncoderDecoder<T>> encdecFactory, Connections<T> connections) {
        super(port, protocolFactory, encdecFactory, connections);
    }
    @Override
    protected void execute(BlockingConnectionHandler<T> handler) {
        new Thread(handler).start();
    }
}
