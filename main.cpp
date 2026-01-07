#include <vector>
#include <algorithm>  // For std::swap
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::milliseconds

using json = nlohmann::json;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

// Function to send sorting steps to the client
void sendSortingStep(const std::vector<int>& arr, websocket::stream<tcp::socket>& ws) {
    json step_json;
    step_json["array"] = arr;
    step_json["message"] = "Sorting Step";
    ws.write(asio::buffer(step_json.dump()));
}

// Bubble Sort
void bubbleSort(std::vector<int>& arr, websocket::stream<tcp::socket>& ws) noexcept {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        for (size_t j = 0; j < arr.size() - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                sendSortingStep(arr, ws);  // Send current sorting step
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Delay to simulate sorting process
            }
        }
    }
}

// Quick Sort
int partition(std::vector<int>& arr, int low, int high, websocket::stream<tcp::socket>& ws) noexcept {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
            sendSortingStep(arr, ws);  // Send current sorting step
        }
    }
    std::swap(arr[i + 1], arr[high]);
    sendSortingStep(arr, ws);  // Send current sorting step
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high, websocket::stream<tcp::socket>& ws) noexcept {
    if (low < high) {
        int pi = partition(arr, low, high, ws);
        quickSort(arr, low, pi - 1, ws);
        quickSort(arr, pi + 1, high, ws);
    }
}

// Merge Sort
void merge(std::vector<int>& arr, int left, int mid, int right, websocket::stream<tcp::socket>& ws) noexcept {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    std::copy(arr.begin() + left, arr.begin() + mid + 1, L.begin());
    std::copy(arr.begin() + mid + 1, arr.begin() + right + 1, R.begin());

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        }
        else {
            arr[k++] = R[j++];
        }
        sendSortingStep(arr, ws);  // Send current sorting step
    }

    while (i < n1) {
        arr[k++] = L[i++];
        sendSortingStep(arr, ws);  // Send current sorting step
    }

    while (j < n2) {
        arr[k++] = R[j++];
        sendSortingStep(arr, ws);  // Send current sorting step
    }
}

void mergeSort(std::vector<int>& arr, int left, int right, websocket::stream<tcp::socket>& ws) noexcept {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, ws);
        mergeSort(arr, mid + 1, right, ws);
        merge(arr, left, mid, right, ws);
    }
}

// Insertion Sort
void insertionSort(std::vector<int>& arr, websocket::stream<tcp::socket>& ws) noexcept {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
        sendSortingStep(arr, ws);  // Send current sorting step
    }
}

// Selection Sort
void selectionSort(std::vector<int>& arr, websocket::stream<tcp::socket>& ws) noexcept {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        int min_idx = i;
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        std::swap(arr[i], arr[min_idx]);
        sendSortingStep(arr, ws);  // Send current sorting step
    }
}
