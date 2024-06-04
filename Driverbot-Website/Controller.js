//Den här är en kod med JavaScript-funktioner som används för att 
// hantera tangentbordsinteraktioner och kommunikation med en MQTT-server 

// Funktion för att ta bort klassen activeKey från alla tangenter
function removeActiveClass(e) {
    e.target.classList.remove("activeKey");
}

// Funktion för att ändra texten i meddelandet
function changeText(keyCode) {
    const messageElement = document.getElementById("message");
    switch (keyCode) {
        case 87:
            messageElement.innerHTML = "Forward";
            break;
        case 65:
            messageElement.innerHTML = "Left";
            break;
        case 83:
            messageElement.innerHTML = "Backward";
            break;
        case 68:
            messageElement.innerHTML = "Right";
            break;
        default:
            messageElement.innerHTML = "Try the W, A, S, or D key";
    }
}

// Funktion för att återställa texten i meddelandet när tangenten släpps
function resetText(e) {
    const messageElement = document.getElementById("message");
    messageElement.innerHTML = "...";

    const key = document.querySelector(`div[data-key="${e.keyCode}"]`);
    if (key) {
        key.classList.remove("activeKey");
    }

    clearTimeout(stopTimeout);
    // Starta en timeout för att skicka ett stoppmeddelande efter 1 sekund
    stopTimeout = setTimeout(() => {
        onSend("stop");
    }, 1000);
}

// Funktion som anropas när en tangent trycks ned
function keyPressed(e) {
    const key = document.querySelector(`div[data-key="${e.keyCode}"]`);
    changeText(e.keyCode);

    // Lägg till klassen activeKey om tangenten är en rörelsetangent
    if ([87, 65, 83, 68].includes(e.keyCode)) {
        key.classList.add("activeKey");
    }

    // Om mellanslagstangenten trycks ned, starta anslutningen
    if (e.keyCode === 32) {
        startConnect();
    }

    // Skicka meddelanden till MQTT-servern baserat på tangenten som tryckts ned
    switch (e.keyCode) {
        case 87:
            onSend("Forward");
            break;
        case 65:
            onSend("Left");
            break;
        case 83:
            onSend("Backward");
            break;
        case 68:
            onSend("Right");
            break;
    }

    clearTimeout(stopTimeout);
}

// Skapar en array med alla tangenter på skärmen och lägger till en lyssnare för övergångshändelser
const keys = Array.from(document.querySelectorAll(".key"));
keys.forEach(key => key.addEventListener("transitionend", removeActiveClass));

// Lyssna på händelserna keydown och keyup på fönstret
window.addEventListener("keydown", keyPressed);
window.addEventListener("keyup", resetText);

let stopTimeout; // Variabel för att hålla timeout-ID

// Funktion för att starta anslutningen till MQTT-servern
function startConnect() {
    const clientID = "clientID_" + Math.floor(Math.random() * 100);
    const host = "maqiatto.com";
    const port = 8883;
    const client = new Paho.MQTT.Client(host, Number(port), clientID);
    const userName = "elvirasofiaandersson@hotmail.com";
    const password = "ElSoAn7823";

    // Anslut till servern med användarnamn och lösenord
    client.connect({ userName, password, onSuccess: onConnect });
}

let Email = "elvirasofiaandersson@hotmail.com/";

// Funktion som körs när anslutningen till MQTT-servern lyckas
function onConnect() {
    const topic = Email + "lampa";
    console.log(topic);
    // Prenumerera på ett ämne och skicka ett testmeddelande
    client.subscribe(topic);
    const message = new Paho.MQTT.Message("Hello World!");
    message.destinationName = topic;
    client.send(message);
}

// Funktion för att skicka meddelanden till MQTT-servern
function onSend(message) {
    const topic = Email + "lampa";
    console.log(message);
    const mqttMessage = new Paho.MQTT.Message(message);
    mqttMessage.destinationName = topic;
    client.send(mqttMessage);
}

// Funktion för att koppla från MQTT-servern
function startDisconnect() {
    client.disconnect();
    // Uppdatera gränssnittet för att visa att anslutningen har avbrutits
    document.getElementById("messages").innerHTML += '<span>Disconnected</span><br/>';
}
