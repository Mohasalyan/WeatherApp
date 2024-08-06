# Weather App
## Table of Contents
- [Description](#description)
- [Features](#features)
- [Screenshots](#screenshots)
- [Installation](#installation)
- [Code Explanation](#code-explanation)
- [Prerequisites](#prerequisites)
- [Configuration](#configuration)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
## Description
The Weather App is a visually appealing and interactive application that provides real-time and historical weather data for various cities. Built using C++ and the ImGui library, the app features a user-friendly graphical interface and offers functionalities such as viewing current weather conditions, displaying various weather details, and managing favorite cities. It leverages the OpenWeatherMap API to retrieve weather information.

## Features
- Fetch current weather data for multiple cities.
- Display detailed weather information for selected cities.
- Favorite cities management.
- User authentication and login system.
- Real-time weather updates with multithreading.

## Screenshots
![Login Screen](https://i.imgur.com/NvQ2rAS.png)
![Main Screen](https://i.imgur.com/vpIVGB9.png)
![City Details Screen](https://i.imgur.com/4cJEQbg.png)

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/weather-app.git
   
## Code Explanation

### Fetching Weather Data
The application uses the `httplib` library to fetch weather data from the OpenWeatherMap API. The `CityWeather` class contains functions to fetch current and historical weather data.

#### Fetching Current Weather Data
- **Function: `FetchWeatherData`**
  - **Purpose:** Fetches current weather data for a specific city.
  - **Description:** Makes an HTTP GET request to the OpenWeatherMap API and updates weather attributes.

#### Fetching Historical Weather Data
- **Function: `FetchHistoricalWeatherData`**
  - **Purpose:** Fetches historical weather data for a specific city.
  - **Description:** Makes HTTP GET requests to the OpenWeatherMap API and stores historical data in a map.

### Updating the UI
The `WeatherApp` class handles rendering the main GUI of the weather app, including time, date, search bar, weather cards, and favorite cities list.

#### Main Screen
- **Function: `RenderGUI`**
  - **Purpose:** Renders the main GUI.
  - **Description:** Sets up the layout and styles for the main screen elements using ImGui.

#### City Details Screen
- **Function: `RenderCityDetails`**
  - **Purpose:** Renders the City Details screen.
  - **Description:** Displays detailed weather information for the selected city and a back button to return to the main screen.

#### Login Screen
- **Function: `RenderLoginForm`**
  - **Purpose:** Renders the login form.
  - **Description:** Displays input fields for username and password, a welcome message, and a login button.

### Favorite Cities Management
The favorite cities feature allows users to maintain a list of their favorite cities for quick access.

- **Function: `RenderFavoriteCities`**
    - **Purpose:** Renders the Favorite Cities list.
    - **Description:** Displays the favorite cities on the main screen and allows users to select a city to view detailed weather information.

### Fetch and Display Weather Data Concurrently

**WeatherApp Class:**
- **Function: `FetchAndDisplayWeather`**
  - **Purpose:** Fetches weather data for a list of cities concurrently and updates the UI with the fetched data.
  - **Description:**
    - Uses multithreading to fetch weather data for multiple cities concurrently.
    - Periodically updates the weather data every 5 minutes to ensure the displayed information is current.

## Prerequisites
- C++ Compiler (e.g., GCC, Clang, MSVC)
- CMake
- Git
- Libraries: `httplib`, `json`, `imgui`, `stb_image`

## Configuration
1. Obtain an API key from [OpenWeatherMap](https://openweathermap.org/api).
2. Set the API key in the project configuration:
   ```cpp
   std::string apiKey = "YOUR_API_KEY";
3. Ensure the credentials.txt file is present in the project directory with the following format:
   ```cpp
   username
   password
## Usage

1. **Run the application:**
2. **Login with your credentials:**
    - Enter your username and password in the login form.
    - Click the "Login" button to authenticate.
3. **Use the search bar to find weather information for different cities:**
    - Enter the city name in the search bar.
    - Click the search icon to fetch weather data for the entered city.
    - The main screen will display the current weather information for the searched city.
4. **View detailed weather information:**
    - Click on a city in the weather cards section to view its detailed weather information.
    - The City Details screen will display temperature, humidity, pressure, and wind speed for the selected city.
    - Use the "Back" button to return to the main screen.
5. **Add cities to your favorites for quick access:**
    - Click the favorite icon next to the city name in the City Details screen to add the city to your favorites.
    - Favorite cities will be displayed in the Favorite Cities list on the right side of the main screen.
    - Click on a city in the Favorite Cities list to view its detailed weather information.
