//Desamma kod som Controller kod som Contoller.js fast med ändring av vad för 
// meddelande som skickas vilket möjliggör egna körnings-funktioner hos bilen när denna sida används

function removeActiveClass(e) {
    e.target.classList.remove("activeKey");
}

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

function resetText(e) {
    document.getElementById("message").innerHTML = "...";
    const key = document.querySelector(`div[data-key="${e.keyCode}"]`);
    if (key) {
        key.classList.remove("activeKey");
    }
    clearTimeout(stopTimeout);
    stopTimeout = setTimeout(() => {
        onSend("stop");
    }, 1000);
}

function keyPressed(e) {
    const key = document.querySelector(`div[data-key="${e.keyCode}"]`);
    changeText(e.keyCode);

    if ([87, 65, 83, 68].includes(e.keyCode)) {
        key.classList.add("activeKey");
    }

    if (e.keyCode === 32) {
        startConnect();
    }

    switch (e.keyCode) {
        case 87:
            onSend("CursedForward");
            break;
        case 65:
            onSend("CursedLeft");
            break;
        case 83:
            onSend("CursedBackward");
            break;
        case 68:
            onSend("CursedRight");
            break;
    }

    clearTimeout(stopTimeout);
}

const keys = Array.from(document.querySelectorAll(".key"));
keys.forEach(key => key.addEventListener("transitionend", removeActiveClass));

window.addEventListener("keydown", keyPressed);
window.addEventListener("keyup", resetText);

let stopTimeout;

function startConnect() {
    const clientID = "clientID_" + Math.floor(Math.random() * 100);
    const host = "maqiatto.com";
    const port = 8883;
    const client = new Paho.MQTT.Client(host, Number(port), clientID);
    const userName = "elvirasofiaandersson@hotmail.com";
    const password = "ElSoAn7823";

    client.connect({ userName, password, onSuccess: onConnect });
}

let Email = "elvirasofiaandersson@hotmail.com/";

function onConnect() {
    const topic = Email + "lampa";
    console.log(topic);
    client.subscribe(topic);
    const message = new Paho.MQTT.Message("Hello World!");
    message.destinationName = topic;
    client.send(message);
}

function onSend(message) {
    const topic = Email + "lampa";
    console.log(message);
    const mqttMessage = new Paho.MQTT.Message(message);
    mqttMessage.destinationName = topic;
    client.send(mqttMessage);
}

function startDisconnect() {
    client.disconnect();
    document.getElementById("messages").innerHTML += '<span>Disconnected</span><br/>';
}
