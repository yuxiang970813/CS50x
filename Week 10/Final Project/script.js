// Access all tabs
chrome.tabs.query({}, function(tabs) {
    let button = document.getElementById("button");

    // Recall last setting
    chrome.storage.local.get(["action"], function (lastSetting) {
        button.value = lastSetting.action;
    });

    // Listen to button
    button.addEventListener("click", function () {
        if (button.value === "OFF") {
            // Turn on switch
            button.value = "ON";

            // Send message to content.js
            let messageON = {action: "ON"};
            for (let i = 0; i < tabs.length; i++) {
                chrome.tabs.sendMessage(tabs[i].id, messageON);
            }

            // Update last setting
            chrome.storage.local.set(messageON);

        } else {
            // Turn off the switch
            button.value = "OFF";

            // Send message to content.js
            let messageOFF = {action: "OFF"}
            for (let i = 0; i < tabs.length; i++) {
                chrome.tabs.sendMessage(tabs[i].id, messageOFF);
            }

            // Update last setting
            chrome.storage.local.set(messageOFF);
        }
    });
});