#include <boost/beast/core.hpp>                // Core Boost.Beast functions for network programming.
#include <boost/beast/websocket.hpp>           // Handles WebSocket communication.
#include <boost/asio/ip/tcp.hpp>               // Provides TCP networking.
#include <thread>                              // Used for managing multiple threads.
#include <vector>                              // Used to store the array that will be sorted.
#include <string>                              // Provides string operations.
#include <chrono>                              // Used for timing (delays between sorting steps).
#include <cstdlib>                             // Provides system commands (e.g., opening the browser).
#include <iostream>                            // Standard input/output operations.
#include <sstream>                             // String stream for processing strings.
#include <nlohmann/json.hpp>                   // JSON library for handling JSON data.
#include <algorithm>                           // Used for sorting algorithms.


using namespace std;                           // Allows using standard functions without std:: prefix.
using json = nlohmann::json;                   // Defines json as an alias for nlohmann::json.
namespace beast = boost::beast;                // Shortens the namespace for Boost.Beast.
namespace websocket = beast::websocket;        // Shortens boost::beast::websocket.
namespace asio = boost::asio;                  // Shortens boost::asio
using tcp = asio::ip::tcp;                     // Defines tcp as boost::asio::ip::tcp.

void openBrowser() {                           // This function attempts to open index.html in the default browser.
#ifdef _WIN32                                  // Ensures the function runs only on Windows.
    // Windows: Opens index.html in the default browser
    std::system("start index.html");
#endif
}
bool is_paused = false;                        // A global flag to control the sorting pause state.

void check_pause() {                           // A function that continuously waits (sleep_for(500ms)) if is_paused is true.
    while (is_paused) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        // std::this_thread::sleep_for(std::chrono::milliseconds(500)) pauses execution for 500 milliseconds.
    }
}

// Bubble Sort Algorithm
void bubble_sort(std::vector<int>& arr, websocket::stream<tcp::socket>& ws) {
    /*Each sorting function:
        Send the unsorted array before sorting
        Sends the initial unsorted array.
        Performs the sorting step-by-step.
        Sends each step as JSON to the client via WebSocket.
        Sleeps for visualization delays.*/
    json unsorted_json;
    unsorted_json["array"] = arr;
    unsorted_json["message"] = "Unsorted Array";
    ws.write(asio::buffer(unsorted_json.dump()));       // Uses ws.write() to send JSON data over the WebSocket.
    int n = arr.size();                                 // Stores the size of the array.
    bool swapped;                                       // Tracks if a swap happened in the current iteration.

    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            check_pause();
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;

                json step_json;                         /*Sends the updated array to the client. Sleeps for 500ms to visualize each step.*/
                step_json["array"] = arr;
                step_json["message"] = "Sorting Step";
                ws.write(asio::buffer(step_json.dump()));
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        if (!swapped) break;                           // If no swaps occur in a pass, the array is sorted, so exit early
    }

    json complete_json;                                // Sends the final sorted array.
    complete_json["array"] = arr;
    complete_json["message"] = "Sorting Complete!";
    ws.write(asio::buffer(complete_json.dump()));
}

// Quick Sort Algorithm
/*Quick Sort follows the divide and conquer strategy:
    Choose a pivot element.
    Partition the array so that elements smaller than the pivot are on the left and larger ones are on the right.
    Recursively apply Quick Sort on the left and right subarrays.
    The check_pause(); function allows pausing.
    Recursive calls handle sorting.
*/
void quick_sort(std::vector<int>& arr, int low, int high, websocket::stream<tcp::socket>& ws) {
    // Send the unsorted array before sorting
    if (low == 0 && high == arr.size() - 1) {
        json unsorted_json;
        unsorted_json["array"] = arr;
        unsorted_json["message"] = "Unsorted Array";
        ws.write(asio::buffer(unsorted_json.dump()));
    }
    if (low < high) {
        int pivot = arr[high];                      // Last element is chosen as the pivot.
        int i = low - 1;                            // Index for elements smaller than the pivot.

        for (int j = low; j < high; j++) {          // Starting
            check_pause();
            if (arr[j] < pivot) {                   // Partitioning: If arr[j] < pivot, swap it to the left section.
                i++;
                std::swap(arr[i], arr[j]);          // Ending
                json step_json;
                step_json["array"] = arr;
                step_json["message"] = "Sorting Step";
                ws.write(asio::buffer(step_json.dump()));
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                // Visualization: Each swap is sent to the WebSocket client.
            }
        }

        std::swap(arr[i + 1], arr[high]);           // The pivot is placed in the correct position.
        quick_sort(arr, low, i, ws);
        quick_sort(arr, i + 1, high, ws);           // Fixed recursion issue
        if (low == 0 && high == arr.size() - 1) {
            json complete_json;
            complete_json["array"] = arr;
            complete_json["message"] = "Sorting Complete!";
            ws.write(asio::buffer(complete_json.dump()));
        }
    }
}

