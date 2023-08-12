#include <cmath>
#include "matplotlibcpp.h"
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <string>
#include <map>
#include <time.h>
//#include "nlohmann/json.hpp"
namespace plt = matplotlibcpp;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void showplot(std::vector<std::string>& dates,std::vector<double>& subject,std::string YLabel,std::string title){
    plt::plot(dates,subject, "b-");
    plt::xlabel("Date");
    plt::ylabel(YLabel);
    plt::title(title);
    plt::xticks(dates);
    plt::show();
}

int main(){
    CURL *curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl=curl_easy_init();
    if(curl){
        curl_easy_setopt(curl,CURLOPT_URL,"https://pastebin.com/raw/3PYLDVXs");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res=curl_easy_perform(curl);
        if(res!=CURLE_OK){
             std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
    std::cout << response << std::endl;

     nlohmann::json jsonData;
    try{
        jsonData= nlohmann::json::parse(response);

    }catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return EXIT_SUCCESS;
    }
    std::vector<std::string> dates;
    std::vector<double> toanScores;
    std::vector<double> lyScores;

    // Extract "TOAN" scores with dates for plotting
    for (const auto &entry : jsonData["LOP11"]) {
        std::string date = entry["date"];
        dates.push_back(date);

        double toanScore = entry["TOAN"];
        toanScores.push_back(toanScore);

        double lyScore = entry["LY"];
        lyScores.push_back(lyScore);
    }
    bool quit=false;
    std::string answer;
    while(!quit){
        std::cout<<"What subject do you want to plot?: "<<"\n";
        std::cin>>answer;
        for(int i=0;i<answer.size();i++){
            answer[i]=tolower(answer[i]);
        }

        if(answer=="physic"){
            showplot(dates,lyScores,"Scores","Physic score over time");
        }else if(answer=="math"){
             showplot(dates,toanScores,"Scores","Math score over time");
        }else{
            std::cout<<"NOT THE SUBJECT IN CODE QUITTING THE PROFRAM"<<"\n";
            return EXIT_SUCCESS;
        }
    }



}