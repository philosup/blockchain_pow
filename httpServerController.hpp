#ifndef __HTTPSERVERCONTROLLER_H__
#define __HTTPSERVERCONTROLLER_H__

#include <iostream>
#include <vector>
#include "httplib.h"

#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/document.h"
// #include "uint256.h"
#include "blockchain/block.hpp"
#include "blockchain/blockchain.hpp"

using namespace std;



class HttpServerController {

private:
    static vector<int> node_ports;
    static httplib::Server svr;
    static int port;
    static Blockchain chain;
    static string host;


    static void _PostMineAll(int difficulty, int begin, int end);
    static void _AddNode();
    static void _Exit();
public:
    HttpServerController() {

        while (CheckPort(port) == true) {
            node_ports.push_back(port++);
        }
        cout << "Http Server Start !!" << endl;


        svr.Get("/hi", [](const httplib::Request& req, httplib::Response& res) {
            res.set_content("Hello World! : " + to_string(port), "text/plain");
        });

        svr.Post("/node", PostNode);
        svr.Post("/mine", PostMine);
        svr.Post("/mine_all", PostMineAll);
        svr.Post("/block", PostBlock);
        svr.Post("/exit", [](const httplib::Request& req, httplib::Response& res) {
            _Exit();
            res.set_content("exit", "text/plain");
            exit(0);
        });


        _AddNode();

        cout << "Listen : " << port << endl;
        svr.listen("0.0.0.0", port);
    };


    static void PostNode(const httplib::Request& req, httplib::Response& res);
    static void PostMine(const httplib::Request& req, httplib::Response& res);
    static void PostMineAll(const httplib::Request& req, httplib::Response& res);
    static void PostBlock(const httplib::Request& req, httplib::Response& res);

    static void AddBlock(int _port, Block& block);
    static void MineByPort(int _port, int difficulty, int begin, int end);
    static bool CheckPort(int port);


};


vector<int> HttpServerController::node_ports;
httplib::Server HttpServerController::svr;
int HttpServerController::port = 8080;
Blockchain HttpServerController::chain;
string HttpServerController::host = "localhost";



void HttpServerController::PostNode(const httplib::Request& req, httplib::Response& res) {
    Document d;
    d.Parse(req.body);

    if (d.HasParseError()) {
        res.set_content("invalid json format", "text/plain");
        return;
    }

    if (d.HasMember("port") == false) {
        res.set_content("required 'port' field", "text/plain");
        return;
    }

    int node_port = d["port"].GetInt();

    node_ports.push_back(node_port);
    cout << "add port : " << node_port << endl;
    res.set_content("add port : " + to_string(node_port), "text/plain");
}

void HttpServerController::PostMine(const httplib::Request& req, httplib::Response& res) {
    int diff = 16;
    long begin = 0;
    long end = 1000'0000;

    Document d;
    d.Parse(req.body);

    if (d.HasParseError() == false)
    {
        if (d.HasMember("difficulty")) {
            Value& v = d["difficulty"];
            diff = v.GetInt();
            // cout << "set difficulty : " << diff << endl;
        }
        else
            cout << "difficulty is not a member" << endl;

        if (d.HasMember("begin") && d.HasMember("end")) {
            begin = d["begin"].GetInt64();
            end = d["end"].GetInt64();
        }
        else {
            cout << "properties(begin/end) are not setted" << endl;
        }
    }

    auto block = chain.getLastBlock();
    cout << "start mining : " << block.index<<", "<<port <<", "<<begin<<"~"<<end<<endl;

    uint256 UINT256_MAX = 0;
    UINT256_MAX--;

    uint256 difficult = UINT256_MAX >>= diff;

    auto prevhash = block.hash;
    auto index = block.index + 1;
    long timestamp = diff;
    for (long i = begin; i < end; i++) {
        auto block2 = Block(index, timestamp, prevhash, i);
        auto hash = calculateHash(&block2);

        if (hash < difficult) {
            cout <<"mined : " << port << ", " << i << endl;
            cout <<block2.GetJSON()<<endl;
            prevhash = block2.hash;

            AddBlock(port, block2);
            //TODO: broadcast to other nodes
            for (auto nPort : node_ports) {
                AddBlock(nPort, block2);
            }

            _PostMineAll(diff, 0, end - begin);
            break;
        }
        if (chain.getLastBlock().index != index - 1)
        {
            break;
        }
    }
    res.set_content(chain.GetJSON(), "application/json");
}

void HttpServerController::PostMineAll(const httplib::Request& req, httplib::Response& res) {

    int diff = 16;
    long begin = 0;
    long end = 1000'0000;

    Document d;
    d.Parse(req.body);

    if (d.HasParseError() == false)
    {
        if (d.HasMember("difficulty")) {
            Value& v = d["difficulty"];
            diff = v.GetInt();
            cout << "set difficulty : " << diff << endl;
        }
        else
            cout << "difficulty is not a member" << endl;

        if (d.HasMember("begin") && d.HasMember("end")) {
            begin = d["begin"].GetInt64();
            end = d["end"].GetInt64();
        }
        else {
            cout << "properties(begin/end) are not setted" << endl;
        }
    }

    _PostMineAll(diff, begin, end);

    res.set_content("start mining !!!", "text/plain");
}

void HttpServerController::PostBlock(const httplib::Request& req, httplib::Response& res) {
    // cout << req.body << endl;
    auto block = Block(req.body);

    chain.addBlock(block);
    //todo: validation
    res.set_content(block.GetJSON(), "application/json");
}

void HttpServerController::_PostMineAll(int difficulty, int begin, int end) {

    thread t(MineByPort, port, difficulty, begin, end);
    t.detach();
    auto range = end - begin;
    for (auto nPort : node_ports) {
        begin = end;
        end = begin + range;
        thread t(MineByPort, nPort, difficulty, begin, end);
        t.detach();
    }    
}

void HttpServerController::_AddNode() {
    for (auto nPort : node_ports) {
        httplib::Client cli(host, nPort);

        char param[100];
        sprintf(param, "{\"port\":%d}", port);
        cli.Post("/node", param, "application/json");
    }    
}

void HttpServerController::_Exit() 
{
    for (auto nPort : node_ports) {
        httplib::Client cli(host, nPort);

        cli.Post("/exit", "", "application/json");
    }
}
bool HttpServerController::CheckPort(int port) {
    httplib::Client cli("localhost", port);

    if (auto res = cli.Get("/hi")) {
        if (res->status == 200) {
            cout << res->body << std::endl;
        }
    }
    else {
        auto err = res.error();
        cout << err << endl;
        return false;
    }

    return true;
}
void HttpServerController::MineByPort(int _port, int difficulty, int begin, int end) {

    httplib::Client cli("localhost", _port);

    char param[200];

    sprintf(param, "{\"difficulty\":%d,\"begin\":%d,\"end\":%d}", difficulty, begin, end);

    auto res = cli.Post("/mine", param, "application/json");
}
void HttpServerController::AddBlock(int _port, Block& block) {
    httplib::Client cli("localhost", _port);

    auto res = cli.Post("/block", block.GetJSON().c_str(), "application/json");
}
#endif // __HTTPSERVERCONTROLLER_H__