import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;
import java.io.*;

public class LogServer
{
        public static void main(String[] args)
        {
            try {
                ServerSocket s = new ServerSocket(8082);

                while (true) {
                    Socket client = s.accept();

                    PrintWriter out = new PrintWriter(client.getOutputStream(), true);
                    BufferedReader in = new BufferedReader(
                        new InputStreamReader(client.getInputStream()));

                    String userInput;
                    do {
                        userInput = in.readLine().trim();
                        out.println(userInput);
                    } while (userInput != null && !userInput.isEmpty());

                    System.out.println("Closed connection");
                    client.close();
                }
            } catch (IOException e) {
                System.out.println("error: " + e.toString());
            }

        }
}
