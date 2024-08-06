#include "WeatherApp.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "httplib.h"  // Included on the same line
#include "json.hpp"
#include "stb_image.h"  // To load the background image
#include <fstream>

// Include GLEW before any other OpenGL headers
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using json = nlohmann::json;

void WeatherApp::SaveCredentials() {
    std::ofstream file("credentials.txt");
    if (file.is_open()) {
        file << username << std::endl;
        file << password << std::endl;
        file.close();
    }
    else {
        std::cerr << "Unable to open file for writing credentials." << std::endl;
    }
}

bool WeatherApp::LoadCredentials() {
    std::ifstream file("credentials.txt");
    if (file.is_open()) {
        file.getline(username, 128);
        file.getline(password, 128);
        file.close();
        return true;
    }
    else {
        std::cerr << "Unable to open file for reading credentials." << std::endl;
        return false;
    }
}

void WeatherApp::RenderLoginForm() {

    // Define RGB colors
    ImVec4 windowBgColor = ImVec4(73.0f / 255.0f, 105.0f / 255.0f, 137.0f / 255.0f, 1.0f);  // Darker background color
    ImVec4 textColor = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);  // White text color
    ImVec4 buttonColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Blue button color
    ImVec4 buttonHoverColor = ImVec4(88.0f / 255.0f, 163.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Lighter blue when hovered
    ImVec4 buttonActiveColor = ImVec4(88.0f / 255.0f, 163.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Darker blue when active
    ImVec4 borderColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);
    ImVec4 titleBgColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Custom title bar color
    ImVec4 titleBgActiveColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, windowBgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, titleBgColor);  // Change title bar background color
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, titleBgActiveColor);

    // Backup previous style settings
    ImGuiStyle& style = ImGui::GetStyle();
    float previousFrameRounding = style.FrameRounding;

    // Set the rounding for frames
    style.FrameRounding = 12.0f;  // Adjust this value for desired roundness

    ImGui::Begin("Login", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::SetWindowPos(ImVec2(600, 250), ImGuiCond_Always);  // Adjust position if needed
    ImGui::SetWindowSize(ImVec2(650, 450), ImGuiCond_Always);  // Increased width and height

    // Center "Welcome to"
    const char* welcomeText1 = "Welcome to";
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth1 = ImGui::CalcTextSize(welcomeText1).x;
    ImGui::SetCursorPosX(((windowWidth - textWidth1) * 0.5f) - 30);

    // Use specific font for welcome text
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);  // Assuming the welcome text font is the fifth font added
    ImGui::Text("%s", welcomeText1);
    ImGui::PopFont();

    // Center "Weather App"
    const char* welcomeText2 = "Weather App";
    float textWidth2 = ImGui::CalcTextSize(welcomeText2).x;
    ImGui::SetCursorPosX(((windowWidth - textWidth2) * 0.5f) - 30);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);  // Assuming the welcome text font is the fifth font added
    ImGui::Text("%s", welcomeText2);
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Username");  // Display Username label
    ImGui::Dummy(ImVec2(0.0f, 5.0f));  // Add margin
    ImGui::InputText("##Username", username, IM_ARRAYSIZE(username));

    ImGui::Dummy(ImVec2(0.0f, 5.0f));  // Add margin

    ImGui::Text("Password");  // Display Password label
    ImGui::Dummy(ImVec2(0.0f, 5.0f));  // Add margin
    ImGui::InputText("##Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

    ImGui::Dummy(ImVec2(0.0f, 5.0f));  // Add margin

    // Center "Login" button and adjust its size
    float buttonWidth = 120.0f;  // Adjust the button width as needed
    float buttonHeight = 55.0f;  // Adjust the button height as needed
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
    if (ImGui::Button("Login", ImVec2(buttonWidth, buttonHeight))) {
        // Validate username and password
        char storedUsername[128];
        char storedPassword[128];

        std::ifstream file("credentials.txt");
        if (file.is_open()) {
            file.getline(storedUsername, 128);
            file.getline(storedPassword, 128);
            file.close();

            if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
                isLoggedIn = true;
                SaveCredentials();
                errorMessage[0] = '\0';  // Clear error message
            }
            else {
                strcpy_s(errorMessage, "Invalid username or password.");
            }
        }
        else {
            std::cerr << "Unable to open credentials file." << std::endl;
        }
    }

    if (errorMessage[0] != '\0') {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", errorMessage);  // Display error message in red
    }

    ImGui::End();

    // Pop all the style colors we pushed
    ImGui::PopStyleColor(8);

    // Restore previous style settings
    style.FrameRounding = previousFrameRounding;
}

