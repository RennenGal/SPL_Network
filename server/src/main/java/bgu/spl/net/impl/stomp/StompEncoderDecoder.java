package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;

import java.nio.charset.StandardCharsets;

public class StompEncoderDecoder implements MessageEncoderDecoder<String> {
    private final StringBuilder buffer = new StringBuilder();
    @Override
    public String decodeNextByte(byte nextByte) {
        if (nextByte == '\u0000') {
            String frame = buffer.toString();
            buffer.setLength(0);
            return frame;
        } else {
            buffer.append((char) nextByte);
            return null;
        }
    }

    @Override
    public byte[] encode(String message) {
        return (message + '\n' + '\u0000').getBytes(StandardCharsets.UTF_8);
    }
}
