//#include <QtCore>
#include "prettyprint.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "strconv.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
// https://qiita.com/yohm/items/0f389ba5c5de4e2df9cf
#include <sstream>
#include <Poco/URI.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include "Poco/Net/Context.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include <string>
#include <map>
#include <iostream>
#include <boost/regex.hpp>
//using namespace std;
unicode_ostream uout(std::cout, GetConsoleOutputCP());
std::map<std::string, std::string> url_parse_test(const std::string &url)
{
    std::map<std::string, std::string> result;
    //std::string url="https://www.google.com:443/webhp?gws_rd=ssl#q=cpp";
    boost::regex ex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
    boost::cmatch what;
    if(regex_match(url.c_str(), what, ex))
    {
        uout << "protocol: " << std::string(what[1].first, what[1].second) << std::endl;
        uout << "domain:   " << std::string(what[2].first, what[2].second) << std::endl;
        uout << "port:     " << std::string(what[3].first, what[3].second) << std::endl;
        uout << "path:     " << std::string(what[4].first, what[4].second) << std::endl;
        uout << "query:    " << std::string(what[5].first, what[5].second) << std::endl;
        uout << "fragment: " << std::string(what[6].first, what[6].second) << std::endl;
        result["scheme"] = std::string(what[1].first, what[1].second);
        result["domain"] = std::string(what[2].first, what[2].second);
        result["port"] = std::string(what[3].first, what[3].second);
        if(result["port"].empty())
        {
            if(result["scheme"]=="http")
            {
                result["port"] = "80";
            }
            else if(result["scheme"]=="https")
            {
                result["port"] = "443";
            }
        }
        result["path"] = std::string(what[4].first, what[4].second);
        result["query"] = std::string(what[5].first, what[5].second);
        result["fragment"] = std::string(what[6].first, what[6].second);
    }
    return result;
}
int main(int argc, char **argv)
{
    std::map<std::string, std::string> parsed = url_parse_test("https://www.google.com:443/webhp?gws_rd=ssl#q=cpp");
    std::cout << parsed << std::endl;
    std::map<std::string, std::string> map = url_parse_test("https://gitlab.com/spider-explorer/spider-software/-/raw/main/spider-software.json");
    httplib::Client cli(format("%s://%s:%s", map["scheme"].c_str(), map["domain"].c_str(), map["port"].c_str()));
    auto res = cli.Get(map["path"].c_str());
    uout << res->status << std::endl;
    uout << res->body << std::endl;
    return 0;
}