GLuint LoadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

GLuint LoadPNGTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

GLuint WeatherApp::LoadIconTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load icon: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void WeatherApp::AddFavoriteCity(const std::string& cityName) { // new item
    favoriteCities.push_back(cityName); // new item
    SaveFavoriteCities(); // new item
}

void WeatherApp::RemoveFavoriteCity(const std::string& cityName) { // new item
    favoriteCities.erase(std::remove(favoriteCities.begin(), favoriteCities.end(), cityName), favoriteCities.end()); // new item
    SaveFavoriteCities(); // new item
}

void WeatherApp::SaveFavoriteCities() { // new item
    std::ofstream file("favorites.txt"); // new item
    if (file.is_open()) { // new item
        for (const auto& city : favoriteCities) { // new item
            file << city << std::endl; // new item
        }
        file.close(); // new item
    }
}

bool WeatherApp::LoadFavoriteCities() { // new item
    std::ifstream file("favorites.txt"); // new item
    if (file.is_open()) { // new item
        std::string city; // new item
        while (std::getline(file, city)) { // new item
            favoriteCities.push_back(city); // new item
        }
        file.close(); // new item
        return true; // new item
    }
    return false; // new item
}

bool WeatherApp::IsFavoriteCity(const std::string& cityName) const { // new item
    return std::find(favoriteCities.begin(), favoriteCities.end(), cityName) != favoriteCities.end(); // new item
}

WeatherApp::WeatherApp() {
    // Initialize GLFW
    if (!glfwInit())
        return;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1920, 1080, "Weather App", NULL, NULL);  // Adjusted window size
    if (!window) {
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Ensure GLEW uses modern techniques for managing OpenGL functionality
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW!\n");
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Load custom fonts
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Light.ttf", 30.0f);  // Default font size
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Regular.ttf", 32.0f);  // Larger font size
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Light.ttf", 60.0f);  // Time and Date font size
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Light.ttf", 40.0f);  // City Cards
    io.Fonts->AddFontFromFileTTF("Externals/Quicksand-Bold.ttf", 45.0f); // Welcome

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Initialize city weather objects
    cities.push_back(CityWeather("London"));
    cities.push_back(CityWeather("New York"));
    cities.push_back(CityWeather("Tokyo"));
    cities.push_back(CityWeather("Paris"));
    cities.push_back(CityWeather("Berlin"));

    // Initialize search bar
    strcpy_s(citySearch, "");
    showCityDetails = false;
    currentCity = nullptr;

    // Load background image
    backgroundTexture = LoadTexture("Externals/wallpaper.jpg");  // Updated image path

    // Load search icon
    searchIconTexture = LoadPNGTexture("Externals/search_icon.png");  // Path to your PNG image

    // Load button icon texture
    detailsButtonIconTexture = LoadIconTexture("Externals/details_icon.png");

    // Load favorite button icon textures
    favoriteButtonIconTexture = LoadIconTexture("Externals/favorite_icon.png");  // Path to your favorite icon PNG
    favoriteButtonIconTextureFilled = LoadIconTexture("Externals/starred_icon.png");
    
    // Initialize the stop flag for the weather fetch thread
    stopThread = false;

    // Initialize login form variables
    isLoggedIn = false; // Start with the login form
    strcpy_s(username, "");
    strcpy_s(password, "");
    // Initialize error message
    strcpy_s(errorMessage, "");

    // Start the weather fetch thread
    weatherThread = std::thread(&WeatherApp::FetchAndDisplayWeather, this);
}



