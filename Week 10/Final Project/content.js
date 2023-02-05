// Receive message
chrome.runtime.onMessage.addListener(function(message, _sender, _sendResponse) {
    if (message.action === "ON") {
        // Wear glasses when button is ON
        wearGlasses();

    } else {
        // Remove glasses when button is OFF
        removeGlasses();
    }
});

// Declare wear glasses function
// Initialize check insert
let checkInsert = false;

function wearGlasses() {
    // Check insert
    if (checkInsert == false) {
        // Insert filter to body if haven't insert
        document.body.insertAdjacentHTML("beforeend", svgFilter);
        document.body.insertAdjacentHTML("beforeend", cssFilter);

        // Update status
        checkInsert = true;
    }

    // Add class to html
    document.documentElement.classList.add("wearColourBlindGlasses")
};

// Declare remove glasses function
function removeGlasses() {
    // Remove class in html
    document.documentElement.classList.remove("wearColourBlindGlasses");
};

// Declare css filter(DOMTokenList can't ontains HTML space characters)
// This css filter cut the screen to small pieces
const cssFilter = "<style>.wearColourBlindGlasses{filter:url(#svgFilter)!important}#cutPixel{border:0;clip:rect(1px,1px,1px,1px);clip-path:inset(50%);height:1px;width:1px;margin:-1px;position:absolute!important;word-wrap:normal!important}</style>";

// Declare CVD glasses clone svg filter
const svgFilter = `
<svg id="cutPixel" aria-hidden="true" xmlns="http://www.w3.org/2000/svg">
    <defs>
        <filter id="svgFilter">
            <feComponentTransfer in="SourceGraphic">
                <feFuncR type="gamma" exponent="2" amplitude="2" offset="0"></feFuncR>
                <feFuncG type="gamma" exponent="2" amplitude="2" offset="0"></feFuncR>
                <feFuncB type="gamma" exponent="2" amplitude="2" offset="0"></feFuncR>
            </feComponentTransfer>
        </filter>
    </defs>
</svg>`;