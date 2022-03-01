import java.net.*;
import java.io.*;

public class Main {
	 public static void main(String args[]) {
		  try {
		  	String FLAG = "ENO_JAVA_URL_DNS_FLAG";
		  	URL MY_URL = new URL("http://" + FLAG + ".thisurldoesnotexist.com/");
		  	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

		  	System.out.println("My URL is <FLAG>.thisurldoesnotexist.com. Can you guess it?");
		  	String YOUR_URL = br.readLine();

		  	if(MY_URL.equals(new URL("http://" + FLAG + "." + YOUR_URL + "/"))) {
		  		System.out.println("You win!");
		  	} else {
		  		System.out.println("You lose!");
		  	}
		  	System.out.println("Bye");
		  }
		  catch(Exception e) {
		  	System.out.println("Ooops");
		  }
	 }

}