WeatherApp::~WeatherApp() {
    // Stop the weather fetch thread
    stopThread = true;
    if (weatherThread.joinable()) {
        weatherThread.join();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    // Delete background texture
    glDeleteTextures(1, &backgroundTexture);
}

void WeatherApp::Run() {
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render the GUI
        if (!isLoggedIn) {
            RenderLoginForm();  // Render the login form if not logged in
        }
        else {
            RenderGUI();  // Render the main GUI if logged in
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void WeatherApp::RenderGUI() {
    // Backup previous style settings
    ImGuiStyle& style = ImGui::GetStyle();
    float previousFrameRounding = style.FrameRounding;

    // Set the rounding for frames
    style.FrameRounding = 12.0f;  // Adjust this value for desired roundness

    // Define custom colors
    ImVec4 windowBgColor = ImVec4(73.0f / 255.0f, 105.0f / 255.0f, 137.0f / 255.0f, 1.0f);  // Darker background color
    ImVec4 textColor = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);  // White text color
    ImVec4 buttonColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Blue button color
    ImVec4 buttonHoverColor = ImVec4(88.0f / 255.0f, 163.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Lighter blue when hovered
    ImVec4 buttonActiveColor = ImVec4(88.0f / 255.0f, 163.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Darker blue when active
    ImVec4 borderColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);
    ImVec4 titleBgColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);  // Custom title bar color
    ImVec4 titleBgActiveColor = ImVec4(88.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f, 1.0f);

    // Apply custom colors
    ImGui::PushStyleColor(ImGuiCol_WindowBg, windowBgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, titleBgColor);  // Change title bar background color
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, titleBgActiveColor);

    // Render background
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1920, 1080), ImGuiCond_Always);
    ImGui::Begin("Background", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoInputs);
    ImGui::Image((void*)(intptr_t)backgroundTexture, ImVec2(1920, 1080));
    ImGui::End();

   
    // Render main content
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1920, 1080), ImGuiCond_Always);
    ImGui::Begin("Weather App", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // Add a margin for the time and date from the beginning
    float margin = 70.0f;  // Adjust this value for desired margin

    // Display time and date
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);  // Assuming the third font is the largest one
    std::string currentTime = GetCurrentTime1();
    std::string currentDate = GetCurrentDate();
    ImGui::SetCursorPos(ImVec2(margin, margin));  // Adjusted position for time
    ImGui::Text(currentTime.c_str());
    ImGui::SetCursorPos(ImVec2(margin, margin + 70));  // Adjusted position for date
    ImGui::Text(currentDate.c_str());
    ImGui::PopFont();

    // Add spacing and search bar
    ImGui::SetCursorPos(ImVec2(margin, margin + 200));  // Adjusted position for search bar
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);  // Use larger font for search bar

    // Increase search bar height and set width
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    ImGui::PushItemWidth(700);  // Set width of the search bar
    ImGui::InputText("##Search", citySearch, IM_ARRAYSIZE(citySearch), ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::PopStyleVar();
    ImGui::PopFont();

    ImGui::SameLine();

    // Make button background transparent
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    // Adjust button size to match search field and use icon
    if (ImGui::ImageButton((void*)(intptr_t)searchIconTexture, ImVec2(45, 45))) {  // Explicit size for the button
        searchCity = citySearch;
        FetchCityDetails(searchCity);
    }

    ImGui::PopStyleColor(3);

    if (showCityDetails && currentCity) {
        RenderCityDetails();
    }

    // Display weather cards
    float cardWidth = 200.0f;
    float cardHeight = 150.0f;
    float startX = margin;  // Adjusted start position for cards
    float startY = margin + 350.0f;  // Adjusted start position for cards
    float spacingX = 50.0f;
    for (size_t i = 0; i < cities.size(); ++i) {
        ImGui::SetCursorPos(ImVec2(startX + i * (cardWidth + spacingX), startY));
        ImGui::BeginGroup();
        cities[i].Render(detailsButtonIconTexture);
        ImGui::EndGroup();

        // Check if the button was pressed
        if (ImGui::IsItemClicked()) {
            currentCity = &cities[i];
            showCityDetails = true;
        }
    }
    // Render Favorite Cities list
    ImGui::SetCursorPos(ImVec2(1550, 300)); // Position for favorite cities list
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.75f, 0.07f, 1.0f));  // Set border color to #FFC107
    ImGui::BeginChild("Favorite Cities List", ImVec2(300, 200), true); // Begin child window for the favorite cities list

    // Center title text
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);  // Use the second font for the title
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize("Favorite Cities:").x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("Favorite Cities:");
    ImGui::PopFont();

    for (const auto& city : favoriteCities) { // Loop through favorite cities
        if (ImGui::Selectable(city.c_str())) { // Display each city as a selectable item
            std::cout << "Selected city: " << city << std::endl;
            searchCity = city;
            FetchCityDetails(searchCity);
            showCityDetails = true;
        }
    }
    ImGui::EndChild(); // End child window for the favorite cities list

    ImGui::PopStyleColor();

    ImGui::End();

    // Restore previous style settings
    style.FrameRounding = previousFrameRounding;

    // Pop all the style colors we pushed
    ImGui::PopStyleColor(8);
}

