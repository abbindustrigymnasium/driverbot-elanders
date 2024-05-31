// Function to remove activeKey class to all keys
// Accepts 1 parameter


function removeActiveClass(e) {
    // Removes activeKey for everything
    e.target.classList.remove("activeKey");
}

// Function to change the text in message
function changeText(e) {
    if (e === 87) {
        document.getElementById("message").innerHTML = "Forward";
    } else if (e === 65) {
        document.getElementById("message").innerHTML = "Left";
    } else if (e === 83) {
        document.getElementById("message").innerHTML = "Backward";
    } else if (e === 68) {
        document.getElementById("message").innerHTML = "Right";
    } else {
        // Clears the message for invalid keys
        document.getElementById("message").innerHTML = "Try the W,A,S or D key";
    }
}

// Function to reset the text in message when the key is released
function resetText(e) {
    // Clears the message when the key is released
    document.getElementById("message").innerHTML = "...";
    // Remove activeKey class from the key
    const key = document.querySelector(`div[data-key="${e.keyCode}"]`);
    if (key) {
        key.classList.remove("activeKey");
    }
    // Clear the previous timeout and set a new one to send "stop"
    clearTimeout(stopTimeout);
    stopTimeout = setTimeout(() => {
        onSend("stop");
    }, 1000); // Set the timeout period as needed (1000 ms = 1 second)
}

// Function when user presses on a key
let stopTimeout; // Variable to hold the timeout ID

// Function called when a key is pressed
function keyPressed(e) {
    // Assigns key "div" to key
    const key = document.querySelector(`div[data-key="${e.keyCode}"]`);
    // Call changeText to change the message
    changeText(e.keyCode);
    // Only applies activeKey to the keys displayed in browser
    if (e.keyCode === 87 || e.keyCode === 65 || e.keyCode === 83 || e.keyCode === 68) {
        // Adds class activeKey
        key.classList.add("activeKey");
    }
    if (e.keyCode === 32) {
        startConnect();
    }
    if (e.keyCode === 87) {
        onSend("CursedForward");
    } else if (e.keyCode === 65) {
        onSend("CursedLeft");
    } else if (e.keyCode === 83) {
        onSend("CursedBackward");
    } else if (e.keyCode === 68) {
        onSend("CursedRight");
    }
    // Clear the previous timeout when a valid key is pressed
    clearTimeout(stopTimeout);
}

// Creates a const array of all the keys on screen
const keys = Array.from(document.querySelectorAll(".key"));
// Listens to the browser and removes activeKey when needed
keys.forEach(key => key.addEventListener("transitionend", removeActiveClass));
// Listens to users and when key is pressed calls keyPressed
window.addEventListener("keydown", keyPressed);
// Listens to users and when key is released calls resetText
window.addEventListener("keyup", resetText);

function startConnect() {
    // Generate a random client ID
    clientID = "clientID_" + parseInt(Math.random() * 100);

    // Fetch the hostname/IP address and port number from the form
    host = "maqiatto.com";
    port = 8883;
    // Initialize new Paho client connection
    client = new Paho.MQTT.Client(host, Number(port), clientID);
    // Set callback handlers
    client.connect({
        userName: "elvirasofiaandersson@hotmail.com",
        password: "ElSoAn7823",
        onSuccess: onConnect,
    });
}

let Email = "elvirasofiaandersson@hotmail.com/";
// Called when the client connects
function onConnect() {
    // Fetch the MQTT topic from the form
    topic = Email + "lampa";
    console.log(topic);
    // Subscribe to the requested topic
    client.subscribe(topic);
    message = new Paho.MQTT.Message("Hello World!");
    message.destinationName = topic;
    client.send(message);

}

function onSend(message) {
    // Fetch the MQTT topic from the form
    topic = Email + "lampa";
    console.log(message);
    message = new Paho.MQTT.Message(message);
    message.destinationName = topic;
    client.send(message);
}

// Called when the disconnection button is pressed
function startDisconnect() {
    client.disconnect();
    document.getElementById("messages").innerHTML += '<span>Disconnected</span><br/>';
}
