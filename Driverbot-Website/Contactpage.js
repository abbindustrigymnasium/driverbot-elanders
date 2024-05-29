document.addEventListener('DOMContentLoaded', function() {
    emailjs.init("IVW2Kr0agBakiDAnh"); // Replace "YOUR_USER_ID" with your actual EmailJS user ID

    document.getElementById('contact-form').addEventListener('submit', function(event) {
        event.preventDefault();

        const serviceID = 'service_cbd19ln'; // Replace "YOUR_SERVICE_ID" with your actual EmailJS service ID
        const templateID = 'template_6ow4yp8'; // Replace "YOUR_TEMPLATE_ID" with your actual EmailJS template ID

        emailjs.sendForm(serviceID, templateID, this)
            .then(() => {
                alert('Message sent successfully!');
            }, (error) => {
                alert('Failed to send the message. Please try again.');
                console.error('Failed to send the message:', error);
            });
    });
});