void WeatherApp::RenderCityDetails() {
    ImGui::SetNextWindowPos(ImVec2(500, 200), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);
    ImGui::Begin("City Details", &showCityDetails, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);  // Assuming the second font is the larger one
    ImGui::Text("%s", currentCity->GetCityName().c_str());

    ImGui::SameLine();  // Position the favorite button next to the city name
    bool isFavorite = IsFavoriteCity(currentCity->GetCityName());
    GLuint iconTexture = isFavorite ? favoriteButtonIconTextureFilled : favoriteButtonIconTexture;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    if (ImGui::ImageButton((void*)(intptr_t)iconTexture, ImVec2(30, 30))) {
        if (isFavorite) {
            RemoveFavoriteCity(currentCity->GetCityName());
        }
        else {
            AddFavoriteCity(currentCity->GetCityName());
        }
    }

    ImGui::PopStyleColor(3);
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0.0f, 20.0f));  // Add vertical margin
    ImGui::Text("Temperature: %.2f C", currentCity->GetTemperature());

    ImGui::Dummy(ImVec2(0.0f, 10.0f));  // Add vertical margin
    ImGui::Text("Humidity: %d%%", currentCity->GetHumidity());

    ImGui::Dummy(ImVec2(0.0f, 10.0f));  // Add vertical margin
    ImGui::Text("Pressure: %d hPa", currentCity->GetPressure());

    ImGui::Dummy(ImVec2(0.0f, 10.0f));  // Add vertical margin
    ImGui::Text("Wind Speed: %.2f m/s", currentCity->GetWindSpeed());

    ImGui::Dummy(ImVec2(0.0f, 20.0f));  // Add vertical margin before the Back button

    // Center the "Back" button and adjust its size
    float buttonWidth = 100.0f;  // Adjust the button width as needed
    float buttonHeight = 40.0f;  // Adjust the button height as needed
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - buttonWidth) * 0.5f);
    if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
        showCityDetails = false;
        currentCity = nullptr;
    }

    ImGui::End();
}

std::string WeatherApp::GetCurrentTime1() {
    auto now = std::time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%H:%M:%S");
    return oss.str();
}

std::string WeatherApp::GetCurrentDate() {
    auto now = std::time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%A, %d %B %Y");
    return oss.str();
}

void WeatherApp::FetchAndDisplayWeather() {
    // List of cities
    std::vector<std::string> cityNames = { "London", "New York", "Tokyo", "Paris", "Berlin" };

    while (!stopThread.load()) {
        for (size_t i = 0; i < cities.size(); ++i) {
            cities[i].UpdateWeatherData();
        }

        // Add a delay to prevent constant API calls
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
}

void WeatherApp::FetchCityDetails(const std::string& city) {
    for (auto& cityWeather : cities) {
        if (cityWeather.GetCityName() == city) {
            cityWeather.UpdateWeatherData();
            currentCity = &cityWeather;
            showCityDetails = true;
            return;
        }
    }

    // If the city is not in the list, create a new CityWeather object
    CityWeather newCity(city);
    newCity.UpdateWeatherData();
    cities.push_back(newCity);
    currentCity = &cities.back();
    showCityDetails = true;
}

int main(int, char**) {
    WeatherApp app;
    app.Run();
    return 0;
}
