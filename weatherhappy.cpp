#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

const std::string API_KEY = "9b3fd2ff1cbe7f6f3adc73241ac50558";

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

std::string getWeatherData(const std::string& city) {
    std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + API_KEY;
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK) {
            return response;
        }
    }
    return "";
}

void displayWeather(const std::string& city, const std::string& jsonData) {
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(jsonData, root)) {
        std::cout << "Weather information for " << city << ":\n";
        std::string weatherDesc = root["weather"][0]["description"].asString();
        std::string temp = root["main"]["temp"].asString();
        std::string humidity = root["main"]["humidity"].asString();
        std::string windSpeed = root["wind"]["speed"].asString();
        std::cout << "Description: " << weatherDesc << "\n";
        std::cout << "Temperature: " << temp << " K\n";
        std::cout << "Humidity: " << humidity << " %\n";
        std::cout << "Wind Speed: " << windSpeed << " m/s\n";
    } else {
        std::cout << "Failed to parse weather data.\n";
    }
}

int main() {
    std::string city;
    std::cout << "Enter city name: ";
    std::getline(std::cin, city);

    std::string weatherData = getWeatherData(city);
    if (!weatherData.empty()) {
        displayWeather(city, weatherData);
    } else {
        std::cout << "Failed to fetch weather data.\n";
    }

    return 0;
}
