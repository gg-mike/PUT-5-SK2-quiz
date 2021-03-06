package put.edu.gui.serverapi;

import java.io.IOException;
import java.net.ConnectException;
import java.net.Socket;

public class ServerApi {
    private final Socket socket;
    private final Reader reader;
    private final Writer writer;
    private final HeartBeatRunner heartBeatRunner;

    public ServerApi(String address, int port) throws ConnectException {
        System.out.printf("connecting to address: %s, port: %d%n", address, port);
        try {
            socket = new Socket(address, port);
            reader = new Reader(socket.getInputStream());
            writer = new Writer(socket.getOutputStream());
            heartBeatRunner = new HeartBeatRunner();
        } catch (IOException e) {
            throw new ConnectException("connection failed");
        }
    }

    public boolean isConnected() {
        return socket != null && socket.isConnected();
    }

    public void disconnect() {
        System.out.println("disconnecting");
        reader.stop();
        writer.stop();
        heartBeatRunner.stop();
        try {
            socket.close();
        } catch (IOException e) {
            System.err.println("socket disconnection error");
        }
        System.out.println("disconnecting succeeded");
    }

    public Socket getSocket() {
        return socket;
    }

    public Reader getReader() {
        return reader;
    }

    public Writer getWriter() {
        return writer;
    }
}
