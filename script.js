let ws;
let array = [];
let isPaused = false;
const canvas = document.getElementById("visualizer");
const ctx = canvas.getContext("2d");

// Initialize WebSocket connection
function initializeWebSocket() {
    ws = new WebSocket("ws://localhost:12345");

    ws.onopen = () => console.log("Connected to WebSocket server");

    ws.onmessage = function (event) {
        try {
            const message = JSON.parse(event.data);
            console.log("Server Message:", message.message);

            switch (message.message) {
                case "Unsorted Array":
                    array = message.array;
                    updateArrayDisplay(array); // Display unsorted array
                    drawBars(array, false);
                    break;

                case "Sorting Step":
                    array = message.array;
                    drawBars(array);
                    break;

                case "Sorting Complete!":
                    array = message.array; // Update array after sorting is complete
                    drawBars(array, true);  // Call drawBars with complete flag
                    displaySortedArray(array);  // Display sorted array in the browser
                    break;

                case "Paused":
                    isPaused = true;
                    break;

                case "Reset":
                    array = [];
                    ctx.clearRect(0, 0, canvas.width, canvas.height);
                    break;
            }
        } catch (e) {
            console.error("Error parsing JSON: ", e);
        }
    };

    ws.onclose = function () {
        console.log("WebSocket connection closed. Reconnecting...");
        setTimeout(initializeWebSocket, 1000); // Reconnect after 1 second
    };

    ws.onerror = function (error) {
        console.error("WebSocket Error:", error);
    };
}

// Start the WebSocket connection
initializeWebSocket();

// Handle button clicks
document.getElementById("start").onclick = function () {
    if (ws.readyState === WebSocket.OPEN) {
        const input = document.getElementById("arrayInput").value;
        const algorithm = document.getElementById("algorithm").value;

        // Convert the input string to an array of integers
        const inputArray = input.split(',').map(num => parseInt(num.trim(), 10));

        // Send the array and selected algorithm to the server
        ws.send(JSON.stringify({
            command: "start",
            algorithm: algorithm,
            array: inputArray
        }));

        updateArrayDisplay(inputArray); // Display unsorted array when sorting starts
        isPaused = false;
    }
};

// Function to handle pausing the sorting process
document.getElementById("pause").onclick = function () {
    if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ command: "pause" }));
        isPaused = true;
    }
};

// Function to handle resetting the sorting process
document.getElementById("reset").onclick = function () {
    if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ command: "reset" }));
        isPaused = false;
    }
};

// Draw sorting bars with element values
function drawBars(array, isComplete = false) {
    const gap = 7;
    const barWidth = Math.min(canvas.width / (array.length * 1.5), 40);

    const maxHeight = canvas.height;
    const padding = 30; // Added padding to prevent bars from touching edges
    const maxValue = Math.max(...array);

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    array.forEach((value, index) => {
        const barHeight = (value / maxValue) * (maxHeight - padding);
        const x = index * (barWidth + gap);
        const y = maxHeight - barHeight;

        // Set bar color
        ctx.fillStyle = isComplete ? "#2ecc71" : "#3498db";
        ctx.fillRect(x, y, barWidth, barHeight);

        // Draw element values above bars
        ctx.fillStyle = "#ffffff";
        ctx.font = "16px Arial";
        ctx.textAlign = "center";
        const textY = Math.max(y - 10, 20); // Ensure text doesn't go above canvas
        ctx.fillText(value, x + barWidth / 2, textY);
    });
    updateSortedArray(array, isComplete);
}

// Function to display the sorted array in the browser
function displaySortedArray(array) {
    const sortedDiv = document.getElementById("sortedArray");
    sortedDiv.innerHTML = ""; // Clear previous data

    array.forEach(value => {
        let span = document.createElement("span");
        span.textContent = value;
        sortedDiv.appendChild(span);
    });
}

function updateArrayDisplay(array) {
    const unsortedDiv = document.getElementById("unsortedArray");
    unsortedDiv.innerHTML = ""; // Clear previous data

    array.forEach(value => {
        let span = document.createElement("span");
        span.textContent = value;
        unsortedDiv.appendChild(span);
    });
}

function updateSortedArray(array, isComplete) {
    const sortedDiv = document.getElementById("sortedArray");
    sortedDiv.innerHTML = ""; // Clear previous data

    if (isComplete) {
        array.forEach(value => {
            let span = document.createElement("span");
            span.textContent = value;
            sortedDiv.appendChild(span);
        });
    }
}
