/*
Fake Weather Client

You are going to develop a program that gets the statistics about weather in the current city using information from a certain server.
The goal is to calculate statistics using the data from weather server.

To communicate with the weather server you have to implement interface IWeatherServer,
which provides the raw string from the real server for the requested day and time.

The real server (i.e. "weather.com") gets the requests in this format:
"<date>;<time>", for example:
"31.08.2018;03:00"

The server answers on requests with string like this:
"20;181;5.1"
This string contains the weather for the requested time and means next:
"<air_temperature_in_celsius>;<wind_direction_in_degrees>:<wind_speed>".
Wind direction value may be in range from 0 to 359 inclusively, temperature may be negative.

The task:
1. Implement fake server, because interacting with real network is inacceptable within the unit tests.
To do this, you need to use real server responses.
Fortunately, you've collected some results for the several dates from the weather server. Each line means "<request>" : "<response>":

"31.08.2018;03:00" : "20;181;5.1"
"31.08.2018;09:00" : "23;204;4.9"
"31.08.2018;15:00" : "33;193;4.3"
"31.08.2018;21:00" : "26;179;4.5"

"01.09.2018;03:00" : "19;176;4.2"
"01.09.2018;09:00" : "22;131;4.1"
"01.09.2018;15:00" : "31;109;4.0"
"01.09.2018;21:00" : "24;127;4.1"

"02.09.2018;03:00" : "21;158;3.8"
"02.09.2018;09:00" : "25;201;3.5"
"02.09.2018;15:00" : "34;258;3.7"
"02.09.2018;21:00" : "27;299;4.0"

IMPORTANT:
* Server returns empty string if request is invalid.
* Real server stores weather only for times 03:00, 09:00, 15:00 and 21:00 for every date. Do not use other hours in a day.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

struct Weather
{
    short temperature = 0;
    unsigned short windDirection = 0;
    double windSpeed = 0;
    bool operator==(const Weather& right)
    {
        return temperature == right.temperature &&
               windDirection == right.windDirection &&
               std::abs(windSpeed - right.windSpeed) < 0.01;
    }
};

class IWeatherServer
{
public:
    virtual ~IWeatherServer() { }
    // Returns raw response with weather for the given day and time in request
    virtual std::string GetWeather(const std::string& request) = 0;
};


class IWeatherClient
{
public:
    virtual ~IWeatherClient() { }
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date) = 0;
};

/* Tests list:
 * Parcer
 * Parse string 20;181:5.1
 * Parse string 20;181:7.1
 * Parse string 20;182:7.2
 * Parse string 21;183:7.3
 * Parse string 22;184:7.4

   Client
   GetAverageTemperature
   GetMinimumTemperature
   GetMaximumTemperature


   GetAverageWindDirection
   GetMaximumWindSpeed

  */

class FakeServer : public IWeatherServer
{
public:
    FakeServer()
    {
        m_answers["31.08.2018;03:00"] = "20;181;5.1";
        m_answers["31.08.2018;09:00"] = "23;204;4.9";
        m_answers["31.08.2018;15:00"] = "33;193;4.3";
        m_answers["31.08.2018;21:00"] = "26;179;4.5";

        m_answers["01.09.2018;03:00"] = "19;176;4.2";
        m_answers["01.09.2018;09:00"] = "22;131;4.1";
        m_answers["01.09.2018;15:00"] = "31;109;4.0";
        m_answers["01.09.2018;21:00"] = "24;127;4.1";

        m_answers["02.09.2018;03:00"] = "21;158;3.8";
        m_answers["02.09.2018;09:00"] = "25;201;3.5";
        m_answers["02.09.2018;15:00"] = "34;258;3.7";
        m_answers["02.09.2018;21:00"] = "27;299;4.0";
    }
    std::string GetWeather(const std::string& request)
    {
        std::string answer = "";
        auto it = m_answers.find(request);
        if(it != m_answers.end())
        {
            answer = m_answers[request];
        }
        return answer;
    }
private:
    std::map<std::string, std::string> m_answers;
};

FakeServer g_fakeServer;

class WeatherClient: public IWeatherClient
{
public:
    WeatherClient()
    {}

    double GetAverageTemperature(IWeatherServer& server, const std::string& date)
    {
        double answer = 0;
        std::string at3 =  GetTemperatureAtTime(server, date, ";03:00");
        std::string at9 =  GetTemperatureAtTime(server, date, ";09:00");
        std::string at15 = GetTemperatureAtTime(server, date, ";15:00");
        std::string at21 = GetTemperatureAtTime(server, date, ";21:00");
        if (!at3.empty() && !at9.empty() && !at15.empty() && !at21.empty())
        {
            answer = ( atof(at3.c_str()) + atof(at9.c_str()) + atof(at15.c_str()) + atof(at21.c_str()) )/4;
        }
        return answer;
    }
    double GetMinimumTemperature(IWeatherServer& server, const std::string& date)
    {
        return 0;
    }
    double GetMaximumTemperature(IWeatherServer& server, const std::string& date)
    {
        return 0;
    }
    double GetAverageWindDirection(IWeatherServer& server, const std::string& date)
    {
        return 0;
    }
    double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date)
    {
        return 0;
    }
private:
    std::string GetTemperatureAtTime (IWeatherServer& server, const std::string& date, const std::string& time)
    {
        std::string response = server.GetWeather(date + time);
        if (response.empty())
        {
            return "";
        }
        else
        {
            return ParseTemperature(response);
        }
    }
    std::string ParseTemperature(std::string text)
    {
        std::size_t pos = text.find(";");
        return text.substr (0, pos);
    }

    std::string ParseWindDir(std::string text)
    {
        std::size_t startPos = text.find(";");
        std::size_t size = text.substr(startPos + 1).find(";");
        return text.substr(startPos + 1, size);
    }

    std::string ParseWindSpeed(std::string text)
    {
        std::size_t startPos = text.find(";");
        std::size_t size = text.substr(startPos + 1).find(";");
        return text.substr(startPos + size + 2);
    }
};


TEST (Weather, AverageTemperature_31_08 )
{
    double average = (20 + 23 + 33 + 26)/4. ;
    WeatherClient wClient;
    ASSERT_EQ(average, wClient.GetAverageTemperature(g_fakeServer, "31.08.2018"));
}

TEST (Weather, AverageTemperature_01_09 )
{
    double average = (19 + 22 + 31 + 24)/4. ;
    WeatherClient wClient;
    ASSERT_EQ(average, wClient.GetAverageTemperature(g_fakeServer, "01.09.2018"));
}

TEST (Weather, AverageTemperature_02_09 )
{
    double average = (21 + 25 + 34 + 27)/4. ;
    WeatherClient wClient;
    ASSERT_EQ(average, wClient.GetAverageTemperature(g_fakeServer, "02.09.2018"));
}
