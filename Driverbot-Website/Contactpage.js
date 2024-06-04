// JavaScript-kod som används för att lyssna efter formulärinsändning och skicka formuläret via EmailJS-tjänsten.
// Möjliggör för skaparen att ta emot meddelanden av användaren via e-post
document.addEventListener('DOMContentLoaded', function() {
    // Initialisera EmailJS med användar-ID
    emailjs.init("IVW2Kr0agBakiDAnh");

    // Lyssna efter formulärinsändning
    document.getElementById('contact-form').addEventListener('submit', function(event) {
        event.preventDefault();

        // Ange tjänst-ID och mall-ID för EmailJS
        const serviceID = 'service_cbd19ln'; 
        const templateID = 'template_6ow4yp8'; 

        // Skicka formuläret via EmailJS
        emailjs.sendForm(serviceID, templateID, this)
            .then(() => {
                alert('Meddelandet skickades framgångsrikt!');
            }, (error) => {
                alert('Misslyckades med att skicka meddelandet. Försök igen.');
                console.error('Misslyckades med att skicka meddelandet:', error);
            });
    });
});