// Insertion Sort Algorithm
/*Insertion Sort works like sorting a hand of cards:
    Pick an element (key).
    Shift larger elements to the right.
    Insert the key in the correct position.
*/
void insertion_sort(std::vector<int>& arr, websocket::stream<tcp::socket>& ws) {
    // Send the unsorted array before sorting
    json unsorted_json;
    unsorted_json["array"] = arr;
    unsorted_json["message"] = "Unsorted Array";
    ws.write(asio::buffer(unsorted_json.dump()));
    size_t n = arr.size();
    for (size_t i = 1; i < n; ++i) {
        check_pause();
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {         // Shifting Elements: Elements greater than key are moved right.
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;                        // The key is placed in the correct position.
        json step_json;
        step_json["array"] = arr;
        step_json["message"] = "Sorting Step";
        ws.write(asio::buffer(step_json.dump()));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // Each step is sent to the WebSocket.
    }
    json complete_json;
    complete_json["array"] = arr;
    complete_json["message"] = "Sorting Complete!";
    ws.write(asio::buffer(complete_json.dump()));
}

// Selection Sort Algorithm
void selection_sort(std::vector<int>& arr, websocket::stream<tcp::socket>& ws) {
    // Send the unsorted array before sorting
    json unsorted_json;
    unsorted_json["array"] = arr;
    unsorted_json["message"] = "Unsorted Array";
    ws.write(asio::buffer(unsorted_json.dump()));
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {                // Selection Sort selects the smallest element and swaps it into position.
        check_pause();
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;                            // Finding the smallest element (min_idx).
            }
        }
        std::swap(arr[i], arr[min_idx]);                // Swap the minimum element into place
        json step_json;
        step_json["array"] = arr;
        step_json["message"] = "Sorting Step";
        ws.write(asio::buffer(step_json.dump()));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    json complete_json;
    complete_json["array"] = arr;
    complete_json["message"] = "Sorting Complete!";
    ws.write(asio::buffer(complete_json.dump()));
}

// Merge Sort Algorithm
void merge(std::vector<int>& arr, int left, int mid, int right, websocket::stream<tcp::socket>& ws) {
    // Splits the array into two subarrays.
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Copies elements into temporary arrays.
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[mid + 1 + i];

    // Merging: Smaller element from L[] or R[] is placed into arr[].
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        }
        else {
            arr[k] = R[j++];
        }
        k++;

        // Send sorting step
        json step_json;
        step_json["array"] = arr;
        step_json["message"] = "Sorting Step";
        ws.write(asio::buffer(step_json.dump()));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
        json step_json;
        step_json["array"] = arr;
        step_json["message"] = "Sorting Step";
        ws.write(asio::buffer(step_json.dump()));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
        json step_json;
        step_json["array"] = arr;
        step_json["message"] = "Sorting Step";
        ws.write(asio::buffer(step_json.dump()));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void merge_sort(std::vector<int>& arr, int left, int right, websocket::stream<tcp::socket>& ws) {
    // Send the unsorted array before sorting
    if (left == 0 && right == arr.size() - 1) {
        json unsorted_json;
        unsorted_json["array"] = arr;
        unsorted_json["message"] = "Unsorted Array";
        ws.write(asio::buffer(unsorted_json.dump()));
    }
    if (left >= right) return;
    // Merge Sort is a recursive divide and conquer sorting algorithm.
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid, ws);
    merge_sort(arr, mid + 1, right, ws);
    merge(arr, left, mid, right, ws);

    // Send final sorted array
    if (left == 0 && right == arr.size() - 1) {
        json complete_json;
        complete_json["array"] = arr;
        complete_json["message"] = "Sorting Complete!";
        ws.write(asio::buffer(complete_json.dump()));
    }
}


// Function to handle commands from the client
void handle_client_commands(websocket::stream<tcp::socket>& ws) {
    boost::beast::flat_buffer buffer;

    while (ws.is_open()) {
        ws.read(buffer);
        auto received_message = boost::beast::buffers_to_string(buffer.data());
        buffer.consume(buffer.size());

        try {
            auto command_json = json::parse(received_message);
            std::string command = command_json["command"];

            if (command == "start") {
                std::vector<int> input_array = command_json["array"];
                std::string algorithm = command_json["algorithm"];
                is_paused = false;

                if (algorithm == "bubble") {
                    bubble_sort(input_array, ws);
                }
                else if (algorithm == "quick") {
                    quick_sort(input_array, 0, input_array.size() - 1, ws);
                }
                else if (algorithm == "insertion") {
                    insertion_sort(input_array, ws);
                }
                else if (algorithm == "merge") {
                    merge_sort(input_array, 0, input_array.size() - 1, ws);
                }
                else if (algorithm == "selection") {
                    selection_sort(input_array, ws);
                }
            }
            else if (command == "pause") {
                is_paused = true;
                json pause_json;
                pause_json["message"] = "Paused";
                ws.write(asio::buffer(pause_json.dump()));
            }
            else if (command == "reset") {
                is_paused = false;
                json reset_json;
                reset_json["message"] = "Reset";
                ws.write(asio::buffer(reset_json.dump()));
            }
        }
        catch (const json::exception& e) {
            std::cerr << "Error parsing command: " << e.what() << std::endl;
        }
    }
}

int main() {
    try {
        openBrowser();
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "WebSocket server is running on port 12345..." << std::endl;

        for (;;) {                           // This is an infinite loop, which will continuously accept incoming connections from clients.
            tcp::socket socket(io_context);  // tcp::socket creates a socket for each new connection. The socket is used to communicate with the client.
            acceptor.accept(socket);         // acceptor.accept(socket) blocks the program until a new client connects. Once a client connects, it provides the socket through which communication with the client will occur.
            // This is a blocking call ? the program waits here until a connection is made.
            websocket::stream<tcp::socket> ws(std::move(socket));
            ws.accept();
            std::cout << "Client connected via WebSocket!" << std::endl;
            handle_client_commands(ws);
        }
    }
    /*catch (const std::exception& e)
        If any exception occurs during the server execution,
        it will be caught here.
        The exception message is printed out,
        which can help you debug any errors.*/
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
