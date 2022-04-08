import java.net.*;
import java.io.*;
import java.util.concurrent.TimeUnit;
import java.util.Random;

public class Main {
	 public static void main(String args[]) {
		  try {
		  	String userInput;
		  	String FLAG = "ENO{JaVA_URL_DNS_FL4G_0n_Th3_N3tW0rk}";
		  	URL MY_URL = new URL("http://" + FLAG + ".thisurldoesnotexist.com/");
		  	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		  	System.out.println("Welcome to @gehaxelt's jURL game. Wait for it... <loading>");
		  	Random rng = new Random();
		  	TimeUnit.MILLISECONDS.sleep(500 + rng.nextInt(2500));
		  	System.out.println("HINT: Brute-Force is NOT the solution. Don't try it! Do you understand: Yes/No?!?!");
		  	userInput = br.readLine();
		  	if(!userInput.toLowerCase().equals("yes")) {
		  		System.exit(0);
		  	}
		  	System.out.println("The game is pretty easy: I'll let you enter an URL and check if it equals <FLAG>.thisurldoesnotexist.com. If you're clever, you'll win a prize ;-)");
		  	System.out.println("What's your URL?");
		  	userInput = br.readLine();

		  	if(MY_URL.equals(new URL("http://" + FLAG + "." + userInput + "/"))) {
		  		System.out.println("You win!");
		  	} else {
		  		System.out.println("You lose!");
		  	}
		  	System.out.println("Bye");
		  }
		  catch(Exception e) {
		  	System.out.println("Ooops");
		  }
	  	System.exit(0);
	 }

}