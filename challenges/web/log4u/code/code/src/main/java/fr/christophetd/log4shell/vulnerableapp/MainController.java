package fr.christophetd.log4shell.vulnerableapp;


import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RestController;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

@RestController
public class MainController {

    private static final Logger logger = LogManager.getLogger("HelloWorld");

    @GetMapping("/")
    public String index(@RequestHeader(value = "X-Api-Version", defaultValue="default") String apiVersion) {
        logger.info("Received a request for API version " + apiVersion);
        return "Welcome to @gehaxelt's log4u API - It's the most secure logging platform on the internet.\nPlease provide a value for X-Api-Version. The current value is: " + apiVersion + "\nYou'll find the flag here: /FLAG ;-)";
    }

}